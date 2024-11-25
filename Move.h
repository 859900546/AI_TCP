#pragma once
#include "ActionStatus.h"
#include "screenshot.h"
#include "Mouse.h"
#include "pid.h"
//#include "i_KM.h"

struct MoveThreadArgs {
    MoveThreadArgs(float wm, float hm, float initial_st, bool dist, bool dist_2, float oky)
        : wm(wm), hm(hm), initial_st(initial_st), dist(dist), dist_2(dist_2), oky(oky)
    {
        ans[0] = 0;
        ans[1] = 0;
        ans[2] = 0;
        ans[3] = 0;
        ans[4] = 0;
    }
    float ans[5];
    float wm;
    float hm;
    float initial_st;
    bool dist;
    bool dist_2;
    float oky;

};

struct Vector2A {
    Vector2A(float x, float y) : x(x), y(y) {};
    float x;
    float y;
};

class Move {
    private:
        ActionStatus* controller;  // ������
        ScreenShot* screenshot;  // ��ͼ
        MOUSE* mouse; // ����
       // i_KM* mouse;
        PID* pid_x;  // �����㷨
        PID* pid_x_J = new PID(setting->BP_Pid_P, setting->BP_Pid_P, setting->BP_Pid_D, setting->BP_Pid_Imax, 1);;
        AimTimer* is_mss;  // ���ֵ�����㷨
        AimTimer* trigger_produce_cancel_move;  // ���ԭ�صȴ��㷨
        float speed; // �ٶ�
        bool check_trigger;  // ���״̬
        int mutex_move = 0;  // ������

        int loss_target_cnt = 0;  // Ŀ��x����

        
    public:
        Move(ActionStatus* controller, ScreenShot* screenshot, MOUSE* mouse, PID* pid_x, AimTimer* is_mss, AimTimer* trigger_produce_cancel_move);
        Move(ActionStatus* controller, ScreenShot* screenshot, MOUSE* mouse, PID* pid_x, AimTimer* is_mss, AimTimer* trigger_produce_cancel_move, double speed);
        float start_move(int *ans);
        void time_sleep(int ms);
        void flash_mirror();
        void thread_move_controller(MoveThreadArgs move_args);
        Vector2A threading_move(MoveThreadArgs move_args);
        void yq_auto();
        bool detect_check = false;

        bool activate_event(int _target);
};

