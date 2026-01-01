#pragma once

#include "metric.h"
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

/*
 * MetricRegistry
 *
 * Owns all metrics.
 * Provides:
 *  - creation/access of counters and gauges
 *  - snapshot of all metrics
 */
class MetricRegistry {
public:
    // Get or create a counter
    std::shared_ptr<Counter> counter(const std::string& name);

    // Get or create a gauge
    std::shared_ptr<Gauge> gauge(const std::string& name);

    // Snapshot all metrics (for exporter thread)
    struct Snapshot {
        std::string name;
        uint64_t counter_value;
        int64_t gauge_value;
        bool is_counter;
    };

    std::vector<Snapshot> snapshot() const;

private:
    // Mutex protects registry structure, NOT metric values
    mutable std::mutex mutex_;

    std::unordered_map<std::string, std::shared_ptr<Counter>> counters_;
    std::unordered_map<std::string, std::shared_ptr<Gauge>> gauges_;
};

