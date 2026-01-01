#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <fstream>
#include <queue>
#include <string>
#include <thread>


/*
 * AsyncLogger
 *
 * - Thread-safe
 * - Non-blocking log() API
 * - Single background thread does file I/O
 */
class AsyncLogger {
public:
	// constructor opens a log file and starts writing
	// single param constructor is made explicit so that 
	// compiler doesn't perform implict conversion
      	explicit AsyncLogger(const std::string& filename);
        
	// Disable copy constructor and '=' operator 
	// as the logger owns the thread and file
	AsyncLogger(const AsyncLogger&) = delete;
	AsyncLogger& operator=(const AsyncLogger&) = delete;

	//fast logging API
	void log(const std::string& level, const std::string& message);  

	//Graceful shutdown
	~AsyncLogger();

private:
	std::condition_variable _cv; // signals worker
	std::mutex _mutex; // protects queue
	std::queue<std::string> _queue; // for log message
	std::ofstream _file; // log file stream

        void workerLoop();
        
	std::thread _worker; // background thread
	
	std::atomic<bool> _running{true}; // shutdown flag

};




