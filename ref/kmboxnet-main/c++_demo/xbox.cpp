#include "../c++_demo/NetConfig/kmboxNet.h"
#include "xbox.h"

soft_xbox_t xbox360;					//����ֱ���������
client_xbox tx;							//���͵�����
client_xbox rx;							//���յ�����
extern SOCKADDR_IN addrSrv;

unsigned int xbox_mac = 0;				//MAC������Ҫ��ȷ��������Ӳ�����ᡣ

/*����Ӧ����*/
int XboxAskRxHandle(client_xbox* rx, client_xbox* tx)		 //���յ�����
{	int ret = 0;
	if (rx->head.cmd != tx->head.cmd)
		ret = err_net_cmd;//���������
	if (rx->head.indexpts != tx->head.indexpts)
		ret = err_net_pts;//ʱ�������
	ReleaseMutex(hMutex_busy());
	return ret;				//û�д��󷵻�0
}
/*
��λ���а���Ϊ�ɿ�״̬��ҡ�˾��С�
*/
int Xbox_Free()
{	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.mac = xbox_mac;
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	memset(&xbox360, 0, sizeof(soft_xbox_t));
	memset(&(tx.soft_xbox), 0, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx,&tx);
}

/*
�ֱ��ϼ�״̬���á�
0���ɿ�
1������
*/
int Xbox_UP(int isdown)
{	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnUp = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}

/*
�ֱ��¼�״̬���á�
0���ɿ�
1������
*/
int Xbox_Down(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnDown = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}


/*
�ֱ����״̬���á�
0���ɿ�
1������
*/
int Xbox_Left(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnLeft = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}

/*
�ֱ��Ҽ�״̬���á�
0���ɿ�
1������
*/
int Xbox_Right(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnRight = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}

/*
�ֱ����Ҽ�״̬���á�
0���ɿ�
1������
*/
int Xbox_UP_Right(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnUp    = isdown;
	xbox360.btnRight = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}

/*
�ֱ����¼�״̬���á�
0���ɿ�
1������
*/
int Xbox_Right_Down(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnDown = isdown;
	xbox360.btnRight = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}

/*
�ֱ������״̬���á�
0���ɿ�
1������
*/
int Xbox_Left_Down(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnDown = isdown;
	xbox360.btnLeft = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}


/*
�ֱ����ϼ�״̬���á�
0���ɿ�
1������
*/
int Xbox_Left_Up(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnUp = isdown;
	xbox360.btnLeft = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}



/*
�ֱ�home��״̬���á�
0���ɿ�
1������
*/
int Xbox_Home(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnHome = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}


/*
�ֱ�Back��״̬���á�
0���ɿ�
1������
*/
int Xbox_Back(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnBack = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}



/*
�ֱ�Start��״̬���á�
0���ɿ�
1������
*/
int Xbox_Start(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnStart = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}


/*
�ֱ���ҡ�˰���״̬���á�
0���ɿ�
1������
*/
int Xbox_L3(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnL3 = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}

/*
�ֱ���ҡ�˰���״̬���á�
0���ɿ�
1������
*/
int Xbox_R3(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnR3 = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}



/*
�ֱ�A��״̬���á�
0���ɿ�
1������
*/
int Xbox_A(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnA = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}


/*
�ֱ�B��״̬���á�
0���ɿ�
1������
*/
int Xbox_B(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnB = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}

/*
�ֱ�X��״̬���á�
0���ɿ�
1������
*/
int Xbox_X(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnX = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}

/*
�ֱ�Y��״̬���á�
0���ɿ�
1������
*/
int Xbox_Y(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnY = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}

/*
�ֱ�LB��״̬���á�
0���ɿ�
1������
*/
int Xbox_LB(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnLB = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}


/*
�ֱ�RB��״̬���á�
0���ɿ�
1������
*/
int Xbox_RB(int isdown)
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.btnRB = isdown;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}



/*
�ֱ���ҡ���������á�
x:ҡ��x��ֵ
y:ҡ��y��ֵ
*/
int Xbox_Left_joystick(short x,short y)
{	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.left_joystick.x = x;
	xbox360.left_joystick.y = y;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}



/*
�ֱ���ҡ���������á�
x:ҡ��x��ֵ
y:ҡ��y��ֵ
*/
int Xbox_Right_joystick(short x, short y)
{	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.right_joystick.x = x;
	xbox360.right_joystick.y = y;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);
}

int Xbox_LT(int value)	//�ֱ�LT��״̬���á�ȡֵ��Χ[0,255]
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.LT= value;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);

}
int Xbox_RT(int value)	//�ֱ�LT��״̬���á�ȡֵ��Χ[0,255]
{
	int err;
	if (GetSocketFd() <= 0)		return err_creat_socket;
	WaitForSingleObject(hMutex_busy(), INFINITE);
	tx.head.indexpts++;				//ָ��ͳ��ֵ
	tx.head.cmd = cmd_xbox_send;	//ָ��
	tx.head.rand = rand();			//�������ֵ
	tx.head.mac = xbox_mac;
	int length = sizeof(cmd_head_t) + sizeof(soft_xbox_t);
	xbox360.RT = value;
	memcpy(&(tx.soft_xbox), &xbox360, sizeof(soft_xbox_t));
	sendto(GetSocketFd(), (const char*)&tx, length, 0, (struct sockaddr*)GetSocketAddress(), sizeof(SOCKADDR_IN));
	SOCKADDR_IN sclient;
	int clen = sizeof(sclient);
	err = recvfrom(GetSocketFd(), (char*)&rx, 1024, 0, (struct sockaddr*)&sclient, &clen);
	return XboxAskRxHandle(&rx, &tx);

}