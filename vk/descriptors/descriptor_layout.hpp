#pragma once
#include "../common.hpp"
namespace vk::descriptor_layout {
vk::DescriptorSetLayout create(const vk::DescriptorSetLayoutCreateInfo &info);
void destroy(vk::DescriptorSetLayout handle);
}
