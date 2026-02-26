#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>

struct Task {
    std::function<void()> func;
    int priority;
    std::chrono::high_resolution_clock::time_point enqueueTime;

    bool operator<(const Task& other) const {
        return priority < other.priority; // max-heap (higher priority first)
    }
};

class TaskScheduler {
private:
    std::vector<std::thread> workers;
    std::priority_queue<Task> taskQueue;

    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> stop;

    std::atomic<int> tasksExecuted;
    std::vector<double> taskLatencies;

    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;

    void workerThread();

public:
    TaskScheduler(size_t threadCount);
    ~TaskScheduler();

    void start();
    void addTask(std::function<void()> task, int priority);
    void shutdown();
    void printMetrics();
};

#endif