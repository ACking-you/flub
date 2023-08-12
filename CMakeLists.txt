cmake_minimum_required(VERSION 3.14)
project(
        flub
        VERSION 0.1
        LANGUAGES CXX
)
include(${PROJECT_SOURCE_DIR}/cmake/check_version.cmake)
# Required version >= 11,default version is 17
check_version(11 17)

# Prevent duplicate targets when relying on source code
if (TARGET ${PROJECT_NAME})
    message(WARNING "Target ${PROJECT_NAME} already exists!")
    return()
endif ()

file(GLOB headers ${PROJECT_SOURCE_DIR}/src/flub/*.h)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR}/src)


if (ENABLE_FLUB_TEST)
    enable_testing()
    add_subdirectory(tests)
endif ()