# CMakeLists.txt for HelloWorld CommonAPI SOME/IP Demo
#
# This CMake file builds both the Server and Client applications.
# It links against CommonAPI, CommonAPI-SOMEIP, and vsomeip3.

cmake_minimum_required(VERSION 3.10)
project(HelloWorld_CommonAPI_Demo)

# Use C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
# CommonAPI Core - the middleware abstraction
find_package(CommonAPI REQUIRED)
# CommonAPI-SomeIP - the SOME/IP binding
find_package(CommonAPI-SomeIP REQUIRED)
# vsomeip3 - the underlying SOME/IP implementation
find_package(vsomeip3 REQUIRED)

# Print found package info (for debugging)
message(STATUS "CommonAPI found: ${CommonAPI_FOUND}")
message(STATUS "CommonAPI-SomeIP found: ${CommonAPI-SomeIP_FOUND}")
message(STATUS "vsomeip3 found: ${vsomeip3_FOUND}")
message(STATUS ${CMAKE_CURRENT_SOURCE_DIR} )
# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/fidl/src-gen
)

# ============================================================
# GENERATED CODE - These are the files from code generation
# ============================================================

# SOME/IP binding layer (transport specific)
set(GENERATED_SOMEIP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/fidl/src-gen/v1/ayman/examples/HelloSomeIPDeployment.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fidl/src-gen/v1/ayman/examples/HelloSomeIPProxy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fidl/src-gen/v1/ayman/examples/HelloSomeIPStubAdapter.cpp
)


# ============================================================
# SERVER APPLICATION
# ============================================================
add_executable(HelloWorldServer
    SRC/Server.cpp
    ${GENERATED_SOMEIP_SOURCES}
)

target_link_libraries(HelloWorldServer
    PUBLIC
    CommonAPI
    CommonAPI-SomeIP
    vsomeip3
)

# ============================================================
# CLIENT APPLICATION
# ============================================================
add_executable(HelloWorldClient
    SRC/Client.cpp
    ${GENERATED_SOMEIP_SOURCES}
)

target_link_libraries(HelloWorldClient
    CommonAPI
    CommonAPI-SomeIP
    vsomeip3
)

# ============================================================
# Build instructions
# ============================================================
# To build:
#   mkdir build && cd build
#   cmake ..
#   make
#
# To run:
#   Terminal 1: ./HelloWorldServer
#   Terminal 2: ./HelloWorldClient