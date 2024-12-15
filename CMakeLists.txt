cmake_minimum_required(VERSION 3.10)

project(KTableProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

set(HEADERS
    ${PROJECT_SOURCE_DIR}/include/ktable.h
    ${PROJECT_SOURCE_DIR}/include/linear.h
    ${PROJECT_SOURCE_DIR}/include/version.h
)

add_executable(tab ${SOURCES} ${HEADERS})
