find_package(Threads REQUIRED)

if(UNIX)
    find_package(DL REQUIRED)
endif()

include(FetchContent)


# boost-cmake
if(WIN32)
    set(Boost_USE_STATIC_LIBS ON CACHE STRING "Boost_USE_STATIC_LIBS" FORCE)
    set(Boost_USE_STATIC_RUNTIME ON CACHE STRING "Boost_USE_STATIC_RUNTIME" FORCE)
endif()

find_package(Boost 1.67 REQUIRED COMPONENTS thread log log_setup system program_options filesystem coroutine locale regex unit_test_framework)
if(Boost_FOUND)
    message(STATUS "** Boost Include: ${Boost_INCLUDE_DIR}")
    message(STATUS "** Boost Libraries Directory: ${Boost_LIBRARY_DIRS}")
    message(STATUS "** Boost Libraries: ${Boost_LIBRARIES}")
    include_directories(${Boost_INCLUDE_DIRS})
else()
    if(WIN32)
        message(FATAL_ERROR "Plase check your vcpkg settings or global environment variables for the boost library.")
    else()
        FetchContent_Declare(boost_cmake
            GIT_REPOSITORY https://github.com/Orphis/boost-cmake.git
            GIT_TAG d3951bc7f0b9d09005f92aedcf6acfc595f050ea)

        FetchContent_GetProperties(boost_cmake)
        if(NOT boost_cmake_POPULATED)
            FetchContent_Populate(boost_cmake)
            add_subdirectory(${boost_cmake_SOURCE_DIR} ${boost_cmake_BINARY_DIR} EXCLUDE_FROM_ALL)
        endif()
    endif()
endif()


# nlohmann_json
FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_TAG master)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# fmt
FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master)

FetchContent_GetProperties(fmt)
if(NOT fmt_POPULATED)
  FetchContent_Populate(fmt)
  add_subdirectory(${fmt_SOURCE_DIR} ${fmt_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# spdlog
FetchContent_Declare(spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG master)

FetchContent_GetProperties(spdlog)
if(NOT spdlog_POPULATED)
  FetchContent_Populate(spdlog)
  add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# utfcpp
FetchContent_Declare(utfcpp
  GIT_REPOSITORY https://github.com/nemtrif/utfcpp.git
  GIT_TAG master)

FetchContent_GetProperties(utfcpp)
if(NOT utfcpp_POPULATED)
  FetchContent_Populate(utfcpp)
  add_subdirectory(${utfcpp_SOURCE_DIR} ${utfcpp_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_Declare(cpp_httplib
  GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
  GIT_TAG master)

FetchContent_GetProperties(cpp_httplib)
if(NOT cpp_httplib_POPULATED)
  FetchContent_Populate(cpp_httplib)
  add_subdirectory(${cpp_httplib_SOURCE_DIR} ${cpp_httplib_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# uriparser
set(URIPARSER_BUILD_DOCS OFF CACHE BOOL "URIPARSER_BUILD_DOCS" FORCE)
set(URIPARSER_BUILD_TESTS OFF CACHE BOOL "URIPARSER_BUILD_TESTS" FORCE)
set(URIPARSER_BUILD_TOOLS OFF CACHE BOOL "URIPARSER_BUILD_TOOLS" FORCE)
set(URIPARSER_ENABLE_INSTALL OFF CACHE BOOL "URIPARSER_BUILD_TOOLS" FORCE)

FetchContent_Declare(uriparser
  GIT_REPOSITORY https://github.com/uriparser/uriparser.git
  GIT_TAG master)

FetchContent_GetProperties(uriparser)
if(NOT uriparser_POPULATED)
  FetchContent_Populate(uriparser)
  add_subdirectory(${uriparser_SOURCE_DIR} ${uriparser_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# oatpp
FetchContent_Declare(oatpp
  GIT_REPOSITORY https://github.com/oatpp/oatpp.git
  GIT_TAG master)

FetchContent_GetProperties(oatpp)
if(NOT oatpp_POPULATED)
  FetchContent_Populate(oatpp)
  add_subdirectory(${oatpp_SOURCE_DIR} ${oatpp_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
