//
// Created by andy on 3/17/2025.
//

#pragma once

#include "vke/pre.hpp"

#include "vke/dependency.hpp"
#include "vke/renderer/pipeline_layout.hpp"
#include "vke/renderer/shader_module.hpp"
#include "vke/vke.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace vke {

    struct GraphicsPipelineBlendFunction {
        vk::BlendFactor source_color, destination_color;
        vk::BlendOp     color_op;
        vk::BlendFactor source_alpha, destination_alpha;
        vk::BlendOp     alpha_op;
    };

    struct GraphicsPipelineBlendAttachment {
        bool                          enable_blending;
        vk::ColorComponentFlags       color_write_mask;
        GraphicsPipelineBlendFunction blend_function;
    };

    constexpr GraphicsPipelineBlendFunction DEFAULT_BLEND_FUNCTION{
      .source_color      = vk::BlendFactor::eSrcAlpha,
      .destination_color = vk::BlendFactor::eOneMinusSrcAlpha,
      .color_op          = vk::BlendOp::eAdd,
      .source_alpha      = vk::BlendFactor::eOne,
      .destination_alpha = vk::BlendFactor::eZero,
      .alpha_op          = vk::BlendOp::eAdd
    };

    constexpr GraphicsPipelineBlendAttachment DEFAULT_BLEND_ATTACHMENT{
      .enable_blending = true,
      .color_write_mask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA,
      .blend_function = DEFAULT_BLEND_FUNCTION,
    };

    struct VertexAttribute {
        uint32_t   location;
        vk::Format format;
        uint32_t   offset;
    };

    class VKE_API VertexBindingBuilder;

    struct VertexBinding {
        uint32_t                     binding;
        uint32_t                     stride;
        vk::VertexInputRate          input_rate = vk::VertexInputRate::eVertex;
        std::vector<VertexAttribute> attributes;

        static VKE_API VertexBindingBuilder builder(uint32_t binding, vk::VertexInputRate input_rate = vk::VertexInputRate::eVertex);
    };

    class VKE_API VertexBindingBuilder {
      public:
        explicit VertexBindingBuilder(uint32_t binding, vk::VertexInputRate input_rate = vk::VertexInputRate::eVertex);

        VertexBindingBuilder& attribute(uint32_t location, vk::Format format) &;
        VertexBindingBuilder  attribute(uint32_t location, vk::Format format) &&;

        VertexBinding build() const;

      private:
        uint32_t                     binding;
        uint32_t                     stride;
        vk::VertexInputRate          input_rate;
        std::vector<VertexAttribute> attributes;
    };

    class VKE_API VertexLayoutBuilder;

    struct VertexLayout {
        std::vector<VertexBinding> bindings;

        static VKE_API VertexLayoutBuilder builder();
    };

    class VKE_API VertexLayoutBuilder {
      public:
        VertexLayoutBuilder();

        VertexLayoutBuilder& binding(const VertexBinding& binding) &;
        VertexLayoutBuilder  binding(const VertexBinding& binding) &&;

      private:
        std::vector<VertexBinding> bindings;
    };

    struct DepthBiasOptions {
        float constant_factor;
        float slope_factor;
        float clamp;
    };

    struct GraphicsFixedFunctionSettings {
        VertexLayout vertex_layout;

        vk::PrimitiveTopology topology                 = vk::PrimitiveTopology::eTriangleList;
        bool                  enable_primitive_restart = false;

        uint32_t tessellation_patch_control_points = 1;

        std::vector<vk::Viewport> viewports;
        std::vector<vk::Rect2D>   scissors;

        vk::CullModeFlags               cull_mode          = vk::CullModeFlagBits::eBack;
        vk::FrontFace                   front_face         = vk::FrontFace::eClockwise;
        vk::PolygonMode                 polygon_mode       = vk::PolygonMode::eFill;
        float                           line_width         = 1.0f;
        bool                            enable_depth_clamp = false;
        std::optional<DepthBiasOptions> depth_bias         = std::nullopt;

        vk::SampleCountFlagBits     rasterization_samples = vk::SampleCountFlagBits::e1;
        std::optional<float>        min_sample_shading    = std::nullopt;
        std::vector<vk::SampleMask> sample_mask; // if this is non-empty, must be at least ceil(rasterization_samples / 32) elements.
        bool                        enable_alpha_to_coverage = false;
        bool                        enable_alpha_to_one      = false;

        bool                    enable_depth_test        = false;
        bool                    enable_depth_write       = false;
        vk::CompareOp           depth_compare_op         = vk::CompareOp::eLess;
        bool                    enable_depth_bounds_test = false;
        bool                    enable_stencil_test      = false;
        vk::StencilOpState      stencil_front            = {};
        vk::StencilOpState      stencil_back             = {};
        std::pair<float, float> depth_bounds             = {0.0f, 0.0f};

        std::optional<vk::LogicOp>                   blend_logic_op    = std::nullopt;
        std::vector<GraphicsPipelineBlendAttachment> blend_attachments = {DEFAULT_BLEND_ATTACHMENT};
        glm::vec4                                    blend_constants   = glm::zero<glm::vec4>();
    };

    struct ShaderStage {
        std::shared_ptr<ShaderModule> shader_module;
        vk::ShaderStageFlagBits       stage;
        std::string                   entry_point;
        // TODO: specialization info
    };

    struct GraphicsRenderingInfo {
        std::vector<vk::Format>   color_attachments;
        std::optional<vk::Format> depth_attachment   = std::nullopt;
        std::optional<vk::Format> stencil_attachment = std::nullopt;
        uint32_t                  view_mask          = 0;
    };

    class VKE_API GraphicsPipeline : public ScopedSlotSubscriber,
                                     public Ownable {
      public:
        struct Settings {
            GraphicsFixedFunctionSettings fixed_function{};
            GraphicsRenderingInfo         rendering_info{};

            std::vector<vk::DynamicState> dynamic_states;
            std::vector<ShaderStage>      shader_stages;

            std::shared_ptr<PipelineLayout> layout;
        };

        explicit GraphicsPipeline(const Settings& settings);

        [[nodiscard]] inline vk::Pipeline handle() const noexcept { return m_Pipeline; }

      private:
        std::shared_ptr<Device>                    m_Device;
        vk::Pipeline                               m_Pipeline;
        std::vector<std::shared_ptr<ShaderModule>> m_ShaderModules;
    };

} // namespace vke
