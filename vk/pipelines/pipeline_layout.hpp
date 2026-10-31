#pragma once
#include "../common.hpp"
namespace vk::pipeline_layout {
vk::PipelineLayout create(const vk::PipelineLayoutCreateInfo &info);
void destroy(vk::PipelineLayout handle);
}
