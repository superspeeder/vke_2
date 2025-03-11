//
// Created by andy on 3/10/2025.
//

#pragma once

#include "vke/pre.hpp"
#include "vke/surface.hpp"
#include "vke/vke.hpp"
#include "vke/window.hpp"

#include <memory>
#include <unordered_map>

namespace vke {

    struct WindowData {
        std::shared_ptr<Surface> surface;
    };

    class VKE_API WindowManager {
      public:
        void add(const std::shared_ptr<Window>& window);
        void remove(const std::shared_ptr<Window>& window);

        [[nodiscard]] std::size_t              count() const noexcept;
        [[nodiscard]] std::shared_ptr<Surface> get_surface(const std::shared_ptr<Window>& window) const;

      private:
        std::unordered_map<std::shared_ptr<Window>, WindowData> m_Windows;
    };

} // namespace vke
