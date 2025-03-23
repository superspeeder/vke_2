//
// Created by andy on 3/17/2025.
//

#include "graphics_pipeline.hpp"

#include "vke/global.hpp"

#include <stdexcept>

namespace vke {
    static uint32_t sizeof_format(const vk::Format format) {
        switch (format) {
        case vk::Format::eUndefined:                            throw std::invalid_argument("Cannot get size of undefined format.");
        case vk::Format::eR4G4UnormPack8:                       return 1;
        case vk::Format::eR4G4B4A4UnormPack16:                  return 2;
        case vk::Format::eB4G4R4A4UnormPack16:                  return 2;
        case vk::Format::eR5G6B5UnormPack16:                    return 2;
        case vk::Format::eB5G6R5UnormPack16:                    return 2;
        case vk::Format::eR5G5B5A1UnormPack16:                  return 2;
        case vk::Format::eB5G5R5A1UnormPack16:                  return 2;
        case vk::Format::eA1R5G5B5UnormPack16:                  return 2;
        case vk::Format::eR8Unorm:                              return 1;
        case vk::Format::eR8Snorm:                              return 1;
        case vk::Format::eR8Uscaled:                            return 1;
        case vk::Format::eR8Sscaled:                            return 1;
        case vk::Format::eR8Uint:                               return 1;
        case vk::Format::eR8Sint:                               return 1;
        case vk::Format::eR8Srgb:                               return 1;
        case vk::Format::eR8G8Unorm:                            return 2;
        case vk::Format::eR8G8Snorm:                            return 2;
        case vk::Format::eR8G8Uscaled:                          return 2;
        case vk::Format::eR8G8Sscaled:                          return 2;
        case vk::Format::eR8G8Uint:                             return 2;
        case vk::Format::eR8G8Sint:                             return 2;
        case vk::Format::eR8G8Srgb:                             return 2;
        case vk::Format::eR8G8B8Unorm:                          return 3;
        case vk::Format::eR8G8B8Snorm:                          return 3;
        case vk::Format::eR8G8B8Uscaled:                        return 3;
        case vk::Format::eR8G8B8Sscaled:                        return 3;
        case vk::Format::eR8G8B8Uint:                           return 3;
        case vk::Format::eR8G8B8Sint:                           return 3;
        case vk::Format::eR8G8B8Srgb:                           return 3;
        case vk::Format::eB8G8R8Unorm:                          return 3;
        case vk::Format::eB8G8R8Snorm:                          return 3;
        case vk::Format::eB8G8R8Uscaled:                        return 3;
        case vk::Format::eB8G8R8Sscaled:                        return 3;
        case vk::Format::eB8G8R8Uint:                           return 3;
        case vk::Format::eB8G8R8Sint:                           return 3;
        case vk::Format::eB8G8R8Srgb:                           return 3;
        case vk::Format::eR8G8B8A8Unorm:                        return 4;
        case vk::Format::eR8G8B8A8Snorm:                        return 4;
        case vk::Format::eR8G8B8A8Uscaled:                      return 4;
        case vk::Format::eR8G8B8A8Sscaled:                      return 4;
        case vk::Format::eR8G8B8A8Uint:                         return 4;
        case vk::Format::eR8G8B8A8Sint:                         return 4;
        case vk::Format::eR8G8B8A8Srgb:                         return 4;
        case vk::Format::eB8G8R8A8Unorm:                        return 4;
        case vk::Format::eB8G8R8A8Snorm:                        return 4;
        case vk::Format::eB8G8R8A8Uscaled:                      return 4;
        case vk::Format::eB8G8R8A8Sscaled:                      return 4;
        case vk::Format::eB8G8R8A8Uint:                         return 4;
        case vk::Format::eB8G8R8A8Sint:                         return 4;
        case vk::Format::eB8G8R8A8Srgb:                         return 4;
        case vk::Format::eA8B8G8R8UnormPack32:                  return 4;
        case vk::Format::eA8B8G8R8SnormPack32:                  return 4;
        case vk::Format::eA8B8G8R8UscaledPack32:                return 4;
        case vk::Format::eA8B8G8R8SscaledPack32:                return 4;
        case vk::Format::eA8B8G8R8UintPack32:                   return 4;
        case vk::Format::eA8B8G8R8SintPack32:                   return 4;
        case vk::Format::eA8B8G8R8SrgbPack32:                   return 4;
        case vk::Format::eA2R10G10B10UnormPack32:               return 4;
        case vk::Format::eA2R10G10B10SnormPack32:               return 4;
        case vk::Format::eA2R10G10B10UscaledPack32:             return 4;
        case vk::Format::eA2R10G10B10SscaledPack32:             return 4;
        case vk::Format::eA2R10G10B10UintPack32:                return 4;
        case vk::Format::eA2R10G10B10SintPack32:                return 4;
        case vk::Format::eA2B10G10R10UnormPack32:               return 4;
        case vk::Format::eA2B10G10R10SnormPack32:               return 4;
        case vk::Format::eA2B10G10R10UscaledPack32:             return 4;
        case vk::Format::eA2B10G10R10SscaledPack32:             return 4;
        case vk::Format::eA2B10G10R10UintPack32:                return 4;
        case vk::Format::eA2B10G10R10SintPack32:                return 4;
        case vk::Format::eR16Unorm:                             return 2;
        case vk::Format::eR16Snorm:                             return 2;
        case vk::Format::eR16Uscaled:                           return 2;
        case vk::Format::eR16Sscaled:                           return 2;
        case vk::Format::eR16Uint:                              return 2;
        case vk::Format::eR16Sint:                              return 2;
        case vk::Format::eR16Sfloat:                            return 2;
        case vk::Format::eR16G16Unorm:                          return 4;
        case vk::Format::eR16G16Snorm:                          return 4;
        case vk::Format::eR16G16Uscaled:                        return 4;
        case vk::Format::eR16G16Sscaled:                        return 4;
        case vk::Format::eR16G16Uint:                           return 4;
        case vk::Format::eR16G16Sint:                           return 4;
        case vk::Format::eR16G16Sfloat:                         return 4;
        case vk::Format::eR16G16B16Unorm:                       return 6;
        case vk::Format::eR16G16B16Snorm:                       return 6;
        case vk::Format::eR16G16B16Uscaled:                     return 6;
        case vk::Format::eR16G16B16Sscaled:                     return 6;
        case vk::Format::eR16G16B16Uint:                        return 6;
        case vk::Format::eR16G16B16Sint:                        return 6;
        case vk::Format::eR16G16B16Sfloat:                      return 6;
        case vk::Format::eR16G16B16A16Unorm:                    return 8;
        case vk::Format::eR16G16B16A16Snorm:                    return 8;
        case vk::Format::eR16G16B16A16Uscaled:                  return 8;
        case vk::Format::eR16G16B16A16Sscaled:                  return 8;
        case vk::Format::eR16G16B16A16Uint:                     return 8;
        case vk::Format::eR16G16B16A16Sint:                     return 8;
        case vk::Format::eR16G16B16A16Sfloat:                   return 8;
        case vk::Format::eR32Uint:                              return 4;
        case vk::Format::eR32Sint:                              return 4;
        case vk::Format::eR32Sfloat:                            return 4;
        case vk::Format::eR32G32Uint:                           return 8;
        case vk::Format::eR32G32Sint:                           return 8;
        case vk::Format::eR32G32Sfloat:                         return 8;
        case vk::Format::eR32G32B32Uint:                        return 12;
        case vk::Format::eR32G32B32Sint:                        return 12;
        case vk::Format::eR32G32B32Sfloat:                      return 12;
        case vk::Format::eR32G32B32A32Uint:                     return 16;
        case vk::Format::eR32G32B32A32Sint:                     return 16;
        case vk::Format::eR32G32B32A32Sfloat:                   return 16;
        case vk::Format::eR64Uint:                              return 8;
        case vk::Format::eR64Sint:                              return 8;
        case vk::Format::eR64Sfloat:                            return 8;
        case vk::Format::eR64G64Uint:                           return 16;
        case vk::Format::eR64G64Sint:                           return 16;
        case vk::Format::eR64G64Sfloat:                         return 16;
        case vk::Format::eR64G64B64Uint:                        return 24;
        case vk::Format::eR64G64B64Sint:                        return 24;
        case vk::Format::eR64G64B64Sfloat:                      return 24;
        case vk::Format::eR64G64B64A64Uint:                     return 32;
        case vk::Format::eR64G64B64A64Sint:                     return 32;
        case vk::Format::eR64G64B64A64Sfloat:                   return 32;
        case vk::Format::eB10G11R11UfloatPack32:                return 4;
        case vk::Format::eE5B9G9R9UfloatPack32:                 return 4;
        case vk::Format::eD16Unorm:                             return 2;
        case vk::Format::eX8D24UnormPack32:                     return 4;
        case vk::Format::eD32Sfloat:                            return 4;
        case vk::Format::eS8Uint:                               return 1;
        case vk::Format::eD16UnormS8Uint:                       return 3;
        case vk::Format::eD24UnormS8Uint:                       return 4;
        case vk::Format::eD32SfloatS8Uint:                      return 8;
        case vk::Format::eBc1RgbUnormBlock:                     throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eBc1RgbSrgbBlock:                      throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eBc1RgbaUnormBlock:                    throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eBc1RgbaSrgbBlock:                     throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eBc2UnormBlock:                        throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eBc2SrgbBlock:                         throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eBc3UnormBlock:                        throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eBc3SrgbBlock:                         throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eBc4UnormBlock:                        throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eBc4SnormBlock:                        throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eBc5UnormBlock:                        throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eBc5SnormBlock:                        throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eBc6HUfloatBlock:                      throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eBc6HSfloatBlock:                      throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eBc7UnormBlock:                        throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eBc7SrgbBlock:                         throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eEtc2R8G8B8UnormBlock:                 throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eEtc2R8G8B8SrgbBlock:                  throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eEtc2R8G8B8A1UnormBlock:               throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eEtc2R8G8B8A1SrgbBlock:                throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eEtc2R8G8B8A8UnormBlock:               throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eEtc2R8G8B8A8SrgbBlock:                throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eEacR11UnormBlock:                     throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eEacR11SnormBlock:                     throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eEacR11G11UnormBlock:                  throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eEacR11G11SnormBlock:                  throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc4x4UnormBlock:                    throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc4x4SrgbBlock:                     throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc5x4UnormBlock:                    throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc5x4SrgbBlock:                     throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc5x5UnormBlock:                    throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc5x5SrgbBlock:                     throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc6x5UnormBlock:                    throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc6x5SrgbBlock:                     throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc6x6UnormBlock:                    throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc6x6SrgbBlock:                     throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc8x5UnormBlock:                    throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc8x5SrgbBlock:                     throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc8x6UnormBlock:                    throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc8x6SrgbBlock:                     throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc8x8UnormBlock:                    throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc8x8SrgbBlock:                     throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc10x5UnormBlock:                   throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc10x5SrgbBlock:                    throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc10x6UnormBlock:                   throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc10x6SrgbBlock:                    throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc10x8UnormBlock:                   throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc10x8SrgbBlock:                    throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc10x10UnormBlock:                  throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc10x10SrgbBlock:                   throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc12x10UnormBlock:                  throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc12x10SrgbBlock:                   throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc12x12UnormBlock:                  throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc12x12SrgbBlock:                   throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eG8B8G8R8422Unorm:                     throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eB8G8R8G8422Unorm:                     throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eG8B8R83Plane420Unorm:                 throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eG8B8R82Plane420Unorm:                 throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eG8B8R83Plane422Unorm:                 throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eG8B8R82Plane422Unorm:                 throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eG8B8R83Plane444Unorm:                 throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eR10X6UnormPack16:                     return 2;
        case vk::Format::eR10X6G10X6Unorm2Pack16:               return 4;
        case vk::Format::eR10X6G10X6B10X6A10X6Unorm4Pack16:     return 8;
        case vk::Format::eG10X6B10X6G10X6R10X6422Unorm4Pack16:  throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eB10X6G10X6R10X6G10X6422Unorm4Pack16:  throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eG10X6B10X6R10X63Plane420Unorm3Pack16: throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eG10X6B10X6R10X62Plane420Unorm3Pack16: throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eG10X6B10X6R10X63Plane422Unorm3Pack16: throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eG10X6B10X6R10X62Plane422Unorm3Pack16: throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eG10X6B10X6R10X63Plane444Unorm3Pack16: throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eR12X4UnormPack16:                     return 2;
        case vk::Format::eR12X4G12X4Unorm2Pack16:               return 4;
        case vk::Format::eR12X4G12X4B12X4A12X4Unorm4Pack16:     return 8;
        case vk::Format::eG12X4B12X4G12X4R12X4422Unorm4Pack16:  throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eB12X4G12X4R12X4G12X4422Unorm4Pack16:  throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eG12X4B12X4R12X43Plane420Unorm3Pack16: throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eG12X4B12X4R12X42Plane420Unorm3Pack16: throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eG12X4B12X4R12X43Plane422Unorm3Pack16: throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eG12X4B12X4R12X42Plane422Unorm3Pack16: throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eG12X4B12X4R12X43Plane444Unorm3Pack16: throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eG16B16G16R16422Unorm:                 throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eB16G16R16G16422Unorm:                 throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eG16B16R163Plane420Unorm:              throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eG16B16R162Plane420Unorm:              throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eG16B16R163Plane422Unorm:              throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eG16B16R162Plane422Unorm:              throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eG16B16R163Plane444Unorm:              throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eG8B8R82Plane444Unorm:                 throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eG10X6B10X6R10X62Plane444Unorm3Pack16: throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eG12X4B12X4R12X42Plane444Unorm3Pack16: throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eG16B16R162Plane444Unorm:              throw std::invalid_argument("Multi-planar formats don't have per-pixel sizes.");
        case vk::Format::eA4R4G4B4UnormPack16:                  return 2;
        case vk::Format::eA4B4G4R4UnormPack16:                  return 2;
        case vk::Format::eAstc4x4SfloatBlock:                   throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc5x4SfloatBlock:                   throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc5x5SfloatBlock:                   throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc6x5SfloatBlock:                   throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc6x6SfloatBlock:                   throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc8x5SfloatBlock:                   throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc8x6SfloatBlock:                   throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc8x8SfloatBlock:                   throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc10x5SfloatBlock:                  throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc10x6SfloatBlock:                  throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc10x8SfloatBlock:                  throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc10x10SfloatBlock:                 throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc12x10SfloatBlock:                 throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eAstc12x12SfloatBlock:                 throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eA1B5G5R5UnormPack16:                  return 2;
        case vk::Format::eA8Unorm:                              return 1;
        case vk::Format::ePvrtc12BppUnormBlockIMG:              throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::ePvrtc14BppUnormBlockIMG:              throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::ePvrtc22BppUnormBlockIMG:              throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::ePvrtc24BppUnormBlockIMG:              throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::ePvrtc12BppSrgbBlockIMG:               throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::ePvrtc14BppSrgbBlockIMG:               throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::ePvrtc22BppSrgbBlockIMG:               throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::ePvrtc24BppSrgbBlockIMG:               throw std::invalid_argument("Compressed formats don't have per-pixel sizes.");
        case vk::Format::eR16G16Sfixed5NV:                      return 4;
        default:                                                throw std::invalid_argument("Invalid format");
        }
    }

