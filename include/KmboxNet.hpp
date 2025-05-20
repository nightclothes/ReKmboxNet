#include "../include/table.hpp"
#include <string>
#include <mutex>
#include <Winsock2.h>
#include <memory>
#include <array>
#include <stdexcept>

// 位操作定义
#define BIT0 0X01 // 第0位
#define BIT1 0X02 // 第1位
#define BIT2 0X04 // 第2位
#define BIT3 0X08 // 第3位
#define BIT4 0X10 // 第4位
#define BIT5 0X20 // 第5位
#define BIT6 0X40 // 第6位
#define BIT7 0X80 // 第7位

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
    FIXME 错误码存在逻辑问题:1.还不够完整 2.定义含糊
*/
enum class ConResult:int{
    ERR_CREAT_SOCKET,	         //创建socket失败
	ERR_NET_SEND,				 //socket发送错误
	ERR_NET_CMD,				 //命令错误
	ERR_NET_PTS,				 //时间戳错误
	SUCCESS,				     //正常执行
};


/**
 * @brief KmboxNet类 - 用于控制键鼠盒子的网络通信
 */
class KmboxNet {
public:
    /**
     * @brief 构造函数
     * @param ip 盒子的IP地址
     * @param port 通信端口号
     * @param mac 盒子的MAC地址
     * @throw std::runtime_error 如果参数无效
     */
    KmboxNet(std::string ip, std::string port, std::string mac) {
        if (ip.empty() || port.empty() || mac.empty()) {
            throw std::runtime_error("Invalid parameters");
        }
        this->ip = ip;
        this->port = port;
        this->mac = mac;
        init_data();
    }
    
    /**
     * @brief 重新设置参数
     * @param ip 盒子的IP地址
     * @param port 通信端口号
     * @param mac 盒子的MAC地址
     * @throw std::runtime_error 如果参数无效
     */
    void reset(std::string ip, std::string port, std::string mac) {
        std::lock_guard<std::mutex> lock(mutex);
        if (ip.empty() || port.empty() || mac.empty()) {
            throw std::runtime_error("Invalid parameters");
        }
        this->ip = ip;
        this->port = port;
        this->mac = mac;
        init_data();
    }

    /**
     * @brief 连接设备
     * @return 连接结果
     */
    ConResult connect() {
        std::lock_guard<std::mutex> lock(mutex);
        
        // 初始化socket
        if (!init_socket()) {
            return ConResult::ERR_CREAT_SOCKET;
        }

        // 设置发送数据
        send_data.head.mac = str2hex(mac, 4);
        send_data.head.rand = rand();
        send_data.head.indexpts = 0;
        send_data.head.cmd = to_value(KmboxNetCommand::CONNECT);

        // 生成密钥
        generate_secret_key();

        // 发送数据
        if (!send_and_receive(sizeof(cmd_head_t))) {
            cleanup_socket();
            return ConResult::ERR_NET_SEND;
        }

        return handle_return(&recv_data, &send_data);
    }

    /**
     * @brief 鼠标移动
     * @param x X轴移动距离
     * @param y Y轴移动距离
     * @return 操作结果
     */
    ConResult mouse_move(int x, int y) {
        std::lock_guard<std::mutex> lock(mutex);
        if (!check_connection()) {
            return ConResult::ERR_NET_SEND;
        }

        // 设置鼠标数据
        mouse_data.x = x;
        mouse_data.y = y;

        // 设置发送数据
        prepare_mouse_data(KmboxNetCommand::MOUSE_MOVE);

        // 发送数据
        bool success = send_and_receive(sizeof(cmd_head_t) + sizeof(soft_mouse_t));

        return success ? handle_return(&recv_data, &send_data) : ConResult::ERR_NET_SEND;
    }

