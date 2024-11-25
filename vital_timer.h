#pragma once
#include <thread>
#include <chrono>
#include <mutex>

class AimTimer {
private:
    bool is_mss;
    //bool happen_enevt = false;
    int goods_count;
    bool mutex_lock;
    const int max_cnt = 8;
    std::mutex mtx;

public:

    AimTimer() : is_mss(false), goods_count(0), mutex_lock(false) {}
    
    void mss_timer(double seconds) {
        // 使用 lock_guard 锁定 mtx
        {
           std::lock_guard<std::mutex> lock(mtx);
       //     mtx.lock();
            is_mss = true;
            goods_count += 1;
         //   mtx.unlock();
        } // lock_guard 自动解锁

        std::this_thread::sleep_for(std::chrono::duration<double>(seconds));

        {
            std::lock_guard<std::mutex> lock(mtx);
            goods_count -= 1;
           // std::cout << "goods_count: " << goods_count << std::endl;

            if (goods_count <= 0) {
                is_mss = false;
            }
        } // lock_guard 自动解锁
    }

    void start_timer(double seconds) {
        if(goods_count >= max_cnt)
            return;
        std::thread t(&AimTimer::mss_timer, this, seconds);
        t.detach();
    }

    bool status() {
        // std::lock_guard<std::mutex> lock(mtx);
        return is_mss;
    }

    void set_status(bool status) {
        // std::lock_guard<std::mutex> lock(mtx);
        is_mss = status;
    }

    void lock() {
        while (mutex_lock) {
            std::this_thread::sleep_for(std::chrono::duration<double>(0.01));
            //std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        mutex_lock = true;
    }
    void unlock() {
        mutex_lock = false;
    }
};