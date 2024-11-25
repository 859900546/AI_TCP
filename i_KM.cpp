#include "i_KM.h"
#include "hid.h"
#include "cmd.h"
#include "key.h"
#include <mmsystem.h>
#include <windows.h>
#pragma comment(lib,"winmm.lib")

#define aVendorID 0x1c1f
#define aProductID 0xc18a

WORD vendor_id=0;
WORD product_id=0;
hid_device *handle=NULL;
DWORD MAX_LENGTH=0;
BOOL using_encrypt=FALSE;
BYTE key[11];

void make_encrtpy(BYTE* dat){
	if(using_encrypt){
		volatile int i;
		for(i=0;i<11;++i){
			dat[i]^=key[i];
		}
	}
}
UINT32 MAKE32(BYTE* dat){
	return (((UINT32)*(dat))<<24)|(((UINT32)*(dat+1))<<16)|(((UINT32)*(dat+2))<<8)|*(dat+3);
}
BOOL open_device()
{
	if(vendor_id||product_id){
		handle=hid_open(vendor_id, product_id, NULL);
	}else{
		handle=my_hid_open();
	}
	if(handle){
		MAX_LENGTH=handle->output_report_length-1;
		hid_set_nonblocking(handle,1);
	}
	return handle!=NULL;
}

int read_data_timeout(BYTE* buf,int timeout)
{
	if(!handle) return -1;
	BYTE buff[64];
	int ret=hid_read_timeout(handle,buff,64,timeout);
	if(ret<0){
		if(handle){
			hid_close(handle);
			handle=NULL;
		}
	}else if(ret>0){
		if(buff[0]==READ_REPORT_ID){
			memcpy(buf,buff+1,ret-1);
			make_encrtpy(buf+1);
			ret-=1;
		}
	}
	return ret;
}
int write_data(BYTE *dat, DWORD length)
{
	if(!handle) return -1;
	if(length>MAX_LENGTH){
		return -2;
	}
	BYTE buf[64];
	buf[0]=WRITE_REPORT_ID;
	memcpy(buf+1,dat,length);
	make_encrtpy(buf+2);
	length=length+1;
	int ret=hid_write(handle,buf,length);
	if(ret<0){
		return -2;
	}
	return 1;
}
int write_cmd(BYTE cmd, BYTE *dat, DWORD length)
{
	if(length>MAX_LENGTH-2) return -2;
	BYTE buf[63];
	buf[0]=(length+1)&0xff;
	buf[1]=cmd;
	if(length>0)
		memcpy(buf+2,dat,length);
	return write_data(buf,length+2);
}

i_KM::i_KM(){
	using_encrypt=FALSE;
	wait_respon=FALSE;
	memset(key,0,sizeof(key));
	hid_init();
}
i_KM::~i_KM(){
	Close();
}
void i_KM::MoveR(int x, int y)
{
	this->MoveRD(x, y, 1, 2);
}
void i_KM::MoveRP(int x, int y)
{
	MoveRD(x,y,1,1);
}
void i_KM::Left_click()
{
	LeftClick(2, 8);
}
void i_KM::Right_click()
{
	RightClick(2, 8);
}
void i_KM::Key_press(int key)
{
	this->KeyPressVirtualCode(key);
}
void i_KM::Key_down(int key)
{
	this->KeyDownVirtualCode(key);
}
void i_KM::Key_up(int key)
{
	KeyUpVirtualCode(key);
}
void i_KM::Close(void)
{
	if(handle){
		hid_close(handle);
		handle=NULL;
	}
	hid_exit();
}
void i_KM::SetWaitRespon(BOOL wait)
{
	wait_respon=wait;
	BYTE cmd[1];
	cmd[0]=wait;
	write_cmd(SET_SYNCMODE,cmd,1);
	BYTE buf[64];
	int count=100;
	do{
		int len=read_data_timeout(buf,10);
		count--;
		if(count<0){
			break;
		}
		if(len>0){
			if(buf[1]!=RESPON_OK){
				continue;
			}
			break;
		}
		if(!handle){
			break;
		}
	}while(true);
}
void i_KM::Reboot(void)
{
	write_cmd(REBOOT,NULL,0);
	BYTE buf[64];
	do{
		int len=read_data_timeout(buf,100);
		if(len>0){
			if(buf[1]!=RESPON_OK){
				continue;
			}
			break;
		}
		if(!handle){
			break;
		}
	}while(true);
}
BOOL i_KM::IsOpen(void)
{
	return handle!=NULL;
}
BOOL i_KM::OpenDevice(void)
{
	vendor_id=aVendorID;
	product_id=aProductID;
	return open_device();
}

