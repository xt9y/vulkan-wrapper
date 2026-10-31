#pragma once
#include "../common.hpp"
namespace vk {
namespace util {
vk::Extent2D extent(uint32_t width, uint32_t height);
    uint32_t mip_levels(uint32_t width, uint32_t height);
    vk::DeviceSize aligned(vk::DeviceSize value, vk::DeviceSize alignment);
    std::string result_string(vk::Result result);
}
}
