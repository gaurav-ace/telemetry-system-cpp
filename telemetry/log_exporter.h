#pragma once

#include "telemetry_exporter.h"
#include "logger/AsyncLogger.h"
#include <memory>

/*
 * LogExporter
 *
 * Exports telemetry snapshots to the async logger.
 * This is a temporary backend used before gRPC.
 */
class LogExporter : public TelemetryExporter {
public:
    explicit LogExporter(std::shared_ptr<AsyncLogger> logger);

    void exportMetrics(
        const std::vector<MetricRegistry::Snapshot>& snapshot
    ) override;

private:
    std::shared_ptr<AsyncLogger> logger_;
};

