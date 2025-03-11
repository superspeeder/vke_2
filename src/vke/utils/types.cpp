//
// Created by andy on 3/5/2025.
//

#include "vke/utils/types.hpp"

#include <ostream>

namespace vke {
    std::ostream& operator<<(std::ostream& os, const Version& version) {
        os << version.major << "." << version.minor << "." << version.patch;
        return os;
    }
} // namespace vke
