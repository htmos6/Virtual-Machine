#ifndef ALU_H
#define ALU_H


#include <cstdint>


class MemoryIO;
class CPU;


class ArithmeticLogicUnit
{
private:
    uint16_t* memoryPtr;
    uint16_t* registersPtr;
    MemoryIO* memoryIOPtr;
    CPU* cpuPtr;

public:
    ArithmeticLogicUnit(uint16_t* memory, uint16_t* registers, MemoryIO* memoryIO, CPU* cpu);

    uint16_t SignExtend(uint16_t immNumber, int immNumberLength) const;
    uint16_t Swap16(uint16_t number);

    void ADD(uint16_t instruction);
    void AND(uint16_t instruction);
    void NOT(uint16_t instruction);

    void BR(uint16_t instruction);
    void JMP(uint16_t instruction);
    void JSR(uint16_t instruction);

    void LD(uint16_t instruction);
    void LDR(uint16_t instruction);
    void LEA(uint16_t instruction);

    void ST(uint16_t instruction);
    void STI(uint16_t instruction);
    void STR(uint16_t instruction);

    void LDI(uint16_t instruction);
};
#endif