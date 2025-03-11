//
// Created by andy on 3/9/2025.
//

#pragma once
#include "vke/pre.hpp"

#ifndef UNICODE
#  define UNICODE
#endif

#include <string>
#include <Windows.h>

#include <glm/glm.hpp>

namespace vke {

    class VKE_API Window
        : public std::enable_shared_from_this<Window>,
          public ScopedSlotSubscriber {
      public:
        struct Settings {
            std::wstring title;
            glm::ivec2   size;
        };

      private:
        explicit Window(const Settings& settings);

      public:
        static std::shared_ptr<Window> create(const Settings& settings);

        ~Window();

        LRESULT process_event(UINT msg, WPARAM wParam, LPARAM lParam);

        [[nodiscard]] bool is_close_requested() const noexcept;

        [[nodiscard]] HINSTANCE get_hinstance() const noexcept;
        [[nodiscard]] HWND      get_handle() const noexcept;

        /**
         *
         * @return Applications should break and close if this function returns false (always returns true unless the WM_QUIT message is received).
         */
        bool process_events();

        [[nodiscard]] vk::Extent2D get_extent() const;

        Slot<void(vk::Extent2D)> on_resize;
        Slot<void(bool&)>        on_close_requested;

      private:
        HINSTANCE m_hinstance;
        HWND      m_Handle;
        bool      m_IsCloseRequested = false;
    };

} // namespace vke
