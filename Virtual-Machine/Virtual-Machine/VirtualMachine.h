#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H


#include <cstdint>


class CPU;
class OS;
class Trap;
class MemoryIO;
class ArithmeticLogicUnit;


class VirtualMachine
{
private:
	CPU* cpuPtr;
	OS* osPtr;
	Trap* trapPtr;
	MemoryIO* memoryIOPtr;
	ArithmeticLogicUnit* aluPtr;

public:
	VirtualMachine(CPU* cpu, OS* os, Trap* trap, MemoryIO* memoryIO, ArithmeticLogicUnit* alu);
	void RunVirtualMachine(int argc, const char* argv[]);
};
#endif