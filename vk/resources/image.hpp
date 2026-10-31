#pragma once
#include "../common.hpp"
namespace vk {
namespace image {
struct resource {
        vk::Image handle{};
        vk::DeviceMemory memory{};
        vk::Extent3D size{};
        vk::Format format = vk::Format::eUndefined;
    };
    resource create(const vk::ImageCreateInfo &info, vk::MemoryPropertyFlags properties);
    void destroy(resource &image);
}
}
