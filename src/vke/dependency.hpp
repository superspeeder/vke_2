//
// Created by andy on 3/17/2025.
//

#pragma once
#include <memory>
#include <vector>

#include "vke/pre.hpp"

namespace vke {
    class VKE_API Ownable {
      public:
        virtual ~Ownable();

        void owns(std::unique_ptr<Ownable> object);

      private:
        std::vector<Ownable*> m_Children;
        Ownable*              m_Owner = nullptr;
    };
} // namespace vke