    /**
     * @brief 键盘按下
     * @param key 按键枚举值
     * @return 操作结果
     */
    ConResult keyboard_down(KeyboardTable key) {
        std::lock_guard<std::mutex> lock(mutex);
        if (!check_connection()) {
            return ConResult::ERR_NET_SEND;
        }

        // 处理按键数据
        if (is_control_key(key)) {
            handle_control_key_down(key);
        } else {
            keyboard_data.button[0] = to_value(key);
        }

        // 设置发送数据
        prepare_keyboard_data();

        return send_and_receive(sizeof(cmd_head_t) + sizeof(soft_keyboard_t)) 
            ? handle_return(&recv_data, &send_data) 
            : ConResult::ERR_NET_SEND;
    }

    /**
     * @brief 键盘抬起
     * @param key 按键枚举值
     * @return 操作结果
     */
    ConResult keyboard_up(KeyboardTable key) {
        std::lock_guard<std::mutex> lock(mutex);
        if (!check_connection()) {
            return ConResult::ERR_NET_SEND;
        }

        // 处理按键数据
        if (is_control_key(key)) {
            handle_control_key_up(key);
        } else {
            keyboard_data.button[0] = to_value(KeyboardTable::NONE);
        }

        // 设置发送数据
        prepare_keyboard_data();

        return send_and_receive(sizeof(cmd_head_t) + sizeof(soft_keyboard_t)) 
            ? handle_return(&recv_data, &send_data) 
            : ConResult::ERR_NET_SEND;
    }

    /**
     * @brief 键盘点击
     * @param key 按键枚举值
     * @param delay 延迟时间(ms)
     * @return 操作结果
     */
    ConResult Keyboard_click(KeyboardTable key, int delay = 0) {
        ConResult ret = keyboard_down(key);
        if (ret != ConResult::SUCCESS) {
            return ret;
        }
        if (delay > 0) {
            Sleep(delay);
        }
        return keyboard_up(key);
    }

    /**
     * @brief 断开连接
     * @return 操作结果
     */
    ConResult disconnect() {
        std::lock_guard<std::mutex> lock(mutex);
        if (!check_connection()) {
            return ConResult::ERR_NET_SEND;
        }
        cleanup_socket();
        return ConResult::SUCCESS;
    }

    // 析构函数
    ~KmboxNet() {
        cleanup_socket();
    }

private:
    std::mutex mutex;
    std::string ip;
    std::string port;
    std::string mac;
    std::array<unsigned char, 16> secret_key{};
    SOCKET io_socket{0};
    SOCKADDR_IN addr_info{};
    client_tx send_data{};
    client_tx recv_data{};
    soft_mouse_t mouse_data{};
    soft_keyboard_t keyboard_data{};

    /**
     * @brief 初始化数据
     */
    void init_data() {
        memset(&mouse_data, 0, sizeof(mouse_data));
        memset(&keyboard_data, 0, sizeof(keyboard_data));
        memset(&send_data, 0, sizeof(send_data));
        memset(&recv_data, 0, sizeof(recv_data));
    }

    /**
     * @brief 初始化socket
     * @return 是否成功
     */
    bool init_socket() {
        WSADATA info;
        if (WSAStartup(MAKEWORD(1, 1), &info) != 0) {
            return false;
        }
        if (LOBYTE(info.wVersion) != 1 || HIBYTE(info.wVersion) != 1) {
            WSACleanup();
            return false;
        }

        io_socket = socket(AF_INET, SOCK_DGRAM, 0);
        addr_info.sin_family = AF_INET;
        addr_info.sin_port = htons(atoi(port.c_str()));
        addr_info.sin_addr.S_un.S_addr = inet_addr(ip.c_str());

        // 设置超时
        int timeout = 1000;
        setsockopt(io_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
        setsockopt(io_socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));

        return true;
    }

    /**
     * @brief 清理socket
     */
    void cleanup_socket() {
        if (io_socket != 0) {
            closesocket(io_socket);
            io_socket = 0;
            WSACleanup();
        }
    }

    /**
     * @brief 生成密钥
     */
    void generate_secret_key() {
        unsigned int mac_value = str2hex(mac, 4);
        secret_key[0] = mac_value >> 24;
        secret_key[1] = mac_value >> 16;
        secret_key[2] = mac_value >> 8;
        secret_key[3] = mac_value;
    }

