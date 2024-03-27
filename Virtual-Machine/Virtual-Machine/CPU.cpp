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
 * @param instruction The 16-bit instruction.
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

    // Update condition flags based on the result in the destination register
    UpdateFlags(DR);
}


/**
 * @brief Performs a bitwise AND operation based on the provided instruction.
 * @param instruction The 16-bit instruction.
 */
void CPU::AND(uint16_t instruction)
{
    // Extract Destination Register (DR), Source Register 1 (SR1), and Immediate Flag (Imm)
    uint16_t DR = (instruction >> 9) & 0x0007; // Destination Register
    uint16_t SR1 = (instruction >> 6) & 0x0007; // Source Register 1
    uint16_t Imm = (instruction >> 5) & 0x0001; // Immediate Flag

    if (Imm)
    {
        // If Immediate Flag is set, extract immediate value and perform bitwise AND
        uint16_t imm5 = (instruction) & 0x001F;
        imm5 = SignExtend(imm5, 5);
        registers[DR] = registers[SR1] & imm5;
    }
    else
    {
        // If Immediate Flag is not set, extract Source Register 2 (SR2) and perform bitwise AND
        uint16_t SR2 = (instruction) & 0x0007; // Source Register 2
        registers[DR] = registers[SR1] & registers[SR2];
    }

    // Update condition flags based on the result in the destination register
    UpdateFlags(DR);
}


/**
 * @brief Performs a bitwise NOT operation based on the provided instruction.
 * @param instruction The 16-bit instruction.
 */
void CPU::NOT(uint16_t instruction)
{
    // Extract Destination Register (DR) and Source Register 1 (SR1)
    uint16_t DR = (instruction >> 9) & 0x0007; // Destination Register
    uint16_t SR1 = (instruction >> 6) & 0x0007; // Source Register 1

    // Perform bitwise NOT operation on the value in source register and store it in destination register
    registers[DR] = ~registers[SR1];

    // Update condition flags based on the result in the destination register
    UpdateFlags(DR);
}


/**
 * @brief Performs a branch operation based on the provided instruction.
 * @param instruction The 16-bit instruction.
 */
void CPU::BR(uint16_t instruction)
{
    // Extract pcOffset and Condition Flag
    uint16_t pcOffset = SignExtend(instruction & 0x01FF, 9);
    uint16_t conditionFlag = (instruction >> 9) & 0x0007;

    // Check if condition flag is set in the Condition Register, then update PC
    if (conditionFlag & registers[Registers::R_COND])
    {
        registers[Registers::R_PC] += pcOffset;
    }
}


/**
 * @brief Performs a jump operation based on the provided instruction.
 * @param instruction The 16-bit instruction.
 */
void CPU::JMP(uint16_t instruction)
{
    // Extract Source Register (SR1)
    uint16_t SR1 = (instruction >> 6) & 0x0007;

    // Set PC to the value in the source register
    registers[Registers::R_PC] = registers[SR1];
}


/**
 * @brief Performs a jump to subroutine operation based on the provided instruction.
 * @param instruction The 16-bit instruction.
 */
void CPU::JSR(uint16_t instruction)
{
    // Extract Long Flag to determine the type of jump
    uint16_t longFlag = (instruction >> 11) & 0x0001;

    // Save the current PC value to R7 (Return Address Register)
    registers[Registers::R_7] = registers[Registers::R_PC];

    if (longFlag)
    {
        // If Long Flag is set, calculate the long PC offset and perform jump
        uint16_t longPCOffset = SignExtend(instruction & 0x07FF, 11);
        registers[Registers::R_PC] += longPCOffset;  // JSR
    }
    else
    {
        // If Long Flag is not set, extract the source register and perform jump
        uint16_t SR1 = (instruction >> 6) & 0x0007;
        registers[Registers::R_PC] = registers[SR1]; // JSRR
    }
}


/**
 * @brief Performs a load operation based on the provided instruction.
 * @param instruction The 16-bit instruction.
 */
void CPU::LD(uint16_t instruction)
{
    // Extract Destination Register (DR) and PC Offset
    uint16_t DR = (instruction >> 9) & 0x0007; // Destination Register
    uint16_t pcOffset = SignExtend(instruction & 0x01FF, 9);

    // Load the value from memory at the calculated address into the destination register
    registers[DR] = MemoryRead(registers[Registers::R_PC] + pcOffset);

    // Update the condition flags based on the value loaded into the destination register
    UpdateFlags(DR);
}


/**
 * @brief Performs a load from base register with offset operation based on the provided instruction.
 * @param instruction The 16-bit instruction.
 */
void CPU::LDR(uint16_t instruction)
{
    // Extract Destination Register (DR), Base Register (SR1), and Offset
    uint16_t DR = (instruction >> 9) & 0x0007; // Destination Register
    uint16_t SR1 = (instruction >> 6) & 0x0007; // Base Register
    uint16_t offset = SignExtend(instruction & 0x003F, 6); // Offset

    // Load the value from memory at the address calculated by adding base register value and offset
    registers[DR] = MemoryRead(registers[SR1] + offset);

    // Update the condition flags based on the value loaded into the destination register
    UpdateFlags(DR);
}


/**
 * @brief Performs a load effective address operation based on the provided instruction.
 * @param instruction The 16-bit instruction.
 */
void CPU::LEA(uint16_t instruction)
{
    // Extract Destination Register (DR) and PC Offset
    uint16_t DR = (instruction >> 9) & 0x0007; // Destination Register
    uint16_t pcOffset = SignExtend(instruction & 0x01FF, 9); // PC Offset

    // Calculate the effective address by adding PC value and PC offset
    registers[DR] = registers[Registers::R_PC] + pcOffset;

    // Update the condition flags based on the effective address value
    UpdateFlags(DR);
}
