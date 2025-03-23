//
// Created by andy on 3/11/2025.
//

#pragma once

#include "vke/dependency.hpp"
#include "vke/pre.hpp"
#include "vke/utils/types.hpp"

#include <stack>

namespace vke {

    /**
     * Image supplier: interface for supplying images to the renderer.
     */
    class VKE_API ImageSupplier {
      public:
        ImageSupplier();
        virtual ~ImageSupplier();

        // This should be guaranteed to apply to the currently looked at image and all images that can be peeked from the current state (i.e. the list
        // of available images should be uniformly the same size, type, and format).
        virtual ImageProperties get_image_properties() = 0;

        // The "peek" functions should view but not change the current image.
        virtual vk::Image peek_image()       = 0;
        virtual uint32_t  peek_image_index() = 0;

        // You must supply a semaphore that is signaled when the image is ready. If the method of image provision doesn't require a semaphore, the
        // bool in the returned tuple will be false.
        virtual std::tuple<vk::Image, uint32_t, bool> next_image(vk::Semaphore read_start_semaphore) = 0;

        virtual const std::vector<vk::Image>& get_images()                       = 0;
        virtual vk::Image                     get_image_by_index(uint32_t index) = 0;

        // This function does whatever the implementor needs to do once the user is done with an image. If a semaphore is passed in, the system will
        // wait on it before any operations are done on the image (to prevent read-after-write and write-after-write faults. For example, swapchain
        // presentation requires that a renderer signal a semaphore which the present call waits on to prevent a frame from being presented and
        // released back into the pool and made available again while it is being rendered to).
        virtual void return_image(vk::Semaphore write_finished_semaphore) = 0;
        inline void  return_image() { return_image(nullptr); };

        // This should be fired with the set of new images passed in whenever images are changed (for example, a surface will fire this on swapchain
        // recreation). Renderers should hook into this to do things like recreate image views or framebuffers.
        Signal<void(const std::vector<vk::Image>&)> on_images_changed;
    };

    class VKE_API Renderer : public ScopedSlotSubscriber,
                             public Ownable {
      public:
        struct Setup {
            uint32_t                       frames_in_flight;
            std::shared_ptr<ImageSupplier> image_supplier;
        };

        struct FrameSync {
            vk::Semaphore read_semaphore, write_semaphore;
            vk::Fence     in_flight_fence;
        };

        struct FrameInfo {
            vk::CommandBuffer command_buffer;
            vk::Image         image;
            uint32_t          image_index;
            uint32_t          frame_index;
            vk::Semaphore     read_semaphore, write_semaphore;
            vk::Fence         in_flight_fence;
            ImageProperties   image_properties;
        };

        explicit Renderer(const Setup& setup);
        ~Renderer() override;

        virtual void render_frame_early(const FrameInfo& frame_info);
        virtual void render_frame(const FrameInfo& frame_info) = 0;

        void render();

      private:
        std::shared_ptr<Device>      m_Device;
        uint32_t                     m_FramesInFlight;
        std::weak_ptr<ImageSupplier> m_ImageSupplier;
        std::vector<FrameSync>       m_SyncObjects;
        uint32_t                     m_CurrentFrame = 0;

        vk::CommandPool                m_CommandPool;
        std::vector<vk::CommandBuffer> m_CommandBuffers;
        bool                           m_AddedToStack = false;

        friend class RendererStack;
    };

    class VKE_API RendererStack {
      public:
        RendererStack();
        ~RendererStack();

        void push(Renderer* renderer);
        void remove(Renderer* renderer);
        void render() const;

      private:
        std::list<Renderer*> m_Renderers;
    };

    void VKE_API push_renderer(Renderer* renderer);
    void VKE_API remove_renderer(Renderer* renderer);

    namespace exception {
        class image_not_available : public std::runtime_error {
          public:
            inline explicit image_not_available() : runtime_error("Image not available") {}
        };
    } // namespace exception
} // namespace vke
