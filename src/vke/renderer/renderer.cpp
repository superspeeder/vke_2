//
// Created by andy on 3/11/2025.
//

#include "renderer.hpp"

#include "vke/global.hpp"
#include "vke/vke.hpp"

namespace vke {
    ImageSupplier::ImageSupplier()  = default;
    ImageSupplier::~ImageSupplier() = default;

    Renderer::Renderer(const Setup& setup)
        : m_Device(global::g_Device), m_FramesInFlight(setup.frames_in_flight), m_ImageSupplier(setup.image_supplier) {

        m_SyncObjects.reserve(m_FramesInFlight);
        for (std::size_t i = 0; i < m_FramesInFlight; i++) {
            m_SyncObjects.emplace_back(m_Device->create_semaphore(), m_Device->create_semaphore(), m_Device->create_fence(true));
        }

        m_CommandPool    = m_Device->handle().createCommandPool({vk::CommandPoolCreateFlagBits::eResetCommandBuffer, m_Device->queues().main.family});
        m_CommandBuffers = m_Device->handle().allocateCommandBuffers({m_CommandPool, vk::CommandBufferLevel::ePrimary, m_FramesInFlight});
    }

    Renderer::~Renderer() {
        if (m_AddedToStack) { global::g_RendererStack->remove(this); }

        m_Device->handle().waitIdle();
        for (const auto& [read_semaphore, write_semaphore, fence] : m_SyncObjects) {
            m_Device->destroy(read_semaphore);
            m_Device->destroy(write_semaphore);
            m_Device->destroy(fence);
        }

        m_Device->destroy(m_CommandPool);
    }

    void Renderer::render() {
        const auto& [read_semaphore, write_semaphore, in_flight_fence] = m_SyncObjects[m_CurrentFrame];
        m_Device->wait_for_fence(in_flight_fence);
        m_Device->reset_fence(in_flight_fence);

        const auto [image, image_index, will_signal_semaphore] = m_ImageSupplier.lock()->next_image(read_semaphore);

        const auto command_buffer = m_CommandBuffers[m_CurrentFrame];

        const auto frame_info = FrameInfo{
          .command_buffer   = command_buffer,
          .image            = image,
          .image_index      = image_index,
          .frame_index      = m_CurrentFrame,
          .read_semaphore   = read_semaphore,
          .write_semaphore  = write_semaphore,
          .in_flight_fence  = in_flight_fence,
          .image_properties = m_ImageSupplier.lock()->get_image_properties()
        };

        render_frame_early(frame_info);

        command_buffer.reset();
        command_buffer.begin({vk::CommandBufferUsageFlagBits::eOneTimeSubmit});
        render_frame(frame_info);
        command_buffer.end();

        vk::CommandBufferSubmitInfo command_buffer_submit_info{};
        command_buffer_submit_info.setCommandBuffer(command_buffer);

        vk::SemaphoreSubmitInfo wait_semaphore_info{};
        wait_semaphore_info.semaphore = read_semaphore;
        wait_semaphore_info.stageMask = vk::PipelineStageFlagBits2::eTopOfPipe;

        vk::SemaphoreSubmitInfo signal_semaphore_info{};
        signal_semaphore_info.semaphore = write_semaphore;

        vk::SubmitInfo2 submit_info{};
        submit_info.setCommandBufferInfos(command_buffer_submit_info);
        if (will_signal_semaphore) { submit_info.setWaitSemaphoreInfos(wait_semaphore_info); }
        submit_info.setSignalSemaphoreInfos(signal_semaphore_info);

        m_Device->queues().main.queue.submit2(submit_info, in_flight_fence);

        m_ImageSupplier.lock()->return_image(write_semaphore);
        m_CurrentFrame = (m_CurrentFrame + 1) % m_FramesInFlight;
    }

    void Renderer::render_frame_early(const FrameInfo& frame_info) {}

    RendererStack::RendererStack()  = default;
    RendererStack::~RendererStack() = default;

    void RendererStack::push(Renderer* renderer) {
        m_Renderers.push_back(renderer);
        renderer->m_AddedToStack = true;
    }

    void RendererStack::remove(Renderer* renderer) {
        m_Renderers.remove(renderer);
        renderer->m_AddedToStack = false;
    }

    void RendererStack::render() const {
        for (Renderer* renderer : m_Renderers) {
            renderer->render();
        }
    }

    void push_renderer(Renderer* renderer) {
        global::g_RendererStack->push(renderer);
    }

    void remove_renderer(Renderer* renderer) {
        global::g_RendererStack->remove(renderer);
    }
} // namespace vke
