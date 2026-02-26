#include "scheduler.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

int main(int argc, char* argv[]) {

    if (argc != 3) {
        std::cout << "Usage: ./scheduler.exe <threads> <tasks>\n";
        return 1;
    }

    int threadCount = std::atoi(argv[1]);
    int totalTasks = std::atoi(argv[2]);

    TaskScheduler scheduler(threadCount);

    std::cout << "Starting scheduler with "
              << threadCount
              << " worker threads.\n";

    scheduler.start();

    for (int i = 0; i < totalTasks; ++i) {

        int priority = rand() % 3; // 0,1,2 priorities

        scheduler.addTask([i]() {
            double result = 0;
            for (int j = 0; j < 5000; ++j) {
                result += std::sqrt(j * i + 1);
            }
        }, priority);
    }

    scheduler.shutdown();
    scheduler.printMetrics();

    return 0;
}