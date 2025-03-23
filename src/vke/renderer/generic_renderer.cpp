//
// Created by andy on 3/20/2025.
//

#include "vke/renderer/generic_renderer.hpp"

#include "vke/global.hpp"
#include "vke/utils/utils.hpp"

#include "vke/vke.hpp"

namespace vke {
    GenericDynamicRenderer::GenericDynamicRenderer(const Setup& setup) : Renderer(setup) {
        const auto image_supplier = setup.image_supplier.get();

        m_ImageViews.resize(image_supplier->get_images().size());
        for (std::size_t i = 0; i < image_supplier->get_images().size(); i++) {
            m_ImageViews[i] = global::g_Device->handle().createImageView(
              vk::ImageViewCreateInfo(
                {}, image_supplier->get_image_by_index(i), vk::ImageViewType::e2D, image_supplier->get_image_properties().format,
                vk::ComponentMapping{vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA},
                vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1)
              )
            );
        }

        register_listener(
          image_supplier->on_images_changed, image_supplier->on_images_changed.append([this, image_supplier](const std::vector<vk::Image>& images) {
              const auto image_props = image_supplier->get_image_properties();
              for (const auto& iv : m_ImageViews) {
                  global::g_Device->destroy(iv);
              }

              m_ImageViews.resize(images.size());
              for (std::size_t i = 0; i < images.size(); i++) {
                  m_ImageViews[i] = global::g_Device->handle().createImageView(
                    vk::ImageViewCreateInfo(
                      {}, images[i], vk::ImageViewType::e2D, image_props.format,
                      vk::ComponentMapping{vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA},
                      vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1)
                    )
                  );
              }
          })
        );
    }

    GenericDynamicRenderer::~GenericDynamicRenderer() {
        for (const auto& iv : m_ImageViews) {
            global::g_Device->destroy(iv);
        }
    }

    void GenericDynamicRenderer::render_frame(const FrameInfo& frame_info) {
        static constexpr vk::ImageSubresourceRange isr{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1};
        utils::insert_layout_transition(
          frame_info.command_buffer, frame_info.image, isr, vk::PipelineStageFlagBits2::eTopOfPipe,
          {vk::ImageLayout::eUndefined, vk::AccessFlagBits2::eNone}, vk::PipelineStageFlagBits2::eColorAttachmentOutput,
          {vk::ImageLayout::eColorAttachmentOptimal, vk::AccessFlagBits2::eColorAttachmentWrite}
        );

        const vk::ClearColorValue   clear_value{m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a};
        vk::RenderingInfo           rendering_info{};
        vk::RenderingAttachmentInfo color_attachment_info{};
        color_attachment_info.clearValue  = clear_value;
        color_attachment_info.imageView   = m_ImageViews[frame_info.image_index];
        color_attachment_info.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
        color_attachment_info.loadOp      = vk::AttachmentLoadOp::eClear;
        color_attachment_info.storeOp     = vk::AttachmentStoreOp::eStore;
        color_attachment_info.resolveMode = vk::ResolveModeFlagBits::eNone;
        rendering_info.setColorAttachments(color_attachment_info);
        rendering_info.renderArea = vk::Rect2D({0, 0}, {frame_info.image_properties.extent.width, frame_info.image_properties.extent.height});
        rendering_info.layerCount = 1;

        frame_info.command_buffer.beginRendering(rendering_info);
        draw(frame_info);
        frame_info.command_buffer.endRendering();

        utils::insert_layout_transition(
          frame_info.command_buffer, frame_info.image, isr, vk::PipelineStageFlagBits2::eColorAttachmentOutput,
          {vk::ImageLayout::eColorAttachmentOptimal, vk::AccessFlagBits2::eColorAttachmentWrite}, vk::PipelineStageFlagBits2::eBottomOfPipe,
          {vk::ImageLayout::ePresentSrcKHR, vk::AccessFlagBits2::eNone}
        );
    }

    void GenericDynamicRenderer::set_viewport(const FrameInfo& frame_info) {
        frame_info.command_buffer.setViewport(
          0, vk::Viewport(0.0f, 0.0f, frame_info.image_properties.extent.width, frame_info.image_properties.extent.height, 0.0f, 1.0f)
        );
    }

    void GenericDynamicRenderer::set_scissor(const FrameInfo& frame_info) {
        frame_info.command_buffer.setScissor(0, vk::Rect2D({0, 0}, {frame_info.image_properties.extent.width, frame_info.image_properties.extent.height}));
    }
} // namespace vke
