#pragma once
#include "../common.hpp"
namespace vk {
void check(Result result, std::string_view message = {});
void check(VkResult result, std::string_view message = {});
}
