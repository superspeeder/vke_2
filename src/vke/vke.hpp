//
// Created by andy on 3/5/2025.
//

#pragma once

#include "vke/pre.hpp"

#include "vke/dependency.hpp"
#include "vke/utils/types.hpp"

#include <vulkan/vulkan.hpp>

namespace vke {
    ////////////////////////
    /// Global Functions ///
    ////////////////////////
    VKE_API Version get_version();
    VKE_API std::string_view get_target_platform_extension();

    VKE_API void    load_vulkan();
    VKE_API void    load_vulkan(vk::Instance instance);
    VKE_API void    load_vulkan(vk::Device device);
    VKE_API Version get_vulkan_instance_version();
    VKE_API std::vector<vk::ExtensionProperties> get_vulkan_instance_extensions();
    VKE_API std::vector<vk::ExtensionProperties> get_vulkan_instance_extensions(const std::string& layer_name);
    VKE_API std::vector<vk::LayerProperties> get_vulkan_instance_layers();
    VKE_API vk::Instance create_instance_easy(std::string_view app_name, Version app_version);
    VKE_API void         load_vulkan(const std::shared_ptr<Instance>& instance);
    VKE_API void         load_vulkan(const std::shared_ptr<Device>& device);

    ///////////////
    /// Structs ///
    ///////////////
    struct Queue {
        vk::Queue queue;
        uint32_t  family;
    };

    struct QueueCollection {
        Queue main;
    };

    ///////////////
    /// Classes ///
    ///////////////
    class VKE_API Instance final : public std::enable_shared_from_this<Instance>,
                                   public Ownable {
        Instance(std::string_view app_name, Version app_version);

      public:
        inline static std::shared_ptr<Instance> create(const std::string_view app_name, const Version app_version) {
            return std::shared_ptr<Instance>(new Instance(app_name, app_version));
        }

        ~Instance() override;

        [[nodiscard]] std::vector<PhysicalDevice>                    get_physical_devices();
        [[nodiscard]] vk::PFN_VoidFunction                           get_proc_addr(std::string_view name) const;
        [[nodiscard]] std::vector<vk::PhysicalDeviceGroupProperties> get_physical_device_groups() const;
        [[nodiscard]] vk::SurfaceKHR             create_headless_surface(const vk::HeadlessSurfaceCreateInfoEXT& create_info) const;
        [[nodiscard]] vk::DebugReportCallbackEXT create_debug_report_callback(const vk::DebugReportCallbackCreateInfoEXT& create_info) const;
        [[nodiscard]] vk::DebugUtilsMessengerEXT create_debug_utils_messenger(const vk::DebugUtilsMessengerCreateInfoEXT& create_info) const;
        [[nodiscard]] vk::SurfaceKHR             create_display_plane_surface(const vk::DisplaySurfaceCreateInfoKHR& create_info) const;

        void destroy_surface(vk::SurfaceKHR surface) const;
        void destroy_debug_report_callback(vk::DebugReportCallbackEXT callback) const;
        void destroy_debug_utils_messenger(vk::DebugUtilsMessengerEXT messenger) const;

        void debug_report_message(
          vk::DebugReportFlagsEXT      flags,
          vk::DebugReportObjectTypeEXT object_type,
          uint64_t                     object,
          size_t                       location,
          int32_t                      message_code,
          const std::string&           layer_prefix,
          const std::string&           message
        ) const;

        void submit_debug_utils_message(
          vk::DebugUtilsMessageSeverityFlagBitsEXT      severity,
          vk::DebugUtilsMessageTypeFlagsEXT             type,
          const vk::DebugUtilsMessengerCallbackDataEXT& callback_data
        ) const;

        inline const vk::Instance& handle() const { return m_Instance; }

        template<instance_destructible T>
        inline void destroy(T object) const {
            m_Instance.destroy(object);
        }

      private:
        vk::Instance m_Instance;
    };

    class VKE_API PhysicalDevice {
      public:
        PhysicalDevice(const std::shared_ptr<Instance>& instance, vk::PhysicalDevice physical_device);

        [[nodiscard]] vk::PhysicalDevice               physical_device() const;
        [[nodiscard]] const std::shared_ptr<Instance>& instance() const;

        VKE_API friend bool operator==(const PhysicalDevice& lhs, const PhysicalDevice& rhs);
        VKE_API friend bool operator!=(const PhysicalDevice& lhs, const PhysicalDevice& rhs);

        [[nodiscard]] std::shared_ptr<Device> create_device(const DeviceOptions& options) const;

        template<instance_destructible T>
        inline void destroy(T object) const {
            m_Instance->destroy(object);
        }

      private:
        std::shared_ptr<Instance> m_Instance;
        vk::PhysicalDevice        m_PhysicalDevice;
    };

    class VKE_API Device : public std::enable_shared_from_this<Device>,
                           public Ownable {
        Device(const PhysicalDevice& physical_device, vk::Device device, QueueCollection queue_collection);

      public:
        inline static std::shared_ptr<Device> create(const PhysicalDevice& physical_device, const vk::Device device, const QueueCollection queue_collection) {
            return std::shared_ptr<Device>(new Device(physical_device, device, queue_collection));
        }

        ~Device();

        [[nodiscard]] inline vk::Device                       handle() const noexcept { return m_Device; }
        [[nodiscard]] inline const std::shared_ptr<Instance>& instance() const noexcept { return m_PhysicalDevice.instance(); }
        [[nodiscard]] inline const PhysicalDevice&            physical_device() const noexcept { return m_PhysicalDevice; }

        template<instance_destructible T>
        inline void destroy(T object) const {
            physical_device().destroy(object);
        }

        template<device_destructible T>
        inline void destroy(T object) const {
            handle().destroy(object);
        }

        [[nodiscard]] inline const QueueCollection& queues() const noexcept { return m_QueueCollection; }

        [[nodiscard]] vk::Semaphore create_semaphore() const;
        [[nodiscard]] vk::Fence     create_fence() const;
        [[nodiscard]] vk::Fence     create_fence(bool signaled) const;

        void wait_for_fence(vk::Fence fence) const;
        void reset_fence(vk::Fence fence) const;

      private:
        PhysicalDevice  m_PhysicalDevice;
        vk::Device      m_Device;
        QueueCollection m_QueueCollection;
    };

} // namespace vke
