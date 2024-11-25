#ifndef I_KM_H__
#define I_KM_H__

#include "cmd.h"
#include <windows.h>

#ifndef LONG
typedef long int LONG;
#endif
#ifndef SHORT
typedef short SHORT;
#endif
#ifndef USHORT
typedef unsigned short USHORT;
#endif
#ifndef BYTE
typedef unsigned char BYTE;
#endif
#ifndef BOOL
typedef int BOOL;
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

class i_KM
{
public:
	i_KM();
	~i_KM();
	void MoveR(int x, int y);
	void MoveRP(int x, int y);
	void Left_click();
	void Right_click();
	void Key_press(int key);
	void Key_down(int key);
	void Key_up(int key);

	BOOL IsOpen(void);//�豸�Ƿ�����
	BOOL OpenDevice(void);//����Ĭ��VID/PID���豸
	BOOL OpenDeviceByID(SHORT vid, SHORT pid);//��������VID/PID���豸
	SHORT GetVID() const;
	SHORT GetPID() const;
	void SetWaitRespon(BOOL wait);//�ú�����������������ָ����Ƿ�ȴ���ת���������أ�Ӧ�ò��ȴ������̿�ѡ�ȴ����߲��ȴ������ȴ�ʱ�豣֤���ε��ü������1����

	BYTE GetVersion(void);//��ȡ�̼��汾
	BYTE GetModel(void);//��ȡ�豸�ͺ�
	LONG GetChipID(void);//��ȡоƬ���к�
	LONG GetStorageSize(void);//��ȡ�洢�ռ��С
	void Close(void);//�ر��豸����
	void Reboot(void);//�����豸

	void DelayRandom(int min,int max);//�����ʱ�����ع���
	void EnableAccurateDelay();//�����ʱ��ȷ��
	
	void RunScript(BYTE mode,BYTE index);//�����ת����������
	//mode=1 �����ű���index���ڽű���ţ�0-99Ϊ�����꣬100-125�Ǹ߼��꣩
	//mode=2 ���⹦�ܡ�index=0��ͣ index=1���� index=2���� index=3ֹͣ index=4��ͣ���� index=5����ֹͣ
	
	void mouse_event(BYTE e,SHORT x, SHORT y, BYTE extra1=0, BYTE extra2=0);
	void LeftDown();//�������
	void LeftUp();//�������
	void LeftClick(int min=0,int max=0);//���������min��max���������ʱ����
	void LeftDoubleClick(int min=0,int max=0);//���˫����min��max���������ʱ����
	void RightDown();//�Ҽ�����
	void RightUp();//�Ҽ�����
	void RightClick(int min=0,int max=0);//�Ҽ�������min��max���������ʱ����
	void MiddleDown();//�м�����
	void MiddleUp();//�м�����
	void MiddleClick(int min=0,int max=0);//�м�������min��max���������ʱ����
	void MouseWheel(int delta);//�����֡�delta��Ҫ-128��127֮��
	void MouseButtonDown(BYTE index);//�������£�����1-8
	void MouseButtonUp(BYTE index);//�������𣬲���1-8
	void MouseButtonClick(BYTE index,int min=0,int max=0);//�������������1-8��min��max���������ʱ����
	void MouseAllUp();//����������м�
	void MoveTo(USHORT x,USHORT y);//�����������ƶ�
//	void MoveR(short x,short y);//�������ƶ�
	void MoveD(USHORT x,USHORT y,BYTE delay=8,BYTE delta=10);//�����켣ƽ���ƶ�
	void MoveRD(short dx,short dy,BYTE delay=8,BYTE delta=10);//�����켣ƽ������ƶ�

	void key_event(BYTE e,const char* key);
	void KeyDownName(const char* keyname);//�԰��������¼��̰���
	void KeyUpName(const char* keyname);//�԰�����������̰���
	void KeyPressName(const char* keyname,int min=0,int max=0);//�԰��������̰���
	void KeyDownCode(BYTE keycode);//��ɨ���밴�¼��̰���
	void KeyUpCode(BYTE keycode);//��ɨ���뵯����̰���
	void KeyPressCode(BYTE keycode,int min=0,int max=0);//��ɨ������̰���
	void KeyDownVirtualCode(BYTE keycode);//��������밴�¼��̰���
	void KeyUpVirtualCode(BYTE keycode);//��������뵯����̰���
	void KeyPressVirtualCode(BYTE keycode,int min=0,int max=0);//�����������̰���
	void SayString(const char* s,int min=0,int max=0);//�������֡���ĸ������
	void SayStringAnsi(const char* s,int min=0,int max=0);//����gbk������ַ���
	void SayStringUnicode(const char* s,int min=0,int max=0);//����unicode������ַ���

	BYTE SetVidPid(USHORT vid, USHORT pid);//����VID/PID
	

	//������ת����ר�ú��������������
	BYTE SetConfigData(int index, SHORT data);//���������������0-251
	SHORT GetConfigData(int index);//��ȡ���������ݣ����0-251

	BYTE SetEncrypt(BYTE *dat);//���ݱ������11�ֽڡ����dat����NULL���������
	BYTE SetLed(BOOL on);//�򿪡��رյ�

	/*	Lock_Mouse
	 *	���������豸�����Ĳ��ֹ���
	 *	option: LOCK_NONE         0x00
			 	LOCK_LEFT_BUTTON  0x01
				LOCK_RIGHT_BUTTON 0x02
				LOCK_WHEEL_BUTTON 0x04
				LOCK_ALL_BUTTON   0x07
				LOCK_X			  0x08
				LOCK_Y			  0x10
				LOCK_X_Y		  0x18
				LOCK_WHEEL		  0x20
				LOCK_ALL		  0x3F */
	BYTE Lock_Mouse(BYTE option);//ת����ר�ú���
	BYTE Lock_KeyBoard(BYTE option);//ת����ר�ú���
	/* Notify_Mouse
	 * ������궯��֪ͨ������Ӧ����ʱ����������
	 * option:	NOTIFY_NONE         0x00
				NOTIFY_LEFT_BUTTON  0x01
				NOTIFY_RIGHT_BUTTON 0x02
				NOTIFY_WHEEL_BUTTON 0x04
				NOTIFY_ALL_BUTTON   0x07
				NOTIFY_X			0x08
				NOTIFY_Y			0x10
				NOTIFY_X_Y		  	0x18
				NOTIFY_WHEEL		0x20
				NOTIFY_ALL		  	0x3F */
	BYTE Notify_Mouse(BYTE option);//ת����ר�ú���
	BYTE Notify_KeyBoard(BYTE option);//ת����ר�ú���
	int Read_Notify(BYTE* out,int timeout);//ת����ר�ú�������ȡNotify_Mouse���������ݡ�out�����豣֤5�ֽڴ�С��timeout=-1���������������������ȴ�ʱ��
	int GetKeyState(BYTE index);//ת����ר�ú�������ȡ��������İ���״̬��˲ʱ��ȡ������ʹ��ʱ��Ҫ�ر�Notify_Mouse��Notify_KeyBoard

	void Set_Freq(int percent);//����dpi�ٷֱ�
	int Get_Freq();//��ȡdpi�ٷֱ�
protected:
	BOOL wait_respon;
};

#endif