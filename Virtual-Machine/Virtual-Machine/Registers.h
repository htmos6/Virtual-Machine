#pragma once
#include <cstdint>

enum register_types
{
	R_0 = 0,
	R_1,
	R_2,
	R_3,
	R_4,
	R_5,
	R_6,
	R_7, 
	R_PC, // Program Counter
	R_COND, // Condition Flags
	R_COUNT,
};

uint16_t registers[R_COUNT];