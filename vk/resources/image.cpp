#include "../internal.hpp"
using vk::internal::require_device;
using vk::internal::require_instance;
using vk::internal::require_physical;
namespace vk {
namespace image {
resource create(const vk::ImageCreateInfo &info, vk::MemoryPropertyFlags properties)
{
    auto d = require_device();
    resource r{};
    r.handle = d.createImage(info);
    r.size = info.extent;
    r.format = info.format;
    try {
        r.memory = memory::allocate(d.getImageMemoryRequirements(r.handle), properties);
        d.bindImageMemory(r.handle, r.memory, 0);
    } catch (...) {
        if (r.memory) d.freeMemory(r.memory);
        d.destroyImage(r.handle);
        throw;
    }
    return r;
}
void destroy(resource &r)
{
    auto d = require_device();
    if (r.handle) d.destroyImage(r.handle);
    if (r.memory) d.freeMemory(r.memory);
    r = {};
}
}
}
