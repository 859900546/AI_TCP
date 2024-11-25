#include "pid.h"
#include <algorithm> // For std::max and std::min
#include <cmath> // For std::abs

extern Setting* setting;

PID::PID(double kp, double ki, double kd, double imax, double speed) :
    speed(speed), Kp(kp), Ki(ki), Kd(kd), imax(imax), p(0), i(0), d(0), last(0), move(0) {}

double PID::cmd_pid(double err, double dt, double w, double wm) {
    p = err * Kp;
    i = i + err * Ki * dt;
    this->is_move = (std::abs(err) > std::abs(last) - setting->PID_Offset * std::abs(move));
    if (!this->is_move) {
        i = 0;
    }
    if (i > imax) {
        i = imax;
    }
    else if (i < -imax) {
        i = -imax;
    }
    if (this->is_move) {
        d = (err - last + move) * Kd / dt;
    }
    last = err;
    move = p + i + d;
    if (move < 0) {
        move = std::max(-wm, move);
    }
    else {
        move = std::min(wm, move);
    }
    return p + i + d;
}

bool PID::target_is_move()
{
    return this->is_move;
}

PID sz_zz() {
    double p = 0.38;
    double i = 2;
    double d = 0;
    double ima = 1.8;
    double speed = 1.0;
    return PID(p, i, d, ima, speed);
}
