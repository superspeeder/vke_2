//
// Created by andy on 3/5/2025.
//

#pragma once

#include "vke/lifecycle.hpp"
#include "vke/renderer/generic_renderer.hpp"
#include "vke/renderer/graphics_pipeline.hpp"
#include "vke/renderer/renderer.hpp"
#include "vke/surface.hpp"
#include "vke/vke.hpp"
#include "vke/window.hpp"
#include "vke/window_manager.hpp"

#include <chrono>

class TestRenderer : public vke::GenericDynamicRenderer {
  public:
    explicit TestRenderer(const Setup& setup, const glm::vec4& clear_color);
    ~TestRenderer() override;

    void draw(const FrameInfo& frame_info) override;

  protected:
    std::shared_ptr<vke::PipelineLayout>   m_PipelineLayout;
    std::unique_ptr<vke::GraphicsPipeline> m_GraphicsPipeline;
};

class RainbowRenderer : public TestRenderer {
  public:
    RainbowRenderer(const Setup& setup, const glm::vec4& clear_color) : TestRenderer(setup, clear_color) {}
    ~RainbowRenderer() override = default;

    void render_frame_early(const FrameInfo& frame_info) override;

  private:
    using clock             = std::chrono::high_resolution_clock;
    clock::time_point start = clock::now();
};
