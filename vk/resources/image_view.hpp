#pragma once
#include "../common.hpp"
namespace vk {
namespace image_view {
vk::ImageView create(const vk::ImageViewCreateInfo &info);
    void destroy(vk::ImageView handle);
}
}
