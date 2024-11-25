#include "Mouse.h"



MOUSE::MOUSE()
{
    DWORD dwDevId = HKMSearchDevice(0x3554, 0xF58E, 0);


    if (dwDevId == 0xFFFFFFFF)
    {
        return;
    }
    this->lpDev = HKMOpen(dwDevId, 0);
    if (!lpDev)
    {
        return;
    }
    //执行操作
    return;
}

MOUSE::MOUSE(int p, int v)
{
    DWORD dwDevId = HKMSearchDevice(p, v, 0);


    if (dwDevId == 0xFFFFFFFF)
    {
        return;
    }
    this->lpDev = HKMOpen(dwDevId, 0);
    if (!lpDev)
    {
        return;
    }
    //执行操作
    return;
}

MOUSE::~MOUSE()
{
    HKMClose(this->lpDev);
}

void MOUSE::MoveR(int x, int y)
{
    HKMMoveR(this->lpDev, x, y);
}

void MOUSE::MoveRP(int x, int y)
{
    HKMMoveRP(this->lpDev, x, y);
}

void MOUSE::Left_click()
{
    //HKMLeftClick(this->lpDev);
    HKMLeftDown(this->lpDev);
    HKMDelayRnd(lpDev, 2, 6);
    HKMLeftUp(this->lpDev);
}

void MOUSE::Right_click()
{
    HKMRightClick(this->lpDev);
}

void MOUSE::Key_press(int key)
{
    HKMKeyPress(this->lpDev, key);
}

void MOUSE::Key_down(int key)
{
    HKMKeyDown(this->lpDev, key);
}

void MOUSE::Key_up(int key)
{
    HKMKeyUp(this->lpDev, key);
}

void MOUSE::SetMouseMoveTimeOut(int time)
{
    HKMSetMouseMoveTimeout(this->lpDev, time);
}

void MOUSE::SetMouseSpeed(int speed)
{
    HKMSetMouseSpeed(this->lpDev, speed);
}

void MOUSE::SetLightMode(int mode)
{
    HKMSetLightMode(lpDev, mode, false);
}

void MOUSE::SetMode(int mode, int mode2)
{
    HKMSetMode(this->lpDev, mode, mode2);
}
