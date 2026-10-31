#include "../internal.hpp"
using vk::internal::require_device;
using vk::internal::require_instance;
using vk::internal::require_physical;
namespace vk {
namespace memory {
uint32_t type_index(uint32_t bits, vk::MemoryPropertyFlags flags)
{
    auto &p = physical_device::get()->memory;
    if (!physical_device::get()->handle) throw std::runtime_error("physical device not initialized");
    for (uint32_t i = 0; i < p.memoryTypeCount; ++i)
        if ((bits & (1u << i)) && (p.memoryTypes[i].propertyFlags & flags) == flags)
            return i;
    throw std::runtime_error("compatible Vulkan memory type not found");
}
vk::DeviceMemory allocate(const vk::MemoryRequirements &requirements,
                          vk::MemoryPropertyFlags flags)
{
    vk::MemoryAllocateInfo info{};
    info.allocationSize = requirements.size;
    info.memoryTypeIndex = type_index(requirements.memoryTypeBits, flags);
    return require_device().allocateMemory(info);
}
void free(vk::DeviceMemory handle)
{
    if (handle) require_device().freeMemory(handle);
}
}
}
