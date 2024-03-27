#pragma once
// LC-3 Model Used 
// Define a macro to represent the maximum memory size, calculated at compile time.
// The maximum memory size is specified as 128 kilobytes (KB).
#define MEMORY_MAX (1 << 16)

// Virtual Machine includes total number of 10 registers.
#define REGISTER_COUNT 10


#include <cstdint>


enum Opcodes
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


enum ConditionFlags
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


enum Registers
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


class CPU
{
	public:
		uint16_t registers[REGISTER_COUNT];
	
		// Ensure that each element of the 'memory' array stores 16 bits of data.
		// If 'uint16_t' is not explicitly specified (and 'int' is used instead), 
		// the size of each element might vary depending on the compiler and system,
		// potentially being interpreted as either 16 or 32 bits.
		uint16_t memory[MEMORY_MAX];

	public:
		CPU();

        void RunVM();

		uint16_t GetProgramCounter() const;
        uint16_t MemoryRead(uint16_t memoryAddress) const;
        uint16_t SignExtend(uint16_t immNumber, int immNumberLength) const;
        void UpdateFlags(uint16_t DR);
        
        void ADD(uint16_t instruction);
        void AND(uint16_t instruction);
        void NOT(uint16_t instruction);

        void LDI(uint16_t instruction);
        void BR(uint16_t instruction);
        void JMP(uint16_t instruction);
        void JSR(uint16_t instruction);
        void LD(uint16_t instruction);
        void LDR(uint16_t instruction);
        void LEA(uint16_t instruction);
        void ST(uint16_t instruction);
        void STI(uint16_t instruction);
        void STR(uint16_t instruction);
};