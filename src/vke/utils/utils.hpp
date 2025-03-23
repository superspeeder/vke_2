//
// Created by andy on 3/10/2025.
//

#pragma once

#include "vke/pre.hpp"
#include "vke/utils/types.hpp"

namespace vke::utils {
    constexpr vk::Extent2D clamp_extent(const vk::Extent2D& value, const vk::Extent2D& min, const vk::Extent2D& max) {
        return {std::clamp(value.width, min.width, max.width), std::clamp(value.height, min.height, max.height)};
    }

    VKE_API void insert_layout_transition(
      vk::CommandBuffer                command_buffer,
      vk::Image                        image,
      const vk::ImageSubresourceRange& isr,
      vk::PipelineStageFlags2          source_stage,
      const ImageTransitionState&      source_state,
      vk::PipelineStageFlags2          destination_stage,
      const ImageTransitionState&      destination_state
    );
} // namespace vke::utils
