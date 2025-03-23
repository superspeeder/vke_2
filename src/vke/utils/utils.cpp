//
// Created by andy on 3/10/2025.
//

#include "utils.hpp"
namespace vke::utils {
    void insert_layout_transition(
      const vk::CommandBuffer          command_buffer,
      const vk::Image                  image,
      const vk::ImageSubresourceRange& isr,
      const vk::PipelineStageFlags2    source_stage,
      const ImageTransitionState&      source_state,
      const vk::PipelineStageFlags2    destination_stage,
      const ImageTransitionState&      destination_state
    ) {
        vk::ImageMemoryBarrier2 imb{};
        imb.image            = image;
        imb.subresourceRange = isr;
        imb.oldLayout        = source_state.layout;
        imb.newLayout        = destination_state.layout;
        imb.srcAccessMask    = source_state.access;
        imb.dstAccessMask    = destination_state.access;
        imb.srcStageMask     = source_stage;
        imb.dstStageMask     = destination_stage;

        vk::DependencyInfo di{};
        di.setImageMemoryBarriers(imb);
        command_buffer.pipelineBarrier2(di);
    }
} // namespace vke::utils
