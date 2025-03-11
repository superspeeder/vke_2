//
// Created by andy on 3/5/2025.
//

#include "main.hpp"

#include <iostream>

#include "vke/lifecycle.hpp"
#include "vke/surface.hpp"
#include "vke/vke.hpp"
#include "vke/window.hpp"
#include "vke/window_manager.hpp"

void on_ready() {
    const auto window1 = vke::Window::create({
      L"Window!",
      {800, 600}
    });

    const auto window2 = vke::Window::create({
      L"Window 2!",
      {800, 600}
    });

    vke::lifecycle::should_close.append([](bool& should_close) {
        if (vke::global::g_WindowManager->count() == 0) should_close = true;
    });

    vke::global::g_WindowManager->get_surface(window1)->on_recreate_swapchain.append(
      [](vk::SwapchainKHR swapchain, const vke::Surface::SwapchainConfiguration& swapchain_configuration) {
          std::cout << "Window 1 Recreate Swapchain\n";
      }
    );

    vke::global::g_WindowManager->get_surface(window2)->on_recreate_swapchain.append(
      [](vk::SwapchainKHR swapchain, const vke::Surface::SwapchainConfiguration& swapchain_configuration) {
          std::cout << "Window 2 Recreate Swapchain\n";
      }
    );
}

void on_cleanup() {}

void setup_listeners() {
    vke::lifecycle::vulkan_is_available.append([] { std::cout << vke::get_vulkan_instance_version() << std::endl; });
    vke::lifecycle::ready.append(on_ready);
    vke::lifecycle::cleanup_user.append(on_cleanup);
}

int main() {
    setup_listeners();
    vke::lifecycle::run();
    return vke::global::g_ExitCode;
}
