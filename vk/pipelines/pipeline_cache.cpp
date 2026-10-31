#include "../internal.hpp"
using vk::internal::require_device;
using vk::internal::require_instance;
using vk::internal::require_physical;
namespace vk {
namespace pipeline_cache {
state *get() { static state data; return &data; }
vk::PipelineCache init(const vk::PipelineCacheCreateInfo &info)
{
    auto &s = *get();
    if (s.handle) throw std::runtime_error("pipeline cache already initialized");
    s.handle = require_device().createPipelineCache(info);
    return s.handle;
}
std::vector<uint8_t> data()
{
    return require_device().getPipelineCacheData(get()->handle);
}
void shutdown()
{
    auto &s = *get();
    if (s.handle) require_device().destroyPipelineCache(s.handle);
    s = {};
}
}
}
