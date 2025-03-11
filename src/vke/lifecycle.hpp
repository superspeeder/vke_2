//
// Created by andy on 3/10/2025.
//

#pragma once

#include "utils/types.hpp"
#include "vke/pre.hpp"

namespace vke {
    struct AppConfiguration {
        std::string   name;
        Version       version;
        DeviceOptions device_options;
    };
} // namespace vke

namespace vke::lifecycle {

    extern VKE_API Slot<void(AppConfiguration&)> configure_app;

    extern VKE_API Slot<void()> start;
    extern VKE_API Slot<void()> cleanup;
    extern VKE_API Slot<void()> cleanup_user;
    extern VKE_API Slot<void()> ready;

    extern VKE_API Slot<void()> pre_update;
    extern VKE_API Slot<void()> update;
    extern VKE_API Slot<void()> post_update;

    extern VKE_API Slot<void()> pre_render;
    extern VKE_API Slot<void()> render;
    extern VKE_API Slot<void()> post_render;

    extern VKE_API Slot<void(bool&)> should_close;

    extern VKE_API Slot<void()> vulkan_is_available;

    VKE_API void run();
    VKE_API void mainloop();

    namespace internal {
        extern VKE_API Slot<void(const std::shared_ptr<Window>&)> register_new_window;
        extern VKE_API Slot<void()> post_instance;
        extern VKE_API Slot<void()> post_physical_device;
        extern VKE_API Slot<void()> post_device;
        extern VKE_API Slot<void()> os_poll;
    } // namespace internal
} // namespace vke::lifecycle
