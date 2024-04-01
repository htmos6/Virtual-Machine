#ifndef MEMORYIO_H
#define MEMORYIO_H


#include <cstdint>


class OS;


class MemoryIO
{
private:
	uint16_t* memoryPtr;
	OS* osPtr;

public:
	MemoryIO(uint16_t* memory, OS* os);

	uint16_t Read(uint16_t memoryAddress);
	void Write(uint16_t address, uint16_t value);
};
#endif