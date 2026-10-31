#pragma once
#include "../common.hpp"
namespace vk {
namespace shader {
std::vector<uint32_t> read(const std::string &path);
    vk::ShaderModule create(const uint32_t *words, size_t count);
    vk::ShaderModule create(const std::vector<uint32_t> &words);
    void destroy(vk::ShaderModule handle);
}
}
