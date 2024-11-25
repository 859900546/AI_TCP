#ifndef ACTIONSTATUS_H
#define ACTIONSTATUS_H

#include <string>
#include <vector>
#include <thread>
#include <windows.h>
#include "setting.h"
#include "vital_timer.h"

extern Setting* setting;

// 前置声明用到的类
namespace pynput {
    namespace mouse {
        class Button;  // 鼠标按钮的虚拟声明
    }
    namespace keyboard {
        class Listener;  // 键盘监听器的虚拟声明
    }
}
static HHOOK hMouseHook;  // 鼠标钩子句柄
static HHOOK hKeyboardHook;  // 键盘钩子句柄
static bool mouse_left_pressed;
static bool mouse_right_pressed;
static bool mouse_middle_pressed;
static bool keyboard_W_pressed;
static bool keyboard_A_pressed;
static bool keyboard_S_pressed;
static bool keyboard_D_pressed;
static bool opened_mirror_status;
static bool max_aim_status;
static bool trigger_status;
static bool trigger_sleep_status;
static bool aim_head_status;
static bool aim_key_status;
static bool auto_aim_status;
static bool moving_status;
static float Time_after_stopping;
static bool aim_type;
static bool autoYq;
static bool moving_listener;
static float ActiveTriggerTime;

static float DownLeftTime = 0;

static int cancel_aim_list_cnt;

LRESULT CALLBACK MouseListen(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void Listener();
void play_sound(bool f);
void child(bool f);
void MoveingListener();
void timer(float seconds, int key);
void start_timer(float seconds, int key);


class ActionStatus {
private:
    pynput::mouse::Button* __mouse_left_key;
    pynput::mouse::Button* __mouse_check_auto_aim;
    pynput::mouse::Button* __mouse_check_aim_head;
    pynput::mouse::Button* __mouse_check_key_aim;

    std::string __keyboard_W_key;
    std::string __keyboard_A_key;
    std::string __keyboard_S_key;
    std::string __keyboard_D_key;
    std::string __keyboard_close_key;
    std::string __keyboard_trigger_key;
    std::string __keyboard_max_aim_key;
    std::string __keyboard_normal_aim_key;
    std::vector<std::string> __change_open_mirror_keys;

    bool check_Xdirection(std::vector<int> ans);
    bool check_Ydirection(std::vector<int> ans);
   // std::vector<std::thread> ThreadList;

public:
    ActionStatus(bool moving_listener,float Time_);
    ActionStatus();
    void init();
    bool AimStatus() const;
    /*static void SetHooks();
    static LRESULT CALLBACK KeyboardListener(int nCode, WPARAM wParam, LPARAM lParam);*/
    bool TriggerListener(std::vector<int> ans);
    bool TriggerListener(std::vector<int> ans, AimTimer* t);
    float GetStoppingTime() const;
    bool GetMovingStatus() const;
    bool GetOpenedMirrorStatus() const;
    bool GetLeftPressed() const;
    bool GetMaxAimStatus() const;
    bool GetTriggerStatus() const;
    bool GetAimHeadStatus() const;
    bool GetAimType() const;
    bool GetAutoYq() const;
    void SetTrigger_sleep_status(bool status);
    void SetTimeAfterStopping(float time_after_stopping);

    
};

#endif // ACTIONSTATUS_H
