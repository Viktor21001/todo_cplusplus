#include "TaskScheduler.h"
#include <iostream>
#include <functional>
#include <ctime>
#include <chrono>
#include <thread>

int main() {
    TaskScheduler scheduler;

    // Функция для выполнения задачи
    auto printTask = [](const std::string& message) {
        std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::cout << "[" << std::ctime(&now) << "] " << message << std::endl;
    };

    // Запланируем задачи с разными задержками
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    // Задача через 5 секунд
    scheduler.Add([=]() { printTask("Task 1: This should run after 5 seconds"); }, now + 5);

    // Задача через 10 секунд
    scheduler.Add([=]() { printTask("Task 2: This should run after 10 seconds"); }, now + 10);

    // Задача через 2 секунды
    scheduler.Add([=]() { printTask("Task 3: This should run after 2 seconds"); }, now + 2);

    // Подождем 15 секунд, чтобы все задачи успели выполниться
    std::this_thread::sleep_for(std::chrono::seconds(15));

    return 0;
}
