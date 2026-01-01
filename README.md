# telemetry-system-cpp


phase 1.1 :
telemetry-system-cpp/
├── logger/
│   ├── AsyncLogger.h
│   ├── AsyncLogger.cpp
├── applications/
│   └── logger_demo_app.cpp
└── CMakeLists.txt



phase 1.2: 

Application threads
   |
   | counter.inc()
   | gauge.set()
   v
┌────────────────────┐
│ MetricRegistry     │
│  - map<string, *>  │
└────────┬───────────┘
         |
         | snapshot()
         v
 Telemetry exporter thread

structure : 

telemetry-system-cpp/
├── logger/
├── telemetry/
│   ├── metric.h
│   ├── metric_registry.h
│   └── metric_registry.cpp
├── applications/
│   └── metrics_demo_app.cpp
└── CMakeLists.txt


phase 1.3:

Application threads
  |
  | counter.inc(), gauge.set()
  v
MetricRegistry
  |
  | snapshot()  (periodic)
  v
TelemetryExporter (thread)
  |
  | export(batch)
  v
AsyncLogger   ← (temporary sink)



telemetry-system-cpp/
├── telemetry/
│   ├── metric.h
│   ├── metric_registry.*
│   ├── telemetry_exporter.h   ← NEW
│   ├── log_exporter.h         ← NEW
│   ├── log_exporter.cpp
│   ├── telemetry_service.h    ← NEW (export loop)
│   └── telemetry_service.cpp
├── examples/
│   └── telemetry_demo.cpp




