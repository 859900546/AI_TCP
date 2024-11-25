#include "ActionStatus.h"
#include <iostream>
#include <chrono>  // ���� std::this_thread::sleep_for
#include <Windows.h>  // ���� Windows �ض����ܣ����� Beep
#include <mutex>

std::vector<int> change_open_mirror_keys = { 'Q', 'E', 'R', 'X', 'F', 'G' ,'1','2','3','4'};

ActionStatus::ActionStatus(bool moving_listener_, float Time_) {
    moving_listener = moving_listener_;
    ActiveTriggerTime = Time_;
    //ThreadList.push_back(std::thread(&ActionStatus::Listener, this));
    //ThreadList.push_back(std::thread(&ActionStatus::KeyboardListener, this));
    //ThreadList.push_back(std::thread(&ActionStatus::MoveingListener, this));
}

ActionStatus::ActionStatus() {
    moving_listener = true;
    ActiveTriggerTime = setting->MovedAfterActiveTriggerTime / 1000.0f;
}

void ActionStatus::init() {
    //for (int i : start_cnt) {
    //    ThreadList[i - 1].detach();  // �����߳�ʱ���룬�������ʾ��
    //}
    std::thread t1(&MoveingListener);
    t1.detach();
    std::thread t(&Listener);
    t.detach();
}

bool ActionStatus::AimStatus() const {
    return mouse_left_pressed || auto_aim_status || aim_key_status;
}



bool ActionStatus::TriggerListener(std::vector<int> ans)
{
    bool position_check = check_Xdirection(ans) && check_Ydirection(ans);
    bool Key_check = trigger_status;
    bool backGround_check = (opened_mirror_status || GetMaxAimStatus()) && !trigger_sleep_status && !mouse_left_pressed;
    bool stopped_time = (Time_after_stopping > ActiveTriggerTime && !moving_status) || !moving_listener;

    return position_check && Key_check && backGround_check && stopped_time;
}

bool ActionStatus::TriggerListener(std::vector<int> ans, AimTimer* t) {

    bool position_check = check_Xdirection(ans) && check_Ydirection(ans);
    bool Key_check = trigger_status;
    bool backGround_check = (opened_mirror_status || GetMaxAimStatus() || t->status()) && !trigger_sleep_status && !mouse_left_pressed;
    bool stopped_time = (Time_after_stopping > ActiveTriggerTime && !moving_status) || !moving_listener;

    return position_check && Key_check && backGround_check && stopped_time;
}



bool ActionStatus::check_Xdirection(std::vector<int> ans) {
    int x = ans[0];
    int y = ans[1];
    int w = ans[2];
    int h = ans[3];
    int res = ans[4];
    int oky = ans[5];
    if (x == 0) {
        return true;
    }
    if (res == 0) {
        if (abs(x) <= w / 2) {
            return true;
        }
    }
    else {
      //  int d = (abs(y) >= t) ? 2 : 4;
        if ((abs(x) <= w / ((abs(oky) >= h / 6) ? 2 : 4) && !aim_head_status) || (abs(x) <= w / 4 && aim_head_status)) {
            return true;
        }
    }
    return false;
}

bool ActionStatus::check_Ydirection(std::vector<int> ans) {
    int x = ans[0];
    int y = ans[1];
    int w = ans[2];
    int h = ans[3];
    int res = ans[4];
    if (y == 0) {
        return true;
    }
    if (res == 0) {
        if (abs(y) <= h / 2) {
            return true;
        }
    }
    else {
        if (h / 3 <= y && y <= h / 2) {
            return true;
        }
    }
    return false;
}

float ActionStatus::GetStoppingTime() const {
    return Time_after_stopping;
}

bool ActionStatus::GetMovingStatus() const {
    return moving_status;
}

bool ActionStatus::GetOpenedMirrorStatus() const {
    return opened_mirror_status;
}

bool ActionStatus::GetLeftPressed() const {
    return mouse_left_pressed;
}

bool ActionStatus::GetMaxAimStatus() const {
    return max_aim_status;
}

