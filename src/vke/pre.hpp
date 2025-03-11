//
// Created by andy on 3/5/2025.
//

#pragma once

#ifdef VKE_SHARED
#  ifdef VKE_SHARED_EXPORTS
#    ifdef WIN32
#      define VKE_API __declspec(dllexport)
#    else
#      define VKE_API
#    endif
#    define VULKAN_HPP_STORAGE_SHARED_EXPORT
#  else
#    ifdef WIN32
#      define VKE_API __declspec(dllimport)
#    else
#      define VKE_API
#    endif
#  endif
#  define VULKAN_HPP_STORAGE_SHARED
#else
#  define VKE_API
#endif

#include <vulkan/vulkan.hpp>

#include <eventpp/callbacklist.h>

namespace vke {
    template<typename T>
    concept instance_destructible = requires(vk::Instance instance, T object) { instance.destroy(object); };

    template<typename T>
    concept device_destructible = requires(vk::Device device, T object) { device.destroy(object); };

    class VKE_API Instance;
    class VKE_API PhysicalDevice;
    struct DeviceOptions;
    class VKE_API Device;
    struct Queue;
    struct QueueCollection;
    class VKE_API Window;
    class VKE_API Surface;
    class VKE_API WindowManager;
    struct WindowData;

    template<typename F>
    class Slot;

    template<typename... Args>
    class Slot<void(Args...)> {
        eventpp::CallbackList<void(Args...)> m_CallbackList;
        using cb_base_t = eventpp::internal_::CallbackListBase<void(Args...), eventpp::DefaultPolicies>;

      public:
        using callback_t = typename cb_base_t::Callback;
        using handle_t   = typename cb_base_t::Handle;

        inline void operator()(Args... args) const { m_CallbackList(args...); }
        inline void invoke(Args... args) const { m_CallbackList(args...); }

        inline auto append(const callback_t& callback) { return m_CallbackList.append(callback); }
        inline void remove(const handle_t& handle) { m_CallbackList.remove(handle); }
    };

    template<void (*callable)()>
    struct run_at_static_init {
        run_at_static_init() { callable(); }
    };

    class ScopedSlotSubscriber {
        std::vector<std::function<void()>> m_Unsubscribers;

      public:
        inline ~ScopedSlotSubscriber() {
            for (auto& callback : m_Unsubscribers) { callback(); }
        }

        template<typename F>
        void register_listener(Slot<F>& slot, typename Slot<F>::handle_t handle) {
            Slot<F>* p_slot = &slot;
            m_Unsubscribers.emplace_back([p_slot, handle] { p_slot->remove(handle); });
        }
    };
} // namespace vke
