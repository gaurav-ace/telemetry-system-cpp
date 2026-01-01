#pragma once

#include "telemetry_exporter.h"
#include "telemetry.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <memory>

class GrpcExporter : public TelemetryExporter {
public:
    explicit GrpcExporter(
        std::shared_ptr<grpc::Channel> channel
    );

    void exportMetrics(
        const std::vector<MetricRegistry::Snapshot>& snapshot
    ) override;

private:
    std::unique_ptr<telemetry::TelemetryService::Stub> stub_;
};

