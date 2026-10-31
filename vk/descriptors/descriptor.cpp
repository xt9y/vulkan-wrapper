#include "../internal.hpp"
using vk::internal::require_device;
using vk::internal::require_instance;
using vk::internal::require_physical;
namespace vk {
namespace descriptor {
state *get() { static state data; return &data; }
vk::DescriptorPool init(const vk::DescriptorPoolCreateInfo &info)
{
    auto &s = *get();
    if (s.pool) throw std::runtime_error("descriptor pool already initialized");
    s.pool = require_device().createDescriptorPool(info);
    return s.pool;
}
std::vector<vk::DescriptorSet> allocate(const std::vector<vk::DescriptorSetLayout> &layouts)
{
    vk::DescriptorSetAllocateInfo info{};
    info.descriptorPool = get()->pool;
    info.descriptorSetCount = static_cast<uint32_t>(layouts.size());
    info.pSetLayouts = layouts.data();
    return require_device().allocateDescriptorSets(info);
}
void reset()
{
    require_device().resetDescriptorPool(get()->pool);
}
void shutdown()
{
    auto &s = *get();
    if (s.pool) require_device().destroyDescriptorPool(s.pool);
    s = {};
}
}
}
