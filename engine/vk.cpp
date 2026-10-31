#include "vk.hpp"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <utility>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace {

std::unique_ptr<vk::DynamicLoader> dynamic_loader;
vk::log::callback log_callback = nullptr;

vk::Device require_device()
{
    auto handle = vk::device::get()->handle;
    if (!handle) throw std::runtime_error("vk::device is not initialized");
    return handle;
}

vk::Instance require_instance()
{
    auto handle = vk::instance::get()->handle;
    if (!handle) throw std::runtime_error("vk::instance is not initialized");
    return handle;
}

vk::PhysicalDevice require_physical()
{
    auto handle = vk::physical_device::get()->handle;
    if (!handle) throw std::runtime_error("vk::physical_device is not initialized");
    return handle;
}

}

namespace vk {

namespace log {
void set_callback(callback fn) { log_callback = fn; }
void write(level severity, std::string_view message)
{
    if (log_callback) {
        log_callback(severity, message);
        return;
    }
    const char *names[] = {"debug", "info", "warning", "error"};
    std::fprintf(stderr, "[vk:%s] %.*s\n", names[static_cast<int>(severity)],
                 static_cast<int>(message.size()), message.data());
}
void debug(std::string_view message) { write(level::debug, message); }
void info(std::string_view message) { write(level::info, message); }
void warning(std::string_view message) { write(level::warning, message); }
void error(std::string_view message) { write(level::error, message); }
}

namespace assertion {
void that(bool condition, std::string_view message)
{
    if (!condition) {
        log::error(message);
        throw std::runtime_error(std::string(message));
    }
}
}

void check(Result result, std::string_view message)
{
    if (result != Result::eSuccess) {
        std::string text(message);
        if (!text.empty()) text += ": ";
        text += vk::to_string(result);
        log::error(text);
        throw std::runtime_error(text);
    }
}

void check(VkResult result, std::string_view message)
{
    check(static_cast<Result>(result), message);
}

namespace loader {
state *get() { static state data; return &data; }
bool init(PFN_vkGetInstanceProcAddr proc)
{
    if (proc == nullptr) {
        dynamic_loader = std::make_unique<vk::DynamicLoader>();
        proc = dynamic_loader->getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
    }
    if (!proc) {
        get()->initialized = false;
        get()->get_instance_proc_addr = nullptr;
        return false;
    }
    VULKAN_HPP_DEFAULT_DISPATCHER.init(proc);
    get()->get_instance_proc_addr = proc;
    get()->initialized = true;
    return true;
}
void shutdown()
{
    get()->initialized = false;
    get()->get_instance_proc_addr = nullptr;
    dynamic_loader.reset();
}
}

namespace instance {
state *get() { static state data; return &data; }
vk::Instance init(const vk::InstanceCreateInfo &info)
{
    if (get()->handle) throw std::runtime_error("vk::instance already initialized");
    if (!loader::get()->initialized && !loader::init())
        throw std::runtime_error("Vulkan loader unavailable");
    auto handle = vk::createInstance(info);
    adopt(handle, true);
    return handle;
}
void adopt(vk::Instance handle, bool owned)
{
    if (!loader::get()->initialized) throw std::runtime_error("vk::loader not initialized");
    if (get()->handle) throw std::runtime_error("vk::instance already initialized");
    get()->handle = handle;
    get()->owned = owned;
    if (handle) VULKAN_HPP_DEFAULT_DISPATCHER.init(handle);
}
std::vector<vk::PhysicalDevice> physical_devices()
{
    return require_instance().enumeratePhysicalDevices();
}
std::vector<vk::ExtensionProperties> extensions(const char *layer)
{
    if (!loader::get()->initialized && !loader::init())
        throw std::runtime_error("Vulkan loader unavailable");
    return vk::enumerateInstanceExtensionProperties(layer);
}
std::vector<vk::LayerProperties> layers()
{
    if (!loader::get()->initialized && !loader::init())
        throw std::runtime_error("Vulkan loader unavailable");
    return vk::enumerateInstanceLayerProperties();
}
void shutdown()
{
    auto &s = *get();
    if (s.owned && s.handle) s.handle.destroy();
    s = {};
}
}

namespace physical_device {
state *get() { static state data; return &data; }
vk::PhysicalDevice adopt(vk::PhysicalDevice handle)
{
    if (!handle) throw std::invalid_argument("null physical device");
    auto &s = *get();
    s.handle = handle;
    s.properties = handle.getProperties();
    s.features = handle.getFeatures();
    s.memory = handle.getMemoryProperties();
    s.queue_families = handle.getQueueFamilyProperties();
    return handle;
}
vk::PhysicalDevice init(uint32_t index)
{
    auto devices = instance::physical_devices();
    if (index >= devices.size()) throw std::out_of_range("physical device index");
    return adopt(devices[index]);
}
vk::PhysicalDevice pick(vk::QueueFlags flags)
{
    auto devices = instance::physical_devices();
    for (auto candidate : devices) {
        for (auto family : candidate.getQueueFamilyProperties()) {
            if (family.queueCount && (family.queueFlags & flags) == flags)
                return adopt(candidate);
        }
    }
    throw std::runtime_error("no compatible physical device");
}
uint32_t find_queue(vk::QueueFlags flags)
{
    auto &families = get()->queue_families;
    for (uint32_t i = 0; i < families.size(); ++i)
        if (families[i].queueCount && (families[i].queueFlags & flags) == flags)
            return i;
    throw std::runtime_error("queue family unavailable");
}
bool supports_extension(const char *name)
{
    for (auto &ext : require_physical().enumerateDeviceExtensionProperties())
        if (std::strcmp(ext.extensionName.data(), name) == 0) return true;
    return false;
}
void shutdown() { *get() = {}; }
}

namespace surface {
state *get() { static state data; return &data; }
void init(vk::SurfaceKHR handle, bool owned)
{
    if (get()->handle) throw std::runtime_error("surface already initialized");
    get()->handle = handle;
    get()->owned = owned;
}
std::vector<vk::SurfaceFormatKHR> formats()
{
    return require_physical().getSurfaceFormatsKHR(get()->handle);
}
std::vector<vk::PresentModeKHR> present_modes()
{
    return require_physical().getSurfacePresentModesKHR(get()->handle);
}
vk::SurfaceCapabilitiesKHR capabilities()
{
    return require_physical().getSurfaceCapabilitiesKHR(get()->handle);
}
bool supports(uint32_t family)
{
    return require_physical().getSurfaceSupportKHR(family, get()->handle);
}
void shutdown()
{
    auto &s = *get();
    if (s.owned && s.handle) require_instance().destroySurfaceKHR(s.handle);
    s = {};
}
}

namespace device {
state *get() { static state data; return &data; }
vk::Device init(const vk::DeviceCreateInfo &info)
{
    if (get()->handle) throw std::runtime_error("device already initialized");
    auto handle = require_physical().createDevice(info);
    adopt(handle, true);
    return handle;
}
void adopt(vk::Device handle, bool owned)
{
    if (!handle) throw std::invalid_argument("null logical device");
    if (get()->handle) throw std::runtime_error("device already initialized");
    get()->handle = handle;
    get()->owned = owned;
    VULKAN_HPP_DEFAULT_DISPATCHER.init(handle);
}
void idle() { require_device().waitIdle(); }
void shutdown()
{
    auto &s = *get();
    if (s.owned && s.handle) s.handle.destroy();
    s = {};
}
}

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

namespace swapchain {
state *get() { static state data; return &data; }
vk::SwapchainKHR init(const vk::SwapchainCreateInfoKHR &info)
{
    auto d = require_device();
    auto &s = *get();
    auto create_info = info;
    if (!create_info.oldSwapchain) create_info.oldSwapchain = s.handle;
    auto handle = d.createSwapchainKHR(create_info);
    std::vector<vk::Image> images;
    try {
        images = d.getSwapchainImagesKHR(handle);
    } catch (...) {
        d.destroySwapchainKHR(handle);
        throw;
    }
    if (s.handle) d.destroySwapchainKHR(s.handle);
    s.handle = handle;
    s.images = std::move(images);
    s.extent = info.imageExtent;
    s.format = info.imageFormat;
    s.color_space = info.imageColorSpace;
    s.size = static_cast<uint32_t>(s.images.size());
    return handle;
}
void shutdown()
{
    auto &s = *get();
    if (s.handle) require_device().destroySwapchainKHR(s.handle);
    s = {};
}
}

namespace memory {
uint32_t type_index(uint32_t bits, vk::MemoryPropertyFlags flags)
{
    auto &p = physical_device::get()->memory;
    if (!physical_device::get()->handle) throw std::runtime_error("physical device not initialized");
    for (uint32_t i = 0; i < p.memoryTypeCount; ++i)
        if ((bits & (1u << i)) && (p.memoryTypes[i].propertyFlags & flags) == flags)
            return i;
    throw std::runtime_error("compatible Vulkan memory type not found");
}
vk::DeviceMemory allocate(const vk::MemoryRequirements &requirements,
                          vk::MemoryPropertyFlags flags)
{
    vk::MemoryAllocateInfo info{};
    info.allocationSize = requirements.size;
    info.memoryTypeIndex = type_index(requirements.memoryTypeBits, flags);
    return require_device().allocateMemory(info);
}
void free(vk::DeviceMemory handle)
{
    if (handle) require_device().freeMemory(handle);
}
}

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

namespace image_view {
vk::ImageView create(const vk::ImageViewCreateInfo &info)
{
    return require_device().createImageView(info);
}
void destroy(vk::ImageView handle)
{
    if (handle) require_device().destroyImageView(handle);
}
}

namespace sampler {
vk::Sampler create(const vk::SamplerCreateInfo &info)
{
    return require_device().createSampler(info);
}
void destroy(vk::Sampler handle)
{
    if (handle) require_device().destroySampler(handle);
}
}

namespace shader {
std::vector<uint32_t> read(const std::string &path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) throw std::runtime_error("cannot open SPIR-V file: " + path);
    auto bytes = file.tellg();
    if (bytes < 0 || bytes % 4) throw std::runtime_error("invalid SPIR-V size");
    std::vector<uint32_t> code(static_cast<size_t>(bytes) / 4);
    file.seekg(0);
    if (!file.read(reinterpret_cast<char *>(code.data()), bytes))
        throw std::runtime_error("cannot read SPIR-V file: " + path);
    return code;
}
vk::ShaderModule create(const uint32_t *words, size_t count)
{
    if (!words || count == 0) throw std::invalid_argument("empty SPIR-V shader");
    vk::ShaderModuleCreateInfo info{};
    info.codeSize = count * sizeof(uint32_t);
    info.pCode = words;
    return require_device().createShaderModule(info);
}
vk::ShaderModule create(const std::vector<uint32_t> &words)
{
    return create(words.data(), words.size());
}
void destroy(vk::ShaderModule handle)
{
    if (handle) require_device().destroyShaderModule(handle);
}
}

