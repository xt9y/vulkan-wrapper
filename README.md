# VulkanEngine

## C build

```c
C_Dependency *vulkan = c_git(
    b, "VulkanEngine", "https://github.com/xt9y/vulkan-engine.git", "main");
c_dep_cbuild(vulkan, "VulkanEngine", C_TARGET_STATIC_LIBRARY);
c_use(app, vulkan);
```

## Headers

- `#include <vk.hpp>`
- `#include <math.hpp>`
- `#include <vulkan/vulkan.hpp>`

## Core

- `vk::check(vk::Result, std::string_view)`
- `vk::check(VkResult, std::string_view)`
- `vk::assertion::that(bool, std::string_view)`

## Log

- `vk::log::set_callback(callback)`
- `vk::log::write(level, std::string_view)`
- `vk::log::debug(std::string_view)`
- `vk::log::info(std::string_view)`
- `vk::log::warning(std::string_view)`
- `vk::log::error(std::string_view)`
- `vk::log::level::{debug, info, warning, error}`

## Loader

- `vk::loader::init(PFN_vkGetInstanceProcAddr)`
- `vk::loader::get()->initialized`
- `vk::loader::get()->get_instance_proc_addr`
- `vk::loader::shutdown()`

## Instance

- `vk::instance::init(const vk::InstanceCreateInfo&)`
- `vk::instance::adopt(vk::Instance, bool)`
- `vk::instance::get()->handle`
- `vk::instance::get()->owned`
- `vk::instance::physical_devices()`
- `vk::instance::extensions(const char*)`
- `vk::instance::layers()`
- `vk::instance::shutdown()`

## Physical device

- `vk::physical_device::init(uint32_t)`
- `vk::physical_device::adopt(vk::PhysicalDevice)`
- `vk::physical_device::pick(vk::QueueFlags)`
- `vk::physical_device::find_queue(vk::QueueFlags)`
- `vk::physical_device::supports_extension(const char*)`
- `vk::physical_device::get()->handle`
- `vk::physical_device::get()->properties`
- `vk::physical_device::get()->features`
- `vk::physical_device::get()->memory`
- `vk::physical_device::get()->queue_families`
- `vk::physical_device::shutdown()`

## Surface

- `vk::surface::init(vk::SurfaceKHR, bool)`
- `vk::surface::get()->handle`
- `vk::surface::get()->owned`
- `vk::surface::formats()`
- `vk::surface::present_modes()`
- `vk::surface::capabilities()`
- `vk::surface::supports(uint32_t)`
- `vk::surface::shutdown()`

## Device

- `vk::device::init(const vk::DeviceCreateInfo&)`
- `vk::device::adopt(vk::Device, bool)`
- `vk::device::get()->handle`
- `vk::device::get()->owned`
- `vk::device::idle()`
- `vk::device::shutdown()`

## Queue

- `vk::queue::init(uint32_t, uint32_t, uint32_t, uint32_t)`
- `vk::queue::get()->graphics`
- `vk::queue::get()->compute`
- `vk::queue::get()->transfer`
- `vk::queue::get()->present`
- `vk::queue::get()->graphics_family`
- `vk::queue::get()->compute_family`
- `vk::queue::get()->transfer_family`
- `vk::queue::get()->present_family`
- `vk::queue::shutdown()`

## Swapchain

- `vk::swapchain::init(const vk::SwapchainCreateInfoKHR&)`
- `vk::swapchain::get()->handle`
- `vk::swapchain::get()->images`
- `vk::swapchain::get()->extent`
- `vk::swapchain::get()->format`
- `vk::swapchain::get()->color_space`
- `vk::swapchain::get()->size`
- `vk::swapchain::shutdown()`

## Memory

- `vk::memory::type_index(uint32_t, vk::MemoryPropertyFlags)`
- `vk::memory::allocate(const vk::MemoryRequirements&, vk::MemoryPropertyFlags)`
- `vk::memory::free(vk::DeviceMemory)`

## Buffer

- `vk::buffer::create(vk::DeviceSize, vk::BufferUsageFlags, vk::MemoryPropertyFlags)`
- `vk::buffer::destroy(resource&)`
- `vk::buffer::map(resource&)`
- `vk::buffer::unmap(resource&)`
- `vk::buffer::flush(const resource&)`
- `vk::buffer::write(resource&, const void*, size_t, size_t)`
- `vk::buffer::descriptor(const resource&, vk::DeviceSize, vk::DeviceSize)`
- `vk::buffer::resource::handle`
- `vk::buffer::resource::memory`
- `vk::buffer::resource::size`
- `vk::buffer::resource::properties`
- `vk::buffer::resource::mapped`

## Image

- `vk::image::create(const vk::ImageCreateInfo&, vk::MemoryPropertyFlags)`
- `vk::image::destroy(resource&)`
- `vk::image::resource::handle`
- `vk::image::resource::memory`
- `vk::image::resource::size`
- `vk::image::resource::format`

## Image view

- `vk::image_view::create(const vk::ImageViewCreateInfo&)`
- `vk::image_view::destroy(vk::ImageView)`

## Sampler

- `vk::sampler::create(const vk::SamplerCreateInfo&)`
- `vk::sampler::destroy(vk::Sampler)`

## Shader

- `vk::shader::read(const std::string&)`
- `vk::shader::create(const uint32_t*, size_t)`
- `vk::shader::create(const std::vector<uint32_t>&)`
- `vk::shader::destroy(vk::ShaderModule)`

## Command

