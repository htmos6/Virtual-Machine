#include "MemoryIO.h"
#include "CPU.h"
#include "OS.h"


MemoryIO::MemoryIO(uint16_t* memory, OS* os)
{
    memoryPtr = memory;
    osPtr = os;
}


/**
 * @brief Reads the 16 bits value from memory at the specified address.
 * @param memoryAddress: The address to read from.
 * @return The 16 bits value read from memory.
 */
uint16_t MemoryIO::Read(uint16_t memoryAddress)
{
    // Check if the memory address corresponds to the keyboard status register
    if (memoryAddress == MemoryMappedRegisters::MR_KBSR)
    {
        // If a key is pressed, set the keyboard status register's most significant bit (bit 15) to indicate input
        if (osPtr->CheckKey())
        {
            memoryPtr[MemoryMappedRegisters::MR_KBSR] = (1 << 15);
            // Read the character from the keyboard and store it in the keyboard data register
            memoryPtr[MemoryMappedRegisters::MR_KBDR] = getchar();
        }
        else
        {
            // If no key is pressed, clear the keyboard status register
            memoryPtr[MemoryMappedRegisters::MR_KBSR] = 0;
        }
    }

    // Return the value stored in memory at the specified address
    return memoryPtr[memoryAddress];
}


/**
 * @brief Writes the 16-bit value to memory at the specified address.
 * @param address: The address to write to.
 * @param value: The 16-bit value to write.
 */
void MemoryIO::Write(uint16_t address, uint16_t value)
{
    memoryPtr[address] = value;
}