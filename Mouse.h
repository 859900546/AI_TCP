#include"HKMR.h"
//#pragma comment(lib,"wyhr.lib")

class MOUSE {
public:
	MOUSE();
	MOUSE(int p, int v);
	~MOUSE();
	void MoveR(int x, int y);
	void MoveRP(int x, int y);
	void Left_click();
	void Right_click();
	void Key_press(int key);
	void Key_down(int key);
	void Key_up(int key);
	void SetMouseMoveTimeOut(int time);
	void SetMouseSpeed(int speed);
	void SetLightMode(int mode);
	void SetMode(int mode, int mode2);
private:
	LPVOID lpDev;
};

