#include "../internal.hpp"
namespace vk::event {
vk::Event create(const vk::EventCreateInfo &info) {
    return vk::internal::require_device().createEvent(info);
}
void destroy(vk::Event handle) {
    if (handle) vk::internal::require_device().destroyEvent(handle);
}
}
