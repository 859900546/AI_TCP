#pragma once
#ifndef _HKM_
#define _HKM_

#define HKM_FAIL		((DWORD)-1) //失败

//HKMSearchDevice、HKMSearchDevice2、HKMSearchDeviceAll参数
#define HDT_ALL		0 //全部模式
#define HDT_KM		1 //键鼠模式
#define HDT_KEY		2 //键盘模式
#define HDT_MOUSE	3 //鼠标模式

//HKMGetKeyboardMode返回值
#define HKT_NONE	0 //无键盘
#define HKT_NORM	1 //普通键盘
#define HKT_GAME	5 //游戏键盘

//HKMGetMouseMode返回值
#define HMT_NONE	0 //无鼠标
#define HMT_REL		1 //相对坐标鼠标
#define HMT_ABS		2 //绝对坐标鼠标
#define HMT_RA		3 //相对坐标鼠标+绝对坐标鼠标
#define HMT_GR		5 //相对坐标游戏鼠标
#define HMT_GRA		7 //相对坐标游戏鼠标+绝对坐标鼠标

//HKMSetMode参数
#define HDEMI_MOVE		0x02 //鼠标移动方式
#define HDEMI_WHEEL		0x03 //鼠标滚轮滚动方式
#define HDEMI_STR		0x04 //输出字符串编码
#define HDEMI_SYNC		0x05 //键盘、鼠标同步方式
#define HDEMI_RND		0x06 //延时随机方式
#define HDEMI_DELAY		0x07 //延时方式
#define HDEMI_FUN		0x20 //函数的参数或者返回值的字符串类型

#define HDEMM_DEFAULT	0x00 //默认鼠标移动方式
#define HDEMM_QUICK		0x01 //鼠标快速移动
#define HDEMM_ABS		0x02 //鼠标绝对鼠标移动
#define HDEMM_BASIC		0x04 //基础模式，只能在鼠标没使用提高精度且移动比例是1:1，win10的Dpi是100%时使用
#define HDEMM_SHIFT		0x00 //变速模式，开始加速，结束减速
#define HDEMM_UNIFORM	0x08 //匀速模式，距离短时会调低速度，仅HDEMM_DEFAULT和包含HDEMM_QUICK模式时，对HKME_MouseMove无效
#define HDEMM_ASSIGN	0x10 //定速模式，使用指定的速度，仅HDEMM_DEFAULT和包含HDEMM_QUICK模式时，对HKME_MouseMove无效
#define HDEWM_SIM		0x00 //鼠标滚轮仿真滚动
#define HDEWM_QUICK		0x01 //鼠标滚轮快速滚动
#define HDESM_ANSI		0x00 //输出字符串ANSI编码方式
#define HDESM_UNICODE	0x01 //输出字符串UNICODE编码方式
#define HDESM_ANSI2		0x02 //输出字符串ANSI编码2方式
#define HDESM_UNICODE2	0x03 //输出字符串UNICODE编码2方式
#define HDESM_CLP		0x04 //输出字符串剪贴板粘贴方式
#define HDEAM_SYNC		0x00 //键盘鼠标操作同步
#define HDEAM_KAS		0x01 //键盘操作异步
#define HDEAM_MAS		0x02 //鼠标操作异步
#define HDERM_UFT		0x00 //延时随机均匀分布
#define HDERM_LITTLE	0x01 //延时随机偏小分布
#define HDEDM_NORMAL	0x00 //不处理窗口消息
#define HDEDM_WNDMSG	0x01 //处理窗口消息
#define HDEFM_UNICODE	0x00 //函数的参数或者返回值的字符串使用UNICODE编码
#define HDEFM_ANSI		0x01 //函数的参数或者返回值的字符串使用ANSI编码

//HKMGetKeyboardLEDState参数
#define KBD_NUM_LOCK	0 //NumLock指示灯
#define KBD_CAPS_LOCK	1 //CapsLock指示灯
#define KBD_SCROLL_LOCK	2 //ScrollLock指示灯

//HKMIsMouseButtonDown参数
#define MSI_LBTN	0 //鼠标左键
#define MSI_RBTN	1 //鼠标右键
#define MSI_MBTN	2 //鼠标中键
#define MSI_XB1BTN	3 //鼠标XButton1键
#define MSI_XB2BTN	4 //鼠标XButton2键

//HKMGetDevInfo参数
#define DI_TYPE		0x01 //设备类型
#define DI_VERSION	0x02 //固件版本
#define DI_RUNTIME	0x03 //运行时间
#define DI_POTIME	0x04 //通电时间
#define DI_RSTTIMES	0x06 //复位次数
#define DI_RUNSTATE	0x07 //运行状态
#define DI_VID		0x08 //USB VID
#define DI_PID		0x09 //USB PID
#define DI_DEVVER	0x0A //USB设备版本

