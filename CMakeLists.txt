# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.16)

# Auto-detect vcpkg toolchain BEFORE project() — required for CMake GUI and
# command-line builds that do not pass -DCMAKE_TOOLCHAIN_FILE explicitly.
# Priority:
#   1. Already set by user (CMake GUI / -DCMAKE_TOOLCHAIN_FILE=...)
#   2. VCPKG_ROOT environment variable
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{VCPKG_ROOT})
        set(CMAKE_TOOLCHAIN_FILE
            "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "vcpkg toolchain file" FORCE)
        message(STATUS "[vcpkg] Toolchain auto-detected from VCPKG_ROOT: ${CMAKE_TOOLCHAIN_FILE}")
    else()
        message(WARNING
            "[vcpkg] VCPKG_ROOT is not set and CMAKE_TOOLCHAIN_FILE was not provided. "
            "Set the VCPKG_ROOT environment variable or pass "
            "-DCMAKE_TOOLCHAIN_FILE=<vcpkg_root>/scripts/buildsystems/vcpkg.cmake")
    endif()
endif()

project ("CEGUI")

# Register custom modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Set ON for DLL (.dll), OFF for static (.lib) — default to shared
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# Enable multi-threaded compilation for MSVC (/MP) and
# explicit parallel job count for Ninja.
if(MSVC)
    # /MP: compile multiple source files in parallel within a single target.
    # Omit the number to let MSVC use all logical cores automatically.
    add_compile_options(/MP)
endif()

# Set C++ standard to C++14 for all targets (can be overridden by sub-projects)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set default install location to <source>/install
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "Default install path" FORCE)
endif()

# Include install helper function (ni_install_target)
include("${CMAKE_SOURCE_DIR}/cmake/install.cmake")

# Renderer codecs
option(BUILD_RENDERER_OPENGL "Use OpenGL3 renderer" ON)
option(BUILD_RENDERER_D3D9 "Use Direct3D 9 renderer" ON)
option(BUILD_RENDERER_D3D10 "Use Direct3D 10 renderer" ON)
option(BUILD_RENDERER_D3D11 "Use Direct3D 11 renderer" ON)
option(BUILD_RENDERER_NULL "Use null renderer (for testing)" OFF)

# Script codecs
option(BUILD_SCRIPT_LUA "Use Lua scripting language" ON)

# Image codecs
option(BUILD_IMAGE_DEVIL "Use DevIL image library" ON)
option(BUILD_IMAGE_STB "Use STB image library" ON)
option(BUILD_IMAGE_TGA "Use TGA image library" ON)
option(BUILD_IMAGE_FREEIMAGE "Use FreeImage library" ON)

# XML codecs
option(BUILD_XML_EXPAT "Use Expat XML library" ON)
option(BUILD_XML_RAPIDXML "Use RapidXML library" ON)
option(BUILD_XML_XERCES "Use Xerces-C++ XML library" ON)
option(BUILD_XML_TINYXML "Use TinyXML library" ON)

# Include sub-projects.
add_subdirectory ("src")
