#include "telemetry_service.h"
#include <chrono>

TelemetryService::TelemetryService(
    MetricRegistry& registry,
    std::shared_ptr<TelemetryExporter> exporter,
    int interval_ms)
    : registry_(registry),
      exporter_(std::move(exporter)),
      interval_ms_(interval_ms) {

    // Start background exporter thread
    worker_ = std::thread(&TelemetryService::run, this);
}

void TelemetryService::run() {
    while (running_) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(interval_ms_)
        );

        // Snapshot metrics
        auto snapshot = registry_.snapshot();

        // Export snapshot
        exporter_->exportMetrics(snapshot);
    }
}

TelemetryService::~TelemetryService() {
    running_ = false;
    if (worker_.joinable()) {
        worker_.join();
    }
}

