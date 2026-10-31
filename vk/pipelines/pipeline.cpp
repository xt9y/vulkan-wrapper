#include "../internal.hpp"
namespace vk::pipeline {
vk::Pipeline graphics(const vk::GraphicsPipelineCreateInfo &info, vk::PipelineCache cache) {
    return vk::internal::require_device().createGraphicsPipeline(cache, info);
}
vk::Pipeline compute(const vk::ComputePipelineCreateInfo &info, vk::PipelineCache cache) {
    return vk::internal::require_device().createComputePipeline(cache, info);
}
void destroy(vk::Pipeline handle) {
    if (handle) vk::internal::require_device().destroyPipeline(handle);
}
}
