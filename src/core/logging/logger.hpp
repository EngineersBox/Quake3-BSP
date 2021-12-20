#pragma once

#ifndef Q_LOGGER_H
#define Q_LOGGER_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <vector>
#include <string>
#include <ctime>
#include <iostream>

namespace QuakeLogger {

	std::string getCurrentTime(const char* date_format) {
		std::time_t rawtime;
		char date_buffer[80];
		std::time(&rawtime);
		struct tm* timeinfo = std::localtime(&rawtime);
		std::strftime(date_buffer, sizeof(date_buffer), date_format, timeinfo);
		return {date_buffer};
	}

	std::string getLogFileName() {
		return "logs/quake_log_" + getCurrentTime("%Y-%m-%d_%H-%M-%S") + ".log";
	}

	void initLogger() {
		try {
			std::vector<spdlog::sink_ptr> sinks;

			auto stdoutSink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
			stdoutSink->set_level(spdlog::level::info);
			sinks.push_back(stdoutSink);

			auto basicFileSink = std::make_shared<spdlog::sinks::basic_file_sink_st>(getLogFileName());
			basicFileSink->set_level(spdlog::level::trace);
			sinks.push_back(basicFileSink);

			std::shared_ptr<spdlog::logger> combinedLogger = std::make_shared<spdlog::logger>("logger", begin(sinks), end(sinks));
			spdlog::set_default_logger(combinedLogger);
			combinedLogger->set_level(spdlog::level::trace);
		} catch (const spdlog::spdlog_ex& ex) {
			std::cout << "Logger init failed: " << ex.what() << std::endl;
			exit(1);
		}
		spdlog::set_pattern("[%Y/%m/%d %H:%M:%S] [%n] [%^%l%$] [thread %t] :: %v");
	}
}

#endif // Q_LOGGER_H