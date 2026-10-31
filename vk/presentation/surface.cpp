#include "../internal.hpp"
using vk::internal::require_device;
using vk::internal::require_instance;
using vk::internal::require_physical;
namespace vk {
namespace surface {
state *get() { static state data; return &data; }
void init(vk::SurfaceKHR handle, bool owned)
{
    if (get()->handle) throw std::runtime_error("surface already initialized");
    get()->handle = handle;
    get()->owned = owned;
}
std::vector<vk::SurfaceFormatKHR> formats()
{
    return require_physical().getSurfaceFormatsKHR(get()->handle);
}
std::vector<vk::PresentModeKHR> present_modes()
{
    return require_physical().getSurfacePresentModesKHR(get()->handle);
}
vk::SurfaceCapabilitiesKHR capabilities()
{
    return require_physical().getSurfaceCapabilitiesKHR(get()->handle);
}
bool supports(uint32_t family)
{
    return require_physical().getSurfaceSupportKHR(family, get()->handle);
}
void shutdown()
{
    auto &s = *get();
    if (s.owned && s.handle) require_instance().destroySurfaceKHR(s.handle);
    s = {};
}
}
}
