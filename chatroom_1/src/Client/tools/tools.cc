#include "tools.h"

std::string Tools::getNowTime() {
    auto now = std::chrono::system_clock::now();

    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_time);
    std::ostringstream buf;
    buf << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");

    return buf.str();
}