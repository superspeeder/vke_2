//
// Created by andy on 3/10/2025.
//

#include "vke/window_manager.hpp"

namespace vke {
    void WindowManager::add(const std::shared_ptr<Window>& window) {
        assert(!m_Windows.contains(window) && "Cannot add a window multiple times");

        WindowData data{};
        data.surface      = Surface::create(window);
        m_Windows[window] = data;
    }

    void WindowManager::remove(const std::shared_ptr<Window>& window) {
        m_Windows.erase(window);
    }

    std::size_t WindowManager::count() const noexcept {
        return m_Windows.size();
    }

    std::shared_ptr<Surface> WindowManager::get_surface(const std::shared_ptr<Window>& window) const {
        return m_Windows.at(window).surface;
    }
} // namespace vke
