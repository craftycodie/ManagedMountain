cmake_minimum_required(VERSION 3.20)

project(tool_hook_harness LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /EHsc /permissive-)
endif()

set(THH_OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")

include(FetchContent)

FetchContent_Declare(
    detours
    GIT_REPOSITORY https://github.com/microsoft/Detours.git
    GIT_TAG v4.0.1
)

FetchContent_MakeAvailable(detours)

if(NOT DEFINED detours_SOURCE_DIR OR detours_SOURCE_DIR STREQUAL "")
    message(FATAL_ERROR "FetchContent did not populate detours (detours_SOURCE_DIR is empty).")
endif()

# v4.0.x ships headers next to sources (src/detours.h); newer trees may use include/detours.h.
if(EXISTS "${detours_SOURCE_DIR}/include/detours.h")
    set(DETOURS_INCLUDE_DIR "${detours_SOURCE_DIR}/include")
elseif(EXISTS "${detours_SOURCE_DIR}/src/detours.h")
    set(DETOURS_INCLUDE_DIR "${detours_SOURCE_DIR}/src")
else()
    message(FATAL_ERROR "Could not find detours.h under ${detours_SOURCE_DIR}")
endif()

set(DETOURS_SRC_DIR "${detours_SOURCE_DIR}/src")

# Name must not collide with FetchContent's logical dependency name "detours" on some generators.
add_library(detours_static STATIC
    "${DETOURS_SRC_DIR}/creatwth.cpp"
    "${DETOURS_SRC_DIR}/detours.cpp"
    "${DETOURS_SRC_DIR}/disasm.cpp"
    "${DETOURS_SRC_DIR}/disolx64.cpp"
    "${DETOURS_SRC_DIR}/image.cpp"
    "${DETOURS_SRC_DIR}/modules.cpp"
)

target_include_directories(detours_static PUBLIC "${DETOURS_INCLUDE_DIR}")

if(MSVC)
    # Keep third-party Detours code as-is; compile it with permissive mode.
    target_compile_options(detours_static PRIVATE /W0 /permissive)
endif()

add_library(common STATIC
    common/logging.cpp
    common/logging.h
)

target_include_directories(common PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_compile_definitions(common PUBLIC
    WIN32_LEAN_AND_MEAN
    NOMINMAX
    UNICODE
    _UNICODE
)

add_executable(tool_launcher
    launcher/main.cpp
)

target_link_libraries(tool_launcher PRIVATE common)

set_target_properties(tool_launcher PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${THH_OUTPUT_DIR}"
)

add_executable(tool_hook_tests
    tests/main.cpp
)

target_link_libraries(tool_hook_tests PRIVATE common)

set_target_properties(tool_hook_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${THH_OUTPUT_DIR}"
)

add_library(tool_hook SHARED
    hookdll/dllmain.cpp
    hookdll/hooks.cpp
    hookdll/hooks.h
    source/cache/cache_file_builder_tag_resource_manager.cpp
    source/cache/cache_file_builder_tag_resource_manager.h
    source/cseries/cseries_asserts.cpp
    source/cseries/cseries_asserts.h
    source/memory/array.cpp
    source/memory/array.h
    source/memory/module.cpp
    source/memory/module.h
    source/tag_files/tag_groups.cpp
    source/tag_files/tag_groups.h
    source/tag_files/tag_resource_internals.cpp
    source/tag_files/tag_resource_internals.h
)

target_link_libraries(tool_hook PRIVATE
    common
    detours_static
    Psapi
)

target_include_directories(tool_hook PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/source"
    "${DETOURS_INCLUDE_DIR}"
)

set_target_properties(tool_hook PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${THH_OUTPUT_DIR}"
    OUTPUT_NAME "tool_hook"
)

add_dependencies(tool_hook_tests tool_launcher tool_hook)
