cmake_minimum_required(VERSION 3.10)

project(awsTest)

set(aws-sdk-cpp_DIR "C:\\runtime\\awsbuild")

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(aws-sdk-cpp REQUIRED)
include_directories(${aws-sdk-cpp_INCLUDE_DIRS})

add_definitions(-DUSE_IMPORT_EXPORT)

add_executable(awsTest ${SOURCE_FILES})

target_link_libraries(awsTest
        aws-cpp-sdk-core
        aws-cpp-sdk-cognito-sync
        aws-cpp-sdk-cognito-idp
        aws-cpp-sdk-cognito-identity
        aws-cpp-sdk-identity-management)