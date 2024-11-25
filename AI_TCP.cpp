#include <iostream>
#include <vector>
#include "TcpClient.h"
#include "Move.h"


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
//int main(){

   
      // 读取设置文件
    setting = new Setting();
    TcpClient client(setting->IP.c_str(), setting->Port); // 连接服务器
    while (!client.Connect())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        puts("waiting for server...");
    }
    puts("connected");

    ActionStatus controller;    // 控制器
    controller.init();

    AimTimer is_mss;  // 瞄准计时器
    AimTimer trigger_produce_cancel_move;  // 触发生产取消移动计时器

    PID pid_x = sz_zz();
    
    
    ScreenShot screenshot = ScreenShot();  // 截图对象

    
    int error_cnt = 0;

    MOUSE mouse;
    mouse.SetMouseSpeed(40);
    mouse.SetMouseMoveTimeOut(23);
    mouse.SetMode(5, 2);
    //  i_KM mouse;
    /*mouse.OpenDeviceByID(0x3554, 0xF58E);
    if (!mouse.IsOpen())
        return 1;
    mouse.SetLed(false);*/

    Move move(&controller, &screenshot, &mouse, &pid_x, &is_mss, &trigger_produce_cancel_move);  // 实例化移动对象

    std::thread t1(&Move::yq_auto, &move);  // 启动移动线程
    t1.detach();  // 线程分离

    play_sound(0);
    while (true)
    {
        if (controller.GetOpenedMirrorStatus() && !controller.GetAimHeadStatus())
            screenshot.max_x_size = 440, screenshot.max_y_size = 320;
        else
            screenshot.max_x_size = 288, screenshot.max_y_size = 288;

        char message[100] = { 0 };
        int ans[6] = { 0 };
       // std::vector<BYTE> byteStream = screenshot.Screen_byte(); // 获取位图字节流
        error_cnt += !client.Sendall(screenshot.Screen_byte()); // 发送字节流
        /*if (error_cnt > 10)
            return 1;*/

        client.Recv(message, 100); // 读取返回消息
       // std::cout << message << std::endl;
        client.getpoint(message, ans);  // 获取坐标点
        //
        move.start_move(ans); // 开始移动
     }
    return 0;
}