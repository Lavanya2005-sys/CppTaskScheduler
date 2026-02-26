#  High-Performance Multithreaded C++ Task Scheduler

A configurable, priority-based multithreaded task scheduler built using modern C++ (C++17).  
This project demonstrates systems-level programming concepts including thread pooling, synchronization primitives, performance instrumentation, and structured runtime metrics export.

---

##  Overview

This project implements a high-performance runtime engine capable of:

- Executing tasks concurrently using a configurable thread pool
- Scheduling tasks based on priority
- Measuring per-task latency
- Benchmarking throughput
- Exporting performance metrics to a structured JSON file

The primary goal is to showcase strong C++ fundamentals, concurrency control, and performance-aware system design.

---

##  Architecture Flow

```
CLI Input (Threads, Tasks)
        ↓
TaskScheduler Engine
        ↓
Priority Queue (Max-Heap)
        ↓
Worker Threads (Thread Pool)
        ↓
Task Execution
        ↓
Performance Metrics Calculation
        ↓
metrics.json Export
```

---

##  Core Components

### 1️⃣ Thread Pool
- Configurable number of worker threads
- Efficient blocking using `std::condition_variable`
- Avoids CPU waste from busy waiting

### 2️⃣ Priority-Based Scheduling
- Uses `std::priority_queue`
- Higher priority tasks execute first
- Insert complexity: O(log n)

### 3️⃣ Synchronization Layer
- `std::mutex` protects shared task queue
- `std::condition_variable` signals workers
- `std::atomic` safely tracks execution count

### 4️⃣ Performance Instrumentation
- Per-task latency measurement (microseconds)
- Total runtime tracking
- Min / Max / Average latency computation
- JSON metrics export for observability

---

##  Features

- ✅ Configurable thread count (CLI)
- ✅ Configurable task volume (CLI)
- ✅ Priority scheduling
- ✅ Thread-safe queue management
- ✅ Latency measurement
- ✅ Throughput benchmarking
- ✅ JSON metrics export
- ✅ Clean modular architecture
- ✅ Optimized compilation (`-O2`)

---

##  Platform & Environment

Tested On:
- Windows 11
- MSYS2 UCRT64 environment
- g++ (C++17 compliant)

Compiler flags used:

```bash
-std=c++17 -Wall -Wextra -O2
```

---

##  Project Structure

```
CppTaskScheduler/
│
├── src/
│   ├── scheduler.h
│   ├── scheduler.cpp
│   └── main.cpp
│
├── metrics.json
├── Makefile
└── README.md
```

---

##  How To Build

Open MSYS2 UCRT64 terminal.

Navigate to the project directory:

```bash
cd "/e/task scheduler/CppTaskScheduler"
```

Compile using:

```bash
g++ -std=c++17 -Wall -Wextra -O2 src/*.cpp -o scheduler.exe
```

If compilation succeeds, it generates:

```
scheduler.exe
```

---

## ▶ How To Run

Run the executable with:

```bash
./scheduler.exe <threads> <tasks>
```

Example:

```bash
./scheduler.exe 8 5000
```

Where:
- `8` = number of worker threads
- `5000` = number of tasks to execute

---

##  Sample Execution Output

```
./scheduler.exe 8 5000

Starting scheduler with 8 worker threads.

===== Performance Metrics =====
Total Tasks Executed: 5000
Total Time: 0.009 seconds
Average Latency: 4458.78 microseconds
Max Latency: 8526.6 microseconds
Min Latency: 8.4 microseconds
================================

Metrics exported to metrics.json
```

---

##  Example metrics.json

```json
{
  "total_tasks": 5000,
  "total_time_sec": 0.009,
  "average_latency_us": 4458.78,
  "max_latency_us": 8526.6,
  "min_latency_us": 8.4
}
```

---

##  Key Engineering Concepts Demonstrated

- Thread lifecycle management
- Mutex-based synchronization
- Condition variable signaling
- Avoidance of busy-waiting
- Atomic operations for thread safety
- O(log n) priority scheduling
- Runtime performance instrumentation
- Structured metrics logging

---

## Performance Characteristics

- Efficient blocking synchronization
- Minimal contention window
- Scales with increased thread count
- High task throughput
- Deterministic shutdown

---

##  Future Enhancements

- Dynamic thread scaling
- Lock contention metrics
- Lock-free queue implementation
- Work-stealing scheduler
- Unit testing suite
