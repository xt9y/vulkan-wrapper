#include "../internal.hpp"
namespace vk::pipeline_layout {
vk::PipelineLayout create(const vk::PipelineLayoutCreateInfo &info) {
    return vk::internal::require_device().createPipelineLayout(info);
}
void destroy(vk::PipelineLayout handle) {
    if (handle) vk::internal::require_device().destroyPipelineLayout(handle);
}
}
