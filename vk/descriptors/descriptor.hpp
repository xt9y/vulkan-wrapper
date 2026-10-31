#pragma once
#include "../common.hpp"
namespace vk {
namespace descriptor {
struct state {
        vk::DescriptorPool pool{};
    };
    state *get();
    vk::DescriptorPool init(const vk::DescriptorPoolCreateInfo &info);
    std::vector<vk::DescriptorSet> allocate(const std::vector<vk::DescriptorSetLayout> &layouts);
    void reset();
    void shutdown();
}
}
