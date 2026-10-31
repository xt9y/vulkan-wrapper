#pragma once
#include "../common.hpp"
namespace vk {
namespace memory {
uint32_t type_index(uint32_t bits, vk::MemoryPropertyFlags flags);
    vk::DeviceMemory allocate(const vk::MemoryRequirements &requirements,
                              vk::MemoryPropertyFlags flags);
    void free(vk::DeviceMemory handle);
}
}