BOOL i_KM::OpenDeviceByID(SHORT vid, SHORT pid)
{
	vendor_id=vid;
	product_id=pid;
	return open_device();
}

SHORT i_KM::GetVID() const
{
	if(handle){
		return handle->vendor_id;
	}
	return 0;
}
SHORT i_KM::GetPID() const
{
	if(handle){
		return handle->product_id;
	}
	return 0;
}
BYTE i_KM::GetVersion(void)
{
	if(!handle)
		return 0;
	else{
		return handle->version;
	}
	return 0;
}

BYTE i_KM::GetModel(void)
{
	if(!handle)
		return 0;
	else{
		return handle->model;
	}
}

LONG i_KM::GetChipID(void)
{
	if(!handle)
		return 0;
	else{
		write_cmd(GET_ID,NULL,0);
		BYTE buf[64];
		long long_result;
		do{
			int len=read_data_timeout(buf,100);
			if(len>0){
				if(buf[1]!=RESPON_GETID){
					continue;
				}
				memcpy(&long_result,buf+2,4);
				long_result+=113666;
				return long_result;
			}else if(len<0){
				break;
			}
		}while(true);
	}
	return 0xffff;
}


LONG i_KM::GetStorageSize(void)
{
	if(!handle){
		return 0;
	}else{
		write_cmd(GET_STORAGE,NULL,0);
		BYTE buf[64];
		long long_result;
		do{
			int len=read_data_timeout(buf,100);
			if(len>0){
				if(buf[1]!=RESPON_STORAGE){
					continue;
				}
				long_result=MAKE32(buf+2);
				long_result-=10;
				return long_result;
			}else if(len<0){
				break;
			}
		}while(true);
	}
	return -1;
}

void i_KM::DelayRandom(int min,int max)
{
	int delay=0;
	if(max>=min&&max>0&&min>=0){
		delay=rand()%(max-min+1)+min;
	}else if(max==0&&min>0){
		delay=min;
	}
	if(delay>0){
		Sleep(delay);
	}
}
void i_KM::EnableAccurateDelay()
{
	
}

void i_KM::RunScript(BYTE mode,BYTE index)
{
	BYTE cmd[2];
	cmd[0]=mode;
	cmd[1]=index;
	write_cmd(RUN_SCRIPT,cmd,2);
}

