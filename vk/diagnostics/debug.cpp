#include "../internal.hpp"
using vk::internal::require_device;
using vk::internal::require_instance;
using vk::internal::require_physical;
namespace vk {
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
}
