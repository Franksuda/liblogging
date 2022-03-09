cmake_minimum_required(VERSION 3.16)

# Set the project name
set(project_name "synyi-logging-client-lib")

project(${project_name} C CXX)

if(WIN32)
    execute_process(COMMAND CMD /c "git describe --tags --always --dirty"
        WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
        OUTPUT_VARIABLE VER)
elseif(UNIX)
    execute_process(COMMAND git describe --tags --always --dirty
        WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
        OUTPUT_VARIABLE VER)
    if(APPLE)
        execute_process(COMMAND sw_vers -productVersion OUTPUT_VARIABLE MACOSX_DEPLOYMENT_TARGET)
        string(REGEX REPLACE "\n$" "" MACOSX_DEPLOYMENT_TARGET "${MACOSX_DEPLOYMENT_TARGET}")
        message(STATUS "MACOSX_DEPLOYMENT_TARGET: ${MACOSX_DEPLOYMENT_TARGET}")
    endif()
else()
    message(fatal "Unsupported platform detected.")
endif()

string(REGEX REPLACE "\n$" "" VER "${VER}")
message(STATUS "Logging SDK version: ${VER}")

configure_file("${CMAKE_CURRENT_LIST_DIR}/impl/synyi/logging/version.c.in" "${CMAKE_CURRENT_BINARY_DIR}/version.c")

file(GLOB_RECURSE src
    ${CMAKE_CURRENT_LIST_DIR}/*.def

    ${CMAKE_CURRENT_LIST_DIR}/*.h
    ${CMAKE_CURRENT_LIST_DIR}/*.hpp

    ${CMAKE_CURRENT_LIST_DIR}/*.c
	${CMAKE_CURRENT_LIST_DIR}/*.cc
	${CMAKE_CURRENT_LIST_DIR}/*.cxx
	${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

list(APPEND src "${CMAKE_CURRENT_BINARY_DIR}/version.c")

file(GLOB_RECURSE public_headers
    ${CMAKE_CURRENT_LIST_DIR}/include/public/*.h
    ${CMAKE_CURRENT_LIST_DIR}/include/public/*.hpp
)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

# -----------------------------------------------------
include(GenerateExportHeader)

add_library(${project_name}
    SHARED ${src}
)

target_link_libraries(${project_name}
    PRIVATE ${CMAKE_THREAD_LIBS_INIT}
    PRIVATE ${DL_LIBRARY}
    PRIVATE fmt::fmt
    PUBLIC oatpp
    PRIVATE spdlog::spdlog
    PRIVATE utf8cpp
    PRIVATE ${Boost_LIBRARIES}
    PUBLIC nlohmann_json::nlohmann_json
    PRIVATE httplib::httplib
    PRIVATE uriparser
)

if(WIN32)
    if(CMAKE_CL_64)
        target_link_options(${project_name}
            PUBLIC "/SUBSYSTEM:WINDOWS,5.02")
    else()
        target_link_options(${project_name}
            PUBLIC "/SUBSYSTEM:WINDOWS,5.01")
    endif()
endif()

target_include_directories(${project_name}
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/public"
    PRIVATE "${CMAKE_CURRENT_LIST_DIR}/include/private"
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/include"
)

file(COPY "${CMAKE_CURRENT_LIST_DIR}/include/public/"
    DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/include/")

string(MAKE_C_IDENTIFIER ${project_name} target_id)
string(TOUPPER ${target_id} target_id)
set(export_macro "${target_id}_API")

target_compile_definitions(${project_name}
    PRIVATE "${no_export_macro}")

if(USE_BOOST_BEAST_HTTP_CLIENT)
    target_compile_definitions(${project_name}
        PRIVATE "USE_BOOST_BEAST_HTTP_CLIENT")
endif()

generate_export_header(${project_name}
    EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/synyi/logging/exports.h"
    # EXPORT_MACRO_NAME ${export_macro}
)

file(COPY "${CMAKE_CURRENT_BINARY_DIR}/include/"
    DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/include/")

# ------------------------------------------------------------------
add_library(${project_name}-static
    STATIC ${src}
)

target_link_libraries(${project_name}-static
    PUBLIC ${CMAKE_THREAD_LIBS_INIT}
    PUBLIC ${DL_LIBRARY}
    PUBLIC fmt::fmt
    PUBLIC oatpp
    PUBLIC spdlog::spdlog
    PUBLIC utf8cpp
    PUBLIC ${Boost_LIBRARIES}
    PUBLIC nlohmann_json::nlohmann_json
    PUBLIC httplib::httplib
    PUBLIC uriparser
)

target_include_directories(${project_name}-static
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/public"
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/private"
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/include"
)

target_compile_definitions(${project_name}-static
    PUBLIC SYNYI_LOGGING_CLIENT_LIB_STATIC_DEFINE)

if(USE_BOOST_BEAST_HTTP_CLIENT)
    target_compile_definitions(${project_name}-static
        PRIVATE "USE_BOOST_BEAST_HTTP_CLIENT")
endif()
