# Set the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(KTableProject VERSION 1.0)

# Enable C++17 (or the version of C++ you want to use)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the include directories for header files
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add all .cpp files from the src folder
file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# Optionally, you can add headers, though they are not required for the build step
set(HEADERS
    ${PROJECT_SOURCE_DIR}/include/ktable.h
    ${PROJECT_SOURCE_DIR}/include/linear.h
    ${PROJECT_SOURCE_DIR}/include/version.h
)

# Create an executable from the source files
add_executable(tab ${SOURCES} ${HEADERS})

# Optionally, specify additional libraries to link against
# target_link_libraries(KTableExecutable PRIVATE pthread)

# Optionally, specify compiler flags if needed (e.g., for debugging, optimizations)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")
