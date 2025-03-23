//
// Created by andy on 3/5/2025.
//

#include "main.hpp"

#include "glm/gtx/color_space.hpp"
#include "vke/utils/utils.hpp"

#include <iostream>

TestRenderer::TestRenderer(const Setup& setup, const glm::vec4& clear_color) : GenericDynamicRenderer(setup) {
    m_ClearColor     = clear_color;
    m_PipelineLayout = vke::PipelineLayout::create({});

    const auto image_props = setup.image_supplier->get_image_properties();

    vke::GraphicsPipeline::Settings settings{};
    settings.dynamic_states.push_back(vk::DynamicState::eViewport);
    settings.dynamic_states.push_back(vk::DynamicState::eScissor);
    settings.fixed_function.viewports.emplace_back();
    settings.fixed_function.scissors.emplace_back();
    settings.shader_stages.emplace_back(vke::ShaderModule::load("res/shaders/test.vert.spv"), vk::ShaderStageFlagBits::eVertex, "main");
    settings.shader_stages.emplace_back(vke::ShaderModule::load("res/shaders/test.frag.spv"), vk::ShaderStageFlagBits::eFragment, "main");
    settings.rendering_info.color_attachments.push_back(image_props.format);
    settings.layout = m_PipelineLayout;

    m_GraphicsPipeline = std::make_unique<vke::GraphicsPipeline>(settings);
}

TestRenderer::~TestRenderer() = default;

void TestRenderer::draw(const FrameInfo& frame_info) {
    frame_info.command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_GraphicsPipeline->handle());
    set_viewport(frame_info);
    set_scissor(frame_info);
    frame_info.command_buffer.draw(3, 1, 0, 0);
}

void RainbowRenderer::render_frame_early(const FrameInfo& frame_info) {
    TestRenderer::render_frame_early(frame_info);

    const std::chrono::duration<double> t = clock::now() - start;

    m_ClearColor = {glm::rgbColor(glm::vec3{fmod(static_cast<float>(t.count()) * 64.0f, 360), 1.0f, 1.0f}), 1.0f};
}

void on_ready() {
    const auto window1 = vke::Window::create({
      L"Window!",
      {800, 600}
    });

    const auto window2 = vke::Window::create({
      L"Window 2!",
      {800, 600}
    });

    const auto window3 = vke::Window::create({
      L"Window 3!",
      {800, 600}
    });

    vke::lifecycle::should_close.append([](bool& should_close) {
        if (vke::global::g_WindowManager->count() == 0) should_close = true;
    });

    auto renderer1 = std::make_unique<TestRenderer>(vke::Renderer::Setup{2, window1->get_surface()}, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    vke::push_renderer(renderer1.get());
    window1->get_surface()->owns(std::move(renderer1));

    auto renderer2 = std::make_unique<TestRenderer>(vke::Renderer::Setup{2, window2->get_surface()}, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    vke::push_renderer(renderer2.get());
    window2->get_surface()->owns(std::move(renderer2));

    auto renderer3 = std::make_unique<RainbowRenderer>(vke::Renderer::Setup{2, window3->get_surface()}, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    vke::push_renderer(renderer3.get());
    window3->get_surface()->owns(std::move(renderer3));
}

void on_cleanup() {}

void setup_listeners() {
    vke::lifecycle::vulkan_is_available.append([] { std::cout << vke::get_vulkan_instance_version() << std::endl; });
    vke::lifecycle::ready.append(on_ready);
    vke::lifecycle::cleanup_user.append(on_cleanup);
}

int main() {
    setup_listeners();
    vke::lifecycle::run();
    return vke::global::g_ExitCode;
}
