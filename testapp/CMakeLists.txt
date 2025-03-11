add_executable(testapp src/main.cpp src/main.hpp)
target_include_directories(testapp PRIVATE src/)
target_link_libraries(testapp PRIVATE vke::engine)

add_custom_target(copy_files COMMAND_EXPAND_LISTS VERBATIM
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:testapp> ${CMAKE_CURRENT_BINARY_DIR})