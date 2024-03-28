#include "Trap.h"
#include "CPU.h"


Trap::Trap()
{
    ;
}


/**
 * @brief Executes 16 bits of instruction by handling different trap vectors.
 * @param cpu: CPU reference used to control the virtual machine's operation.
 * @param instruction: The 16 bits of instruction.
 */
void Trap::Proxy(CPU& cpu, uint16_t instruction)
{
    // Save the return address
    cpu.registers[Registers::R_7] = cpu.registers[Registers::R_PC];

    // Switch based on the trap vector
    switch (instruction & 0x00FF)
    {
        case TRAP_GETC:
            GETC(cpu);
            break;
        case TRAP_OUT:
            OUT(cpu);
            break;
        case TRAP_PUTS:
            PUTS(cpu);
            break;
        case TRAP_IN:
            IN(cpu);
            break;
        case TRAP_PUTSP:
            PUTSP(cpu);
            break;
        case TRAP_HALT:
            HALT(cpu);
            break;
    }
}


/**
 * @brief Reads a character from the console and stores it in R0.
 * @param cpu: CPU reference used to control the virtual machine's operation.
 */
void Trap::GETC(CPU& cpu)
{
    cpu.registers[Registers::R_0] = (uint16_t)getchar();
    cpu.UpdateFlags(Registers::R_0);
}


/**
 * @brief Outputs a character stored in R0 to the console.
 * @param cpu: CPU reference used to control the virtual machine's operation.
 */
void Trap::OUT(CPU& cpu)
{
    putc((char)cpu.registers[Registers::R_0], stdout);
    fflush(stdout);
}


/**
 * @brief Outputs a string stored in memory starting at the address in R0 to the console.
 * @param cpu: CPU reference used to control the virtual machine's operation.
 */
void Trap::PUTS(CPU& cpu)
{
    // Iterate through memory and output characters until null terminator is encountered
    uint16_t* c = cpu.memory + cpu.registers[Registers::R_0];
    while (*c)
    {
        putc((char)*c, stdout);
        ++c;
    }
    fflush(stdout);
}


/**
 * @brief Reads a character from the console and stores it in R0.
 * @param cpu: CPU reference used to control the virtual machine's operation.
 */
void Trap::IN(CPU& cpu)
{
    printf("Enter a character: ");
    char c = getchar();
    putc(c, stdout);
    fflush(stdout);
    cpu.registers[Registers::R_0] = (uint16_t)c;
    cpu.UpdateFlags(Registers::R_0);
}


/**
 * @brief Outputs a string stored in memory starting at the address in R0 to the console.
 * @param cpu: CPU reference used to control the virtual machine's operation.
 */
void Trap::PUTSP(CPU& cpu)
{
    // Iterate through memory and output characters until null terminator is encountered
    uint16_t* c = cpu.memory + cpu.registers[Registers::R_0];
    while (*c)
    {
        char char1 = (*c) & 0x00FF;
        putc(char1, stdout);
        char char2 = (*c) >> 8;
        if (char2) putc(char2, stdout);
        ++c;
    }
    fflush(stdout);
}


/**
 * @brief Halts the execution of the virtual machine.
 * @param cpu: CPU reference used to control the virtual machine's operation.
 */
void Trap::HALT(CPU& cpu)
{
    puts("HALT");
    fflush(stdout);
    cpu.running = false;
}
