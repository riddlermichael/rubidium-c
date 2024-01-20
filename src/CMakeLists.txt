# compile options and sanitizers' settings act on current directory and below
include(sanitizers)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(rbc)

if(IS_TOP_LEVEL_PROJECT)
    # build app
    set(APP rbc-main)
    add_executable(${APP} main.c)
    target_link_libraries(${APP} PUBLIC RbC::RbC)
    use_sanitizers(${APP})
endif()
