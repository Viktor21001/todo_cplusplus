#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H

#include <iostream>
#include <queue>
#include <functional>
#include <chrono>
#include <ctime>
#include <thread>
#include <mutex>
#include <condition_variable>

class TaskScheduler {
public:
    TaskScheduler() : stop(false) {
        worker = std::thread(&TaskScheduler::run, this);
    }

    ~TaskScheduler() {
        {
            std::unique_lock<std::mutex> lock(mtx);
            stop = true;
            cv.notify_all();
        }
        if (worker.joinable()) {
            worker.join();
        }
    }

    void Add(std::function<void()> task, std::time_t timestamp) {
        std::unique_lock<std::mutex> lock(mtx);
        tasks.emplace(timestamp, task);
        cv.notify_all();
    }

private:
    struct Task {
        std::time_t timestamp;
        std::function<void()> task;

        Task(std::time_t ts, std::function<void()> t) : timestamp(ts), task(std::move(t)) {}

        bool operator>(const Task& other) const {
            return timestamp > other.timestamp;
        }
    };

    void run() {
        while (true) {
            std::unique_lock<std::mutex> lock(mtx);
            if (stop && tasks.empty()) {
                break;
            }
            if (tasks.empty()) {
                cv.wait(lock);
            } else {
                auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                if (tasks.top().timestamp <= now) {
                    auto task = tasks.top().task;
                    tasks.pop();
                    lock.unlock();
                    task();
                } else {
                    cv.wait_until(lock, std::chrono::system_clock::from_time_t(tasks.top().timestamp));
                }
            }
        }
    }

    std::priority_queue<Task, std::vector<Task>, std::greater<Task>> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    bool stop;
    std::thread worker;
};

#endif
