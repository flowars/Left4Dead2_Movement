#pragma once

#include "BaseTypes.h"
#include <malloc.h>
#include <memory.h>
#include <limits.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>

#define FORCEINLINE				__forceinline
#define LittleLong( val )			( val )

#define DECL_ALIGN(x)			/* */
#define ALIGN16 DECL_ALIGN(16)