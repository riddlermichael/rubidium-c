cmake_minimum_required(VERSION 3.21)

set(VERSION)
file(STRINGS VERSION VERSION)

project(rubidium-c
    DESCRIPTION "Rubidium C library"
    LANGUAGES C
    VERSION ${VERSION})

set(MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${MODULE_PATH}")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" CACHE INTERNAL "CMake module path" FORCE)
include(Utils)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "Install prefix" FORCE)
endif()

option(BUILD_TESTING "Build tests")
if(${BUILD_TESTING})
    include(CTest)
    enable_testing()
endif()

add_subdirectory(src)

if(PROJECT_IS_TOP_LEVEL AND BUILD_TESTING)
    set(CATCH_INSTALL_DOCS OFF CACHE BOOL "Catch2's documentation won't be included in the installation")
    add_subdirectory(tests)
endif()
