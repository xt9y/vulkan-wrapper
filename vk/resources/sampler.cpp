#include "../internal.hpp"
using vk::internal::require_device;
using vk::internal::require_instance;
using vk::internal::require_physical;
namespace vk {
namespace sampler {
vk::Sampler create(const vk::SamplerCreateInfo &info)
{
    return require_device().createSampler(info);
}
void destroy(vk::Sampler handle)
{
    if (handle) require_device().destroySampler(handle);
}
}
}