    static std::size_t sample_mask_req(vk::SampleCountFlagBits count) {
        switch (count) {
        case vk::SampleCountFlagBits::e1:  return 1;
        case vk::SampleCountFlagBits::e2:  return 1;
        case vk::SampleCountFlagBits::e4:  return 1;
        case vk::SampleCountFlagBits::e8:  return 1;
        case vk::SampleCountFlagBits::e16: return 1;
        case vk::SampleCountFlagBits::e32: return 1;
        case vk::SampleCountFlagBits::e64: return 2;
        default:                           return 0;
        }
    }

    VertexBindingBuilder VertexBinding::builder(const uint32_t binding, const vk::VertexInputRate input_rate) {
        return VertexBindingBuilder(binding, input_rate);
    }

    VertexBindingBuilder::VertexBindingBuilder(const uint32_t binding, const vk::VertexInputRate input_rate)
        : binding(binding), stride(0), input_rate(input_rate) {}

    VertexBindingBuilder& VertexBindingBuilder::attribute(uint32_t location, const vk::Format format) & {
        const uint32_t sz  = sizeof_format(format);
        const uint32_t off = stride;
        stride += sz;
        attributes.emplace_back(location, format, off);
        return *this;
    }

    VertexBindingBuilder VertexBindingBuilder::attribute(uint32_t location, const vk::Format format) && {
        const uint32_t sz  = sizeof_format(format);
        const uint32_t off = stride;
        stride += sz;
        attributes.emplace_back(location, format, off);
        return *this;
    }

