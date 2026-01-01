#include "telemetry.pb.h"
#include "telemetry.grpc.pb.h"

#include <grpcpp/grpcpp.h>
#include <iostream>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class TelemetryServiceImpl final
    : public telemetry::TelemetryService::Service {
public:
    Status PushMetrics(ServerContext*,
                       const telemetry::MetricBatch* request,
                       telemetry::Ack* response) override {

        std::cout << "Received telemetry batch:\n";

        for (const auto& metric : request->metrics()) {
            if (metric.has_counter_value()) {
                std::cout << "  "
                          << metric.name()
                          << " counter="
                          << metric.counter_value()
                          << "\n";
            } else if (metric.has_gauge_value()) {
                std::cout << "  "
                          << metric.name()
                          << " gauge="
                          << metric.gauge_value()
                          << "\n";
            }
        }

        response->set_success(true);
        return Status::OK;
    }
};

int main() {
    std::string address("0.0.0.0:50051");

    TelemetryServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(address,
                             grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Telemetry server listening on " << address << "\n";

    server->Wait();
    return 0;
}

