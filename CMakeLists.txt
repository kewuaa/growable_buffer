cmake_minimum_required(VERSION 3.15)

project(growable_buffer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_CXX_FLAGS "-O0 -Wall -g -ggdb")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)


set(BUILD_TESTS TRUE CACHE BOOL "if to build tests")

set(DEFAULT_BUFFER_SIZE 1024 CACHE STRING "default buffer size")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if (NOT DEFINED BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS FALSE)
endif()

include(GenerateExportHeader)

add_library(${PROJECT_NAME})
generate_export_header(
    ${PROJECT_NAME}
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/growable_buffer_export.hpp
)
target_compile_definitions(
    ${PROJECT_NAME}
    PUBLIC
        DEFAULT_BUFFER_SIZE=${DEFAULT_BUFFER_SIZE}
)
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        src
        ${CMAKE_CURRENT_BINARY_DIR}
)
target_sources(${PROJECT_NAME} PRIVATE src/growable_buffer.cpp)

if (BUILD_TESTS)
    find_package(fmt)

    include(FetchContent)
    FetchContent_Declare(
        ut
        GIT_REPOSITORY "https://github.com/boost-ext/ut.git"
        GIT_TAG master
    )
    FetchContent_MakeAvailable(ut)

    enable_testing()
    add_subdirectory(tests)
endif()
