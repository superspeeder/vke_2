//
// Created by andy on 3/10/2025.
//

#include "global.hpp"

#include "vke/vke.hpp"

namespace vke::global {
    std::shared_ptr<Instance>      g_Instance;
    std::optional<PhysicalDevice>  g_PhysicalDevice;
    std::shared_ptr<Device>        g_Device;
    bool                           g_WantsQuit = false;
    int                            g_ExitCode  = 0;
    std::shared_ptr<WindowManager> g_WindowManager;
} // namespace vke::global
