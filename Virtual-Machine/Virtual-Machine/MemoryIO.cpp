#include "MemoryIO.h"
#include "CPU.h"


MemoryIO::MemoryIO()
{
	;
}


/**
 * @brief Reads the 16 bits value from memory at the specified address.
 * @param memoryAddress: The address to read from.
 * @return The 16 bits value read from memory.
 */
uint16_t MemoryIO::Read(uint16_t memoryAddress) const
{
	return memory[memoryAddress];
}


/**
 * @brief Writes the 16-bit value to memory at the specified address.
 * @param address: The address to write to.
 * @param value: The 16-bit value to write.
 */
void MemoryIO::Write(uint16_t address, uint16_t value)
{
	memory[address] = value;
}