#cmake version setup
cmake_minimum_required(VERSION 3.1...3.16)

#project set up
project(ResourceAllocation VERSION 1.0
    DESCRIPTION "Resource Allocation"
        LANGUAGES  CXX
    )

if(NOT CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER "g++")
endif()

# The executable code is here
add_subdirectory(apps)

# The compiled library code is here
add_subdirectory(src)

# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
    add_subdirectory(tests)
endif()
