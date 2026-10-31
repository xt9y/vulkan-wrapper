#pragma once
#include "../common.hpp"
namespace vk {
namespace loader {
struct state {
        bool initialized = false;
        PFN_vkGetInstanceProcAddr get_instance_proc_addr = nullptr;
    };
    state *get();
    bool init(PFN_vkGetInstanceProcAddr proc = nullptr);
    void shutdown();
}
}
