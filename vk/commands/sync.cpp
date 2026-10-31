#include "../internal.hpp"
using vk::internal::require_device;
using vk::internal::require_instance;
using vk::internal::require_physical;
namespace vk {
namespace sync {
vk::Fence fence(bool signaled)
{
    vk::FenceCreateInfo info{};
    if (signaled) info.flags = vk::FenceCreateFlagBits::eSignaled;
    return require_device().createFence(info);
}
vk::Semaphore semaphore() { return require_device().createSemaphore({}); }
vk::Semaphore timeline(uint64_t initial_value)
{
    vk::SemaphoreTypeCreateInfo type{};
    type.semaphoreType = vk::SemaphoreType::eTimeline;
    type.initialValue = initial_value;
    vk::SemaphoreCreateInfo info{};
    info.pNext = &type;
    return require_device().createSemaphore(info);
}
void destroy(vk::Fence handle)
{
    if (handle) require_device().destroyFence(handle);
}
void destroy(vk::Semaphore handle)
{
    if (handle) require_device().destroySemaphore(handle);
}
}
}
