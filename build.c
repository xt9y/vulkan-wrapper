#include <cbuild.h>

void build(C_Build *b)
{
    C_Target *engine = c_static_library(b, "VulkanEngine");
    c_sources(engine, "engine/vk.cpp");
    c_include(engine, "engine");
    c_flag(engine, "-std=c++17");

    C_Dependency *headers = c_git(
        b, "VulkanHeaders", "https://github.com/KhronosGroup/Vulkan-Headers.git", "main");
    c_dep_header_only(headers);
    c_dep_checkout(headers, "include");
    c_dep_include(headers, "include");
    c_use(engine, headers);

#ifdef __linux__
    c_link_system(engine, "dl");
#endif
    c_default_target(b, engine);
}
