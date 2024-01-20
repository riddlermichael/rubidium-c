#pragma once

#ifdef __cplusplus
	#define RBC_BEGIN_EXTERN_C extern "C" {
	#define RBC_END_EXTERN_C }
#else
	#define RBC_BEGIN_EXTERN_C
	#define RBC_END_EXTERN_C
#endif
