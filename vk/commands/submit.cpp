#include "../internal.hpp"
namespace vk::submit {
void queue(vk::Queue q, const std::vector<vk::SubmitInfo> &submits, vk::Fence f) {
    q.submit(submits, f);
}
void wait(vk::Queue q) { q.waitIdle(); }
void fence(vk::Fence f, uint64_t timeout) {
    vk::internal::require_device().waitForFences(f, true, timeout);
}
void reset(vk::Fence f) { vk::internal::require_device().resetFences(f); }
void present(vk::Queue q, const vk::PresentInfoKHR &info) { q.presentKHR(info); }
}
