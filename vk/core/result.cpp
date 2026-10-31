#include "../internal.hpp"
namespace vk {
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
}
