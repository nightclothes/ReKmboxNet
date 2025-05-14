#include "../include/table.hpp"
#include <string>
#include <mutex>
#include <Winsock2.h>
/*
    命令码
*/
enum class KmboxNetCommand : unsigned int {
    CONNECT = 0xaf3c2828,           // 连接盒子
    MOUSE_MOVE = 0xaede7345,        // 鼠标移动
    MOUSE_LEFT = 0x9823AE8D,        // 鼠标左键控制
    MOUSE_MIDDLE = 0x97a3AE8D,      // 鼠标中键控制
    MOUSE_RIGHT = 0x238d8212,       // 鼠标右键控制
    MOUSE_WHEEL = 0xffeead38,       // 鼠标滚轮控制
    MOUSE_AUTOMOVE = 0xaede7346,    // 鼠标自动模拟人工移动控制
    KEYBOARD_ALL = 0x123c2c2f,      // 键盘所有参数控制
    REBOOT = 0xaa8855aa,            // 盒子重启
    BAZER_MOVE = 0xa238455a,        // 鼠标贝塞尔移动
    MONITOR = 0x27388020,           // 监控盒子上的物理键鼠数据
    DEBUG = 0x27382021,             // 开启调试信息
    MASK_MOUSE = 0x23234343,        // 屏蔽物理键鼠
    UNMASK_ALL = 0x23344343,        // 解除屏蔽物理键鼠
    SET_CONFIG = 0x1d3d3323,        // 设置IP配置信息
    SET_VIDPID = 0xffed3232,        // 设置device端的vidpid
    SHOW_PIC = 0x12334883,          // 显示图片
    TRACE_ENABLE = 0xbbcdddac       // 使能硬件修正功能
};
/**
 * @brief 将枚举类型转换为对应的数值
 * @param key 枚举值
 * @return 对应的数值
 */
inline unsigned int to_value(KmboxNetCommand key) {
    return static_cast<unsigned int>(key);
}


/*
    数据包头部
*/
typedef struct
{	
	unsigned int  mac;			//盒子的mac地址（必须）
	unsigned int  rand;			//随机值
	unsigned int  indexpts;		//时间戳
	unsigned int  cmd;			//指令码
}cmd_head_t;

/*
    数据包数据1
*/
typedef struct
{
	unsigned char buff[1024];	//
}cmd_data_t;

/*
    数据包数据2
*/
typedef struct
{
	unsigned short buff[512];	//
}cmd_u16_t;

/*
    鼠标数据
*/
typedef struct
{
	int button;
	int x;
	int y;
	int wheel;
	int point[10];//用于贝塞尔曲线控制(预留5阶导)
}soft_mouse_t;

/*
    键盘数据
*/
typedef struct
{
	char ctrl;
	char resvel;
	char button[10];
}soft_keyboard_t;

/*
    整体数据
*/
typedef struct
{
	cmd_head_t head;
	union {
		cmd_data_t      u8buff;		  //buff
		cmd_u16_t       u16buff;	  //U16
		soft_mouse_t    cmd_mouse;    //鼠标发送指令
		soft_keyboard_t cmd_keyboard; //键盘发送指令
	};  
}client_tx;

/*
    通信结果
*/
enum class ConResult:int{
    ERR_CREAT_SOCKET = -9000,	//创建socket失败
	ERR_NET_VERSION,			//socket版本错误
	ERR_NET_TX,					//socket发送错误
	ERR_NET_RX_TIMEOUT,			//socket接收超时
	ERR_NET_CMD,				//命令错误
	ERR_NET_PTS,				//时间戳错误
	SUCCESS = 0,				//正常执行
	USB_DEV_TX_TIMEOUT,			//USB devic发送失败
};


/*
    KmboxNet外设功能
*/
class KmboxNet {
public:
    /**
     * 构造函数
     * @param ip 盒子的IP地址
     * @param port 通信端口号
     * @param mac 盒子的MAC地址
     */
    KmboxNet(std::string ip, std::string port, std::string mac){
        this->ip = ip;
        this->port = port;
        this->mac = mac;
    }
    
    /**
     * 重新设置参数
     * @param ip 盒子的IP地址
     * @param port 通信端口号
     * @param mac 盒子的MAC地址
     */
    void reset(std::string ip, std::string port, std::string mac){
        std::lock_guard<std::mutex> lock(mutex);
        this->ip = ip;
        this->port = port;
        this->mac = mac;
    }

