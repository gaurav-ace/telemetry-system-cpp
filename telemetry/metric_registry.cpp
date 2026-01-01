#include "metric_registry.h"

std::shared_ptr<Counter>
MetricRegistry::counter(const std::string& name) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = counters_.find(name);
    if (it != counters_.end()) {
        return it->second;
    }

    // Create new counter if not exists
    auto ctr = std::make_shared<Counter>(name);
    counters_[name] = ctr;
    return ctr;
}

std::shared_ptr<Gauge>
MetricRegistry::gauge(const std::string& name) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = gauges_.find(name);
    if (it != gauges_.end()) {
        return it->second;
    }

    // Create new gauge if not exists
    auto g = std::make_shared<Gauge>(name);
    gauges_[name] = g;
    return g;
}

std::vector<MetricRegistry::Snapshot>
MetricRegistry::snapshot() const {
    std::vector<Snapshot> result;

    std::lock_guard<std::mutex> lock(mutex_);

    // Copy all counters
    for (const auto& [name, ctr] : counters_) {
        result.push_back({
            name,
            ctr->value(),
            0,
            true
        });
    }

    // Copy all gauges
    for (const auto& [name, g] : gauges_) {
        result.push_back({
            name,
            0,
            g->value(),
            false
        });
    }

    return result;
}

