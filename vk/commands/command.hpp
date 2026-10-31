#pragma once
#include "../common.hpp"
namespace vk {
namespace command {
struct state {
        vk::CommandPool pool{};
        uint32_t family = UINT32_MAX;
    };
    state *get();
    vk::CommandPool init(uint32_t family, vk::CommandPoolCreateFlags flags =
                         vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
    std::vector<vk::CommandBuffer> allocate(uint32_t count = 1,
                                            vk::CommandBufferLevel level =
                                            vk::CommandBufferLevel::ePrimary);
    vk::CommandBuffer begin_one_time();
    void end(vk::CommandBuffer cmd);
    void free(const std::vector<vk::CommandBuffer> &commands);
    void shutdown();
}
}
