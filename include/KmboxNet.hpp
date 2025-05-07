#include "../include/table.hpp"

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
    KmboxNet外设功能
*/
class KmboxNet {
public:
    KmboxNet();
    ~KmboxNet();
private:

        
};

