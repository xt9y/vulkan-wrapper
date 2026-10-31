#include "../internal.hpp"
namespace vk::submit {
void queue(vk::Queue q, const std::vector<vk::SubmitInfo> &submits, vk::Fence f) {
    q.submit(submits, f);
}
void wait(vk::Queue q) { q.waitIdle(); }
vk::Result fence(vk::Fence f, uint64_t timeout) {
    return vk::internal::require_device().waitForFences(f, true, timeout);
}
void reset(vk::Fence f) { vk::internal::require_device().resetFences(f); }
vk::Result present(vk::Queue q, const vk::PresentInfoKHR &info) {
    return q.presentKHR(info);
}
}
