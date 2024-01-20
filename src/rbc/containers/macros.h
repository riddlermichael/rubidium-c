#pragma once

#include <rbc/core/macros.h>

#define RBC_METHOD(type, name) RBC_CONCAT(RBC_CONCAT(type, _), name)
