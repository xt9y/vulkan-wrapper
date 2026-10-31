#pragma once
#include "../common.hpp"
namespace vk::framebuffer {
vk::Framebuffer create(const vk::FramebufferCreateInfo &info);
void destroy(vk::Framebuffer handle);
}
