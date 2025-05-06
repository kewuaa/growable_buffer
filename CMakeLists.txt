CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

PROJECT(growable_buffer LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_EXPORT_COMPILE_COMMANDS on)
SET(CMAKE_CXX_FLAGS "-O0 -Wall -g -ggdb")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)


SET(BUILD_TESTS TRUE CACHE BOOL "if to build tests")

SET(DEFAULT_BUFFER_SIZE 1024 CACHE STRING "default buffer size")

if (NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug)
endif()

if (NOT DEFINED BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS FALSE)
endif()

include(GenerateExportHeader)

ADD_LIBRARY(${PROJECT_NAME})
generate_export_header(
    ${PROJECT_NAME}
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/growable_buffer_export.hpp
)
TARGET_COMPILE_DEFINITIONS(
    ${PROJECT_NAME}
    PUBLIC
        DEFAULT_BUFFER_SIZE=${DEFAULT_BUFFER_SIZE}
)
TARGET_INCLUDE_DIRECTORIES(
    ${PROJECT_NAME}
    PUBLIC
        src
        ${CMAKE_CURRENT_BINARY_DIR}
)
TARGET_SOURCES(${PROJECT_NAME} PRIVATE src/growable_buffer.cpp)

if (BUILD_TESTS)
    FIND_PACKAGE(fmt)

    INCLUDE(FetchContent)
    FetchContent_Declare(
        ut
        GIT_REPOSITORY "https://github.com/boost-ext/ut.git"
        GIT_TAG master
    )
    FetchContent_MakeAvailable(ut)

    ENABLE_TESTING()
    ADD_SUBDIRECTORY(tests)
endif()
