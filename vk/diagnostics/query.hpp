#pragma once
#include "../common.hpp"
namespace vk::query {
vk::QueryPool create(const vk::QueryPoolCreateInfo &info);
void destroy(vk::QueryPool handle);
}
