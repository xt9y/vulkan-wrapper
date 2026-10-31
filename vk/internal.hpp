#pragma once
#include "vk.hpp"
#include <stdexcept>
namespace vk::internal {
inline vk::Device require_device() {
    auto h = vk::device::get()->handle;
    if (!h) throw std::runtime_error("vk::device is not initialized");
    return h;
}
inline vk::Instance require_instance() {
    auto h = vk::instance::get()->handle;
    if (!h) throw std::runtime_error("vk::instance is not initialized");
    return h;
}
inline vk::PhysicalDevice require_physical() {
    auto h = vk::physical_device::get()->handle;
    if (!h) throw std::runtime_error("vk::physical_device is not initialized");
    return h;
}
}
