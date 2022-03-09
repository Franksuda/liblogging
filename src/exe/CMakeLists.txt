# Set the project name
project(synyi-logging-exe C CXX)

file(GLOB_RECURSE src
    ${CMAKE_CURRENT_LIST_DIR}/*.h
    ${CMAKE_CURRENT_LIST_DIR}/*.c
	${CMAKE_CURRENT_LIST_DIR}/*.cc
	${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

# Add an executable for testing purpose
add_executable(synyi-logging-exe ${src})

target_link_libraries(synyi-logging-exe
    PUBLIC synyi-logging-client-lib-static
)