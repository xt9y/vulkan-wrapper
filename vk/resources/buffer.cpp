#include "../internal.hpp"
#include <cstring>
using vk::internal::require_device;
using vk::internal::require_instance;
using vk::internal::require_physical;
namespace vk {
namespace buffer {
resource create(vk::DeviceSize size, vk::BufferUsageFlags usage,
                vk::MemoryPropertyFlags properties)
{
    auto d = require_device();
    vk::BufferCreateInfo info{};
    info.size = size;
    info.usage = usage;
    info.sharingMode = vk::SharingMode::eExclusive;
    resource r{};
    r.handle = d.createBuffer(info);
    r.size = size;
    r.properties = properties;
    try {
        r.memory = memory::allocate(d.getBufferMemoryRequirements(r.handle), properties);
        d.bindBufferMemory(r.handle, r.memory, 0);
    } catch (...) {
        if (r.memory) d.freeMemory(r.memory);
        d.destroyBuffer(r.handle);
        throw;
    }
    return r;
}
void destroy(resource &r)
{
    auto d = require_device();
    if (r.mapped) d.unmapMemory(r.memory);
    if (r.handle) d.destroyBuffer(r.handle);
    if (r.memory) d.freeMemory(r.memory);
    r = {};
}
void *map(resource &r)
{
    if (!r.mapped) r.mapped = require_device().mapMemory(r.memory, 0, VK_WHOLE_SIZE);
    return r.mapped;
}
void unmap(resource &r)
{
    if (r.mapped) require_device().unmapMemory(r.memory);
    r.mapped = nullptr;
}
void flush(const resource &r)
{
    if (r.properties & vk::MemoryPropertyFlagBits::eHostCoherent) return;
    vk::MappedMemoryRange range{};
    range.memory = r.memory;
    range.offset = 0;
    range.size = VK_WHOLE_SIZE;
    require_device().flushMappedMemoryRanges(range);
}
void write(resource &r, const void *data, size_t bytes, size_t offset)
{
    if (!data || offset > r.size || bytes > r.size - offset)
        throw std::out_of_range("buffer write exceeds buffer size");
    std::memcpy(static_cast<char *>(map(r)) + offset, data, bytes);
    flush(r);
}
vk::DescriptorBufferInfo descriptor(const resource &r,
                                     vk::DeviceSize offset, vk::DeviceSize range)
{
    vk::DescriptorBufferInfo info{};
    info.buffer = r.handle;
    info.offset = offset;
    info.range = range;
    return info;
}
}
}
