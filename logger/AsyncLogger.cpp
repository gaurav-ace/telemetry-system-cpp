#include "AsyncLogger.h"
#include <chrono>
#include <iostream>

AsyncLogger::AsyncLogger(const std::string& filename) {
    // Open file in append mode
    _file.open(filename, std::ios::out | std::ios::app);

    if (!_file.is_open()) {
        throw std::runtime_error("Failed to open log file");
    }

    // Start background worker thread
    _worker = std::thread(&AsyncLogger::workerLoop, this);
}

void AsyncLogger::log(const std::string& level,
                      const std::string& message) {
    // Build log line (cheap string ops)
    auto now = std::chrono::system_clock::now();
    auto ts = std::chrono::duration_cast<std::chrono::milliseconds>(
                  now.time_since_epoch())
                  .count();

    std::string logLine =
        std::to_string(ts) + " [" + level + "] " + message + "\n";

    {
        // Critical section: push into queue
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.push(std::move(logLine));
    }

    // Notify worker thread that data is available
    _cv.notify_one();
}

void AsyncLogger::workerLoop() {
    while (_running) {
        std::unique_lock<std::mutex> lock(_mutex);

        // Wait until:
        // - new log arrives
        // - OR shutdown requested
        _cv.wait(lock, [&] {
            return !_queue.empty() || !_running;
        });

        // Drain queue
        while (!_queue.empty()) {
            _file << _queue.front();
            _queue.pop();
        }

        // Release lock before I/O flush
        lock.unlock();

        // Flush written logs to OS buffers
        _file.flush();
    }

    // Shutdown path: flush remaining logs
    while (true) {
        std::unique_lock<std::mutex> lock(_mutex);
        if (_queue.empty()) {
            break;
        }

        _file << _queue.front();
        _queue.pop();
    }

    _file.flush();
}

AsyncLogger::~AsyncLogger() {
    // Signal worker thread to stop
    _running = false;

    // Wake up worker if waiting
    _cv.notify_one();

    // Join worker thread
    if (_worker.joinable()) {
        _worker.join();
    }

    // Close file
    _file.close();
}




