#pragma once

#include <rbc/core/keywords.h>

/**
 * The RBC_ENABLED macro implements a safe compile time check for features of Rubidium-C.
 * Features can be in three states:
 * <ul>
 * <li> 0 or undefined: this will lead to a compile error when testing for it
 * <li> -1: the feature is not available
 * <li> 1: the feature is available
 * </ul>
 */
#define RBC_ENABLED(feature) (1 / RBC_FEATURE_##feature == 1)
#define RBC_DISABLED(feature) (1 / RBC_FEATURE_##feature == -1)
#define RBC_REQUIRE_ENABLED(feature) \
	RBC_STATIC_ASSERT_X(RBC_FEATURE_##feature == 1, "Required feature " #feature " for file " __FILE__ " not available.")

#define RBC_HAS(feature) (1 / RBC_HAS_##feature == 1)
#define RBC_USE(engine) (1 / RBC_USE_##engine == 1)

#include <rbc/rbc_features.h>