void i_KM::mouse_event(BYTE e,SHORT x, SHORT y, BYTE extra1, BYTE extra2)
{
	BYTE cmd[12];
	memset(cmd,0,12);
	cmd[0]=e;
	if(e>=1&&e<=7){
		write_cmd(MOVEEVENT,cmd,5);
	}else if(e==8){
		if(x<0) x=0;
		if(y<0) y=0;
		int screenx=GetSystemMetrics(SM_CXSCREEN);
		int screeny=GetSystemMetrics(SM_CYSCREEN);
		if(x>=screenx) x=screenx-1;
		if(y>=screeny) y=screeny-1;
		long ax=x,ay=y;

		ax=(ax<<15)/screenx;
		ay=(ay<<15)/screeny;
		cmd[1]=(BYTE)((ax&0xff00)>>8);
		cmd[2]=ax&0xff;
		cmd[3]=(BYTE)((ay&0xff00)>>8);
		cmd[4]=ay&0xff;
		write_cmd(MOVEEVENT,cmd,5);
	}else if(e==9){
		if(x<-128||x>127||y<-128||y>127) return;
		cmd[1]=(BYTE)x;
		cmd[2]=(BYTE)y;
		write_cmd(MOVEEVENT,cmd,5);
	}else if(e==91){
		if(x<-32768||x>32767||y<-32768||y>32767) return;
		cmd[1]=(BYTE)((x&0xff00)>>8);
		cmd[2]=x&0xff;
		cmd[3]=(BYTE)((y&0xff00)>>8);
		cmd[4]=y&0xff;
		write_cmd(MOVEEVENT,cmd,5);
	}else if(e==10){
		if(x<-128||x>127) return;
		cmd[1]=(BYTE)x;
		write_cmd(MOVEEVENT,cmd,5);
	}else if(e==11){
		if(x<0) x=0;
		if(y<0) y=0;
		
		cmd[1]=(BYTE)((x&0xff00)>>8);
		cmd[2]=x&0xff;
		cmd[3]=(BYTE)((y&0xff00)>>8);
		cmd[4]=y&0xff;
		int screenx=GetSystemMetrics(SM_CXSCREEN);
		int screeny=GetSystemMetrics(SM_CYSCREEN);
		cmd[5]=(BYTE)((screenx&0xff00)>>8);
		cmd[6]=screenx&0xff;
		cmd[7]=(BYTE)((screeny&0xff00)>>8);
		cmd[8]=screeny&0xff;
		cmd[9]=extra1;
		cmd[10]=extra2;
		write_cmd(MOVEEVENT,cmd,12);
	}else if(e==12){
		
		cmd[1]=(BYTE)((x&0xff00)>>8);
		cmd[2]=x&0xff;
		cmd[3]=(BYTE)((y&0xff00)>>8);
		cmd[4]=y&0xff;
		int screenx=GetSystemMetrics(SM_CXSCREEN);
		int screeny=GetSystemMetrics(SM_CYSCREEN);
		cmd[5]=(BYTE)((screenx&0xff00)>>8);
		cmd[6]=screenx&0xff;
		cmd[7]=(BYTE)((screeny&0xff00)>>8);
		cmd[8]=screeny&0xff;
		cmd[9]=extra1;
		cmd[10]=extra2;
		write_cmd(MOVEEVENT,cmd,12);
	}else if(e==13||e==14){
		cmd[1]=x;
		write_cmd(MOVEEVENT,cmd,5);
	}
	if(wait_respon){
		do{
			BYTE buf[64];
			int len=read_data_timeout(buf,100);
			if(len>0){
				if(buf[1]!=RESPON_EVENT){
					continue;
				}
				break;
			}else if(len<0){
				break;
			}
		}while(true);
	}
}
void i_KM::LeftDown()
{
	mouse_event(1,0,0);
}
void i_KM::LeftUp()
{
	mouse_event(2,0,0);
}
void i_KM::LeftClick(int min,int max)
{
	LeftDown();
	DelayRandom(min,max);
	LeftUp();
}
void i_KM::LeftDoubleClick(int min,int max)
{
	LeftDown();
	DelayRandom(min,max);
	LeftUp();
	DelayRandom(min,max);
	LeftDown();
	DelayRandom(min,max);
	LeftUp();
}
void i_KM::RightDown()
{
	mouse_event(3,0,0);
}
void i_KM::RightUp()
{
	mouse_event(4,0,0);
}
void i_KM::RightClick(int min,int max)
{
	RightDown();
	DelayRandom(min,max);
	RightUp();
}
void i_KM::MiddleDown()
{
	mouse_event(5,0,0);
}
void i_KM::MiddleUp()
{
	mouse_event(6,0,0);
}
void i_KM::MiddleClick(int min,int max)
{
	MiddleDown();
	DelayRandom(min,max);
	MiddleUp();
}
void i_KM::MouseButtonDown(BYTE index)
{
	if(index<1||index>8) return;
	index-=1;
	mouse_event(13,index,0);
}
void i_KM::MouseButtonUp(BYTE index)
{
	if(index<1||index>8) return;
	index-=1;
	mouse_event(14,index,0);
}
void i_KM::MouseButtonClick(BYTE index,int min,int max)
{
	MouseButtonDown(index);
	DelayRandom(min,max);
	MouseButtonUp(index);
}
void i_KM::MouseWheel(int delta)
{
	mouse_event(10,delta,0);
}
void i_KM::MoveTo(USHORT x,USHORT y)
{
	mouse_event(8,x,y);
}
//void i_KM::MoveR(short x,short y)
//{
//	mouse_event(91,x,y);
//}
void i_KM::MoveD(USHORT x,USHORT y,BYTE delay,BYTE delta)
{
	mouse_event(11,x,y,delay,delta);
}
void i_KM::MoveRD(short dx,short dy,BYTE delay,BYTE delta)
{
	mouse_event(12,dx,dy,delay,delta);
}
void i_KM::MouseAllUp()
{
	mouse_event(7,0,0);
}

