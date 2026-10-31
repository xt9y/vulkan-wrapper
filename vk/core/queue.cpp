#include "../internal.hpp"
using vk::internal::require_device;
using vk::internal::require_instance;
using vk::internal::require_physical;
namespace vk {
namespace queue {
state *get() { static state data; return &data; }
void init(uint32_t graphics_family, uint32_t present_family,
          uint32_t compute_family, uint32_t transfer_family)
{
    auto d = require_device();
    auto &s = *get();
    s.graphics_family = graphics_family;
    s.present_family = present_family == UINT32_MAX ? graphics_family : present_family;
    s.compute_family = compute_family == UINT32_MAX ? graphics_family : compute_family;
    s.transfer_family = transfer_family == UINT32_MAX ? graphics_family : transfer_family;
    s.graphics = d.getQueue(s.graphics_family, 0);
    s.present = d.getQueue(s.present_family, 0);
    s.compute = d.getQueue(s.compute_family, 0);
    s.transfer = d.getQueue(s.transfer_family, 0);
}
void shutdown() { *get() = {}; }
}
}
