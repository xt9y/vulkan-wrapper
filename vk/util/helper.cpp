#include "../internal.hpp"
using vk::internal::require_device;
using vk::internal::require_instance;
using vk::internal::require_physical;
namespace vk {
namespace util {
vk::Extent2D extent(uint32_t width, uint32_t height) { return {width, height}; }
uint32_t mip_levels(uint32_t width, uint32_t height)
{
    uint32_t n = width > height ? width : height, levels = 0;
    while (n) { ++levels; n >>= 1; }
    return levels;
}
vk::DeviceSize aligned(vk::DeviceSize value, vk::DeviceSize alignment)
{
    if (!alignment) return value;
    auto remainder = value % alignment;
    return remainder ? value + alignment - remainder : value;
}
std::string result_string(vk::Result result) { return vk::to_string(result); }
}
}