void i_KM::key_event(BYTE e,const char* key)
{
	BYTE cmd[2];
	cmd[0]=e;
	if(e==1||e==2){
		cmd[1]=getkeycode(key);
		write_cmd(KEYEVENT,cmd,2);
	}else if(e==3){
		cmd[1]=0;
		write_cmd(KEYEVENT,cmd,2);
	}
	if(wait_respon){
		do{
			BYTE buf[64];
			int len=read_data_timeout(buf,100);
			if(len>0){
				if(buf[1]!=RESPON_EVENT){
					continue;
				}
				break;
			}else if(len<0){
				break;
			}
		}while(true);
	}
}
void i_KM::KeyDownName(const char* keyname)
{
	key_event(1,keyname);
}
void i_KM::KeyUpName(const char* keyname)
{
	key_event(2,keyname);
}
void i_KM::KeyPressName(const char* keyname,int min,int max)
{
	KeyDownName(keyname);
	DelayRandom(min,max);
	KeyUpName(keyname);
}
void i_KM::KeyDownCode(BYTE keycode)
{
	BYTE cmd[2];
	cmd[0]=1;
	cmd[1]=keycode;
	write_cmd(KEYEVENT,cmd,2);
	if(wait_respon){
		do{
			BYTE buf[64];
			int len=read_data_timeout(buf,100);
			if(len>0){
				if(buf[1]!=RESPON_EVENT){
					continue;
				}
				break;
			}else if(len<0){
				break;
			}
		}while(true);
	}
}
void i_KM::KeyUpCode(BYTE keycode)
{
	BYTE cmd[2];
	cmd[0]=2;
	cmd[1]=keycode;
	write_cmd(KEYEVENT,cmd,2);
	if(wait_respon){
		do{
			BYTE buf[64];
			int len=read_data_timeout(buf,100);
			if(len>0){
				if(buf[1]!=RESPON_EVENT){
					continue;
				}
				break;
			}else if(len<0){
				break;
			}
		}while(true);
	}
}
void i_KM::KeyPressCode(BYTE keycode,int min,int max)
{
	KeyDownCode(keycode);
	DelayRandom(min,max);
	KeyUpCode(keycode);
}
void i_KM::KeyDownVirtualCode(BYTE keycode)
{
	BYTE cmd[2];
	cmd[0]=1;
	cmd[1]=getKeyCodeByVirtualCode(keycode);
	write_cmd(KEYEVENT,cmd,2);
	if(wait_respon){
		do{
			BYTE buf[64];
			int len=read_data_timeout(buf,100);
			if(len>0){
				if(buf[1]!=RESPON_EVENT){
					continue;
				}
				break;
			}else if(len<0){
				break;
			}
		}while(true);
	}
}
void i_KM::KeyUpVirtualCode(BYTE keycode)
{
	BYTE cmd[2];
	cmd[0]=2;
	cmd[1]=getKeyCodeByVirtualCode(keycode);
	write_cmd(KEYEVENT,cmd,2);
	if(wait_respon){
		do{
			BYTE buf[64];
			int len=read_data_timeout(buf,100);
			if(len>0){
				if(buf[1]!=RESPON_EVENT){
					continue;
				}
				break;
			}else if(len<0){
				break;
			}
		}while(true);
	}
}
void i_KM::KeyPressVirtualCode(BYTE vcode,int min,int max)
{
	BYTE keycode=getKeyCodeByVirtualCode(vcode);
	KeyDownCode(keycode);
	DelayRandom(min,max);
	KeyUpCode(keycode);
}
const char k1[49]="`1234567890-=qwertyuiop[]\\asdfghjkl;'zxcvbnm,./ ";
const char k2[48]="~!@#$%^&*()_+QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>?";
BOOL CanInput(char c)
{
	int i;
	for(i=0;i<=48;++i){
		if(k1[i]==c){
			return TRUE;
		}
	}
	for(i=0;i<=47;++i){
		if(k2[i]==c){
			return TRUE;
		}
	}
	return FALSE;
}
BOOL needShift(char c)
{
	for(int i=0;i<=47;++i){
		if(k2[i]==c){
			return TRUE;
		}
	}
	return FALSE;
}
char getUnShiftKey(char c)
{
	for(int i=0;i<=47;++i){
		if(k2[i]==c){
			return k1[i];
		}
	}
	return 0;
}
void i_KM::SayString(const char* s,int min,int max)
{
	int len=strlen(s),i;
	BOOL shift=FALSE;
	for(i=0;i<len;++i){
		char c=s[i];
		BOOL need_shift=needShift(c);
		std::string keyname;
		keyname=c;
		if(need_shift){
			keyname=getUnShiftKey(c);
		}
		if(need_shift&&!shift){
			KeyDownCode(KEY_LEFT_SHIFT);
			DelayRandom(min,max);
			shift=TRUE;
		}

		KeyPressName(keyname.c_str(),min,max);
		DelayRandom(min,max);
		if(i==len-1){
			need_shift=FALSE;
		}else{
			need_shift=needShift(s[i+1]);
		}

		if(!need_shift&&shift){
			KeyUpCode(KEY_LEFT_SHIFT);
			DelayRandom(min,max);
			shift=FALSE;
		}
	}
}
void i_KM::SayStringAnsi(const char* s,int min,int max)
{
	int len=strlen(s);
	for(int i=0;i<len;){
		BYTE c=s[i];
		int code;
		if(c<128){
			code=c;
			i+=1;
		}else if(i<len-1){
			code=c;
			code=code*256+(BYTE)s[i+1];
			i+=2;
		}
		char temp[15];
		int temp_len=sprintf_s(temp,"%d",code);
		KeyDownCode(KEY_LEFT_ALT);
		DelayRandom(min,max);
		for(int j=0;j<temp_len;++j){
			BYTE c2=temp[j]-'0';
			if(c2==0) c2=10;
			BYTE keycode=KEYPAD_1+c2-1;
			KeyDownCode(keycode);
			DelayRandom(min,max);
			KeyUpCode(keycode);
			DelayRandom(min,max);
		}
		KeyUpCode(KEY_LEFT_ALT);
	}
}
int ConvertANSIToUnicode(const char* strANSI,WCHAR **p_utf8)
{
	int nLen = ::MultiByteToWideChar(CP_ACP,MB_ERR_INVALID_CHARS,(LPCCH)strANSI,-1,NULL,0);
	if(nLen>0)
	{
		WCHAR * wszUTF_8 = new WCHAR[nLen+1];
		memset(wszUTF_8, 0, nLen * 2 + 2);
		nLen = MultiByteToWideChar(CP_ACP, 0, (LPCCH)strANSI, -1, wszUTF_8, nLen);
		*p_utf8=new WCHAR[nLen+1];
		memcpy(*p_utf8,wszUTF_8,sizeof(WCHAR)*(nLen+1));
		delete[] wszUTF_8;
		return nLen;
	}else{
		*p_utf8=NULL;
		return 0;
	}
}
void i_KM::SayStringUnicode(const char* s,int min,int max)
{
	WCHAR *uni_str;
	int len=ConvertANSIToUnicode(s,&uni_str);
	for(int i=0;i<len;++i){
		int code=(int)uni_str[i];
		char temp[15];
		int temp_len=sprintf_s(temp,"%d",code);
		KeyDownCode(KEY_LEFT_ALT);
		DelayRandom(min,max);
		for(int j=0;j<temp_len;++j){
			BYTE c2=temp[j]-'0';
			if(c2==0) c2=10;
			BYTE keycode=KEYPAD_1+c2-1;
			KeyDownCode(keycode);
			DelayRandom(min,max);
			KeyUpCode(keycode);
			DelayRandom(min,max);
		}
		KeyUpCode(KEY_LEFT_ALT);
	}
}

