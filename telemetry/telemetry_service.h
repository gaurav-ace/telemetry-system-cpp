#pragma once

#include "metric_registry.h"
#include "telemetry_exporter.h"
#include <atomic>
#include <memory>
#include <thread>

/*
 * TelemetryService
 *
 * Periodically snapshots metrics and exports them.
 */
class TelemetryService {
public:
    TelemetryService(
        MetricRegistry& registry,
        std::shared_ptr<TelemetryExporter> exporter,
        int interval_ms
    );

    ~TelemetryService();

private:
    void run();

private:
    MetricRegistry& registry_;   // registry owned by app
    std::shared_ptr<TelemetryExporter> exporter_;

    int interval_ms_;
    std::atomic<bool> running_{true};
    std::thread worker_;
};

