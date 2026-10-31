#include "../internal.hpp"
namespace vk::pipeline {
vk::Pipeline graphics(const vk::GraphicsPipelineCreateInfo &info, vk::PipelineCache cache) {
    auto result = vk::internal::require_device().createGraphicsPipeline(cache, info);
    vk::check(result.result, "graphics pipeline creation");
    return result.value;
}
vk::Pipeline compute(const vk::ComputePipelineCreateInfo &info, vk::PipelineCache cache) {
    auto result = vk::internal::require_device().createComputePipeline(cache, info);
    vk::check(result.result, "compute pipeline creation");
    return result.value;
}
void destroy(vk::Pipeline handle) {
    if (handle) vk::internal::require_device().destroyPipeline(handle);
}
}
