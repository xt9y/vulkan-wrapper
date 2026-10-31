#include "../internal.hpp"
using vk::internal::require_device;
using vk::internal::require_instance;
using vk::internal::require_physical;
namespace vk {
namespace command {
state *get() { static state data; return &data; }
vk::CommandPool init(uint32_t family, vk::CommandPoolCreateFlags flags)
{
    auto &s = *get();
    if (s.pool) throw std::runtime_error("command pool already initialized");
    vk::CommandPoolCreateInfo info{};
    info.queueFamilyIndex = family;
    info.flags = flags;
    s.pool = require_device().createCommandPool(info);
    s.family = family;
    return s.pool;
}
std::vector<vk::CommandBuffer> allocate(uint32_t count, vk::CommandBufferLevel level)
{
    vk::CommandBufferAllocateInfo info{};
    info.commandPool = get()->pool;
    info.level = level;
    info.commandBufferCount = count;
    return require_device().allocateCommandBuffers(info);
}
vk::CommandBuffer begin_one_time()
{
    auto commands = allocate();
    auto command = commands.front();
    vk::CommandBufferBeginInfo info{};
    info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
    command.begin(info);
    return command;
}
void end(vk::CommandBuffer cmd) { cmd.end(); }
void free(const std::vector<vk::CommandBuffer> &commands)
{
    require_device().freeCommandBuffers(get()->pool, commands);
}
void shutdown()
{
    auto &s = *get();
    if (s.pool) require_device().destroyCommandPool(s.pool);
    s = {};
}
}
}
