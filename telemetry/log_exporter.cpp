#include "log_exporter.h"

LogExporter::LogExporter(std::shared_ptr<AsyncLogger> logger)
    : logger_(std::move(logger)) {}

void LogExporter::exportMetrics(
    const std::vector<MetricRegistry::Snapshot>& snapshot) {

    for (const auto& s : snapshot) {
        if (s.is_counter) {
            logger_->log(
                "METRIC",
                s.name + " counter=" + std::to_string(s.counter_value)
            );
        } else {
            logger_->log(
                "METRIC",
                s.name + " gauge=" + std::to_string(s.gauge_value)
            );
        }
    }
}

