#include "ArithmeticLogicUnit.h"
#include "MemoryIO.h"


ArithmeticLogicUnit::ArithmeticLogicUnit(MemoryIO* memoryIO)
{
    this->memoryIO = memoryIO;
}


/**
 * @brief Performs an addition operation based on the provided instruction.
 * @param instruction: The 16-bit instruction.
 */
void ArithmeticLogicUnit::ADD(uint16_t instruction)
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
 * @param instruction: The 16-bit instruction.
 */
void ArithmeticLogicUnit::AND(uint16_t instruction)
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
 * @param instruction: The 16-bit instruction.
 */
void ArithmeticLogicUnit::NOT(uint16_t instruction)
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
 * @param instruction: The 16-bit instruction.
 */
void ArithmeticLogicUnit::BR(uint16_t instruction)
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
void ArithmeticLogicUnit::JMP(uint16_t instruction)
{
    // Extract Source Register (SR1)
    uint16_t SR1 = (instruction >> 6) & 0x0007;

    // Set PC to the value in the source register
    registers[Registers::R_PC] = registers[SR1];
}


/**
 * @brief Performs a jump to subroutine operation based on the provided instruction.
 * @param instruction: The 16-bit instruction.
 */
void ArithmeticLogicUnit::JSR(uint16_t instruction)
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
 * @param instruction: The 16-bit instruction.
 */
void ArithmeticLogicUnit::LD(uint16_t instruction)
{
    // Extract Destination Register (DR) and PC Offset
    uint16_t DR = (instruction >> 9) & 0x0007; // Destination Register
    uint16_t pcOffset = SignExtend(instruction & 0x01FF, 9);

    // Load the value from memory at the calculated address into the destination register
    registers[DR] = memoryIO->Read(registers[Registers::R_PC] + pcOffset);

    // Update the condition flags based on the value loaded into the destination register
    UpdateFlags(DR);
}


/**
 * @brief Performs a load from base register with offset operation based on the provided instruction.
 * @param instruction: The 16-bit instruction.
 */
void ArithmeticLogicUnit::LDR(uint16_t instruction)
{
    // Extract Destination Register (DR), Base Register (SR1), and Offset
    uint16_t DR = (instruction >> 9) & 0x0007; // Destination Register
    uint16_t SR1 = (instruction >> 6) & 0x0007; // Base Register
    uint16_t offset = SignExtend(instruction & 0x003F, 6); // Offset

    // Load the value from memory at the address calculated by adding base register value and offset
    registers[DR] = memoryIO->Read(registers[SR1] + offset);

    // Update the condition flags based on the value loaded into the destination register
    UpdateFlags(DR);
}


/**
 * @brief Performs a load effective address operation based on the provided instruction.
 * @param instruction: The 16-bit instruction.
 */
void ArithmeticLogicUnit::LEA(uint16_t instruction)
{
    // Extract Destination Register (DR) and PC Offset
    uint16_t DR = (instruction >> 9) & 0x0007; // Destination Register
    uint16_t pcOffset = SignExtend(instruction & 0x01FF, 9); // PC Offset

    // Calculate the effective address by adding PC value and PC offset
    registers[DR] = registers[Registers::R_PC] + pcOffset;

    // Update the condition flags based on the effective address value
    UpdateFlags(DR);
}


/**
 * @brief Performs a store operation based on the provided instruction.
 * @param instruction: The 16-bit instruction.
 */
void ArithmeticLogicUnit::ST(uint16_t instruction)
{
    // Extract Destination Register (DR) and PC Offset
    uint16_t DR = (instruction >> 9) & 0x0007; // Destination Register
    uint16_t pcOffset = SignExtend(instruction & 0x01FF, 9); // PC Offset

    // Write the value from DR register to memory at the address calculated by adding PC value and PC offset
    memoryIO->Write(registers[Registers::R_PC] + pcOffset, registers[DR]);
}


/**
 * @brief Performs an indirect store operation based on the provided instruction.
 * @param instruction: The 16-bit instruction.
 */
void ArithmeticLogicUnit::STI(uint16_t instruction)
{
    // Extract Destination Register (DR) and PC Offset
    uint16_t DR = (instruction >> 9) & 0x0007; // Destination Register
    uint16_t pcOffset = SignExtend(instruction & 0x01FF, 9); // PC Offset

    // Perform an indirect store by first reading the memory at the address calculated by adding PC value and PC offset,
    // then storing the value from DR register to the memory location read.
    memoryIO->Write(memoryIO->Read(registers[Registers::R_PC] + pcOffset), registers[DR]);
}


/**
 * @brief Performs a store register operation based on the provided instruction.
 * @param instruction: The 16-bit instruction.
 */
void ArithmeticLogicUnit::STR(uint16_t instruction)
{
    // Extract Destination Register (DR), Base Register (SR1), and Offset
    uint16_t DR = (instruction >> 9) & 0x0007; // Destination Register
    uint16_t SR1 = (instruction >> 6) & 0x0007; // Base Register
    uint16_t offset = SignExtend(instruction & 0x003F, 6); // Offset

    // Write the value from DR register to memory at the address calculated by adding SR1 register value and offset
    memoryIO->Write(registers[SR1] + offset, registers[DR]);
}


/**
 * @brief Performs a load indirect operation based on the provided instruction.
 * @param instruction: The 16-bit instruction.
 */
void ArithmeticLogicUnit::LDI(uint16_t instruction)
{
    // Extract Destination Register (DR) and PC Offset
    uint16_t DR = (instruction >> 9) & 0x0007; // Destination Register
    uint16_t pc_offset = SignExtend(instruction & 0x01FF, 9); // PC Offset

    // Calculate the effective address by adding PC value and PC offset,
    // then load the value from the memory location pointed by the calculated address into the destination register
    registers[DR] = memoryIO->Read(memoryIO->Read(registers[Registers::R_PC] + pc_offset));

    // Update the condition flags based on the value loaded into the destination register
    UpdateFlags(DR);
}