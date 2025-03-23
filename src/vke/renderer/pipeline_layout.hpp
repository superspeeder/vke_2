//
// Created by andy on 3/17/2025.
//

#pragma once

#include "vke/pre.hpp"

namespace vke {

    class VKE_API PipelineLayout {
      public:
        struct Settings {};

      private:
        explicit PipelineLayout(const Settings&);

      public:
        static std::shared_ptr<PipelineLayout> create(const Settings&);

        ~PipelineLayout();

        [[nodiscard]] inline vk::PipelineLayout handle() const noexcept { return m_PipelineLayout; };

      private:
        std::shared_ptr<Device> m_Device;
        vk::PipelineLayout      m_PipelineLayout;
    };

} // namespace vke