namespace command {
state *get() { static state data; return &data; }
vk::CommandPool init(uint32_t family, vk::CommandPoolCreateFlags flags)
{
    auto &s = *get();
    if (s.pool) throw std::runtime_error("command pool already initialized");
    vk::CommandPoolCreateInfo info{};
    info.queueFamilyIndex = family;
    info.flags = flags;
    s.pool = require_device().createCommandPool(info);
    s.family = family;
    return s.pool;
}
std::vector<vk::CommandBuffer> allocate(uint32_t count, vk::CommandBufferLevel level)
{
    vk::CommandBufferAllocateInfo info{};
    info.commandPool = get()->pool;
    info.level = level;
    info.commandBufferCount = count;
    return require_device().allocateCommandBuffers(info);
}
vk::CommandBuffer begin_one_time()
{
    auto commands = allocate();
    auto command = commands.front();
    vk::CommandBufferBeginInfo info{};
    info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
    command.begin(info);
    return command;
}
void end(vk::CommandBuffer cmd) { cmd.end(); }
void free(const std::vector<vk::CommandBuffer> &commands)
{
    require_device().freeCommandBuffers(get()->pool, commands);
}
void shutdown()
{
    auto &s = *get();
    if (s.pool) require_device().destroyCommandPool(s.pool);
    s = {};
}
}

