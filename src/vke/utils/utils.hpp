//
// Created by andy on 3/10/2025.
//

#pragma once

#include "vke/pre.hpp"

namespace vke::utils {
    constexpr vk::Extent2D clamp_extent(const vk::Extent2D& value, const vk::Extent2D& min, const vk::Extent2D& max) {
        return {std::clamp(value.width, min.width, max.width), std::clamp(value.height, min.height, max.height)};
    }
} // namespace vke::utils
