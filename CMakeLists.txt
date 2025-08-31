cmake_minimum_required(VERSION 3.16)
project(Doppler_Shift C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Raylib dependency via FetchContent
include(FetchContent)
# Share FetchContent downloads/builds in ~/.cache/cmake by default
if(NOT DEFINED FETCHCONTENT_BASE_DIR)
  set(FETCHCONTENT_BASE_DIR "$ENV{HOME}/.cache/cmake" CACHE PATH "Base dir for FetchContent downloads" FORCE)
endif()

set(RAYLIB_VERSION 5.0)
# Prefer system package if available
find_package(raylib ${RAYLIB_VERSION} QUIET)
if(NOT raylib_FOUND)
  message(STATUS "raylib not found system-wide; fetching ${RAYLIB_VERSION}...")
  FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG ${RAYLIB_VERSION}
  )
  FetchContent_MakeAvailable(raylib)
endif()

# Warnings
if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wno-unused-parameter)
endif()

# Source
add_executable(doppler_shift
  src/main.c
)

# Link
if (UNIX AND NOT APPLE)
  target_link_libraries(doppler_shift PRIVATE raylib m)
else()
  target_link_libraries(doppler_shift PRIVATE raylib)
endif()

# Include raylib headers if it's a fetched target
if (TARGET raylib)
  target_include_directories(doppler_shift PRIVATE ${raylib_SOURCE_DIR}/src)
endif()

# Install
install(TARGETS doppler_shift RUNTIME DESTINATION bin)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
