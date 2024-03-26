#pragma once
#include <cstdint>

// LC-3 Model Used 

// Define a macro to represent the maximum memory size, calculated at compile time.
// The maximum memory size is specified as 128 kilobytes (KB).
#define MEMORY_MAX (1 << 16)

// Ensure that each element of the 'memory' array stores 16 bits of data.
// If 'uint16_t' is not explicitly specified (and 'int' is used instead), 
// the size of each element might vary depending on the compiler and system,
// potentially being interpreted as either 16 or 32 bits.
uint16_t memory[MEMORY_MAX];
