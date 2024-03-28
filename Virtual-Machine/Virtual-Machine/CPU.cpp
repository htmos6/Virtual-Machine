#include <iostream>

#include "Trap.h"
#include "MemoryIO.h"
#include "ArithmeticLogicUnit.h"
#include "CPU.h"


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
	while (this->running)
	{
		// Fetch Instruction. Read the memory location pointed by program counter.
		uint16_t instruction = memoryIO->Read(registers[Registers::R_PC]++);

        // Extract the opcode from the instruction by considering bits [15:12]
        uint16_t operation = (instruction >> 12);

		switch (operation)
		{
            case OP_ADD:
                arithmeticLogicUnit->ADD(instruction);
                break;
            case OP_AND:
                arithmeticLogicUnit->AND(instruction);
                break;
            case OP_NOT:
                arithmeticLogicUnit->NOT(instruction);
                break;
            case OP_BR:
                arithmeticLogicUnit->BR(instruction);
                break;
            case OP_JMP:
                arithmeticLogicUnit->JMP(instruction);
                break;
            case OP_JSR:
                arithmeticLogicUnit->JSR(instruction);
                break;
            case OP_LD:
                arithmeticLogicUnit->LD(instruction);
                break;
            case OP_LDI:
                arithmeticLogicUnit->LDI(instruction);
                break;
            case OP_LDR:
                arithmeticLogicUnit->LDR(instruction);
                break;
            case OP_LEA:
                arithmeticLogicUnit->LEA(instruction);
                break;
            case OP_ST:
                arithmeticLogicUnit->ST(instruction);
                break;
            case OP_STI:
                arithmeticLogicUnit->STI(instruction);
                break;
            case OP_STR:
                arithmeticLogicUnit->STR(instruction);
                break;
            case OP_TRAP:
                trap->Proxy(*this, instruction);
                break;
            case OP_RES:
            case OP_RTI:
            default:
                abort();
                break;
		}

        //restore_input_buffering();
	}
}


/**
 * @brief Retrieves the value of the program counter (PC).
 * @return The value of the program counter.
 */
uint16_t CPU::GetProgramCounter() const
{
    return registers[Registers::R_PC];
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
 * @param DR: The index of the destination register whose value is used to update the flags.
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