#include "logger_until.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"

void logger_until::init() {
	auto max_size = 1048576 * 5;
	auto max_files = 3;
	auto logger = spdlog::rotating_logger_mt("runtime", "logs/rotating.txt", max_size, max_files);
	spdlog::set_level(spdlog::level::debug);
}