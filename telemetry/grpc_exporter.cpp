#include "grpc_exporter.h"

GrpcExporter::GrpcExporter(
    std::shared_ptr<grpc::Channel> channel)
    : stub_(telemetry::TelemetryService::NewStub(channel)) {}

void GrpcExporter::exportMetrics(
    const std::vector<MetricRegistry::Snapshot>& snapshot) {

    telemetry::MetricBatch batch;

    for (const auto& s : snapshot) {
        telemetry::Metric* metric = batch.add_metrics();
        metric->set_name(s.name);

        if (s.is_counter) {
            metric->set_counter_value(s.counter_value);
        } else {
            metric->set_gauge_value(s.gauge_value);
        }
    }

    telemetry::Ack ack;
    grpc::ClientContext context;

    grpc::Status status =
        stub_->PushMetrics(&context, batch, &ack);

    if (!status.ok() || !ack.success()) {
        // In real systems:
        // retry, drop, or log failure
    }
}

