//
// Created by andy on 3/20/2025.
//

#pragma once

#include "vke/pre.hpp"

#include "vke/renderer/renderer.hpp"
#include "vke/vke.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace vke {
    class VKE_API GenericDynamicRenderer : public Renderer {
      public:
        explicit GenericDynamicRenderer(const Setup& setup);

        ~GenericDynamicRenderer() override;

        void render_frame(const FrameInfo& frame_info) override;

        virtual void draw(const FrameInfo& frame_info) = 0;

        static void set_viewport(const FrameInfo& frame_info);
        static void set_scissor(const FrameInfo& frame_info);

      protected:
        glm::vec4 m_ClearColor = glm::zero<glm::vec4>();

      private:
        std::vector<vk::ImageView> m_ImageViews;
    };
} // namespace vke
