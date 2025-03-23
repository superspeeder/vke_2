//
// Created by andy on 3/17/2025.
//

#pragma once
#include "vke/pre.hpp"

#include "vke/dependency.hpp"
#include "vke/vke.hpp"

#include <filesystem>

namespace vke {

    class VKE_API ShaderModule : public std::enable_shared_from_this<ShaderModule> {
        explicit ShaderModule(vk::ShaderModule);

      public:
        ~ShaderModule();

        static std::shared_ptr<ShaderModule> load(const std::filesystem::path& path);
        static std::shared_ptr<ShaderModule> create(const std::vector<uint32_t>& code);

        [[nodiscard]] inline vk::ShaderModule handle() const noexcept { return m_ShaderModule; };

      private:
        std::shared_ptr<Device> m_Device;
        vk::ShaderModule        m_ShaderModule;
    };

} // namespace vke
