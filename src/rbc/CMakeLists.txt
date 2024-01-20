set(RBC_HOME ${ROOT_DIR} CACHE PATH "rbc home" FORCE)
set(RBC_ROOT_DIR "${RBC_HOME}/rbc" CACHE PATH "rbc root" FORCE)
set(PRODUCT_NAME rbc)

set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
if(NOT DEFINED CMAKE_C_VISIBILITY_PRESET AND NOT DEFINED CMAKE_VISIBILITY_INLINES_HIDDEN)
    set(CMAKE_C_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
endif()

dump(CMAKE_RUNTIME_OUTPUT_DIRECTORY)

option(BUILD_SHARED_LIBS "Build shared libraries")
# in rare cases an user may want to build some libs in other way than BUILD_SHARED_LIBS determines
# (for example all used libs are shared, but Rubidium should be built as static)
if(DEFINED RBC_USE_SHARED_LIBS)
    set(BUILD_SHARED_LIBS "${RBC_USE_SHARED_LIBS}")
endif()

# generate version file
set(GENERATED_INCLUDE_DIR "${CMAKE_BINARY_DIR}/include")
set(VERSION_FILE "${GENERATED_INCLUDE_DIR}/${PRODUCT_NAME}/${PRODUCT_NAME}_version.h")
configure_file(version.h.in "${VERSION_FILE}")

# collect features
include(features)

# collect sources
set(MODULES
    containers
    core
    proto-dcp2
    time
    utils)
# TODO should we use cache variables?
set(HEADERS)
set(SOURCES)
foreach(X ${MODULES})
    add_c_module(${X} ${RBC_ROOT_DIR})
endforeach()
add_subdirectory(sync)
source_group(TREE "${ROOT_DIR}" FILES ${HEADERS} ${SOURCES})

# create lib
set(INCLUDE_DIRS
    $<BUILD_INTERFACE:${RBC_HOME}>
    $<BUILD_INTERFACE:${GENERATED_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

if(${BUILD_SHARED_LIBS})
    set(LIB ${PRODUCT_NAME}-shared)
else()
    set(LIB ${PRODUCT_NAME})
endif()

add_library(${LIB} ${HEADERS} ${SOURCES})
add_library(RbC::RbC ALIAS ${LIB})
target_compile_definitions(${LIB}
    PUBLIC $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:RBC_STATIC>
    PRIVATE RBC_EXPORTS)
target_include_directories(${LIB}
    PUBLIC $<BUILD_INTERFACE:${INCLUDE_DIRS}>
    PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set(OUTPUT_DIR "${CMAKE_BINARY_DIR}")
set_target_properties(${LIB} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR} # .lib/.a for static, .lib for shared on Windows
    DEBUG_POSTFIX d
    LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR} # .so
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR} # .dll
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION})
set_target_properties(${LIB} PROPERTIES EXPORT_NAME RbC)

if(WIN32)
    target_compile_definitions(${LIB} PRIVATE WIN32_LEAN_AND_MEAN)
    if(COMPILER_MSVC_LIKE)
        target_compile_definitions(${LIB} PRIVATE NOMINMAX)
    endif()
elseif(UNIX)
    target_compile_definitions(${LIB} PRIVATE _GNU_SOURCE)
endif()

use_sanitizers(${LIB})

if(IS_TOP_LEVEL_PROJECT AND NOT CMAKE_SKIP_INSTALL_RULES)
    include(install)
endif()
