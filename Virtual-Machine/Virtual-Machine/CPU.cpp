#include "CPU.h"
#include <iostream>

CPU::CPU()
{
	// Since exactly one condition flag is settled at any given time, set the Z flag.
	registers[Registers::R_COND] = ConditionFlags::FL_ZERO;

	// Set the PC to starting position
	// PC_START = 0x3000 is the default
	registers[Registers::R_PC] = 0x3000;
}


void CPU::RunVM()
{
    int i = 1;
	while (i > 0)
	{
		// Fetch Instruction. Read the memory location pointed by program counter.
		uint16_t instruction = MemoryRead(registers[Registers::R_PC]++);

        // Extract the opcode from the instruction by considering bits [15:12]
        uint16_t operation = (instruction >> 12);

		switch (operation)
		{
            case OP_ADD:
                ADD(instruction);
                break;
            default:
                break;
		}
        i--;
	}
}


uint16_t CPU::GetProgramCounter() const
{
    return registers[Registers::R_PC];
}


/**
 * @brief Reads the 16 bits value from memory at the specified address.
 * @param memoryAddress: The address to read from.
 * @return The 16 bits value read from memory.
 */
uint16_t CPU::MemoryRead(uint16_t memoryAddress) const
{
    return memory[memoryAddress];
}


/**
 * @brief Sign extends a given immediate number.
 * @param immNumber: The immediate number to sign extend.
 * @param immNumberLength: The length of the immediate number in bits.
 * @return The sign extended immediate number.
 */
uint16_t CPU::SignExtend(uint16_t immNumber, int immNumberLength) const
{
    // Determine the sign bit of the immediate number
    int signBit = (immNumber >> (immNumberLength - 1)) & 0x0001;

    // If the sign bit is set, perform sign extension
    if (signBit)
    {
        return immNumber | (0xFFFF << immNumberLength);
    }

    // If sign bit is zero (0), return the original number
    return immNumber;
}


/**
 * @brief Updates the condition flags based on the value in the specified register.
 * @param DR The index of the destination register whose value is used to update the flags.
 */
void CPU::UpdateFlags(uint16_t DR)
{
    if (registers[DR] == 0)
    {
        // Set zero flag if the value in the register is zero
        registers[Registers::R_COND] = ConditionFlags::FL_ZERO;
    }
    // Check if the leftmost bit indicates negative for a 16-bit number
    else if ((registers[DR] >> 15) == 1)
    {
        // Set negative flag if the leftmost bit is 1
        registers[Registers::R_COND] = ConditionFlags::FL_NEGATIVE;
    }
    else
    {
        // Set positive flag if neither zero nor negative conditions are met
        registers[Registers::R_COND] = ConditionFlags::FL_POSITIVE;
    }
}



/**
 * @brief Performs an addition operation based on the provided instruction.
 * @param instruction: The 16 bits instruction.
 */
void CPU::ADD(uint16_t instruction)
{
    // Extract Destination Register (DR), Source Register 1 (SR1), and Immediate Flag (Imm)
    uint16_t DR = (instruction >> 9) & 0x0007; // Destination Register
    uint16_t SR1 = (instruction >> 6) & 0x0007; // Source Register 1
    uint16_t Imm = (instruction >> 5) & 0x0001; // Immediate Flag

    if (Imm)
    {
        // If Immediate Flag is set, extract immediate value and perform addition
        uint16_t imm5 = (instruction) & 0x001F;
        imm5 = SignExtend(imm5, 5);
        registers[DR] = registers[SR1] + imm5;
    }
    else
    {
        // If Immediate Flag is not set, extract Source Register 2 (SR2) and perform addition
        uint16_t SR2 = (instruction) & 0x0007; // Source Register 2
        registers[DR] = registers[SR1] + registers[SR2];
    }

    UpdateFlags(DR);
    //std::cout << std::dec << registers[R_2] << std::dec;
}
