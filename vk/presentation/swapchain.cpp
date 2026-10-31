#include "../internal.hpp"
using vk::internal::require_device;
using vk::internal::require_instance;
using vk::internal::require_physical;
namespace vk {
namespace swapchain {
state *get() { static state data; return &data; }
vk::SwapchainKHR init(const vk::SwapchainCreateInfoKHR &info)
{
    auto d = require_device();
    auto &s = *get();
    auto create_info = info;
    if (!create_info.oldSwapchain) create_info.oldSwapchain = s.handle;
    auto handle = d.createSwapchainKHR(create_info);
    std::vector<vk::Image> images;
    try {
        images = d.getSwapchainImagesKHR(handle);
    } catch (...) {
        d.destroySwapchainKHR(handle);
        throw;
    }
    if (s.handle) d.destroySwapchainKHR(s.handle);
    s.handle = handle;
    s.images = std::move(images);
    s.extent = info.imageExtent;
    s.format = info.imageFormat;
    s.color_space = info.imageColorSpace;
    s.size = static_cast<uint32_t>(s.images.size());
    return handle;
}
void shutdown()
{
    auto &s = *get();
    if (s.handle) require_device().destroySwapchainKHR(s.handle);
    s = {};
}
}
}