bool ActionStatus::GetTriggerStatus() const {
    return trigger_status;
}

bool ActionStatus::GetAimHeadStatus() const {
    return aim_head_status;
}

bool ActionStatus::GetAimType() const {
    return aim_type;
}

bool ActionStatus::GetAutoYq() const {
    return autoYq;
}

void ActionStatus::SetTrigger_sleep_status(bool status) {
    trigger_sleep_status = status;
}

void ActionStatus::SetTimeAfterStopping(float time_after_stopping) {
    Time_after_stopping = time_after_stopping;
}

std::mutex mtx_autoaim;

void timer(float seconds, int key) {
    std::lock_guard<std::mutex> lock(mtx_autoaim); // ʹ�û�����������������

    if (key == 0) {
        trigger_sleep_status = true;
        std::this_thread::sleep_for(std::chrono::duration<float>(seconds));
        trigger_sleep_status = false;
    }
    else if (key == 1) {
        auto_aim_status = false;
        cancel_aim_list_cnt++;
        std::this_thread::sleep_for(std::chrono::duration<float>(seconds));
        cancel_aim_list_cnt--;
        if (cancel_aim_list_cnt <= 0) {
            auto_aim_status = true;
        }
    }
}

void start_timer(float seconds, int key) {
    std::thread t(&timer, seconds, key);
    t.detach(); // �����̣߳�ʹ���ں�̨����
}

void play_sound(bool f) {
    // ��Ҫʵ�ֲ��������Ĺ���
    auto child_thread = std::thread(&child, f);
    child_thread.detach();
}

void child(bool f) {
    if (f) {
        Beep(440, 800);  // ʾ���еķ�����
    }
    else {
        MessageBeep(MB_ICONASTERISK);  // ʾ���е���Ϣ������
    }
}


