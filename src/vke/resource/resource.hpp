//
// Created by andy on 3/23/2025.
//

#pragma once

#include <cstdint>
#include <cstdlib>
#include <vulkan/vulkan.hpp>

/**
 * Resources loaded from disk are represented one of two ways:
 *      1. As a JSON file with the resource information
 *      2. As a binary resource file, generally encapsulating the actual resource.
 *
 * The JSON representation is preferred when in development, but loading and storing the binary representation is more efficient, so that should be
 * used in distribution builds. Additionally, binary resources can be packed together and compressed, while JSON resources cannot.
 *
 * >
 * > Even though JSON can definitely be compressed, it is preferred that JSON only be used during development due to increased readability.
 * Compression defeats this purpose.
 * >
 *
 * Packing binary assets together should be done with care, because while a packed file has a predictable header and a resource table which allows the
 * system to seek directly to the desired resource, for best efficiency you should be packing together resources which might be used in the same
 * scenario and then wrapping them as an asset group.
 *
 * Asset groups allow you to make a packed binary resource file like `level1.bin` and a group called `level1` which lets the system treat all those
 * resources as having the same lifetime. As a result, the system can read the assets and load them from disk in order from the file, improving load
 * times and reducing OS overhead (instead of having to read the table and seek for resources, the system can just read the group header and know to
 * load and read all assets sequentially. Goodbye calls to `fseek`!). Asset groups also give good load safety and let you do better at streaming assets.
 *
 * Resource scopes are a very important part of this system. Every resource exists in a scope, which determines when the application is allowed to start
 * loading them and when it must clean them up. Preloaded resources will be loaded when a scope opens, generally from memory (the asset is parsed into
 * a queue for its scope and when the scope opens, the asset becomes available).
 *
 * There are many built-in resource scopes (such as `image`), but scopes can also be created either as assets or from code. This is useful for anything
 * that is custom (for example, if I'm making an RPG, and I have my character dialog stored in a file with a special format, I can use this to set up
 * a loader and read them just like any asset).
 *
 * Resources are also useful for maintaining dependencies.
 * They are allowed to form a graph, which then means the system can make sure that they are kept loaded while required.
 * Lifetime management is incredibly important, and using the resource system can make sure that you don't have to do anything weird in order to make
 * sure that something persists the right amount of time. For example, a window surface might be treated as a resource, and you could place the renderer
 * you are currently using as a child of the window surface. That way, when the surface is being destroyed the system makes sure that the renderer is
 * cleaned up (and makes sure that you never try to render to a non-existent surface too).
 *
 */
namespace vke::res {
    constexpr std::size_t MAX_RESOURCE_SCOPE_NAME_LENGTH = 256; // in reality, this is 255 characters and a null terminator. In the binary
                                                                // representation of a resource, the null terminator will be removed.

    struct ResourceHeader {
        /**
         * Can this resource have child-resources?
         */
        bool allow_children : 1;

        /**
         * Can this resource be a child resource or must it be a root-level resource?
         */
        bool allow_as_child : 1;

        /**
         * Is this resource static?
         *
         * Static resources can be optimized since they are considered to not change after loading. This is mainly used for assets, and is generally
         * what you want. If this is not true, the resource must be protected against race conditions when doing things in multiple threads.
         */
        bool is_static : 1;

        /**
         * Does this resource always exist?
         *
         * Enabling this allows for certain optimizations, since resource handles to permanent resources don't have to check if a resource is still
         * loaded before using it
         *
         * For example, loaded images are in the `image` resource scope, which is available after the lifecycle event `device_ready` is emitted.
         * As a result, permanent images (which might be something like a little spinner animation for showing that the game is saving) will only be
         * unloaded during the cleanup phase of the application. Additionally, the resource handle for an image may hold it as a `vk::Image` (which
         * can also be used to index the resource map). Normally, the resource handle would have to load an identifier that could be used to load it
         * if the resource is unloaded (though this is generally held as a unique id which can be used to reach the resource, just with one or two
         * layers of indirection).
         */
        bool is_permanent : 1;

        /**
         * Does the system need to preserve this resource unless it is manually cleaned up?
         *
         * This is extremely important, and mainly is used for non-asset resources (for example, a render target).
         * The resource system is great for managing resources, since it can help to maintain lifetimes and safety.
         * However, generally the system will use a blend of an LFU and an LRU cache to hold onto resources, and will then unload them if they aren't
         * used enough. Setting this flag prevents them from being managed by the cache, instead keeping them in storage until they are manually
         * unloaded and cleaned up (or at the application lifecycle stage which their resource scope is required to clean them up at).
         *
         * This should also be used for resources which are in active use. While the cache *shouldn't* unload them if the system parameters are right,
         * you probably don't want to be reloading your scene from disk every frame (in the case of something wrong happening that makes it the least
         * frequently used and least recently used item on the list).
         */
        bool allow_auto_unload : 1;

        uint8_t padding : 3;
    };

    struct ResourceScope {
        uint8_t scope_name_length;
        char    scope_name[MAX_RESOURCE_SCOPE_NAME_LENGTH];
    };

    union ResourceId {
        uint64_t    marker_id; // This is the unique id used for loading the resource from disk or accessing it from the map. Generally non-ephemeral.
        const char* resource_name; // This is the name which can be used to load from disk. Not preferred, and lifetime must be externally managed.
        void* resource_handle; // this field is used for things like vulkan object handles. typed ResourceHandle objects can use the type information
                               // to properly convert this to the right type.
    };

    class Resource {
      public:
        Resource();

        bool allow_auto_unload() const noexcept;
        bool allow_as_child() const noexcept;
        bool allow_children() const noexcept;
        bool is_static() const noexcept;
        bool is_permanent() const noexcept;

      private:
    };

} // namespace vke::res