- `vk::command::init(uint32_t, vk::CommandPoolCreateFlags)`
- `vk::command::get()->pool`
- `vk::command::get()->family`
- `vk::command::allocate(uint32_t, vk::CommandBufferLevel)`
- `vk::command::begin_one_time()`
- `vk::command::end(vk::CommandBuffer)`
- `vk::command::free(const std::vector<vk::CommandBuffer>&)`
- `vk::command::shutdown()`

## Descriptor

- `vk::descriptor::init(const vk::DescriptorPoolCreateInfo&)`
- `vk::descriptor::get()->pool`
- `vk::descriptor::allocate(const std::vector<vk::DescriptorSetLayout>&)`
- `vk::descriptor::reset()`
- `vk::descriptor::shutdown()`

## Pipeline cache

- `vk::pipeline_cache::init(const vk::PipelineCacheCreateInfo&)`
- `vk::pipeline_cache::get()->handle`
- `vk::pipeline_cache::data()`
- `vk::pipeline_cache::shutdown()`

## Debug

- `vk::debug::init(const vk::DebugUtilsMessengerCreateInfoEXT&)`
- `vk::debug::get()->messenger`
- `vk::debug::name(vk::ObjectType, uint64_t, const char*)`
- `vk::debug::label(vk::CommandBuffer, const char*, const std::array<float,4>&)`
- `vk::debug::end_label(vk::CommandBuffer)`
- `vk::debug::shutdown()`

## Synchronization

- `vk::sync::fence(bool)`
- `vk::sync::semaphore()`
- `vk::sync::timeline(uint64_t)`
- `vk::sync::destroy(vk::Fence)`
- `vk::sync::destroy(vk::Semaphore)`

## Utility

- `vk::util::extent(uint32_t, uint32_t)`
- `vk::util::mip_levels(uint32_t, uint32_t)`
- `vk::util::aligned(vk::DeviceSize, vk::DeviceSize)`
- `vk::util::result_string(vk::Result)`

## Math scalars

- `vk::util::math::pi`
- `vk::util::math::radians(float)`
- `vk::util::math::degrees(float)`
- `vk::util::math::clamp(T, T, T)`
- `vk::util::math::lerp(T, T, float)`
- `vk::util::math::min(T, T)`
- `vk::util::math::max(T, T)`

## Math vectors

- `vk::util::math::vec2`
- `vk::util::math::vec3`
- `vk::util::math::vec4`
- `vk::util::math::vec2f`
- `vk::util::math::vec3f`
- `vk::util::math::vec4f`
- `vk::util::math::dot(vec2, vec2)`
- `vk::util::math::dot(vec3, vec3)`
- `vk::util::math::dot(vec4, vec4)`
- `vk::util::math::cross(vec3, vec3)`
- `vk::util::math::length_squared(vec2|vec3|vec4)`
- `vk::util::math::length(vec2|vec3|vec4)`
- `vk::util::math::normalize(vec2|vec3|vec4)`
- `vk::util::math::distance(vec2|vec3)`
- `vk::util::math::reflect(vec3, vec3)`
- `vk::util::math::project(vec3, vec3)`

## Math quaternions

- `vk::util::math::quat`
- `vk::util::math::quatf`
- `vk::util::math::quat::identity()`
- `vk::util::math::dot(quat, quat)`
- `vk::util::math::normalize(quat)`
- `vk::util::math::conjugate(quat)`
- `vk::util::math::inverse(quat)`
- `vk::util::math::operator*(quat, quat)`
- `vk::util::math::angle_axis(float, vec3)`
- `vk::util::math::euler(float, float, float)`
- `vk::util::math::nlerp(quat, quat, float)`
- `vk::util::math::slerp(quat, quat, float)`
- `vk::util::math::rotate(quat, vec3)`

## Math matrices

- `vk::util::math::mat4`
- `vk::util::math::mat4f`
- `vk::util::math::mat4::identity()`
- `vk::util::math::mat4::data()`
- `vk::util::math::mat4::operator()(size_t, size_t)`
- `vk::util::math::operator*(mat4, mat4)`
- `vk::util::math::operator*(mat4, vec4)`
- `vk::util::math::transpose(mat4)`
- `vk::util::math::translate(vec3)`
- `vk::util::math::scale(vec3)`
- `vk::util::math::rotation(quat)`
- `vk::util::math::trs(vec3, quat, vec3)`
- `vk::util::math::look_at(vec3, vec3, vec3)`
- `vk::util::math::perspective(float, float, float, float, bool)`
- `vk::util::math::orthographic(float, float, float, float, float, float, bool)`

## Native Vulkan API

- `vk::Instance`
- `vk::PhysicalDevice`
- `vk::Device`
- `vk::Queue`
- `vk::SurfaceKHR`
- `vk::SwapchainKHR`
- `vk::Buffer`
- `vk::Image`
- `vk::ImageView`
- `vk::DeviceMemory`
- `vk::Sampler`
- `vk::ShaderModule`
- `vk::Pipeline`
- `vk::PipelineLayout`
- `vk::RenderPass`
- `vk::Framebuffer`
- `vk::CommandPool`
- `vk::CommandBuffer`
- `vk::DescriptorPool`
- `vk::DescriptorSet`
- `vk::DescriptorSetLayout`
- `vk::Fence`
- `vk::Semaphore`
- `vk::Event`
- `vk::QueryPool`
- `vk::AccelerationStructureKHR`
- `vk::VideoSessionKHR`
- `vk::DebugUtilsMessengerEXT`
- `vk::DispatchLoaderDynamic`
- `vk::DynamicLoader`
- `VULKAN_HPP_DEFAULT_DISPATCHER`
