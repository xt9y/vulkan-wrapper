#pragma once
#include "../common.hpp"
namespace vk {
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
}