BYTE i_KM::SetVidPid(USHORT vid, USHORT pid)
{
	if(!handle) return 2;
	BYTE cmd[8];
	cmd[0]=0;
	cmd[1]=0;
	cmd[2]=0;
	cmd[3]=4;
	cmd[4]=vid&0xff;
	cmd[5]=(vid&0xff00)>>8;
	cmd[6]=pid&0xff;
	cmd[7]=(pid&0xff00)>>8;
	if(write_cmd(SET_FLASH,cmd,8)==-1) return 1;
	BYTE buf[64];
	do{
		int len=read_data_timeout(buf,100);
		if(len>0){
			if(buf[1]!=RESPON_SET_FLASH){
				continue;
			}
			return 0;
		}
	}while(true);
	return 1;
}

BYTE i_KM::SetConfigData(int index, SHORT data)
{
	if(!handle) return 2;
	if(index<0||index>=252) return 2;
	BYTE cmd[6];
	int addr=index*2+8;

	cmd[0]=(addr>>24)&0xff;
	cmd[1]=(addr>>16)&0xff;
	cmd[2]=(addr>>8)&0xff;
	cmd[3]=(addr>>0)&0xff;
	cmd[4]=(data&0xff00)>>8;
	cmd[5]=data&0xff;
	if(write_cmd(SET_FLASH,cmd,6)==-1) return 1;
	BYTE buf[64];
	do{
		int len=read_data_timeout(buf,100);
		if(len>0){
			if(buf[1]!=RESPON_SET_FLASH){
				continue;
			}
			return 0;
		}
	}while(true);
	return 1;
}

