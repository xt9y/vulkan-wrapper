#include "../internal.hpp"
using vk::internal::require_device;
using vk::internal::require_instance;
using vk::internal::require_physical;
namespace vk {
namespace instance {
state *get() { static state data; return &data; }
vk::Instance init(const vk::InstanceCreateInfo &info)
{
    if (get()->handle) throw std::runtime_error("vk::instance already initialized");
    if (!loader::get()->initialized && !loader::init())
        throw std::runtime_error("Vulkan loader unavailable");
    auto handle = vk::createInstance(info);
    adopt(handle, true);
    return handle;
}
void adopt(vk::Instance handle, bool owned)
{
    if (!loader::get()->initialized) throw std::runtime_error("vk::loader not initialized");
    if (get()->handle) throw std::runtime_error("vk::instance already initialized");
    get()->handle = handle;
    get()->owned = owned;
    if (handle) VULKAN_HPP_DEFAULT_DISPATCHER.init(handle);
}
std::vector<vk::PhysicalDevice> physical_devices()
{
    return require_instance().enumeratePhysicalDevices();
}
std::vector<vk::ExtensionProperties> extensions(const char *layer)
{
    if (!loader::get()->initialized && !loader::init())
        throw std::runtime_error("Vulkan loader unavailable");
    return vk::enumerateInstanceExtensionProperties(layer);
}
std::vector<vk::LayerProperties> layers()
{
    if (!loader::get()->initialized && !loader::init())
        throw std::runtime_error("Vulkan loader unavailable");
    return vk::enumerateInstanceLayerProperties();
}
void shutdown()
{
    auto &s = *get();
    if (s.owned && s.handle) s.handle.destroy();
    s = {};
}
}
}
