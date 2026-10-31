#include "../internal.hpp"
#include <memory>
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
namespace { std::unique_ptr<vk::detail::DynamicLoader> dynamic_loader; }
using vk::internal::require_device;
using vk::internal::require_instance;
using vk::internal::require_physical;
namespace vk {
namespace loader {
state *get() { static state data; return &data; }
bool init(PFN_vkGetInstanceProcAddr proc)
{
    if (proc == nullptr) {
        dynamic_loader = std::make_unique<vk::detail::DynamicLoader>();
        proc = dynamic_loader->getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
    }
    if (!proc) {
        get()->initialized = false;
        get()->get_instance_proc_addr = nullptr;
        return false;
    }
    VULKAN_HPP_DEFAULT_DISPATCHER.init(proc);
    get()->get_instance_proc_addr = proc;
    get()->initialized = true;
    return true;
}
void shutdown()
{
    get()->initialized = false;
    get()->get_instance_proc_addr = nullptr;
    dynamic_loader.reset();
}
}
}
