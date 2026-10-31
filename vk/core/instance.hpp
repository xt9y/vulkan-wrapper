#pragma once
#include "../common.hpp"
namespace vk {
namespace instance {
struct state {
        vk::Instance handle{};
        vk::Instance *api() { return &handle; }
        const vk::Instance *api() const { return &handle; }
        bool owned = false;
    };
    state *get();
    vk::Instance init(const vk::InstanceCreateInfo &info);
    void adopt(vk::Instance handle, bool owned = false);
    std::vector<vk::PhysicalDevice> physical_devices();
    std::vector<vk::ExtensionProperties> extensions(const char *layer = nullptr);
    std::vector<vk::LayerProperties> layers();
    void shutdown();
}
}
