#pragma once

#define     cmd_xbox_send       0x29e92210	//xbox����ָ��
HANDLE			hMutex_busy();				//��ȡ�����ź���
SOCKADDR_IN    *GetSocketAddress();			//��ȡ��ַ
SOCKET			GetSocketFd();				//socketͨ�ž��

typedef struct
{
	signed short x;
	signed short y;
}game_pad_t;

typedef struct
{
	char btnUp;		//��
	char btnRight;	//��
	char btnDown;	//��
	char btnLeft;	//��
	char btnStart;	//��ʼ
	char btnBack;	//����
	char btnL3;		//��ҡ��
	char btnR3;		//��ҡ��
	char btnA;		//A��
	char btnB;		//B��
	char btnX;		//X��
	char btnY;		//Y��
	char btnLB;		//LB��
	char btnRB;		//RB��
	char btnHome;	//home��
	game_pad_t left_joystick; //��ҡ��
	game_pad_t right_joystick;//��ҡ��
	unsigned char LT;//������
	unsigned char RT;//������
}soft_xbox_t;



typedef struct
{
	cmd_head_t head;
	union {
		cmd_data_t      u8buff;		  //buff
		cmd_u16_t       u16buff;	  //U16
		soft_xbox_t     soft_xbox;    // 
	};
}client_xbox;


/*************************xbox���API*****************************/
int Xbox_Free();			//�ͷ�������ƣ��ָ�Ӳ��ӳ��
int Xbox_UP(int isdown);	//�ֱ��ϼ�״̬���á�
int Xbox_Down(int isdown);	//�ֱ��¼�״̬���á�
int Xbox_Left(int isdown);	//�ֱ����״̬���á�
int Xbox_Right(int isdown); //�ֱ��Ҽ�״̬���á�
int Xbox_UP_Right(int isdown);//�ֱ����Ҽ�״̬����
int Xbox_Right_Down(int isdown);//�ֱ����¼�״̬����
int Xbox_Left_Down(int isdown);//�ֱ����¼�״̬����
int Xbox_Left_Up(int isdown);//�ֱ����ϼ�״̬����
int Xbox_Home(int isdown);	//�ֱ�home��״̬���á�
int Xbox_Back(int isdown);	//�ֱ�Back��״̬���á�
int Xbox_Start(int isdown); //�ֱ�Start��״̬���á�
int Xbox_L3(int isdown);	//�ֱ���ҡ�˰���״̬���á�
int Xbox_R3(int isdown);	//�ֱ���ҡ�˰���״̬���á�
int Xbox_A(int isdown);		//�ֱ�A��״̬���á�
int Xbox_B(int isdown);		//�ֱ�B��״̬���á�
int Xbox_X(int isdown);		//�ֱ�X��״̬���á�
int Xbox_Y(int isdown);		//�ֱ�Y��״̬���á�
int Xbox_LB(int isdown);	//�ֱ�LB��״̬���á�
int Xbox_RB(int isdown);	//�ֱ�RB��״̬���á�
int Xbox_Left_joystick(short x, short y);	//�ֱ���ҡ���������á�XYȡֵ��Χ[-32767,32767]
int Xbox_Right_joystick(short x, short y);	//�ֱ���ҡ���������á�XYȡֵ��Χ[-32767,32767]
int Xbox_LT(int value);		//�ֱ�LT��״̬���á�ȡֵ��Χ[0,255]
int Xbox_RT(int value);		//�ֱ�RT��״̬���á�ȡֵ��Χ[0,255]