#include "../internal.hpp"
namespace vk::descriptor_update {
void apply(const std::vector<vk::WriteDescriptorSet> &writes,
           const std::vector<vk::CopyDescriptorSet> &copies) {
    vk::internal::require_device().updateDescriptorSets(writes, copies);
}
void write(const std::vector<vk::WriteDescriptorSet> &writes) { apply(writes, {}); }
void copy(const std::vector<vk::CopyDescriptorSet> &copies) { apply({}, copies); }
}