    VertexBinding VertexBindingBuilder::build() const {
        return VertexBinding{.binding = binding, .stride = stride, .input_rate = input_rate, .attributes = attributes};
    }

    VertexLayoutBuilder VertexLayout::builder() {
        return VertexLayoutBuilder();
    }

    VertexLayoutBuilder::VertexLayoutBuilder() {}

    VertexLayoutBuilder& VertexLayoutBuilder::binding(const VertexBinding& binding) & {
        bindings.push_back(binding);
        return *this;
    }

    VertexLayoutBuilder VertexLayoutBuilder::binding(const VertexBinding& binding) && {
        bindings.push_back(binding);
        return *this;
    }

    GraphicsPipeline::GraphicsPipeline(const Settings& settings) : m_Device(global::g_Device) {
        vk::GraphicsPipelineCreateInfo create_info{};

        std::vector<vk::PipelineShaderStageCreateInfo> shader_stages;
        for (const auto& [shader_module, stage, entry_point] : settings.shader_stages) {
            shader_stages.emplace_back(vk::PipelineShaderStageCreateFlags(), stage, shader_module->handle(), entry_point.c_str());
            m_ShaderModules.emplace_back(shader_module);
        }

        vk::PipelineVertexInputStateCreateInfo           vertex_input_state{};
        std::vector<vk::VertexInputAttributeDescription> vertex_attribute_descriptions{};
        std::vector<vk::VertexInputBindingDescription>   vertex_binding_descriptions{};
        for (const auto& [binding, stride, input_rate, attributes] : settings.fixed_function.vertex_layout.bindings) {
            vertex_binding_descriptions.emplace_back(binding, stride, input_rate);
            for (const auto& [location, format, offset] : attributes) {
                vertex_attribute_descriptions.emplace_back(location, binding, format, offset);
            }
        }
        vertex_input_state.setVertexBindingDescriptions(vertex_binding_descriptions);
        vertex_input_state.setVertexAttributeDescriptions(vertex_attribute_descriptions);

        vk::PipelineInputAssemblyStateCreateInfo input_assembly_state{};
        input_assembly_state.topology               = vk::PrimitiveTopology::eTriangleList;
        input_assembly_state.primitiveRestartEnable = false;

        vk::PipelineTessellationStateCreateInfo tessellation_state{};
        tessellation_state.patchControlPoints = settings.fixed_function.tessellation_patch_control_points;

        vk::PipelineViewportStateCreateInfo viewport_state{};
        viewport_state.setViewports(settings.fixed_function.viewports);
        viewport_state.setScissors(settings.fixed_function.scissors);

        vk::PipelineRasterizationStateCreateInfo rasterization_state{};
        rasterization_state.cullMode         = settings.fixed_function.cull_mode;
        rasterization_state.frontFace        = settings.fixed_function.front_face;
        rasterization_state.polygonMode      = settings.fixed_function.polygon_mode;
        rasterization_state.lineWidth        = settings.fixed_function.line_width;
        rasterization_state.depthClampEnable = settings.fixed_function.enable_depth_clamp;
        if (settings.fixed_function.depth_bias.has_value()) {
            rasterization_state.depthBiasEnable         = true;
            rasterization_state.depthBiasConstantFactor = settings.fixed_function.depth_bias->constant_factor;
            rasterization_state.depthBiasSlopeFactor    = settings.fixed_function.depth_bias->slope_factor;
            rasterization_state.depthBiasClamp          = settings.fixed_function.depth_bias->clamp;
        } else {
            rasterization_state.depthBiasEnable = false;
        }

        vk::PipelineMultisampleStateCreateInfo multisample_state{};
        multisample_state.rasterizationSamples = settings.fixed_function.rasterization_samples;
        if (settings.fixed_function.min_sample_shading.has_value()) {
            multisample_state.sampleShadingEnable = true;
            multisample_state.minSampleShading    = settings.fixed_function.min_sample_shading.value();
        } else {
            multisample_state.sampleShadingEnable = false;
        }

        if (settings.fixed_function.sample_mask.empty()) {
            multisample_state.pSampleMask = nullptr;
        } else {
            VKE_ASSERT(
              settings.fixed_function.sample_mask.size() >= sample_mask_req(settings.fixed_function.rasterization_samples), "Sample mask doesn't have enough values"
            );
            multisample_state.pSampleMask = settings.fixed_function.sample_mask.data();
        }
        multisample_state.alphaToCoverageEnable = settings.fixed_function.enable_alpha_to_coverage;
        multisample_state.alphaToOneEnable      = settings.fixed_function.enable_alpha_to_one;

        vk::PipelineDepthStencilStateCreateInfo depth_stencil_state{};
        depth_stencil_state.depthTestEnable       = settings.fixed_function.enable_depth_test;
        depth_stencil_state.depthWriteEnable      = settings.fixed_function.enable_depth_write;
        depth_stencil_state.depthCompareOp        = settings.fixed_function.depth_compare_op;
        depth_stencil_state.depthBoundsTestEnable = settings.fixed_function.enable_depth_bounds_test;
        depth_stencil_state.stencilTestEnable     = settings.fixed_function.enable_stencil_test;
        depth_stencil_state.front                 = settings.fixed_function.stencil_front;
        depth_stencil_state.back                  = settings.fixed_function.stencil_back;
        depth_stencil_state.minDepthBounds        = settings.fixed_function.depth_bounds.first;
        depth_stencil_state.maxDepthBounds        = settings.fixed_function.depth_bounds.second;

        vk::PipelineColorBlendStateCreateInfo color_blend_state{};
        if (settings.fixed_function.blend_logic_op.has_value()) {
            color_blend_state.logicOpEnable = true;
            color_blend_state.logicOp       = settings.fixed_function.blend_logic_op.value();
        } else {
            color_blend_state.logicOpEnable = false;
        }

        std::vector<vk::PipelineColorBlendAttachmentState> color_blend_attachments{};
        for (const auto& [enable_blending, color_write_mask, blend_function] : settings.fixed_function.blend_attachments) {
            color_blend_attachments.emplace_back(
              enable_blending, blend_function.source_color, blend_function.destination_color, blend_function.color_op, blend_function.source_alpha,
              blend_function.destination_alpha, blend_function.alpha_op, color_write_mask
            );
        }

        color_blend_state.setAttachments(color_blend_attachments);

        color_blend_state.blendConstants[0] = settings.fixed_function.blend_constants.r;
        color_blend_state.blendConstants[1] = settings.fixed_function.blend_constants.g;
        color_blend_state.blendConstants[2] = settings.fixed_function.blend_constants.b;
        color_blend_state.blendConstants[3] = settings.fixed_function.blend_constants.a;

        vk::PipelineDynamicStateCreateInfo dynamic_state{};
        dynamic_state.setDynamicStates(settings.dynamic_states);

        vk::PipelineRenderingCreateInfo dynamic_rendering_info{};
        dynamic_rendering_info.setColorAttachmentFormats(settings.rendering_info.color_attachments);
        dynamic_rendering_info.setDepthAttachmentFormat(settings.rendering_info.depth_attachment.value_or(vk::Format::eUndefined));
        dynamic_rendering_info.setStencilAttachmentFormat(settings.rendering_info.stencil_attachment.value_or(vk::Format::eUndefined));
        dynamic_rendering_info.setViewMask(settings.rendering_info.view_mask);

        create_info.setStages(shader_stages);
        create_info.setPVertexInputState(&vertex_input_state);
        create_info.setPInputAssemblyState(&input_assembly_state);
        create_info.setPTessellationState(&tessellation_state);
        create_info.setPViewportState(&viewport_state);
        create_info.setPRasterizationState(&rasterization_state);
        create_info.setPMultisampleState(&multisample_state);
        create_info.setPDepthStencilState(&depth_stencil_state);
        create_info.setPColorBlendState(&color_blend_state);
        create_info.setPDynamicState(&dynamic_state);
        create_info.setPNext(&dynamic_rendering_info);

        create_info.setLayout(settings.layout->handle());

        // TODO: derived pipelines

        // TODO: transform the builtin dynamic rendering support to allow using a render pass instead
        // create_info.setRenderPass(settings.render_pass->handle());
        // create_info.setSubpass(settings->subpass);

        m_Pipeline = m_Device->handle().createGraphicsPipeline(/* TODO: pipeline cache */ nullptr, create_info).value;
    }
} // namespace vke
