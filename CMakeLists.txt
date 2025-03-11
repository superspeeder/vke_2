cmake_minimum_required(VERSION 3.30)
project(vke2)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include(FetchContent)
FetchContent_Declare(VulkanHeaders
        GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Headers.git
        GIT_TAG v1.4.309
)
FetchContent_Declare(glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1)
FetchContent_Declare(eventpp
        GIT_REPOSITORY https://github.com/wqking/eventpp.git
        GIT_TAG v0.1.3
)

set(BUILD_SHARED_LIBS OFF)
FetchContent_MakeAvailable(VulkanHeaders glm)

set(BUILD_SHARED_LIBS ON)
FetchContent_MakeAvailable(eventpp)

add_library(engine SHARED
        src/vke/vke.cpp
        src/vke/vke.hpp
        src/vke/pre.hpp
        src/vke/utils/types.cpp
        src/vke/utils/types.hpp
        src/vke/window.cpp
        src/vke/window.hpp
        src/vke/surface.cpp
        src/vke/surface.hpp
        src/vke/utils/utils.cpp
        src/vke/utils/utils.hpp
        src/vke/lifecycle.cpp
        src/vke/lifecycle.hpp
        src/vke/global.cpp
        src/vke/global.hpp
        src/vke/window_manager.cpp
        src/vke/window_manager.hpp)
target_include_directories(engine PUBLIC src)
target_link_libraries(engine PUBLIC Vulkan::Headers glm::glm eventpp::eventpp)
target_compile_definitions(engine PUBLIC VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 NOMINMAX)

target_compile_definitions(engine PUBLIC $<IF:$<STREQUAL:$<TARGET_PROPERTY:engine,TYPE>,SHARED_LIBRARY>,VKE_SHARED,>)
target_compile_definitions(engine PRIVATE $<IF:$<STREQUAL:$<TARGET_PROPERTY:engine,TYPE>,SHARED_LIBRARY>,VKE_SHARED_EXPORTS,>)

add_library(vke::engine ALIAS engine)

add_subdirectory(testapp)
