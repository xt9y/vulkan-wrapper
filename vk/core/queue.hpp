#pragma once
#include "../common.hpp"
namespace vk {
namespace queue {
struct state {
        vk::Queue graphics{};
        vk::Queue compute{};
        vk::Queue transfer{};
        vk::Queue present{};
        uint32_t graphics_family = UINT32_MAX;
        uint32_t compute_family = UINT32_MAX;
        uint32_t transfer_family = UINT32_MAX;
        uint32_t present_family = UINT32_MAX;
    };
    state *get();
    void init(uint32_t graphics_family, uint32_t present_family = UINT32_MAX,
              uint32_t compute_family = UINT32_MAX, uint32_t transfer_family = UINT32_MAX);
    void shutdown();
}
}