//HKMCheckPressedKeys参数
#define CKM_CHS			0x01 //检查按键，返回包含中文的信息
#define CKM_MOUSE		0x02 //检查按键，返回包含被按下的鼠标键信息
#define CKM_ANSI		0x04 //检查按键，返回的字符串使用ANSI编码

//DPI模式,HKMOpen和HKMOpen2参数
#define DPIM_PHYSICAL	0 //每个显示器DPI感知，windows系统从win8.1开始支持此模式，win8.1以前等效于系统DPI感知
#define DPIM_SYSTEM		1 //系统DPI感知
#define DPIM_UNAWARE	2 //无DPI感知
#define DPIM_DISABLE	3 //DPI不可用,不释放和执行DPI检测进程，不是所有情况都能正常工作，在win8.1以前的系统中系统DPI是100%时或者当前上下文的DPI感知是系统DPI感知时可正常工作，在win8.1及win8.1以后的系统中所有显示器DPI是100%时或当前上下文的DPI感知是每个显示器DPI感知时可正常工作
#define DPIM_CURRENT	4 //当前上下文的DPI感知

//获得设备字符串序号
#define DS_MFR		0x01 //厂商名
#define DS_PROD		0x02 //产品名

//设备运行状态
#define DRS_NOCONNECT	((DWORD)-1) //未连接
#define DRS_PREPARE		0x00 //准备
#define DRS_WORK		0x01 //工作
#define DRS_EDIT		0x02 //编辑
#define DRS_DISABLE		0x03 //禁止操作

//延时复位模式
#define DRM_DEV			0x00 //设备复位
#define DRM_KM			0x01 //键盘鼠标复位

