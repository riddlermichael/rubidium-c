# prepare Catch2
include(FetchContent)
FetchContent_Declare(
    Catch2
    DOWNLOAD_EXTRACT_TIMESTAMP ON
    URL "${CMAKE_SOURCE_DIR}/ext/Catch2.zip"
    URL_MD5 bc15e4ec79c2cb21b657882066c04e5a
)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")

include(Catch)
use_sanitizers(Catch2)

# test project
project(rbc-tests LANGUAGES CXX)

# collect tests
set(MODULES_WITH_TESTS
    containers
    core
    utils)
set(SOURCES)
foreach(X ${MODULES_WITH_TESTS})
    add_cxx_module(${X}/test ${RBC_ROOT_DIR})
endforeach()

# build test app
set(TEST_APP ${PROJECT_NAME})
add_executable(${TEST_APP} main.cpp ${SOURCES})
target_link_libraries(${TEST_APP} PRIVATE Catch2::Catch2)
target_link_libraries(${TEST_APP} PRIVATE RbC::RbC)
use_sanitizers(${TEST_APP})

catch_discover_tests(${TEST_APP})
