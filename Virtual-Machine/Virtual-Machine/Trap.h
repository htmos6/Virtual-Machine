#ifndef TRAP_H
#define TRAP_H

#include "CPU.h"


enum TrapCodes : uint16_t
{
    TRAP_GETC = 0x0020,  // get character from keyboard, not echoed onto the terminal
    TRAP_OUT = 0x0021,   // output a character
    TRAP_PUTS = 0x0022,  // output a word string
    TRAP_IN = 0x0023,    // get character from keyboard, echoed onto the terminal
    TRAP_PUTSP = 0x0024, // output a byte string
    TRAP_HALT = 0x0025   // halt the program
};


class Trap
{
    public:
        Trap();
        void Proxy(CPU& cpu, uint16_t instruction);
        void GETC(CPU& cpu);
        void OUT(CPU& cpu);
        void PUTS(CPU& cpu);
        void IN(CPU& cpu);
        void PUTSP(CPU& cpu);
        void HALT(CPU& cpu);
};
#endif