namespace descriptor {
state *get() { static state data; return &data; }
vk::DescriptorPool init(const vk::DescriptorPoolCreateInfo &info)
{
    auto &s = *get();
    if (s.pool) throw std::runtime_error("descriptor pool already initialized");
    s.pool = require_device().createDescriptorPool(info);
    return s.pool;
}
std::vector<vk::DescriptorSet> allocate(const std::vector<vk::DescriptorSetLayout> &layouts)
{
    vk::DescriptorSetAllocateInfo info{};
    info.descriptorPool = get()->pool;
    info.descriptorSetCount = static_cast<uint32_t>(layouts.size());
    info.pSetLayouts = layouts.data();
    return require_device().allocateDescriptorSets(info);
}
void reset()
{
    require_device().resetDescriptorPool(get()->pool);
}
void shutdown()
{
    auto &s = *get();
    if (s.pool) require_device().destroyDescriptorPool(s.pool);
    s = {};
}
}

namespace pipeline_cache {
state *get() { static state data; return &data; }
vk::PipelineCache init(const vk::PipelineCacheCreateInfo &info)
{
    auto &s = *get();
    if (s.handle) throw std::runtime_error("pipeline cache already initialized");
    s.handle = require_device().createPipelineCache(info);
    return s.handle;
}
std::vector<uint8_t> data()
{
    return require_device().getPipelineCacheData(get()->handle);
}
void shutdown()
{
    auto &s = *get();
    if (s.handle) require_device().destroyPipelineCache(s.handle);
    s = {};
}
}

