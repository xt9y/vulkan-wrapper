#pragma once
#include "../common.hpp"
namespace vk {
namespace buffer {
struct resource {
        vk::Buffer handle{};
        vk::DeviceMemory memory{};
        vk::DeviceSize size = 0;
        vk::MemoryPropertyFlags properties{};
        void *mapped = nullptr;
    };
    resource create(vk::DeviceSize size, vk::BufferUsageFlags usage,
                    vk::MemoryPropertyFlags properties);
    void destroy(resource &buffer);
    void *map(resource &buffer);
    void unmap(resource &buffer);
    void flush(const resource &buffer);
    void write(resource &buffer, const void *data, size_t bytes, size_t offset = 0);
    vk::DescriptorBufferInfo descriptor(const resource &buffer,
                                        vk::DeviceSize offset = 0,
                                        vk::DeviceSize range = VK_WHOLE_SIZE);
}
}
