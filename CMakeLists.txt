cmake_minimum_required(VERSION 3.13)

set(project_name synyi-logging-client) ## rename your project here

if(TARGET ${project_name})
    return()
endif()

# include(CMakeDetermineSystem)

# if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
#     if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "^[Xx]86$")
#         set(VCPKG_TARGET_TRIPLET "x86-windows-static" CACHE STRING "VCPKG_TARGET_TRIPLET" FORCE)
#     elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "^[Aa][Mm][Dd]64$")
#         set(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "VCPKG_TARGET_TRIPLET" FORCE)
#     elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "^[Aa][Rr][Mm]$")
#         set(VCPKG_TARGET_TRIPLET "arm-windows-static" CACHE STRING "VCPKG_TARGET_TRIPLET" FORCE)
#     elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "^[Aa][Rr][Mm]64$")
#         set(VCPKG_TARGET_TRIPLET "arm64-windows-static" CACHE STRING "VCPKG_TARGET_TRIPLET" FORCE)
#     else()
#         message(AUTHOR_WARNING "Failed to determine the system arch.")
#     endif()
#     message(STATUS "Using vcpkg for ${VCPKG_TARGET_TRIPLET}, ${CMAKE_HOST_SYSTEM_PROCESSOR}")
#     set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "CMAKE_TOOLCHAIN_FILE" FORCE)
# endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# include(cotire)

project(${project_name} C CXX)

set(BUILD_SHARED_LIBS OFF CACHE STRING "BUILD_SHARED_LIBS" FORCE)

set(CMAKE_CXX_EXTENSIONS OFF CACHE STRING "CMAKE_CXX_EXTENSIONS" FORCE)
set(CMAKE_CXX_STANDARD 17 CACHE STRING "Set to 11 or 14 to enable C++11, C++14 or C++17 builds, respectively." FORCE)
set(CMAKE_C_STANDARD 11 CACHE STRING "Set to 99 or 11 to enable C99 or C11 builds, respectively." FORCE)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

add_compile_definitions("__STDC_WANT_LIB_EXT1__=1")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${CMAKE_BINARY_DIR})

if(WIN32 OR WIN64)
    # include(BuildForWinXP)

    add_compile_definitions("WIN32_LEAN_AND_MEAN" "_CRT_SECURE_NO_WARNINGS" "NOMINMAX")
    if(MSVC)
        add_compile_options("/source-charset:utf-8" "/EHsc" "/Zc:threadSafeInit-")
        include(MSVCRuntime)
        configure_msvc_runtime()
    endif()
else()
    if(UNIX)
        if(APPLE)
            add_compile_options("-m64" "-fPIC" "-march=native")
            set(CMAKE_MACOSX_RPATH 1 CACHE STRING "CMAKE_MACOSX_RPATH" FORCE)
        else()
            add_compile_options("-fPIC" )
            if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
                add_compile_options("-m64" "-march=westmere")
            endif()
        endif()
    endif()
endif()

include_directories(BEFORE SYSTEM "${CMAKE_CURRENT_LIST_DIR}/include/")

option(USE_BOOST_BEAST_HTTP_CLIENT "Use boost.beast as the http client backend." ON)
option(USE_BOOST_BEAST_HTTPS "Use boost.beast as the https client backend." ON)

include(external/InitializeExternalComponents.cmake)

add_compile_definitions("USE_SPDLOG")

add_subdirectory(src)

option(SYNYI_LOGGING_SDK_BUILD_TESTS "Build unit tests for the project." OFF)

if(SYNYI_LOGGING_SDK_BUILD_TESTS)
    add_subdirectory(test)
endif()