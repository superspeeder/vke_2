//
// Created by andy on 3/17/2025.
//

#include "shader_module.hpp"

#include "vke/global.hpp"

#include <fstream>

namespace vke {
    ShaderModule::ShaderModule(const vk::ShaderModule module) : m_Device(global::g_Device), m_ShaderModule(module) {}

    ShaderModule::~ShaderModule() {
        m_Device->destroy(m_ShaderModule);
    }

    std::shared_ptr<ShaderModule> ShaderModule::load(const std::filesystem::path& path) {
        std::ifstream f(path, std::ios::in | std::ios::binary | std::ios::ate);
        if (!f.is_open()) { throw std::invalid_argument("failed to open file"); }

        const std::streamsize len = f.tellg();
        f.seekg(0, std::ios::beg);
        std::vector<uint32_t> code(len / sizeof(uint32_t));
        f.read(reinterpret_cast<char*>(code.data()), len);
        f.close();

        return create(code);
    }

    std::shared_ptr<ShaderModule> ShaderModule::create(const std::vector<uint32_t>& code) {
        vk::ShaderModuleCreateInfo create_info{};
        create_info.setCode(code);
        return std::shared_ptr<ShaderModule>(new ShaderModule(global::g_Device->handle().createShaderModule(create_info)));
    }
} // namespace vke
