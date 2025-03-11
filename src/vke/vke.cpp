//
// Created by andy on 3/5/2025.
//

#ifdef WIN32
#  define VK_USE_PLATFORM_WIN32_KHR
#endif

#include "vke.hpp"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE;

namespace vke {
    constexpr Version VERSION{0, 1, 0};

    Version get_version() {
        return VERSION;
    }

    std::string_view get_target_platform_extension() {
#ifdef WIN32
        return VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
#else
#  error "Unsupported platform"
#endif
    }

    void load_vulkan() {
        VULKAN_HPP_DEFAULT_DISPATCHER.init();
    }

    void load_vulkan(const vk::Instance instance) {
        VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);
    }

    void load_vulkan(const vk::Device device) {
        VULKAN_HPP_DEFAULT_DISPATCHER.init(device);
    }

    Version get_vulkan_instance_version() {
        return Version(vk::enumerateInstanceVersion());
    }

    std::vector<vk::ExtensionProperties> get_vulkan_instance_extensions() {
        return vk::enumerateInstanceExtensionProperties();
    }

    std::vector<vk::ExtensionProperties> get_vulkan_instance_extensions(const std::string& layer_name) {
        return vk::enumerateInstanceExtensionProperties(layer_name);
    }

    std::vector<vk::LayerProperties> get_vulkan_instance_layers() {
        return vk::enumerateInstanceLayerProperties();
    }

    vk::Instance create_instance_easy(const std::string_view app_name, const Version app_version) {
        vk::InstanceCreateInfo   create_info{};
        vk::ApplicationInfo      app_info{};
        std::vector<const char*> extensions{VK_KHR_SURFACE_EXTENSION_NAME, get_target_platform_extension().data()};
        std::vector<const char*> layers{};

        app_info.setApiVersion(vk::ApiVersion14).setEngineVersion(static_cast<uint32_t>(VERSION)).setPEngineName("VKE_engine");
        app_info.setPApplicationName(app_name.data()).setApplicationVersion(static_cast<uint32_t>(app_version));

        create_info.setPApplicationInfo(&app_info);
        create_info.setPEnabledExtensionNames(extensions).setPEnabledLayerNames(layers);

        return vk::createInstance(create_info);
    }

    Instance::Instance(const std::string_view app_name, const Version app_version) {
        m_Instance = create_instance_easy(app_name, app_version);
    }

    Instance::~Instance() {
        m_Instance.destroy();
    }

    std::vector<PhysicalDevice> Instance::get_physical_devices() {
        const auto raw_physical_devices = m_Instance.enumeratePhysicalDevices();

        std::vector<PhysicalDevice> physical_devices;
        physical_devices.reserve(raw_physical_devices.size());
        for (const auto& pd : raw_physical_devices) { physical_devices.emplace_back(shared_from_this(), pd); }

        return physical_devices;
    }

    vk::PFN_VoidFunction Instance::get_proc_addr(const std::string_view name) const {
        return m_Instance.getProcAddr(name.data());
    }

    std::vector<vk::PhysicalDeviceGroupProperties> Instance::get_physical_device_groups() const {
        return m_Instance.enumeratePhysicalDeviceGroups();
    }

    vk::SurfaceKHR Instance::create_headless_surface(const vk::HeadlessSurfaceCreateInfoEXT& create_info) const {
        return m_Instance.createHeadlessSurfaceEXT(create_info);
    }

    vk::DebugReportCallbackEXT Instance::create_debug_report_callback(const vk::DebugReportCallbackCreateInfoEXT& create_info) const {
        return m_Instance.createDebugReportCallbackEXT(create_info);
    }

    vk::DebugUtilsMessengerEXT Instance::create_debug_utils_messenger(const vk::DebugUtilsMessengerCreateInfoEXT& create_info) const {
        return m_Instance.createDebugUtilsMessengerEXT(create_info);
    }

    vk::SurfaceKHR Instance::create_display_plane_surface(const vk::DisplaySurfaceCreateInfoKHR& create_info) const {
        return m_Instance.createDisplayPlaneSurfaceKHR(create_info);
    }

    void Instance::destroy_surface(const vk::SurfaceKHR surface) const {
        m_Instance.destroySurfaceKHR(surface);
    }

    void Instance::destroy_debug_report_callback(const vk::DebugReportCallbackEXT callback) const {
        m_Instance.destroyDebugReportCallbackEXT(callback);
    }

    void Instance::destroy_debug_utils_messenger(const vk::DebugUtilsMessengerEXT messenger) const {
        m_Instance.destroyDebugUtilsMessengerEXT(messenger);
    }

    void Instance::debug_report_message(
      const vk::DebugReportFlagsEXT      flags,
      const vk::DebugReportObjectTypeEXT object_type,
      const uint64_t                     object,
      const size_t                       location,
      const int32_t                      message_code,
      const std::string&                 layer_prefix,
      const std::string&                 message
    ) const {
        m_Instance.debugReportMessageEXT(flags, object_type, object, location, message_code, layer_prefix.c_str(), message.c_str());
    }

    void Instance::submit_debug_utils_message(
      const vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
      const vk::DebugUtilsMessageTypeFlagsEXT        type,
      const vk::DebugUtilsMessengerCallbackDataEXT&  callback_data
    ) const {
        m_Instance.submitDebugUtilsMessageEXT(severity, type, callback_data);
    }

    void load_vulkan(const std::shared_ptr<Instance>& instance) {
        load_vulkan(instance->handle());
    }

    PhysicalDevice::PhysicalDevice(const std::shared_ptr<Instance>& instance, vk::PhysicalDevice physical_device)
        : m_Instance(instance), m_PhysicalDevice(physical_device) {}

    vk::PhysicalDevice PhysicalDevice::physical_device() const {
        return m_PhysicalDevice;
    }

    const std::shared_ptr<Instance>& PhysicalDevice::instance() const {
        return m_Instance;
    }

    std::shared_ptr<Device> PhysicalDevice::create_device(const DeviceOptions& options) const {
        vk::DeviceCreateInfo                   create_info{};
        std::vector<const char*>               extensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;

        uint32_t main_family = UINT32_MAX;

        uint32_t queue_index = 0;
        for (const auto queue_family_properties = m_PhysicalDevice.getQueueFamilyProperties(); const auto& properties : queue_family_properties) {
            if (properties.queueFlags & vk::QueueFlagBits::eGraphics && main_family == UINT32_MAX) { main_family = queue_index; }
            queue_index++;
        }

        std::array<float, 1> queue_priorities = {1.0f};
        queue_create_infos.emplace_back(vk::DeviceQueueCreateFlags{}, main_family, queue_priorities);

        vk::StructureChain<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan11Features, vk::PhysicalDeviceVulkan12Features, vk::PhysicalDeviceVulkan13Features, vk::PhysicalDeviceVulkan14Features>
          features_chain;

        auto& features              = features_chain.get<vk::PhysicalDeviceFeatures2>().features;
        features.geometryShader     = true;
        features.tessellationShader = true;
        features.largePoints        = true;
        features.wideLines          = true;
        features.sparseBinding      = true;
        features.multiDrawIndirect  = true;

        auto& v11f                = features_chain.get<vk::PhysicalDeviceVulkan11Features>();
        v11f.shaderDrawParameters = true;

        auto& v12f             = features_chain.get<vk::PhysicalDeviceVulkan12Features>();
        v12f.timelineSemaphore = true;

        auto& v13f              = features_chain.get<vk::PhysicalDeviceVulkan13Features>();
        v13f.dynamicRendering   = true;
        v13f.synchronization2   = true;
        v13f.inlineUniformBlock = true;

        auto& v14f          = features_chain.get<vk::PhysicalDeviceVulkan14Features>();
        v14f.pushDescriptor = true;

        create_info.setPEnabledExtensionNames(extensions);
        create_info.setQueueCreateInfos(queue_create_infos);
        create_info.setPNext(&features_chain.get<vk::PhysicalDeviceFeatures2>());

        const vk::Device                  device = m_PhysicalDevice.createDevice(create_info);
        vk::detail::DispatchLoaderDynamic dldy{};
        dldy.init();
        dldy.init(m_Instance->handle());
        dldy.init(device);

        const vk::Queue main_queue = device.getQueue(main_family, 0, dldy);

        const QueueCollection queues{
          .main = {main_queue, main_family}
        };

        return Device::create(*this, device, queues);
    }

    Device::Device(const PhysicalDevice& physical_device, const vk::Device device, const QueueCollection queue_collection)
        : m_PhysicalDevice(physical_device), m_Device(device), m_QueueCollection(queue_collection) {}

    Device::~Device() {
        m_Device.destroy();
    }

    void load_vulkan(const std::shared_ptr<Device>& device) {
        load_vulkan(device->handle());
    }

    bool operator==(const PhysicalDevice& lhs, const PhysicalDevice& rhs) {
        return lhs.m_PhysicalDevice == rhs.m_PhysicalDevice && lhs.m_Instance == rhs.m_Instance;
    }

    bool operator!=(const PhysicalDevice& lhs, const PhysicalDevice& rhs) {
        return lhs.m_PhysicalDevice != rhs.m_PhysicalDevice || lhs.m_Instance != rhs.m_Instance;
    }
} // namespace vke
