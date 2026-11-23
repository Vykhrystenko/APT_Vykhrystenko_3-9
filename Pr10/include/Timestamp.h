#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

inline std::string currentTimestamp() {
    const auto now = std::chrono::system_clock::now();

    const std::time_t currentTime =
        std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &currentTime);
#else
    localtime_r(&currentTime, &localTime);
#endif

    std::ostringstream output;

    output << std::put_time(
        &localTime,
        "%Y-%m-%d %H:%M:%S"
    );

    return output.str();
}