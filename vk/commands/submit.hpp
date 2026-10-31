#pragma once
#include "../common.hpp"
namespace vk::submit {
void queue(vk::Queue queue, const std::vector<vk::SubmitInfo> &submits, vk::Fence fence = {});
void wait(vk::Queue queue);
vk::Result fence(vk::Fence handle, uint64_t timeout = UINT64_MAX);
void reset(vk::Fence handle);
vk::Result present(vk::Queue queue, const vk::PresentInfoKHR &info);
}
