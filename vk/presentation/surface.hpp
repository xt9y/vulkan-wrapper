#pragma once
#include "../common.hpp"
namespace vk {
namespace surface {
struct state {
        vk::SurfaceKHR handle{};
        bool owned = false;
    };
    state *get();
    void init(vk::SurfaceKHR handle, bool owned = false);
    std::vector<vk::SurfaceFormatKHR> formats();
    std::vector<vk::PresentModeKHR> present_modes();
    vk::SurfaceCapabilitiesKHR capabilities();
    bool supports(uint32_t queue_family);
    void shutdown();
}
}
