#pragma once
#include "../common.hpp"
namespace vk::render_pass {
vk::RenderPass create(const vk::RenderPassCreateInfo &info);
void destroy(vk::RenderPass handle);
}
