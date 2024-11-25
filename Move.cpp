#include "move.h"


Move::Move(ActionStatus* controller, ScreenShot* screenshot, MOUSE* mouse, PID* pid_x, AimTimer* is_mss, AimTimer* trigger_produce_cancel_move) {
    this->controller = controller;
    this->screenshot = screenshot;
    this->mouse = mouse;
    this->pid_x = pid_x;
    this->is_mss = is_mss;
    this->trigger_produce_cancel_move = trigger_produce_cancel_move;
    this->speed = 1.0;
    this->check_trigger = false;
}

Move::Move(ActionStatus* controller, ScreenShot* screenshot, MOUSE* mouse, PID* pid_x, AimTimer* is_mss, AimTimer* trigger_produce_cancel_move, double speed) {
    this->controller = controller;
    this->screenshot = screenshot;
    this->mouse = mouse;
    this->pid_x = pid_x;
    this->is_mss = is_mss;
    this->trigger_produce_cancel_move = trigger_produce_cancel_move;
    this->speed = speed;
    this->check_trigger = false;

}

void Move::time_sleep(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void Move::flash_mirror()
{
    if (!controller->GetAimHeadStatus() && controller->GetOpenedMirrorStatus()) {
        mouse->Key_down(51);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        mouse->Key_up(51);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        mouse->Key_down(49);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        mouse->Key_up(49);
    }
}

void Move::thread_move_controller(MoveThreadArgs move_args)
{
    int flag = this->mutex_move;
    MoveThreadArgs move_args_copy = move_args;
    int flag_1 = move_args_copy.ans[0];
    int flag_2 = move_args_copy.ans[1];
    while (flag == this->mutex_move)
    {
        Vector2A args = this->threading_move(move_args_copy);
       // break;
        if (int(args.x) == 0 && int(args.y) == 0)
            break;
        if(flag_1 > 0)
            move_args_copy.ans[0] = setting->Block_Rate_X * max(move_args_copy.ans[0] - args.x, 0);
        else
            move_args_copy.ans[0] = setting->Block_Rate_X * min(0, move_args_copy.ans[0] - args.x);
        
        if(flag_2 > 0)
            move_args_copy.ans[1] = setting->Block_Rate_Y * max(move_args_copy.ans[1] - args.y, 0);
        else
            move_args_copy.ans[1] = setting->Block_Rate_Y * min(0, move_args_copy.ans[1] - args.y);

        if (int(move_args_copy.ans[0]) == 0 && int(move_args_copy.ans[1]) == 0)
            break;
        time_sleep(setting->BlockThreading_SleepTime);
    }
    return;
}

Vector2A Move::threading_move(MoveThreadArgs args)
{

    float x = args.ans[0];
    float y = args.ans[1];
    float w = args.ans[2];
    float h = args.ans[3];
    int res = args.ans[4];
    float wm = args.wm;
    float hm = args.hm;
    float initial_st = args.initial_st;
    bool dist = args.dist;
    bool dist_2 = args.dist_2;
    float oky = args.oky;

    if (controller->GetAimHeadStatus()) {
        if ((res == 0 && abs(x) >= w * 1.6) || (res == 1 && abs(x) >= w) ||
            (res == 0 && abs(oky) >= h * 1.5) || (res == 1 && abs(oky) >= h) ||
            (controller->GetMovingStatus() &&!controller->GetOpenedMirrorStatus() && !controller->GetMaxAimStatus())) {
            x *= setting->HR_ThreadMove_First_X_Rate * (x > 0 ? 2 : 1);

            if (controller->GetMovingStatus() && !controller->GetMaxAimStatus()) {
                x *= setting->HR_MoveingAndNotMaxAim_Rate;
            }
            if (!dist && !controller->GetMaxAimStatus()) {
                x *= 0.8;
            }
            if (x < 0) {
                x = max(-wm * setting->HR_MaxMaximum_X_Multiple, x);
            }
            else {
                x = min(wm * setting->HR_MaxMaximum_X_Multiple, x);
            }
            if (res == 1 && h > w) {
                y = y - h / 3 * initial_st;
            }
            if (y < 0) {
                y = max(-hm * setting->HR_MaxMaximum_Y_Multiple, y);
            }
            else {
                y = min(hm * setting->HR_MaxMaximum_Y_Multiple, y);
            }
            if (!controller->GetMaxAimStatus()) {
                y *= setting->HR_NotMaxAim_Y_Rate;
            }
            if (controller->GetMaxAimStatus() || controller->GetOpenedMirrorStatus())
                x *= setting->HR_MaxAimOrOpenedMirror_X_Rate;
            if (controller->AimStatus()) {
                mouse->MoveR(int(x), round(y));
                return Vector2A(x, y);
            }
        }
        else {
            x = pid_x->cmd_pid(x, setting->Average_One_round_Time / 1000.0f, w, wm * setting->HP_MaxMaximum_Y_Multiple);

            if (x < 0) {
                x = max(-wm * setting->HP_MaxMaximum_Y_Multiple, x);
            }
            else {
                x = min(wm * setting->HP_MaxMaximum_Y_Multiple, x);
            }
            if (res == 1 && h > w) {
                y = y - h / 3 * initial_st;
            }
            if (y < 0) {
                y = max(-hm, y);
            }
            else {
                y = min(hm, y);
            }
            if (controller->GetLeftPressed()) {
                y *= setting->HP_LeftPressed_Y_Rate;
            }
            /*if (okx <= w / 5 && !y) {
                return 0;
            }*/
            if (((-oky > -h / 2 && oky < 0) || (0 < oky < h / 2)) && !controller->GetMaxAimStatus() &&
                !controller->GetOpenedMirrorStatus()) {
                y *= setting->HP_Normal_Y_Rate;
             //   y *= 0.88;
            }
            else if (-h / 5 < oky && oky < h / 2 && res == 1 && !controller->GetMaxAimStatus() &&
                !controller->GetOpenedMirrorStatus()) {
              //  y *= 0.68;
                y *= setting->HP_Normal_Y_Rate;
            }
            if (controller->GetOpenedMirrorStatus())
                x *= setting->HP_OpenedMirror_X_Rate;
            if (controller->AimStatus()) {
                mouse->MoveR(int(x), round(y));
                return Vector2A(x, y);
            }
        }
    }
    else {
        if ((res == 0 && abs(x) >= w * 1.5) || (res == 1 && abs(x) >= w * 0.8) || abs(oky) >= h * 1.5 ||
            (controller->GetMovingStatus() && !controller->GetMaxAimStatus())) {

            x *= setting->BR_First_Rate;
            int doubleVal = controller->GetTriggerStatus() ? 1 : 1;
            if (controller->GetMovingStatus()) {
                x *= setting->BR_MoveingAndNotMaxAim_Rate;
            }
            if (x < 0 && !controller->GetOpenedMirrorStatus()) {
                x = max(-wm * setting->BR_MaxMaximum_X_Multiple * doubleVal, x);
            }
            else if (x > 0 && !controller->GetOpenedMirrorStatus()) {
                x = min(wm * setting->BR_MaxMaximum_X_Multiple * doubleVal, x);
            }
            if (y < 0) {
                y = max(-hm * setting->BR_MaxMaximum_Y_Multiple, y);
            }
            else {
                y = min(hm * setting->BR_MaxMaximum_Y_Multiple, y);
            }
            if (controller->AimStatus()) {
                mouse->MoveR(int(x), round(y));
                return Vector2A(x, y);
            }
        }
        else {
            int doubleVal = controller->GetTriggerStatus() ? 2 : 2;
            x = pid_x_J->cmd_pid(x, setting->Average_One_round_Time / 1000.0f, w, wm * setting->BR_MaxMaximum_X_Multiple * doubleVal);
            if (controller->GetOpenedMirrorStatus())
                x *= setting->BP_OpenedMirror_X_Rate;
            
            if (x < 0 && dist_2) {
                x = max(-wm * setting->BR_MaxMaximum_X_Multiple * doubleVal, x);
            }
            else if (x > 0 && dist_2) {
                x = min(wm * setting->BR_MaxMaximum_X_Multiple * doubleVal, x);
            }
            if (y < 0) {
                y = max(-hm * setting->BR_MaxMaximum_Y_Multiple * doubleVal, y);
            }
            else {
                y = min(hm * setting->BR_MaxMaximum_Y_Multiple * doubleVal, y);
            }
            if (controller->GetLeftPressed()) {
                y = 0;
            }
            if (controller->AimStatus()) {
                mouse->MoveR(int(x), round(y));
                return Vector2A(x, y);
            }
        }

    }
}

void Move::yq_auto()
{
    while (true)
    {
        float i = setting->YQ_basis;
        int j = setting->YQ_MaxLength;
        if (!this->detect_check || controller->GetMaxAimStatus())
        {
            time_sleep(10);
            continue;
        }
        while (controller->GetLeftPressed())
        {
            if (j <= 0)
            {
                time_sleep(setting->YQ_Per_SleepTime);
                continue;
            }
            mouse->MoveRP(0, int(i));
            i += setting->YQ_Y_Increase;
            j -= setting->YQ_Length_Decline;
            time_sleep(setting->YQ_Per_SleepTime);
        }
        time_sleep(30);
    }
}

bool Move::activate_event(int _target)  // 判断当前是否有事件发生
{
    return _target || controller->GetLeftPressed()
        || controller->GetMaxAimStatus()
        || controller->GetTriggerStatus();
}

//bool xold_flag = true;
//bool yold_flag = true;
//
//int xold_decline_cnt = 0;
//int yold_decline_cnt = 0;


float Move::start_move(int* ans) {
    if (this->activate_event(ans[5]));
        is_mss->start_timer(setting->ProcessHappenedEvent_ActiteTime / 1000.0f);
    //  std::cout<<"is_mss->status():"<<is_mss->status()<<std::endl;
    if (!is_mss->status())
    {
        if (!controller->GetAimHeadStatus())
            time_sleep(setting->ProcessNotEvent_SleepTime_B);
        else {
            time_sleep(setting->ProcessNotEvent_SleepTime_H);
            return 0;
        }
    }
    if (!(controller->GetLeftPressed()
        || ans[5]
        || controller->GetMaxAimStatus())
        && controller->GetTriggerStatus()) {
        trigger_produce_cancel_move->start_timer(setting->TriggerDelayShot_Time / 1000.0f);
    }
    if (!ans[5] || (!controller->AimStatus() && !controller->GetTriggerStatus()))
    {
        if (!ans[5])
        {
            this->detect_check = false;

            screenshot->size_add(), loss_target_cnt++;
            if (loss_target_cnt >= 2) {
                ++mutex_move%=10, loss_target_cnt = 0;
            }
        }
        else if (controller->GetAimHeadStatus())
            screenshot->size_sub();
        //  time_sleep(2);
        return 0;
    }

    this->detect_check = true;

    if (controller->GetAimHeadStatus())
        screenshot->size_sub();
    float x = ans[0] * speed;
    float y = ans[1] * speed;
    float w = ans[2] * speed;
    float h = ans[3] * speed;
    int res = ans[4];
  //  std::cout<<x<<" "<<y<<" "<<w<<" "<<h<<" "<<res<<std::endl;
    //if (!controller->AimStatus() && !controller->GetTriggerStatus()) {
    //    //  std::this_thread::sleep_for(std::chrono::milliseconds(2));
    //   //   time_sleep(2);
    //    return 0;
    //}

    bool start_trriger = false;

    if (0 < y && y < h / 2 && !controller->GetAimHeadStatus() && res == 1) {
        y = 0;
    }
    else if (-h * 3 < y && y < h / 2 && !controller->GetAimHeadStatus() && res == 0) {
        y = 0;
    }
    if ((res == 0 && 0 <= std::abs(x) && std::abs(x) <= w * 0.12 + 1) ||
        (res == 1 && 0 <= std::abs(x) && std::abs(x) <= w * 0.085 + 1)) {
        x = 0;
    }

    // 扳机数据处理
    std::vector<int> t = { int(x), int(y), int(w), int(h), res, int(ans[1]) };

    if (abs(x) < w / 5 && res == 0 && controller->GetLeftPressed()) {
        x = 0;
    }
    if (!controller->GetAimHeadStatus() && abs(x) < w / 4 && res && controller->GetLeftPressed()) {
        x = 0;
    }

    float oky = y;
    float okx = x;
    float initial_st = setting->Initial_st_Y_Rate;


    //if (xold_flag != x > 0 ? true : false)
    //    xold_decline_cnt += 1;
    //else
    //    xold_decline_cnt = 0;

    //if (yold_flag != y > 0 ? true : false)
    //    yold_decline_cnt += 1;
    //else
    //    yold_decline_cnt = 0;

    //if (xold_decline_cnt > 2) {
    // //   x *= 0.5;
    //    xold_decline_cnt = 0;
    //}

    //if (yold_decline_cnt > 2) {
    ////    y *= 0.5;
    //    yold_decline_cnt = 0;
    //}

    //xold_flag = x > 0 ? true : false;
    //yold_flag = y > 0 ? true : false;

    if (y < 0) {
        y *= initial_st;
    }
    else {
        y *= initial_st;
    }

    float hm, wm;

    if (res == 1) {
        hm = h / 12;
        wm = w / 6;
    }
    else {
        hm = h / 2.5;
        wm = w / 1.5;
    }

    hm = max(hm, 5.0);
    wm = max(wm, 5.0);

    // 扳机
    if (controller->TriggerListener(t,trigger_produce_cancel_move)) {
        if (!check_trigger) {
            check_trigger = true;
        }
        else {
            start_trriger = true;
            start_timer(setting->Per_Trigger_Time / 1000.0f, 0);
            //mouse->Left_click();
            //flash_mirror();
            check_trigger = false;
       //     return 0;
        }
    }
    else {
        check_trigger = false;
    }

    if (trigger_produce_cancel_move->status() && controller->GetTriggerStatus() && (controller->GetOpenedMirrorStatus() || !controller->GetMovingStatus())) {
        //this->screenshot->x_size = 80;  // 慢增长
        //this->screenshot->y_size = 80;
        if (start_trriger)
            mouse->Left_click();
        //std::cout << trigger_produce_cancel_move->status() << std::endl;
        return 0;
    }

    if (start_trriger)
    {
        float sensitivity = 0.9;
        if (controller->GetAimHeadStatus() && res == 1)
            y -= h / 3;
        mouse->MoveRP(int(x / 1.0), int(y / 1.0));
        mouse->Left_click();
        return 0;
    }

    //bool dist = (res == 0 && w <= 25) || (res == 1 && w <= 75);
    bool dist = true;
    bool dist_2 = (res == 0 && w <= 10) || (res == 1 && w <= 25);

    MoveThreadArgs move_args(wm, hm, initial_st, dist, dist_2, oky);
    move_args.ans[0] = x;
    move_args.ans[1] = y;
    move_args.ans[2] = w;
    move_args.ans[3] = h;
    move_args.ans[4] = res;

    ++(this->mutex_move) %= 10;

    
    if (controller->GetMaxAimStatus() || (controller->GetTriggerStatus() && controller->GetOpenedMirrorStatus() && !controller->GetAimHeadStatus()))
        pid_x->Kp = setting->MP_Pid_P,  pid_x->Ki = setting->MP_Pid_I, pid_x->Kd = setting->MP_Pid_D, pid_x->imax = setting->MP_Pid_Imax;
    else {
        pid_x->Kp = setting->HP_Pid_P, pid_x->Ki = setting->HP_Pid_I, pid_x->Kd = setting->HP_Pid_D, pid_x->imax = setting->HP_Pid_Imax;
        std::thread t2(&Move::thread_move_controller, this, move_args);
        t2.detach();
        return -1;
    }
    if (controller->GetAimHeadStatus()) {
        if ((res == 0 && abs(x) >= w * 3.5) || (res == 1 && abs(x) >= w * 1.5) ||
            (res == 0 && abs(oky) >= h * 1.5) || (res == 1 && abs(oky) >= h) ||
            ((controller->GetMovingStatus() || !dist) && !controller->GetTriggerStatus() && !controller->GetMaxAimStatus())) {
            if ((res == 0 && abs(x) >= w * 3.5) || (res == 1 && abs(x) >= w * 1.5) ||
                (res == 0 && abs(oky) >= h * 1.5) || (res == 1 && abs(oky) >= h))
            {
                move_args.ans[0] = x * 1.5;
                move_args.ans[1] = y;
                move_args.ans[2] = w;
                move_args.ans[3] = h;
                move_args.ans[4] = res;
                std::thread t2(&Move::thread_move_controller, this, move_args);
                t2.detach();
                return -1;
            }
            x *= setting->MP_First_X_Rate;

            if (x < 0) {
                x = max(-wm * setting->MP_MaxMaximum_X_Multiple, x);
            }
            else {
                x = min(wm * setting->MP_MaxMaximum_X_Multiple, x);
            }
            if (res == 1 && h > w) {
                y = y - h / 3 * initial_st;
            }
            if (y < 0) {
                y = max(-hm * setting->MP_MaxMaximum_Y_Multiple, y);
            }
            else {
                y = min(hm * setting->MP_MaxMaximum_Y_Multiple, y);
            }
            if (controller->GetMaxAimStatus() || controller->GetOpenedMirrorStatus())
                x *= setting->MP_OpenedMirror_X_Rate;
            if (controller->AimStatus()) {
                mouse->MoveR(int(x), int(y));
            }
        }
        else {
            x = pid_x->cmd_pid(x, setting->Average_One_round_Time / 1000.0f, w, wm * setting->MP_MaxMaximum_X_Multiple);
            if (x < 0) {
                x = max(-wm * setting->MP_MaxMaximum_X_Multiple, x);
            }
            else {
                x = min(wm * setting->MP_MaxMaximum_X_Multiple, x);
            }
            if (res == 1 && h > w) {
                y = y - h / 3 * initial_st;
            }
            if (y < 0) {
                y = max(-hm * setting->MP_MaxMaximum_Y_Multiple, y);
            }
            else {
                y = min(hm * setting->MP_MaxMaximum_Y_Multiple, y);
            }
            if (controller->GetLeftPressed()) {
                y = 0;
            }
            y *= setting->MP_Y_Rate;
            /*if (okx <= w / 5 && !y) {
                return 0;
            }*/
           
            if (controller->GetOpenedMirrorStatus())
                x *= setting->MP_OpenedMirror_X_Rate;
            if (controller->AimStatus()) {
                mouse->MoveR(int(x), round(y));
            }
        }
    }
    else {
        if ((res == 0 && abs(x) >= w * 1.5) || (res == 1 && abs(x) >= w * 0.8) || abs(oky) >= h * 1.5 ||
            (controller->GetMovingStatus() && !controller->GetMaxAimStatus())) {

            x *= setting->MP_First_X_Rate;

            int doubleVal = controller->GetTriggerStatus() ? 2 : 2;
            if (controller->GetMovingStatus()) {
                x *= setting->BR_MoveingAndNotMaxAim_Rate;
            }
            if (x < 0 && !controller->GetOpenedMirrorStatus()) {
                x = max(-wm * setting->BR_MaxMaximum_X_Multiple * doubleVal, x);
            }
            else if (x > 0 && !controller->GetOpenedMirrorStatus()) {
                x = min(wm * setting->BR_MaxMaximum_X_Multiple * doubleVal, x);
            }
            if (y < 0) {
                y = max(-hm, y);
            }
            else {
                y = min(hm, y);
            }
            if (controller->AimStatus()) {
                mouse->MoveR(int(x), round(y));
            }
        }
        else {
            int doubleVal = controller->GetTriggerStatus() ? 2 : 2;
            x = pid_x_J->cmd_pid(x, setting->Average_One_round_Time / 1000.0f, w, wm * setting->BR_MaxMaximum_X_Multiple * doubleVal);
            if (controller->GetOpenedMirrorStatus())
                x *= setting->BP_OpenedMirror_X_Rate;
            
            if (x < 0 && dist_2) {
                x = max(-wm * setting->BR_MaxMaximum_X_Multiple * doubleVal, x);
            }
            else if (x > 0 && dist_2) {
                x = min(wm * setting->BR_MaxMaximum_X_Multiple * doubleVal, x);
            }
            if (y < 0) {
                y = max(-hm * doubleVal, y);
            }
            else {
                y = min(hm * doubleVal, y);
            }
            if (controller->GetLeftPressed()) {
                y = 0;
            }
            if (controller->AimStatus()) {
                mouse->MoveR(int(x), round(y));
            }
        }

    }
    return sqrt(int(x)*int(x)+int(y)*int(y));
}