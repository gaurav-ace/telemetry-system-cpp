#pragma once

#include "metric_registry.h"
#include <vector>

/*
 * TelemetryExporter
 *
 * Abstract interface.
 * Allows different export backends (log, gRPC, etc).
 */
class TelemetryExporter {
public:
    virtual ~TelemetryExporter() = default;

    // Export one snapshot batch
    virtual void exportMetrics(
        const std::vector<MetricRegistry::Snapshot>& snapshot
    ) = 0;
};

