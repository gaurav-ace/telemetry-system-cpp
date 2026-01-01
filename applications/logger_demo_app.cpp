#include "logger/AsyncLogger.h"
#include <thread>

int main() {
    AsyncLogger logger("telemetry.log");

    // Simulate multiple log calls
    for (int i = 0; i < 10000; ++i) {
        logger.log("INFO", "log message " + std::to_string(i));
    }

    // Give logger time to flush before exit
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    return 0;
}