namespace debug {
state *get() { static state data; return &data; }
vk::DebugUtilsMessengerEXT init(const vk::DebugUtilsMessengerCreateInfoEXT &info)
{
    auto &s = *get();
    if (s.messenger) throw std::runtime_error("debug messenger already initialized");
    s.messenger = require_instance().createDebugUtilsMessengerEXT(info);
    return s.messenger;
}
void name(vk::ObjectType type, uint64_t handle, const char *text)
{
    vk::DebugUtilsObjectNameInfoEXT info{};
    info.objectType = type;
    info.objectHandle = handle;
    info.pObjectName = text;
    require_device().setDebugUtilsObjectNameEXT(info);
}
void label(vk::CommandBuffer cmd, const char *text, const std::array<float,4> &color)
{
    vk::DebugUtilsLabelEXT info{};
    info.pLabelName = text;
    info.color = color;
    cmd.beginDebugUtilsLabelEXT(info);
}
void end_label(vk::CommandBuffer cmd) { cmd.endDebugUtilsLabelEXT(); }
void shutdown()
{
    auto &s = *get();
    if (s.messenger) require_instance().destroyDebugUtilsMessengerEXT(s.messenger);
    s = {};
}
}

namespace sync {
vk::Fence fence(bool signaled)
{
    vk::FenceCreateInfo info{};
    if (signaled) info.flags = vk::FenceCreateFlagBits::eSignaled;
    return require_device().createFence(info);
}
vk::Semaphore semaphore() { return require_device().createSemaphore({}); }
vk::Semaphore timeline(uint64_t initial_value)
{
    vk::SemaphoreTypeCreateInfo type{};
    type.semaphoreType = vk::SemaphoreType::eTimeline;
    type.initialValue = initial_value;
    vk::SemaphoreCreateInfo info{};
    info.pNext = &type;
    return require_device().createSemaphore(info);
}
void destroy(vk::Fence handle)
{
    if (handle) require_device().destroyFence(handle);
}
void destroy(vk::Semaphore handle)
{
    if (handle) require_device().destroySemaphore(handle);
}
}

namespace util {
vk::Extent2D extent(uint32_t width, uint32_t height) { return {width, height}; }
uint32_t mip_levels(uint32_t width, uint32_t height)
{
    uint32_t n = width > height ? width : height, levels = 0;
    while (n) { ++levels; n >>= 1; }
    return levels;
}
vk::DeviceSize aligned(vk::DeviceSize value, vk::DeviceSize alignment)
{
    if (!alignment) return value;
    auto remainder = value % alignment;
    return remainder ? value + alignment - remainder : value;
}
std::string result_string(vk::Result result) { return vk::to_string(result); }
}

}
