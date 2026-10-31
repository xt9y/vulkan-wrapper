#pragma once

#ifndef VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#endif

#include <vulkan/vulkan.hpp>
#include "math.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace vk {

namespace log {
    enum class level { debug, info, warning, error };
    using callback = void (*)(level, std::string_view);
    void set_callback(callback fn);
    void write(level severity, std::string_view message);
    void debug(std::string_view message);
    void info(std::string_view message);
    void warning(std::string_view message);
    void error(std::string_view message);
}

namespace assertion {
    void that(bool condition, std::string_view message);
}

void check(Result result, std::string_view message = {});
void check(VkResult result, std::string_view message = {});

namespace loader {
    struct state {
        bool initialized = false;
        PFN_vkGetInstanceProcAddr get_instance_proc_addr = nullptr;
    };
    state *get();
    bool init(PFN_vkGetInstanceProcAddr proc = nullptr);
    void shutdown();
}

namespace instance {
    struct state {
        vk::Instance handle{};
        bool owned = false;
    };
    state *get();
    vk::Instance init(const vk::InstanceCreateInfo &info);
    void adopt(vk::Instance handle, bool owned = false);
    std::vector<vk::PhysicalDevice> physical_devices();
    std::vector<vk::ExtensionProperties> extensions(const char *layer = nullptr);
    std::vector<vk::LayerProperties> layers();
    void shutdown();
}

namespace physical_device {
    struct state {
        vk::PhysicalDevice handle{};
        vk::PhysicalDeviceProperties properties{};
        vk::PhysicalDeviceFeatures features{};
        vk::PhysicalDeviceMemoryProperties memory{};
        std::vector<vk::QueueFamilyProperties> queue_families;
    };
    state *get();
    vk::PhysicalDevice init(uint32_t index = 0);
    vk::PhysicalDevice adopt(vk::PhysicalDevice handle);
    vk::PhysicalDevice pick(vk::QueueFlags flags = vk::QueueFlagBits::eGraphics);
    uint32_t find_queue(vk::QueueFlags flags);
    bool supports_extension(const char *name);
    void shutdown();
}

namespace surface {
    struct state {
        vk::SurfaceKHR handle{};
        bool owned = false;
    };
    state *get();
    void init(vk::SurfaceKHR handle, bool owned = false);
    std::vector<vk::SurfaceFormatKHR> formats();
    std::vector<vk::PresentModeKHR> present_modes();
    vk::SurfaceCapabilitiesKHR capabilities();
    bool supports(uint32_t queue_family);
    void shutdown();
}

namespace device {
    struct state {
        vk::Device handle{};
        bool owned = false;
    };
    state *get();
    vk::Device init(const vk::DeviceCreateInfo &info);
    void adopt(vk::Device handle, bool owned = false);
    void idle();
    void shutdown();
}

namespace queue {
    struct state {
        vk::Queue graphics{};
        vk::Queue compute{};
        vk::Queue transfer{};
        vk::Queue present{};
        uint32_t graphics_family = UINT32_MAX;
        uint32_t compute_family = UINT32_MAX;
        uint32_t transfer_family = UINT32_MAX;
        uint32_t present_family = UINT32_MAX;
    };
    state *get();
    void init(uint32_t graphics_family, uint32_t present_family = UINT32_MAX,
              uint32_t compute_family = UINT32_MAX, uint32_t transfer_family = UINT32_MAX);
    void shutdown();
}

namespace swapchain {
    struct state {
        vk::SwapchainKHR handle{};
        std::vector<vk::Image> images;
        vk::Extent2D extent{};
        vk::Format format = vk::Format::eUndefined;
        vk::ColorSpaceKHR color_space = vk::ColorSpaceKHR::eSrgbNonlinear;
        uint32_t size = 0;
    };
    state *get();
    vk::SwapchainKHR init(const vk::SwapchainCreateInfoKHR &info);
    void shutdown();
}

namespace memory {
    uint32_t type_index(uint32_t bits, vk::MemoryPropertyFlags flags);
    vk::DeviceMemory allocate(const vk::MemoryRequirements &requirements,
                              vk::MemoryPropertyFlags flags);
    void free(vk::DeviceMemory handle);
}

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

namespace image {
    struct resource {
        vk::Image handle{};
        vk::DeviceMemory memory{};
        vk::Extent3D size{};
        vk::Format format = vk::Format::eUndefined;
    };
    resource create(const vk::ImageCreateInfo &info, vk::MemoryPropertyFlags properties);
    void destroy(resource &image);
}

namespace image_view {
    vk::ImageView create(const vk::ImageViewCreateInfo &info);
    void destroy(vk::ImageView handle);
}

namespace sampler {
    vk::Sampler create(const vk::SamplerCreateInfo &info);
    void destroy(vk::Sampler handle);
}

namespace shader {
    std::vector<uint32_t> read(const std::string &path);
    vk::ShaderModule create(const uint32_t *words, size_t count);
    vk::ShaderModule create(const std::vector<uint32_t> &words);
    void destroy(vk::ShaderModule handle);
}

namespace command {
    struct state {
        vk::CommandPool pool{};
        uint32_t family = UINT32_MAX;
    };
    state *get();
    vk::CommandPool init(uint32_t family, vk::CommandPoolCreateFlags flags =
                         vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
    std::vector<vk::CommandBuffer> allocate(uint32_t count = 1,
                                            vk::CommandBufferLevel level =
                                            vk::CommandBufferLevel::ePrimary);
    vk::CommandBuffer begin_one_time();
    void end(vk::CommandBuffer cmd);
    void free(const std::vector<vk::CommandBuffer> &commands);
    void shutdown();
}

namespace descriptor {
    struct state {
        vk::DescriptorPool pool{};
    };
    state *get();
    vk::DescriptorPool init(const vk::DescriptorPoolCreateInfo &info);
    std::vector<vk::DescriptorSet> allocate(const std::vector<vk::DescriptorSetLayout> &layouts);
    void reset();
    void shutdown();
}

namespace pipeline_cache {
    struct state {
        vk::PipelineCache handle{};
    };
    state *get();
    vk::PipelineCache init(const vk::PipelineCacheCreateInfo &info = {});
    std::vector<uint8_t> data();
    void shutdown();
}

namespace debug {
    struct state {
        vk::DebugUtilsMessengerEXT messenger{};
    };
    state *get();
    vk::DebugUtilsMessengerEXT init(const vk::DebugUtilsMessengerCreateInfoEXT &info);
    void name(vk::ObjectType type, uint64_t handle, const char *text);
    void label(vk::CommandBuffer cmd, const char *text, const std::array<float,4> &color =
               {0.0f, 0.0f, 0.0f, 1.0f});
    void end_label(vk::CommandBuffer cmd);
    void shutdown();
}

namespace sync {
    vk::Fence fence(bool signaled = false);
    vk::Semaphore semaphore();
    vk::Semaphore timeline(uint64_t initial_value = 0);
    void destroy(vk::Fence handle);
    void destroy(vk::Semaphore handle);
}

namespace util {
    vk::Extent2D extent(uint32_t width, uint32_t height);
    uint32_t mip_levels(uint32_t width, uint32_t height);
    vk::DeviceSize aligned(vk::DeviceSize value, vk::DeviceSize alignment);
    std::string result_string(vk::Result result);
}

}
