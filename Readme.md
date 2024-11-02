# TaskScheduler

- Очередь с приоритетом: Задачи хранятся в очереди priority_queue, упорядоченной по времени выполнения. Самая "срочная" задача находится наверху очереди.

- Рабочий поток: Функция run в рабочем потоке постоянно проверяет очередь задач. Когда время выполнения задачи наступает, задача извлекается из очереди и выполняется. Если задачи нет или ее время еще не наступило, поток ожидает.

- Синхронизация: Мьютекс и условная переменная (cv) используются для обеспечения корректного доступа к очереди задач и для "пробуждения" потока, когда добавляется новая задача.

# Запуск

1. g++ main.cpp -o TaskScheduler -lpthread
2. ./TaskScheduler
