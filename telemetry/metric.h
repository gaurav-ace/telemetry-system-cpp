#pragma once

#include <atomic>
#include <string>


/*
 * Base Metric class
 * only name, value handling will be in derived class
 */

class Metric {

public:
	explicit Metric(std::string name)
		: name_(std::move(name)) {}

	virtual ~Metric() = default;  // ??

        const std::string& name() const {
		return name_;
	}

private:
	std::string name_;

};


/*
 * Counter metric.
 * - Monotonically increasing
 * - Thread-safe via atomic
 */
class Counter : public Metric {
public:
    explicit Counter(const std::string& name)
        : Metric(name), value_(0) {}

    // Increment counter by 1
    void inc() {
        value_.fetch_add(1, std::memory_order_relaxed);
    }

    // Increment by arbitrary value
    void add(uint64_t v) {
        value_.fetch_add(v, std::memory_order_relaxed);
    }

    // Read current value
    uint64_t value() const {
        return value_.load(std::memory_order_relaxed);
    }

private:
    std::atomic<uint64_t> value_;
};


/*
 * Gauge metric.
 * - Can go up/down
 * - Thread-safe via atomic
 */
class Gauge : public Metric {
public:
    explicit Gauge(const std::string& name)
        : Metric(name), value_(0) {}

    // Set gauge to value
    void set(int64_t v) {
        value_.store(v, std::memory_order_relaxed);
    }

    // Read current value
    int64_t value() const {
        return value_.load(std::memory_order_relaxed);
    }

private:
    std::atomic<int64_t> value_;
};


