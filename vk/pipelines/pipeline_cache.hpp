#pragma once
#include "../common.hpp"
namespace vk {
namespace pipeline_cache {
struct state {
        vk::PipelineCache handle{};
    };
    state *get();
    vk::PipelineCache init(const vk::PipelineCacheCreateInfo &info = {});
    std::vector<uint8_t> data();
    void shutdown();
}
}