    /**
     * @brief 检查连接状态
     * @return 是否已连接
     */
    bool check_connection() const {
        return io_socket != 0;
    }

    /**
     * @brief 判断是否为控制键
     * @param key 按键枚举值
     * @return 是否为控制键
     */
    bool is_control_key(KeyboardTable key) const {
        return to_value(key) >= to_value(KeyboardTable::LEFTCONTROL) 
            && to_value(key) <= to_value(KeyboardTable::RIGHT_GUI);
    }

    /**
     * @brief 处理控制键按下
     * @param key 控制键
     */
    void handle_control_key_down(KeyboardTable key) {
        switch (key) {
            case KeyboardTable::LEFTCONTROL:  keyboard_data.ctrl |= BIT0; break;
            case KeyboardTable::RIGHTCONTROL: keyboard_data.ctrl |= BIT4; break;
            case KeyboardTable::LEFTSHIFT:    keyboard_data.ctrl |= BIT1; break;
            case KeyboardTable::RIGHTSHIFT:   keyboard_data.ctrl |= BIT5; break;
            case KeyboardTable::LEFTALT:      keyboard_data.ctrl |= BIT2; break;
            case KeyboardTable::RIGHTALT:     keyboard_data.ctrl |= BIT6; break;
            case KeyboardTable::LEFT_GUI:     keyboard_data.ctrl |= BIT3; break;
            case KeyboardTable::RIGHT_GUI:    keyboard_data.ctrl |= BIT7; break;
            default: break;
        }
    }

    /**
     * @brief 处理控制键抬起
     * @param key 控制键
     */
    void handle_control_key_up(KeyboardTable key) {
        switch (key) {
            case KeyboardTable::LEFTCONTROL:  keyboard_data.ctrl &= ~BIT0; break;
            case KeyboardTable::RIGHTCONTROL: keyboard_data.ctrl &= ~BIT4; break;
            case KeyboardTable::LEFTSHIFT:    keyboard_data.ctrl &= ~BIT1; break;
            case KeyboardTable::RIGHTSHIFT:   keyboard_data.ctrl &= ~BIT5; break;
            case KeyboardTable::LEFTALT:      keyboard_data.ctrl &= ~BIT2; break;
            case KeyboardTable::RIGHTALT:     keyboard_data.ctrl &= ~BIT6; break;
            case KeyboardTable::LEFT_GUI:     keyboard_data.ctrl &= ~BIT3; break;
            case KeyboardTable::RIGHT_GUI:    keyboard_data.ctrl &= ~BIT7; break;
            default: break;
        }
    }

    /**
     * @brief 准备鼠标数据
     * @param cmd 命令
     */
    void prepare_mouse_data(KmboxNetCommand cmd) {
        send_data.head.cmd = to_value(cmd);
        send_data.cmd_mouse = mouse_data;
        send_data.head.indexpts++;
        send_data.head.rand = rand();
    }

    /**
     * @brief 准备键盘数据
     */
    void prepare_keyboard_data() {
        send_data.head.cmd = to_value(KmboxNetCommand::KEYBOARD_ALL);
        send_data.cmd_keyboard = keyboard_data;
        send_data.head.indexpts++;
        send_data.head.rand = rand();
    }

    /**
     * @brief 发送和接收数据
     * @param send_size 发送数据大小
     * @return 是否成功
     */
    bool send_and_receive(size_t send_size) {
        // 发送数据
        int err = sendto(io_socket, (char*)&send_data, send_size, 0, 
                        (SOCKADDR*)&addr_info, sizeof(addr_info));
        if (err == SOCKET_ERROR) {
            return false;
        }

        // 接收数据
        int from_len = sizeof(addr_info);
        err = recvfrom(io_socket, (char*)&recv_data, 1024, 0, 
                      (SOCKADDR*)&addr_info, &from_len);
        return err != SOCKET_ERROR;
    }

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

