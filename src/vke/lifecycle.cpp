//
// Created by andy on 3/10/2025.
//

#include "lifecycle.hpp"

#include "global.hpp"
#include "window.hpp"
#include "window_manager.hpp"

#include "vke/vke.hpp"

#include <ios>
#include <iostream>

namespace vke::lifecycle {
    Slot<void(AppConfiguration&)> configure_app;

    Slot<void()> start;
    Slot<void()> cleanup;
    Slot<void()> cleanup_user;
    Slot<void()> ready;

    Slot<void()> pre_update;
    Slot<void()> update;
    Slot<void()> post_update;

    Slot<void()> pre_render;
    Slot<void()> render;
    Slot<void()> post_render;

    Slot<void(bool&)> should_close;

    Slot<void()> vulkan_is_available;

    namespace internal {
        Slot<void(const std::shared_ptr<Window>&)> register_new_window;
        Slot<void()>                               post_instance;
        Slot<void()>                               post_physical_device;
        Slot<void()>                               post_device;
        Slot<void()>                               os_poll;
    } // namespace internal

    void setup_listeners() {
        start.append([] {
            load_vulkan();
            vulkan_is_available();

            AppConfiguration configuration{"TestApp", vke::Version(0, 1, 0), {}};
            configure_app(configuration);

            global::g_Instance = vke::Instance::create(configuration.name, configuration.version);
            load_vulkan(global::g_Instance);
            internal::post_instance();

            global::g_PhysicalDevice = global::g_Instance->get_physical_devices()[0];
            internal::post_physical_device();

            global::g_Device = global::g_PhysicalDevice->create_device(configuration.device_options);
            load_vulkan(global::g_Device);
            internal::post_device();

            global::g_WindowManager = std::make_shared<WindowManager>();
        });

        cleanup.append([] {
            global::g_Device->handle().waitIdle();
            cleanup_user();

            global::g_WindowManager.reset();

            global::g_Device.reset();
            global::g_PhysicalDevice.reset();
            global::g_Instance.reset();
        });

        internal::register_new_window.append([](const std::shared_ptr<Window>& window) {
            window->register_listener(internal::os_poll, internal::os_poll.append([window = window->weak_from_this()] {
                if (!window.lock()->process_events()) global::g_WantsQuit = true;
            }));

            global::g_WindowManager->add(window);
        });
    }

    run_at_static_init<setup_listeners> rasi_setup_listeners;

    // TODO: for multithreading support in systems, I need some mechanism to allow threads to tell the system that they aren't ready yet. Maybe a
    //       managed threadpool and a list of condition variables which can act as barriers between stages. With condition variables I can also go the
    //       other way and allow threads to wait for certain lifecycle points.
    void run() {
        start();
        ready();
        mainloop();
        cleanup();
    }

    void mainloop() {
        bool should_close = false;
        lifecycle::should_close(should_close);
        while (!should_close && !global::g_WantsQuit) {
            internal::os_poll();

            pre_update();
            update();
            post_update();

            pre_render();
            render();
            post_render();

            lifecycle::should_close(should_close);
        }
    }
} // namespace vke::lifecycle
