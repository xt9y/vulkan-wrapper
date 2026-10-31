#include "../internal.hpp"
using vk::internal::require_device;
using vk::internal::require_instance;
using vk::internal::require_physical;
namespace vk {
namespace device {
state *get() { static state data; return &data; }
vk::Device init(const vk::DeviceCreateInfo &info)
{
    if (get()->handle) throw std::runtime_error("device already initialized");
    auto handle = require_physical().createDevice(info);
    adopt(handle, true);
    return handle;
}
void adopt(vk::Device handle, bool owned)
{
    if (!handle) throw std::invalid_argument("null logical device");
    if (get()->handle) throw std::runtime_error("device already initialized");
    get()->handle = handle;
    get()->owned = owned;
    VULKAN_HPP_DEFAULT_DISPATCHER.init(handle);
}
void idle() { require_device().waitIdle(); }
void shutdown()
{
    auto &s = *get();
    if (s.owned && s.handle) s.handle.destroy();
    s = {};
}
}
}
