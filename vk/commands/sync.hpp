#pragma once
#include "../common.hpp"
namespace vk {
namespace sync {
vk::Fence fence(bool signaled = false);
    vk::Semaphore semaphore();
    vk::Semaphore timeline(uint64_t initial_value = 0);
    void destroy(vk::Fence handle);
    void destroy(vk::Semaphore handle);
}
}
