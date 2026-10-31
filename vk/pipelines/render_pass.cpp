#include "../internal.hpp"
namespace vk::render_pass {
vk::RenderPass create(const vk::RenderPassCreateInfo &info) {
    return vk::internal::require_device().createRenderPass(info);
}
void destroy(vk::RenderPass handle) {
    if (handle) vk::internal::require_device().destroyRenderPass(handle);
}
}
