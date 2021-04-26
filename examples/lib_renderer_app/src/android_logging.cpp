#include "android_logging.hpp"

#ifdef __ANDROID__

#include <spdlog/spdlog.h>
#include <spdlog/sinks/android_sink.h>

#include <memory>

void setup_android_logging()
{
	auto sink = std::shared_ptr<spdlog::sinks::android_sink_mt>(new spdlog::sinks::android_sink_mt("vr"));
	std::vector<std::shared_ptr<spdlog::sinks::sink>> vec;
	vec.push_back(sink);
	auto logger = std::shared_ptr<spdlog::logger>(new spdlog::logger("vr", vec.begin(), vec.end()));
	spdlog::set_default_logger(logger);
}

#endif