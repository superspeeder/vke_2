//
// Created by andy on 3/10/2025.
//

#pragma once

#include "pre.hpp"

namespace vke::global {
    extern VKE_API std::shared_ptr<Instance> g_Instance;
    extern VKE_API std::optional<PhysicalDevice> g_PhysicalDevice;
    extern VKE_API std::shared_ptr<Device> g_Device;
    extern VKE_API bool                    g_WantsQuit;
    extern VKE_API int                     g_ExitCode;
    extern VKE_API std::shared_ptr<WindowManager> g_WindowManager;
    extern VKE_API std::shared_ptr<RendererStack> g_RendererStack;
} // namespace vke::global