void Listener() {
    // ע�ᴰ����
    WNDCLASS wc = {};
    wc.lpfnWndProc = MouseListen;
    wc.lpszClassName = L"1";
    RegisterClass(&wc);

    // �������ڣ���Ϊ�˽�����Ϣ������ʾ���ڣ�
    HWND hwnd = CreateWindow(wc.lpszClassName, L"1", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);

    // ע��ԭʼ�����豸�����̺����
    RAWINPUTDEVICE Rid[2];
    Rid[0].usUsagePage = 0x01; // ����
    Rid[0].usUsage = 0x06;
    Rid[0].dwFlags = RIDEV_INPUTSINK;
    Rid[0].hwndTarget = hwnd;

    Rid[1].usUsagePage = 0x01; // ���
    Rid[1].usUsage = 0x02;
    Rid[1].dwFlags = RIDEV_INPUTSINK;
    Rid[1].hwndTarget = hwnd;

    RegisterRawInputDevices(Rid, 2, sizeof(Rid[0]));

    // ��Ϣѭ��
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
// ����ԭʼ����Ļص�����
LRESULT CALLBACK MouseListen(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_INPUT) {
        UINT dataSize = 0;
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));

        if (dataSize > 0) {
            BYTE* buffer = new BYTE[dataSize];
            if (buffer) {
                GetRawInputData((HRAWINPUT)lParam, RID_INPUT, buffer, &dataSize, sizeof(RAWINPUTHEADER));
                RAWINPUT* raw = (RAWINPUT*)buffer;

                if (raw->header.dwType == RIM_TYPEMOUSE) {
                    RAWMOUSE& mouseData = raw->data.mouse;

                    // �����갴ť״̬
                    // ���
                    if (mouseData.usButtonFlags & RI_MOUSE_BUTTON_1_DOWN) {
                        
                        mouse_left_pressed = true;
                    }
                    else if (mouseData.usButtonFlags & RI_MOUSE_BUTTON_1_UP) {
                        mouse_left_pressed = false;
                        
                        if (!aim_head_status)
                            opened_mirror_status = false;

                        if (!max_aim_status && auto_aim_status && !opened_mirror_status)
                            start_timer(setting->LeftPressedAfterCancelAim_Time / 1000.0f, 1);
                    }

                    // �Ҽ�
                    if (mouseData.usButtonFlags & RI_MOUSE_BUTTON_2_DOWN) {
                        mouse_right_pressed = true;
                    }
                    else if (mouseData.usButtonFlags & RI_MOUSE_BUTTON_2_UP) {
                        mouse_right_pressed = false;
                        opened_mirror_status = !opened_mirror_status;
                    }

                    // �м�
                    if (mouseData.usButtonFlags & RI_MOUSE_BUTTON_3_DOWN);
                    else if (mouseData.usButtonFlags & RI_MOUSE_BUTTON_3_UP);

                    // ����²��
                    if (mouseData.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN);

                    else if (mouseData.usButtonFlags & RI_MOUSE_BUTTON_4_UP) {

                        auto_aim_status = !auto_aim_status;
                        play_sound(auto_aim_status);
                    }

                    // ������ϼ�
                    if (mouseData.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN);
                    else if (mouseData.usButtonFlags & RI_MOUSE_BUTTON_5_UP) {
                        aim_head_status = !aim_head_status;
                        play_sound(aim_head_status);
                    }

                    // ��������ֹ���
                    if (mouseData.usButtonFlags & RI_MOUSE_WHEEL) {
                        opened_mirror_status = false;
                    }
                }
                else if (raw->header.dwType == RIM_TYPEKEYBOARD) {
                    RAWKEYBOARD& keyboardData = raw->data.keyboard;
                    DWORD vkCode = keyboardData.VKey;
                    bool keyDown = (keyboardData.Flags & RI_KEY_BREAK) == 0;

                    // ��ӡ���̰�����Ϣ
                    if (keyDown) {
                        switch (vkCode) {
                            case 65:  // A
                                keyboard_A_pressed = true;
                                break;
                            case 68:  // D
                                keyboard_D_pressed = true;
                                break;
                            case 87:  // W
                                keyboard_W_pressed = true;
                                break;
                            case 83:  // S
                                keyboard_S_pressed = true;
                                break;
                        }
                        
                    }
                    else {
                        switch (vkCode) {
                            case 192:  // `
                                trigger_status = !trigger_status;
                                play_sound(trigger_status);
                                break;
                            case 65:  // A
                                keyboard_A_pressed = false;
                                break;
                            case 68:  // D
                                keyboard_D_pressed = false;
                                break;
                            case 87:  // W
                                keyboard_W_pressed = false;
                                break;
                            case 83:  // S
                                keyboard_S_pressed = false;
                                break;
                            case 123:  // F12
                                play_sound(0);
                                Sleep(3000);
                                exit(0);
                            case 112:  // F1
                                max_aim_status = !max_aim_status;
                                play_sound(max_aim_status);
                                break;
                            case 187:  // +
                                max_aim_status = true; 
                                play_sound(max_aim_status);
                                break;
                            case 189:  // -
                                max_aim_status = false;
                                play_sound(max_aim_status);
                                break;
                           //caps
                            case 20:  // caps
                                max_aim_status = !max_aim_status;
                                play_sound(max_aim_status);
                                break;
                        }
                        for(int i:change_open_mirror_keys)
                            if (vkCode == i)
                                opened_mirror_status = false;
                    }
                }

                delete[] buffer;
            }
        }
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}


// �ƶ�������
void MoveingListener() {
    while (moving_listener) {
        //moving_status?std::cout<<"moving"<<std::endl:std::cout<< Time_after_stopping <<std::endl;
        auto start_time = std::chrono::steady_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        bool ws = (keyboard_W_pressed == keyboard_S_pressed);
        bool ad = (keyboard_A_pressed == keyboard_D_pressed);
        moving_status = !(ws && ad);
        if (moving_status) {
            Time_after_stopping = 0;
        }
        else {
            auto end_time = std::chrono::steady_clock::now();
            float elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() / 1000.0;
            Time_after_stopping += elapsed_time;
            Time_after_stopping = min(1.0, Time_after_stopping);
        }
    }
}
