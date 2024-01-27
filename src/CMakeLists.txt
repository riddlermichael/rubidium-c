# compile options and sanitizers' settings act on current directory and below
include(sanitizers)

set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
if(NOT DEFINED CMAKE_C_VISIBILITY_PRESET AND NOT DEFINED CMAKE_VISIBILITY_INLINES_HIDDEN)
    set(CMAKE_C_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
endif()

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(rbc)

if(IS_TOP_LEVEL_PROJECT)
    # build app
    set(APP rbc-main)
    add_executable(${APP} main.c)
    target_link_libraries(${APP} PUBLIC RbC::RbC)
    use_sanitizers(${APP})
endif()
