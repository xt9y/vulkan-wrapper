#pragma once
#include "../common.hpp"
namespace vk {
namespace physical_device {
struct state {
        vk::PhysicalDevice handle{};
        vk::PhysicalDevice *api() { return &handle; }
        const vk::PhysicalDevice *api() const { return &handle; }
        vk::PhysicalDeviceProperties properties{};
        vk::PhysicalDeviceFeatures features{};
        vk::PhysicalDeviceMemoryProperties memory{};
        std::vector<vk::QueueFamilyProperties> queue_families;
    };
    state *get();
    vk::PhysicalDevice init(uint32_t index = 0);
    vk::PhysicalDevice adopt(vk::PhysicalDevice handle);
    vk::PhysicalDevice pick(vk::QueueFlags flags = vk::QueueFlagBits::eGraphics);
    uint32_t find_queue(vk::QueueFlags flags);
    bool supports_extension(const char *name);
    void shutdown();
}
}
