#pragma once
#include "../common.hpp"
namespace vk::pipeline {
vk::Pipeline graphics(const vk::GraphicsPipelineCreateInfo &info, vk::PipelineCache cache = {});
vk::Pipeline compute(const vk::ComputePipelineCreateInfo &info, vk::PipelineCache cache = {});
void destroy(vk::Pipeline handle);
}
