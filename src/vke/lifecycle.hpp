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

    extern VKE_API Signal<void(AppConfiguration&)> configure_app;

    extern VKE_API Signal<void()> start;
    extern VKE_API Signal<void()> cleanup;
    extern VKE_API Signal<void()> cleanup_user;
    extern VKE_API Signal<void()> ready;

    extern VKE_API Signal<void()> pre_update;
    extern VKE_API Signal<void()> update;
    extern VKE_API Signal<void()> post_update;

    extern VKE_API Signal<void()> pre_render;
    extern VKE_API Signal<void()> render;
    extern VKE_API Signal<void()> post_render;

    extern VKE_API Signal<void(bool&)> should_close;

    extern VKE_API Signal<void()> vulkan_is_available;

    VKE_API void run();
    VKE_API void mainloop();

    namespace internal {
        extern VKE_API Signal<void(const std::shared_ptr<Window>&)> register_new_window;
        extern VKE_API Signal<void()> post_instance;
        extern VKE_API Signal<void()> post_physical_device;
        extern VKE_API Signal<void()> post_device;
        extern VKE_API Signal<void()> os_poll;
    } // namespace internal
} // namespace vke::lifecycle
