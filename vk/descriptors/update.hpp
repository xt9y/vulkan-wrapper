#pragma once
#include "../common.hpp"
namespace vk::descriptor_update {
void write(const std::vector<vk::WriteDescriptorSet> &writes);
void copy(const std::vector<vk::CopyDescriptorSet> &copies);
void apply(const std::vector<vk::WriteDescriptorSet> &writes,
           const std::vector<vk::CopyDescriptorSet> &copies);
}
