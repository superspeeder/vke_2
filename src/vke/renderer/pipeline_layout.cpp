//
// Created by andy on 3/17/2025.
//

#include "pipeline_layout.hpp"

#include "vke/global.hpp"
#include "vke/vke.hpp"

namespace vke {
    PipelineLayout::PipelineLayout(const Settings& settings) : m_Device(global::g_Device) {
        vk::PipelineLayoutCreateInfo create_info{};
        m_PipelineLayout = m_Device->handle().createPipelineLayout(create_info);
    }

    std::shared_ptr<PipelineLayout> PipelineLayout::create(const Settings& settings) {
        return std::shared_ptr<PipelineLayout>(new PipelineLayout(settings));
    }

    PipelineLayout::~PipelineLayout() {
        m_Device->destroy(m_PipelineLayout);
    }
} // namespace vke
