set(FEATURE_FILE "${GENERATED_INCLUDE_DIR}/rbc/rbc_features.h")

file(WRITE ${FEATURE_FILE} "/*
 * This file is auto-generated.
 * Do not edit this manually.
 * Do not include this directly, use #include <rbc/core/features.h> instead.
 */\n\n")

include(CMakeDependentOption)

# regular features
option(FULL_SOURCE_LOCATION_INFO "Print full source location info" ON)

set(FEATURES
    FULL_SOURCE_LOCATION_INFO)

foreach(FEATURE ${FEATURES})
    if(${FEATURE})
        set(VALUE 1)
    else()
        set(VALUE "(-1)")
    endif()
    file(APPEND ${FEATURE_FILE} "#define RBC_FEATURE_${FEATURE} ${VALUE}\n")
    dump(${FEATURE})
endforeach()
