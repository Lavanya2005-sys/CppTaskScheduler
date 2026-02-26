#include "scheduler.h"
#include <iostream>
#include <numeric>
#include <algorithm> 
#include <fstream>

TaskScheduler::TaskScheduler(size_t threadCount)
    : stop(false), tasksExecuted(0) {
    workers.reserve(threadCount);
}

TaskScheduler::~TaskScheduler() {
    shutdown();
}

void TaskScheduler::start() {
    startTime = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < workers.capacity(); ++i) {
        workers.emplace_back(&TaskScheduler::workerThread, this);
    }
}

void TaskScheduler::workerThread() {
    while (true) {
        Task task;

        {
            std::unique_lock<std::mutex> lock(queueMutex);

            condition.wait(lock, [this] {
                return stop || !taskQueue.empty();
            });

            if (stop && taskQueue.empty())
                return;

            task = taskQueue.top();
            taskQueue.pop();
        }

        auto startExec = std::chrono::high_resolution_clock::now();
        task.func();
        auto endExec = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::micro> latency =
            endExec - task.enqueueTime;

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            taskLatencies.push_back(latency.count());
        }

        tasksExecuted++;
    }
}

void TaskScheduler::addTask(std::function<void()> taskFunc, int priority) {
    Task task;
    task.func = taskFunc;
    task.priority = priority;
    task.enqueueTime = std::chrono::high_resolution_clock::now();

    {
        std::lock_guard<std::mutex> lock(queueMutex);
        taskQueue.push(task);
    }

    condition.notify_one();
}

void TaskScheduler::shutdown() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stop = true;
    }

    condition.notify_all();

    for (auto &worker : workers) {
        if (worker.joinable())
            worker.join();
    }

    endTime = std::chrono::high_resolution_clock::now();
}


void TaskScheduler::printMetrics() {
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        endTime - startTime
    );

    double seconds = duration.count() / 1000.0;

    double avgLatency = std::accumulate(
        taskLatencies.begin(),
        taskLatencies.end(),
        0.0
    ) / taskLatencies.size();

    double maxLatency = *std::max_element(
        taskLatencies.begin(),
        taskLatencies.end()
    );

    double minLatency = *std::min_element(
        taskLatencies.begin(),
        taskLatencies.end()
    );

    std::cout << "\n===== Performance Metrics =====\n";
    std::cout << "Total Tasks Executed: " << tasksExecuted << "\n";
    std::cout << "Total Time: " << seconds << " seconds\n";
    std::cout << "Average Latency: " << avgLatency << " microseconds\n";
    std::cout << "Max Latency: " << maxLatency << " microseconds\n";
    std::cout << "Min Latency: " << minLatency << " microseconds\n";
    std::cout << "================================\n";

    // Save to JSON
    std::ofstream file("metrics.json");
    file << "{\n";
    file << "  \"total_tasks\": " << tasksExecuted << ",\n";
    file << "  \"total_time_sec\": " << seconds << ",\n";
    file << "  \"average_latency_us\": " << avgLatency << ",\n";
    file << "  \"max_latency_us\": " << maxLatency << ",\n";
    file << "  \"min_latency_us\": " << minLatency << "\n";
    file << "}\n";
    file.close();

    std::cout << "Metrics exported to metrics.json\n";
}