#ifndef CPU_H
#define CPU_H

// LC-3 Model Used 
// Define a macro to represent the maximum memory size, calculated at compile time.
// The maximum memory size is specified as 128 kilobytes (KB).
#define MEMORY_MAX (1 << 16)

// Virtual Machine includes total number of 10 registers.
#define REGISTER_COUNT 10


#include <cstdio>
#include <cstdint>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
// windows only
#include <Windows.h>
#include <conio.h>  // _kbhit


class Trap;
class ArithmeticLogicUnit;
class MemoryIO;
class OS;


enum Opcodes : uint16_t
{
    OP_BR = 0, // branch
    OP_ADD,    // add 
    OP_LD,     // load
    OP_ST,     // store
    OP_JSR,    // jump register
    OP_AND,    // bitwise and
    OP_LDR,    // load register
    OP_STR,    // store register
    OP_RTI,    // unused
    OP_NOT,    // bitwise not
    OP_LDI,    // load indirect
    OP_STI,    // store indirect
    OP_JMP,    // jump
    OP_RES,    // reserved (unused)
    OP_LEA,    // load effective address
    OP_TRAP    // execute trap
};


enum ConditionFlags : uint16_t
{
    // The condition flags represent the state of a computation's result.
    // They are stored in the last 3 bits of a 16-bit unsigned integer.
    // The flag results can be retrieved from there.

    // The POSITIVE flag indicates that the result is positive.
    FL_POSITIVE = (1 << 0),

    // The ZERO flag indicates that the result is zero.
    FL_ZERO = (1 << 1),

    // The NEGATIVE flag indicates that the result is negative.
    FL_NEGATIVE = (1 << 2)
};


enum Registers : uint16_t
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
};


enum MemoryMappedRegisters : uint16_t
{
    MR_KBSR = 0xFE00, // keyboard status
    MR_KBDR = 0xFE02  // keyboard data
};


class CPU
{
    public:
        // Array to store 16-bit registers.
		uint16_t registers[REGISTER_COUNT];

        // Ensure that each element of the "memory" array stores 16 bits of data.
        // If "uint16_t" is not explicitly specified (and "int" is used instead), 
        // the size of each element might vary depending on the compiler and system,
        // potentially being interpreted as either 16 or 32 bits.
        uint16_t memory[MEMORY_MAX];

        // Boolean flag to control the execution state of the Virtual Machine.
        int running = 1;

	public:
		CPU();
        ~CPU();
		
        void UpdateFlags(uint16_t DR);

        void ReadImageFile(FILE* file, ArithmeticLogicUnit* alu);
        int ReadImage(const char* imagePath, ArithmeticLogicUnit* alu);
};
#endif