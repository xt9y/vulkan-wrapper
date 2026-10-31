#pragma once
#include "../common.hpp"
namespace vk {
namespace log {
enum class level { debug, info, warning, error };
    using callback = void (*)(level, std::string_view);
    void set_callback(callback fn);
    void write(level severity, std::string_view message);
    void debug(std::string_view message);
    void info(std::string_view message);
    void warning(std::string_view message);
    void error(std::string_view message);
}
}
