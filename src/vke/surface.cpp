//
// Created by andy on 3/10/2025.
//

#define VK_USE_PLATFORM_WIN32_KHR
#include "surface.hpp"

#include "lifecycle.hpp"
#include "utils/utils.hpp"

namespace vke {
    static vk::SurfaceFormatKHR select_surface_format(const std::vector<vk::SurfaceFormatKHR>& formats) {
        for (const auto& format : formats) {
            if (format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) { return format; }
        }
        return formats[0];
    }

    static vk::PresentModeKHR select_present_mode(const std::vector<vk::PresentModeKHR>& modes, const bool prefer_vsync = true) {
        bool has_immediate = false;
        for (const auto& mode : modes) {
            if (mode == vk::PresentModeKHR::eMailbox) { return mode; }
            if (mode == vk::PresentModeKHR::eImmediate) { has_immediate = true; }
        }
        if (!prefer_vsync && has_immediate) { return vk::PresentModeKHR::eImmediate; }
        return vk::PresentModeKHR::eFifo;
    }

    Surface::Surface(const std::shared_ptr<Device>& device, const std::shared_ptr<Window>& window) : m_Device(device), m_Window(window) {
        // Create surface
        {
            vk::Win32SurfaceCreateInfoKHR create_info{};
            create_info.hinstance = window->get_hinstance();
            create_info.hwnd      = window->get_handle();

            m_Surface = m_Device->instance()->handle().createWin32SurfaceKHR(create_info);
        }

        recreate_swapchain();

        register_listener(m_Window->on_resize, m_Window->on_resize.append([&](vk::Extent2D extent) { m_PendingRecreateSwapchain = true; }));
        register_listener(lifecycle::pre_render, lifecycle::pre_render.append([&] {
            if (m_PendingRecreateSwapchain) { recreate_swapchain(); }
        }));
    }

    Surface::~Surface() {
        m_Device->destroy(m_Swapchain);
        m_Device->destroy(m_Surface);
    }

    void Surface::recreate_swapchain() {
        {
            const vk::PhysicalDevice& physical_device      = m_Device->physical_device().physical_device();
            const auto                surface_capabilities = physical_device.getSurfaceCapabilitiesKHR(m_Surface);
            const auto                formats              = physical_device.getSurfaceFormatsKHR(m_Surface);
            const auto                present_modes        = physical_device.getSurfacePresentModesKHR(m_Surface);

            const auto surface_format            = select_surface_format(formats);
            m_SwapchainConfiguration.format      = surface_format.format;
            m_SwapchainConfiguration.color_space = surface_format.colorSpace;

            m_SwapchainConfiguration.present_mode = select_present_mode(present_modes);

            m_SwapchainConfiguration.extent = surface_capabilities.currentExtent;
            if (m_SwapchainConfiguration.extent.height == UINT32_MAX) {
                m_SwapchainConfiguration.extent = utils::clamp_extent(
                  m_Window->get_extent(), surface_capabilities.minImageExtent, surface_capabilities.maxImageExtent
                );
            }

            m_SwapchainConfiguration.min_image_count = surface_capabilities.minImageCount + 1;
            if (surface_capabilities.maxImageCount > 0 && m_SwapchainConfiguration.min_image_count > surface_capabilities.maxImageCount) {
                m_SwapchainConfiguration.min_image_count = surface_capabilities.maxImageCount;
            }

            m_SwapchainConfiguration.pre_transform = surface_capabilities.currentTransform;
        }

        vk::SwapchainCreateInfoKHR create_info{};
        create_info.surface          = m_Surface;
        create_info.minImageCount    = m_SwapchainConfiguration.min_image_count;
        create_info.imageFormat      = m_SwapchainConfiguration.format;
        create_info.imageColorSpace  = m_SwapchainConfiguration.color_space;
        create_info.imageExtent      = m_SwapchainConfiguration.extent;
        create_info.imageArrayLayers = 1;
        create_info.imageUsage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eTransferSrc;
        create_info.imageSharingMode = vk::SharingMode::eExclusive;
        create_info.preTransform     = m_SwapchainConfiguration.pre_transform;
        create_info.compositeAlpha   = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        create_info.presentMode      = m_SwapchainConfiguration.present_mode;
        create_info.clipped          = true;
        create_info.oldSwapchain     = m_Swapchain;

        m_Swapchain = m_Device->handle().createSwapchainKHR(create_info);
        m_Images    = m_Device->handle().getSwapchainImagesKHR(m_Swapchain);
        on_images_changed(m_Images);

        if (create_info.oldSwapchain) {
            // only fire this when actually recreating the swapchain, but not when we first create it.
            on_recreate_swapchain(m_Swapchain, m_SwapchainConfiguration);
            m_Device->destroy(create_info.oldSwapchain);
        }

        m_PendingRecreateSwapchain = false;
    }

    ImageProperties Surface::get_image_properties() {
        return ImageProperties{
          {m_SwapchainConfiguration.extent.width, m_SwapchainConfiguration.extent.height, 1},
          m_SwapchainConfiguration.format,
          vk::ImageType::e2D
        };
    }

    vk::Image Surface::peek_image() {
        return m_Images[m_CurrentImageIndex];
    }

    uint32_t Surface::peek_image_index() {
        return m_CurrentImageIndex;
    }

    std::tuple<vk::Image, uint32_t, bool> Surface::next_image(const vk::Semaphore read_start_semaphore) {
        try {
            const auto ir = m_Device->handle().acquireNextImageKHR(m_Swapchain, UINT64_MAX, read_start_semaphore);
            if (ir.result == vk::Result::eSuboptimalKHR) { m_PendingRecreateSwapchain = true; }
            m_CurrentImageIndex = ir.value;
            return std::make_tuple(m_Images[m_CurrentImageIndex], m_CurrentImageIndex, true);
        } catch (vk::OutOfDateKHRError& e) {
            m_PendingRecreateSwapchain = true;
            throw exception::image_not_available();
        }
    }

    const std::vector<vk::Image>& Surface::get_images() {
        return m_Images;
    }

    vk::Image Surface::get_image_by_index(const uint32_t index) {
        return m_Images[index];
    }

    void Surface::return_image(const vk::Semaphore write_finished_semaphore) {
        vk::PresentInfoKHR present_info{};
        present_info.setWaitSemaphores(write_finished_semaphore);
        present_info.setSwapchains(m_Swapchain);
        present_info.setImageIndices(m_CurrentImageIndex);
        [[maybe_unused]] auto _ = m_Device->queues().main.queue.presentKHR(present_info);
    }
} // namespace vke
