//
// Created by andy on 3/9/2025.
//

#include "vke/window.hpp"

#include "global.hpp"
#include "lifecycle.hpp"
#include "window_manager.hpp"

#include <iostream>

namespace vke {
    static constexpr wchar_t WC_NAME[] = L"VKE_WC";

    static LRESULT CALLBACK static_window_proc(const HWND hwnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) {
        if (msg == WM_CREATE) {
            const CREATESTRUCTW* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
            SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
            return 0;
        }

        if (const LONG_PTR ptr = GetWindowLongPtrW(hwnd, GWLP_USERDATA)) {
            const auto window = reinterpret_cast<Window*>(ptr);
            return window->process_event(msg, wParam, lParam);
        }

        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }

    static void register_window_class_if_necessary() {
        const HINSTANCE hInstance = GetModuleHandle(nullptr);

        WNDCLASSW existing_class{};
        std::memset(&existing_class, 0, sizeof(WNDCLASSW));

        if (!GetClassInfoW(hInstance, WC_NAME, &existing_class)) {
            WNDCLASSEXW wc{};
            std::memset(&wc, 0, sizeof(wc));
            wc.cbSize        = sizeof(wc);
            wc.hInstance     = hInstance;
            wc.lpszClassName = WC_NAME;
            wc.lpfnWndProc   = static_window_proc;
            wc.style         = CS_VREDRAW | CS_HREDRAW;

            RegisterClassExW(&wc);
        }
    }

    Window::Window(const Settings& settings) {
        register_window_class_if_necessary();

        m_hinstance = GetModuleHandle(nullptr);

        constexpr DWORD style    = WS_OVERLAPPEDWINDOW;
        constexpr DWORD ex_style = WS_EX_OVERLAPPEDWINDOW;

        m_Handle = CreateWindowExW(
          ex_style, WC_NAME, settings.title.c_str(), style, CW_USEDEFAULT, CW_USEDEFAULT, settings.size.x, settings.size.y, nullptr, nullptr, m_hinstance, this
        );

        ShowWindow(m_Handle, SW_NORMAL);
    }

    std::shared_ptr<Window> Window::create(const Settings& settings) {
        const auto window = std::shared_ptr<Window>(new Window(settings));
        lifecycle::internal::register_new_window(window);
        return window;
    }

    Window::~Window() {
        DestroyWindow(m_Handle);
    }

    LRESULT Window::process_event(const UINT msg, const WPARAM wParam, const LPARAM lParam) {
        switch (msg) {
        case WM_DESTROY: return 0;
        case WM_CLOSE:   {
            m_IsCloseRequested = true;
            on_close_requested(m_IsCloseRequested);
            return 0;
        }
        case WM_SIZE: {
            UINT width  = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            on_resize({width, height});
            return 0;
        }
        default: break;
        }

        return DefWindowProcW(m_Handle, msg, wParam, lParam);
    }

    bool Window::is_close_requested() const noexcept {
        return m_IsCloseRequested;
    }

    HINSTANCE Window::get_hinstance() const noexcept {
        return m_hinstance;
    }

    HWND Window::get_handle() const noexcept {
        return m_Handle;
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    bool Window::process_events() {
        MSG msg{};
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE) > 0) {
            if (msg.message == WM_QUIT) { return false; }

            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        if (m_IsCloseRequested) {
            on_closed();
            global::g_WindowManager->remove(shared_from_this());
        }
        return true;
    }

    vk::Extent2D Window::get_extent() const {
        RECT r;
        GetClientRect(m_Handle, &r);
        return {static_cast<uint32_t>(r.right), static_cast<uint32_t>(r.bottom)};
    }

    std::shared_ptr<Surface> Window::get_surface() {
        return global::g_WindowManager->get_surface(shared_from_this());
    }
} // namespace vke
