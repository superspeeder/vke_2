//
// Created by andy on 3/10/2025.
//

#include "vke/lifecycle.hpp"

#include "vke/global.hpp"
#include "vke/renderer/renderer.hpp"
#include "vke/window.hpp"
#include "vke/window_manager.hpp"

#include "vke/vke.hpp"

#include <iostream>

namespace vke::lifecycle {
    Signal<void(AppConfiguration&)> configure_app;

    Signal<void()> start;
    Signal<void()> cleanup;
    Signal<void()> cleanup_user;
    Signal<void()> ready;

    Signal<void()> pre_update;
    Signal<void()> update;
    Signal<void()> post_update;

    Signal<void()> pre_render;
    Signal<void()> render;
    Signal<void()> post_render;

    Signal<void(bool&)> should_close;

    Signal<void()> vulkan_is_available;

    namespace internal {
        Signal<void(const std::shared_ptr<Window>&)> register_new_window;
        Signal<void()>                               post_instance;
        Signal<void()>                               post_physical_device;
        Signal<void()>                               post_device;
        Signal<void()>                               os_poll;
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
            global::g_RendererStack = std::make_shared<RendererStack>();
        });

        cleanup.append([] {
            global::g_Device->handle().waitIdle();
            cleanup_user();

            global::g_RendererStack.reset();
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
            global::g_RendererStack->render();
            post_render();

            lifecycle::should_close(should_close);
        }
    }
} // namespace vke::lifecycle
