//
// Created by andy on 3/5/2025.
//

#pragma once

#include "vke/pre.hpp"

#include <cinttypes>

#include <vulkan/vulkan.hpp>

namespace vke {
    struct Version {
        uint32_t major, minor, patch;

        constexpr Version(const uint32_t major, const uint32_t minor, const uint32_t patch) : major(major), minor(minor), patch(patch) {}
        explicit constexpr Version(const uint32_t vk_version_number)
            : major(VK_API_VERSION_MAJOR(vk_version_number)), minor(VK_API_VERSION_MINOR(vk_version_number)),
              patch(VK_API_VERSION_PATCH(vk_version_number)) {}

        explicit constexpr operator uint32_t() const { return VK_MAKE_API_VERSION(0, major, minor, patch); }

        VKE_API friend std::ostream& operator<<(std::ostream& os, const Version& version);
    };

    struct DeviceOptions {};
} // namespace vke
