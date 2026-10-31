#include "../internal.hpp"
namespace vk::framebuffer {
vk::Framebuffer create(const vk::FramebufferCreateInfo &info) {
    return vk::internal::require_device().createFramebuffer(info);
}
void destroy(vk::Framebuffer handle) {
    if (handle) vk::internal::require_device().destroyFramebuffer(handle);
}
}
