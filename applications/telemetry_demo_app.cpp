#include "logger/AsyncLogger.h"
#include "telemetry/grpc_exporter.h"
#include "telemetry/telemetry_service.h"
#include "telemetry/metric_registry.h"
#include <thread>

#include <memory>
#include <grpcpp/grpcpp.h>


int main() {
    // Shared logger
    auto logger = std::make_shared<AsyncLogger>("telemetry.log");

    // Metric registry
    MetricRegistry registry;

    // Metrics
    auto packets = registry.counter("packets_rx");
    auto cpu = registry.gauge("cpu_usage");

    // Exporter
    //auto exporter = std::make_shared<LogExporter>(logger);

    auto channel = grpc::CreateChannel(
       "localhost:50051",
       grpc::InsecureChannelCredentials()
    );

    auto exporter = std::make_shared<GrpcExporter>(channel);


    // Telemetry service: snapshot every 1 second
    TelemetryService telemetry(
        registry,
        exporter,
        1000
    );

    // Simulate application workload
    for (int i = 0; i < 10; ++i) {
        packets->add(100);
        cpu->set(i * 10);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}

