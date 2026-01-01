#include "telemetry/metric_registry.h"
#include <iostream>
#include <thread>

int main() {
    MetricRegistry registry;

    auto packets = registry.counter("packets_rx");
    auto cpu = registry.gauge("cpu_usage");

    // Simulate updates
    for (int i = 0; i < 10; ++i) {
        packets->inc();
        cpu->set(i * 10);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Take snapshot
    auto snap = registry.snapshot();

    for (const auto& s : snap) {
        if (s.is_counter) {
            std::cout << s.name << " = " << s.counter_value << "\n";
        } else {
            std::cout << s.name << " = " << s.gauge_value << "\n";
        }
    }

    return 0;
}

