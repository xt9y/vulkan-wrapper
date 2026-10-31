#include "../internal.hpp"
#include <cstring>
using vk::internal::require_device;
using vk::internal::require_instance;
using vk::internal::require_physical;
namespace vk {
namespace physical_device {
state *get() { static state data; return &data; }
vk::PhysicalDevice adopt(vk::PhysicalDevice handle)
{
    if (!handle) throw std::invalid_argument("null physical device");
    auto &s = *get();
    s.handle = handle;
    s.properties = handle.getProperties();
    s.features = handle.getFeatures();
    s.memory = handle.getMemoryProperties();
    s.queue_families = handle.getQueueFamilyProperties();
    return handle;
}
vk::PhysicalDevice init(uint32_t index)
{
    auto devices = instance::physical_devices();
    if (index >= devices.size()) throw std::out_of_range("physical device index");
    return adopt(devices[index]);
}
vk::PhysicalDevice pick(vk::QueueFlags flags)
{
    auto devices = instance::physical_devices();
    for (auto candidate : devices) {
        for (auto family : candidate.getQueueFamilyProperties()) {
            if (family.queueCount && (family.queueFlags & flags) == flags)
                return adopt(candidate);
        }
    }
    throw std::runtime_error("no compatible physical device");
}
uint32_t find_queue(vk::QueueFlags flags)
{
    auto &families = get()->queue_families;
    for (uint32_t i = 0; i < families.size(); ++i)
        if (families[i].queueCount && (families[i].queueFlags & flags) == flags)
            return i;
    throw std::runtime_error("queue family unavailable");
}
bool supports_extension(const char *name)
{
    for (auto &ext : require_physical().enumerateDeviceExtensionProperties())
        if (std::strcmp(ext.extensionName.data(), name) == 0) return true;
    return false;
}
void shutdown() { *get() = {}; }
}
}
