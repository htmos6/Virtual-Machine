#ifndef ALU_H
#define ALU_H


#include <cstdint>
#include "CPU.h"


class ArithmeticLogicUnit : public CPU
{
	public:
        ArithmeticLogicUnit();
        
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