//设备指示灯模式
#define DLM_DEFAULT		0x00 //默认
#define DLM_LIGHT		0x01 //灯亮
#define DLM_DARK		0x02 //灯灭
#define DLM_NORST		0x08 //模式不复位
#define  DECLSPEC_IMPORT __declspec(dllimport)
#define DWORD unsigned long
#define LPDWORD DWORD*
#define LPVOID void*
#define LPWSTR wchar_t*
#define WINAPI __stdcall
#define DWORD_PTR DWORD
#define LPLONG long*
#define LPCWSTR const wchar_t*
#ifdef __cplusplus
extern "C" {
#endif
	DECLSPEC_IMPORT DWORD WINAPI HKMGetVersion(void);
	DECLSPEC_IMPORT DWORD WINAPI HKMSearchDevice(DWORD dwVid, DWORD dwPid, DWORD dwDeviceType);
	DECLSPEC_IMPORT DWORD WINAPI HKMSearchDevice2(DWORD dwVid, DWORD dwPid, DWORD dwSN, DWORD dwDeviceType);
	DECLSPEC_IMPORT LPDWORD WINAPI HKMSearchDeviceAll(DWORD dwVid, DWORD dwPid, DWORD dwDeviceType);
	DECLSPEC_IMPORT LPVOID WINAPI HKMOpen(DWORD dwDeviceId, DWORD dwDpiMode);
	DECLSPEC_IMPORT LPVOID WINAPI HKMOpen2(DWORD dwDeviceId1, DWORD dwDeviceId2, DWORD dwDpiMode);
	DECLSPEC_IMPORT bool WINAPI HKMIsOpen(LPVOID lpHKMData, DWORD dwFlags);
	DECLSPEC_IMPORT bool WINAPI HKMClose(LPVOID lpHKMData);
	DECLSPEC_IMPORT DWORD WINAPI HKMGetDevInfo(LPVOID lpHKMData, DWORD dwIndex, bool bMouse);
	DECLSPEC_IMPORT LPWSTR WINAPI HKMGetDevString(LPVOID lpHKMData, DWORD dwIndex, bool bMouse, LPDWORD lpLength);
	DECLSPEC_IMPORT DWORD WINAPI HKMGetSerialNumber(LPVOID lpHKMData, bool bMouse);
	DECLSPEC_IMPORT DWORD WINAPI HKMGetKeyboardMode(LPVOID lpHKMData);
	DECLSPEC_IMPORT DWORD WINAPI HKMGetMouseMode(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMIsKeyBusy(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMIsMouseBusy(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMIsKeyDown(LPVOID lpHKMData, DWORD_PTR dwVKey);
	DECLSPEC_IMPORT bool WINAPI HKMIsMouseButtonDown(LPVOID lpHKMData, DWORD dwIndex);
	DECLSPEC_IMPORT bool WINAPI HKMGetKeyboardLEDState(LPVOID lpHKMData, DWORD dwIndex);
	DECLSPEC_IMPORT bool WINAPI HKMGetCursorPos(LPVOID lpHKMData, LPLONG lpX, LPLONG lpY);
	DECLSPEC_IMPORT DWORD WINAPI HKMGetCursorPos2(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMSetMode(LPVOID lpHKMData, DWORD dwIndex, DWORD dwMode);
	DECLSPEC_IMPORT bool WINAPI HKMSetKeyInterval(LPVOID lpHKMData, DWORD dwMinTime, DWORD dwMaxTime);
	DECLSPEC_IMPORT bool WINAPI HKMSetMouseInterval(LPVOID lpHKMData, DWORD dwMinTime, DWORD dwMaxTime);
	DECLSPEC_IMPORT bool WINAPI HKMSetAbsMouseScrnRes(LPVOID lpHKMData, long lWidth, long lHeight);
	DECLSPEC_IMPORT bool WINAPI HKMSetMousePosPrecision(LPVOID lpHKMData, DWORD dwPrecision);
	DECLSPEC_IMPORT bool WINAPI HKMSetMouseSpeed(LPVOID lpHKMData, DWORD dwMouseSpeed);
	DECLSPEC_IMPORT bool WINAPI HKMSetMouseMoveTimeout(LPVOID lpHKMData, DWORD dwTimeout);
	DECLSPEC_IMPORT bool WINAPI HKMSetMousePosMaxOffset(LPVOID lpHKMData, DWORD dwOffset);
	DECLSPEC_IMPORT bool WINAPI HKMKeyPress(LPVOID lpHKMData, DWORD_PTR dwVKey);
	DECLSPEC_IMPORT bool WINAPI HKMKeyDown(LPVOID lpHKMData, DWORD_PTR dwVKey);
	DECLSPEC_IMPORT bool WINAPI HKMKeyUp(LPVOID lpHKMData, DWORD_PTR dwVKey);
	DECLSPEC_IMPORT bool WINAPI HKMMoveTo(LPVOID lpHKMData, long nX, long nY);
	DECLSPEC_IMPORT bool WINAPI HKMMoveR(LPVOID lpHKMData, long nX, long nY);
	DECLSPEC_IMPORT bool WINAPI HKMMoveR2(LPVOID lpHKMData, long nX, long nY);
	DECLSPEC_IMPORT bool WINAPI HKMMoveRP(LPVOID lpHKMData, long nX, long nY);
	DECLSPEC_IMPORT bool WINAPI HKMLeftClick(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMRightClick(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMMiddleClick(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMXBtn1Click(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMXBtn2Click(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMLeftDoubleClick(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMRightDoubleClick(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMMiddleDoubleClick(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMXBtn1DoubleClick(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMXBtn2DoubleClick(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMLeftDown(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMRightDown(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMMiddleDown(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMXBtn1Down(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMXBtn2Down(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMLeftUp(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMRightUp(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMMiddleUp(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMXBtn1Up(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMXBtn2Up(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMMouseWheel(LPVOID lpHKMData, long nCount);
	DECLSPEC_IMPORT bool WINAPI HKMMouseWheelP(LPVOID lpHKMData, long nCount);
	DECLSPEC_IMPORT bool WINAPI HKMReleaseKeyboard(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMReleaseMouse(LPVOID lpHKMData);
	DECLSPEC_IMPORT bool WINAPI HKMOutputString(LPVOID lpHKMData, LPCWSTR lpString);
	DECLSPEC_IMPORT bool WINAPI HKMDelayRnd(LPVOID lpHKMData, DWORD dwMinTime, DWORD dwMaxTime);
	DECLSPEC_IMPORT bool WINAPI HKMVerifyUserData(LPVOID lpHKMData, LPCWSTR lpString, bool bMouse);
	DECLSPEC_IMPORT DWORD WINAPI HKMVerifyUserData2(LPVOID lpHKMData, LPCWSTR lpString, bool bMouse);
	DECLSPEC_IMPORT bool WINAPI HKMSetResetMode(LPVOID lpHKMData, DWORD dwMode, bool bMouse);
	DECLSPEC_IMPORT bool WINAPI HKMSetResetTime(LPVOID lpHKMData, DWORD dwTime, bool bMouse);
	DECLSPEC_IMPORT bool WINAPI HKMSetLightMode(LPVOID lpHKMData, DWORD dwMode, bool bMouse);
	DECLSPEC_IMPORT bool WINAPI HKMIsOSMouseAccelerateEnabled(void);
	DECLSPEC_IMPORT bool WINAPI HKMEnableOSMouseAccelerate(bool bEnable, bool bSave);
	DECLSPEC_IMPORT int WINAPI HKMGetOSMouseSpeed(void);
	DECLSPEC_IMPORT bool WINAPI HKMSetOSMouseSpeed(int nSpeed, bool bSave);
	DECLSPEC_IMPORT bool WINAPI HKMFreeData(LPVOID lpData);
	DECLSPEC_IMPORT DWORD WINAPI HKMGetDataCount(LPVOID lpData);
	DECLSPEC_IMPORT DWORD WINAPI HKMGetDataUnitInt(LPVOID lpData, DWORD dwIndex);

#ifdef __cplusplus
}
#endif

#endif //_HKM_