SHORT i_KM::GetConfigData(int index)
{
	if(!handle) return -1;
	if(index<0||index>=252) return -2;
	BYTE cmd[5];
	int addr=index*2+8;
	
	cmd[0]=(addr>>24)&0xff;
	cmd[1]=(addr>>16)&0xff;
	cmd[2]=(addr>>8)&0xff;
	cmd[3]=(addr>>0)&0xff;
	cmd[4]=2;
	short short_result=0x7ffd;
	if(write_cmd(GET_FLASH,cmd,5)==-1) return -1;
	BYTE buf[64];
	do{
		int len=read_data_timeout(buf,100);
		if(len>0){
			if(buf[1]!=RESPON_GET_FLASH){
				continue;
			}
			short_result=short(buf[2])<<8|buf[3];
			return short_result;
		}
	}while(true);
	return short_result;
}
BYTE i_KM::SetEncrypt(BYTE *dat)
{
	if(!handle) return -1;
	if(dat){
		if(write_cmd(SET_ENCRYPT,dat,11)==-1) return -1;
		using_encrypt=TRUE;
		memcpy(key,dat,11);
	}else{
		BYTE dat[13];
		memset(dat,0,13);
		if(write_cmd(CLEAR_ENCRYPT,dat,13)==-1) return -1;
		using_encrypt=FALSE;
	}
	BYTE buf[64];
	do{
		int len=read_data_timeout(buf,100);
		if(len>0){
			if(buf[1]!=RESPON_OK){
				continue;
			}
			return 1;
		}
	}while(true);
	return 0;
}
BYTE i_KM::SetLed(BOOL on)
{
	if(!handle) return -1;
	BYTE dat[1];
	dat[0]=on!=0;
	if(write_cmd(SET_LED,dat,1)==-1) return -1;
	BYTE buf[64];
	do{
		int len=read_data_timeout(buf,100);
		if(len>0){
			if(buf[1]!=RESPON_OK){
				continue;
			}
			return 1;
		}
	}while(true);
	return 0;
}
BYTE i_KM::Lock_Mouse(BYTE option)
{
	if(!handle) return -1;
	BYTE dat[1];
	dat[0]=option;
	if(write_cmd(LOCK_MOUSE,dat,1)==-1) return -1;
	BYTE buf[64];
	do{
		int len=read_data_timeout(buf,100);
		if(len>0){
			if(buf[1]!=RESPON_OK){
				continue;
			}
			return 1;
		}
	}while(true);
	return 0;
}
BYTE i_KM::Notify_Mouse(BYTE option)
{
	if(!handle) return -1;
	BYTE dat[1];
	dat[0]=option;
	if(write_cmd(ENABLE_MOUSE_NOTIFY,dat,1)==-1) return -1;
	BYTE buf[64];
	do{
		int len=read_data_timeout(buf,100);
		if(len>0){
			if(buf[1]!=RESPON_OK){
				continue;
			}
			return 1;
		}
	}while(true);
	return 0;
}
BYTE i_KM::Lock_KeyBoard(BYTE option)
{
	if(!handle) return -1;
	BYTE dat[1];
	dat[0]=option;
	if(write_cmd(LOCK_KEYBOARD,dat,1)==-1) return -1;
	BYTE buf[64];
	do{
		int len=read_data_timeout(buf,100);
		if(len>0){
			if(buf[1]!=RESPON_OK){
				continue;
			}
			return 1;
		}
	}while(true);
	return 0;
}
BYTE i_KM::Notify_KeyBoard(BYTE option)
{
	if(!handle) return -1;
	BYTE dat[1];
	dat[0]=option;
	if(write_cmd(ENABLE_KEYBOARD_NOTIFY,dat,1)==-1) return -1;
	BYTE buf[64];
	do{
		int len=read_data_timeout(buf,100);
		if(len>0){
			if(buf[1]!=RESPON_OK){
				continue;
			}
			return 1;
		}
	}while(true);
	return 0;
}
int i_KM::Read_Notify(BYTE* out,int timeout)
{
	BYTE buf[64];
read_notify:
	int len=read_data_timeout(buf,timeout);
	if(len>0){
		if(buf[1]!=NOTIFY_MOUSE){
			goto read_notify;
		}
		memcpy(out,buf+2,buf[0]-1);
		return buf[0]-1;
	}
	return 0;
}
int i_KM::GetKeyState(BYTE index)
{
	BYTE buf[64];
	buf[0]=index;
	if(write_cmd(GET_KEY_STATE,buf,1)==-1) return -1;
read_notify:
	int len=read_data_timeout(buf,-1);
	if(len>0){
		if(buf[1]!=RESPON_KEYSTATE){
			goto read_notify;
		}
		if(buf[2]!=index){
			goto read_notify;
		}
		return buf[3];
	}
	return 0;
}

void i_KM::Set_Freq(int time)
{
	if(!handle) return;
	BYTE dat[1];
	dat[0]=time;
	if(write_cmd(SET_FREQ,dat,1)==-1) return;
	BYTE buf[64];
	do{
		int len=read_data_timeout(buf,100);
		if(len>0){
			if(buf[1]!=RESPON_OK){
				continue;
			}
			return;
		}
	}while(true);
	return;
}
int i_KM::Get_Freq()
{
	if(!handle) return -1;
	if(write_cmd(GET_FREQ,NULL,0)==0) return -1;
	BYTE buf[64];
	do{
		int len=read_data_timeout(buf,100);
		if(len>0){
			if(buf[1]!=RESPON_FREQ){
				continue;
			}
			return buf[2];
		}
	}while(true);
	return -1;
}
