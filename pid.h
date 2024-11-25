#pragma once
#ifndef PID_H
#define PID_H

#include "setting.h" // For std::abs

class PID {
public:
    double speed;
    double Kp;
    double Ki;
    double Kd;
    double imax;
    double p;
    double i;
    double d;
    double last;
    double move;

    bool is_move=false;

    PID(double kp, double ki, double kd, double imax, double speed);

    double cmd_pid(double err, double dt, double w, double wm);

    bool target_is_move();
};

PID sz_zz();

#endif // PID_H
