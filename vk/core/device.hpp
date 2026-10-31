#pragma once
#include "../common.hpp"
namespace vk {
namespace device {
struct state {
        vk::Device handle{};
        bool owned = false;
    };
    state *get();
    vk::Device init(const vk::DeviceCreateInfo &info);
    void adopt(vk::Device handle, bool owned = false);
    void idle();
    void shutdown();
}
}
