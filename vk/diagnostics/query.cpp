#include "../internal.hpp"
namespace vk::query {
vk::QueryPool create(const vk::QueryPoolCreateInfo &info) {
    return vk::internal::require_device().createQueryPool(info);
}
void destroy(vk::QueryPool handle) {
    if (handle) vk::internal::require_device().destroyQueryPool(handle);
}
}