    /**
     * 连接
     * @return 连接结果
     */
    ConResult connect(){
        std::lock_guard<std::mutex> lock(mutex);
        // socket
        WSADATA info;
        if(WSAStartup(MAKEWORD(1, 1), &info) != 0){
            return ConResult::ERR_CREAT_SOCKET;
        }
        if (LOBYTE(info.wVersion) != 1 || HIBYTE(info.wVersion) != 1) {
            return ConResult::ERR_NET_VERSION;
        }
        io_socket = socket(AF_INET, SOCK_DGRAM, 0); // 创建UDP套接字
        addr_info.sin_family = AF_INET; // 设置地址族为IPv4
        addr_info.sin_port = htons(atoi(port.c_str())); // 设置端口号
        addr_info.sin_addr.S_un.S_addr = inet_addr(ip.c_str()); // 设置IP地址

        // 设置发送数据
        send_data.head.mac = str2hex(mac, 4); // 设置mac地址
        srand((unsigned)time(NULL)); // 设置随机种子
        send_data.head.rand = rand(); // 设置随机值
        send_data.head.indexpts = 0; // 设置指令统计值
        send_data.head.cmd = to_value(KmboxNetCommand::CONNECT); // 设置命令码
        memset(&mouse_data, 0, sizeof(mouse_data));	//软件鼠标数据清零
		memset(&keyboard_data, 0, sizeof(keyboard_data));//软件键盘数据清零

        // 生成密钥
        secret_key[0] = send_data.head.mac >> 24; 
        secret_key[1] = send_data.head.mac >> 16;
        secret_key[2] = send_data.head.mac >> 8; 
        secret_key[3] = send_data.head.mac  >> 0; 

        // 发送数据
        int err = sendto(io_socket, (char*)&send_data, sizeof(cmd_head_t), 0, (SOCKADDR*)&addr_info, sizeof(addr_info));

        // 接收数据
        int from_len = sizeof(addr_info);
        err = recvfrom(io_socket, (char*)&recv_data, 1024, 0, (SOCKADDR*)&addr_info, &from_len);

        // 处理返回结果
        return handle_return(&recv_data, &send_data);
    }

    ConResult move_mouse(int x, int y){
        std::lock_guard<std::mutex> lock(mutex);
        if (io_socket == 0){
            return ConResult::ERR_NET_TX;   
        }

        // 设置鼠标数据
        mouse_data.x = x;
        mouse_data.y = y;

        // 设置发送数据
        send_data.head.cmd = to_value(KmboxNetCommand::MOUSE_MOVE);
        send_data.cmd_mouse = mouse_data;
        send_data.head.indexpts++;
        send_data.head.rand = rand();

        // 发送数据
        int err = sendto(io_socket, (char*)&send_data, sizeof(cmd_head_t) + sizeof(soft_mouse_t), 0, (SOCKADDR*)&addr_info, sizeof(addr_info));

        // 接收数据
        int from_len = sizeof(addr_info);
        err = recvfrom(io_socket, (char*)&recv_data, 1024, 0, (SOCKADDR*)&addr_info, &from_len);

        // 更新鼠标数据
        mouse_data.x = 0;
        mouse_data.y = 0;

        // 处理返回结果
        return handle_return(&recv_data, &send_data);
    }


    ConResult disconnect(){
        std::lock_guard<std::mutex> lock(mutex);
        return ConResult::SUCCESS;
    }

    ~KmboxNet() = default;
private:
    std::mutex mutex;

    std::string ip; 
    std::string port;
    std::string mac;

    unsigned char secret_key[16] = {0}; // 加密密钥 

    SOCKET io_socket = 0; //键鼠网络通信句柄
    SOCKADDR_IN addr_info; // 地址信息

    client_tx send_data; // 发送数据
    client_tx recv_data; // 接收数据

    soft_mouse_t mouse_data; // 鼠标数据
    soft_keyboard_t keyboard_data; // 键盘数据

    /**
     * 将字符串转换为十六进制数
     * @param pbSrc 源字符串
     * @param nLen 源字符串的长度
     * @return 转换后的十六进制数
     */
    unsigned int str2hex(std::string pbSrc, int nLen)
    {	char h1, h2;
        unsigned char s1, s2;
        int i;
        unsigned int pbDest[16] = { 0 };
        for (i = 0; i < nLen; i++) {
            h1 = pbSrc[2 * i]; // 获取第一个字符
            h2 = pbSrc[2 * i + 1]; // 获取第二个字符
            // 将字符转换为数字，'A'-'F'转换为10-15，'a'-'f'转换为10-15
            s1 = toupper(h1) - 0x30;
            if (s1 > 9)
                s1 -= 7;
            s2 = toupper(h2) - 0x30;
            if (s2 > 9)
                s2 -= 7;
            // 将两个数字合并为一个十六进制数
            pbDest[i] = s1 * 16 + s2;
        }
        // 将转换后的十六进制数合并为一个整数
        return pbDest[0] << 24 | pbDest[1] << 16 | pbDest[2] << 8 | pbDest[3];
    }

    /**
     * 处理返回结果
     * @param recv 接收到的数据
     * @param send 发送的数据
     * @return 处理结果
     */
    ConResult handle_return(client_tx* recv, client_tx* send)		 
    {
        ConResult ret = ConResult::SUCCESS;
        if (recv->head.cmd != send->head.cmd)
            ret = ConResult::ERR_NET_CMD;//命令码错误
        if (recv->head.indexpts != send->head.indexpts)
            ret= ConResult::ERR_NET_PTS;//时间戳错误
        return ret;				
    }


};

