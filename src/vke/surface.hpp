//
// Created by andy on 3/10/2025.
//

#pragma once

#include "global.hpp"
#include "vke/pre.hpp"

#include "vke/vke.hpp"
#include "vke/window.hpp"

#include "vke/renderer/renderer.hpp"

namespace vke {
    class VKE_API Surface : public std::enable_shared_from_this<Surface>,
                            public ScopedSlotSubscriber,
                            public ImageSupplier,
                            public Ownable {
        Surface(const std::shared_ptr<Device>& device, const std::shared_ptr<Window>& window);

      public:
        static constexpr std::size_t FRAMES_IN_FLIGHT = 2;

        struct SwapchainConfiguration {
            vk::Format                      format;
            vk::ColorSpaceKHR               color_space;
            vk::Extent2D                    extent;
            vk::PresentModeKHR              present_mode;
            uint32_t                        min_image_count;
            vk::SurfaceTransformFlagBitsKHR pre_transform;
        };

        [[nodiscard]] static inline std::shared_ptr<Surface> create(const std::shared_ptr<Window>& window) {
            return std::shared_ptr<Surface>(new Surface(global::g_Device, window));
        }

        ~Surface() override;

        void recreate_swapchain();

        [[nodiscard]] inline vk::SwapchainKHR              swapchain() const { return m_Swapchain; };
        [[nodiscard]] inline vk::SurfaceKHR                surface() const { return m_Surface; };
        [[nodiscard]] inline const SwapchainConfiguration& configuration() const { return m_SwapchainConfiguration; }
        [[nodiscard]] inline const std::vector<vk::Image>& images() const { return m_Images; };

        ImageProperties                       get_image_properties() override;
        vk::Image                             peek_image() override;
        uint32_t                              peek_image_index() override;
        std::tuple<vk::Image, uint32_t, bool> next_image(vk::Semaphore read_start_semaphore) override;
        const std::vector<vk::Image>&         get_images() override;
        vk::Image                             get_image_by_index(uint32_t index) override;
        void                                  return_image(vk::Semaphore write_finished_semaphore) override;

        Signal<void(vk::SwapchainKHR new_swapchain, const SwapchainConfiguration& new_configuration)> on_recreate_swapchain;

      private:
        std::shared_ptr<Device> m_Device;
        std::shared_ptr<Window> m_Window;

        SwapchainConfiguration m_SwapchainConfiguration;
        bool                   m_PendingRecreateSwapchain = false;

        vk::SurfaceKHR         m_Surface;
        vk::SwapchainKHR       m_Swapchain;
        std::vector<vk::Image> m_Images;

        uint32_t m_CurrentImageIndex = 0;
    };
} // namespace vke
