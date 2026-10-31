#pragma once
#include "../common.hpp"
namespace vk {
namespace debug {
struct state {
        vk::DebugUtilsMessengerEXT messenger{};
    };
    state *get();
    vk::DebugUtilsMessengerEXT init(const vk::DebugUtilsMessengerCreateInfoEXT &info);
    void name(vk::ObjectType type, uint64_t handle, const char *text);
    void label(vk::CommandBuffer cmd, const char *text, const std::array<float,4> &color =
               {0.0f, 0.0f, 0.0f, 1.0f});
    void end_label(vk::CommandBuffer cmd);
    void shutdown();
}
}
