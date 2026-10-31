#include "../internal.hpp"
namespace vk::descriptor_layout {
vk::DescriptorSetLayout create(const vk::DescriptorSetLayoutCreateInfo &info) {
    return vk::internal::require_device().createDescriptorSetLayout(info);
}
void destroy(vk::DescriptorSetLayout handle) {
    if (handle) vk::internal::require_device().destroyDescriptorSetLayout(handle);
}
}
