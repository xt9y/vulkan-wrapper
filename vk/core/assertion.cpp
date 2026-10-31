#include "../internal.hpp"
using vk::internal::require_device;
using vk::internal::require_instance;
using vk::internal::require_physical;
namespace vk {
namespace assertion {
void that(bool condition, std::string_view message)
{
    if (!condition) {
        log::error(message);
        throw std::runtime_error(std::string(message));
    }
}
}
}
