#pragma once
#include "../common.hpp"
namespace vk::event {
vk::Event create(const vk::EventCreateInfo &info);
void destroy(vk::Event handle);
}
