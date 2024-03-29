#ifndef MEMORYIO_H
#define MEMORYIO_H


#include <cstdint>
#include "CPU.h"


class MemoryIO : public CPU
{
	public:
		MemoryIO();

		uint16_t Read(uint16_t memoryAddress);
		void Write(uint16_t address, uint16_t value);
};
#endif