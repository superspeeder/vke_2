//
// Created by andy on 3/17/2025.
//

#include "dependency.hpp"

namespace vke {
    Ownable::~Ownable() {
        for (const auto child : m_Children) {
            delete child;
        }
    }

    void Ownable::owns(std::unique_ptr<Ownable> object) {
        object->m_Owner = this;
        m_Children.emplace_back(object.release());
    }
} // namespace vke
