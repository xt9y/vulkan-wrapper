#pragma once
#include "../common.hpp"
namespace vk {
namespace sampler {
vk::Sampler create(const vk::SamplerCreateInfo &info);
    void destroy(vk::Sampler handle);
}
}
