#include "Trap.h"
#include "CPU.h"


Trap::Trap()
{
    ;
}


/**
 * @brief Executes 16 bits of instruction by handling different trap vectors.
 * This function processes trap instructions by switching based on the trap vector
 * and invoking corresponding trap handling functions.
 * @param cpu: Reference to the CPU object used to control the virtual machine's operation.
 * @param instruction: The 16 bits of instruction to execute.
 */
void Trap::Proxy(CPU& cpu, uint16_t instruction)
{
    // Save the return address
    cpu.registers[Registers::R_7] = cpu.registers[Registers::R_PC];

    // Switch based on the trap vector
    switch (instruction & 0x00FF)
    {
    case TRAP_GETC:
        GETC(cpu); // Handle GETC trap
        break;
    case TRAP_OUT:
        OUTC(cpu); // Handle OUTC trap
        break;
    case TRAP_PUTS:
        PUTS(cpu); // Handle PUTS trap
        break;
    case TRAP_IN:
        INC(cpu); // Handle INC trap
        break;
    case TRAP_PUTSP:
        PUTSP(cpu); // Handle PUTSP trap
        break;
    case TRAP_HALT:
        HALT(cpu); // Handle HALT trap
        break;
    }
}

/**
 * @brief Reads a character from the console and stores it in register R0.
 * This function prompts the user to enter a character from the console
 * and stores the ASCII value of the character in register R0.
 * @param cpu: Reference to the CPU object used to control the virtual machine's operation.
 */
void Trap::GETC(CPU& cpu)
{
    // Read character from console
    cpu.registers[Registers::R_0] = (uint16_t)getchar();
    // Update condition flags based on the result
    cpu.UpdateFlags(Registers::R_0);
}

/**
 * @brief Outputs a character stored in register R0 to the console.
 * This function outputs the character stored in register R0 to the console.
 * @param cpu Reference to the CPU object used to control the virtual machine's operation.
 */
void Trap::OUTC(CPU& cpu)
{
    // Output character to console
    putc((char)cpu.registers[Registers::R_0], stdout);
    // Flush output buffer to ensure immediate display
    fflush(stdout);
}

/**
 * @brief Outputs a string stored in memory starting at the address in register R0 to the console.
 * This function outputs a null-terminated string stored in memory
 * starting at the address specified in register R0 to the console.
 * @param cpu Reference to the CPU object used to control the virtual machine's operation.
 */
void Trap::PUTS(CPU& cpu)
{
    // Iterate through memory and output characters until null terminator is encountered
    uint16_t* c = cpu.memory + cpu.registers[Registers::R_0];
    while (*c)
    {
        // Output character to console
        putc((char)*c, stdout); 
        // Move to the next character in memory
        ++c; 
    }
    // Flush output buffer to ensure immediate display
    fflush(stdout); 
}

/**
 * @brief Reads a character from the console and stores it in register R0.
 * This function prompts the user to enter a character from the console
 * and stores the ASCII value of the character in register R0.
 * @param cpu Reference to the CPU object used to control the virtual machine's operation.
 */
void Trap::INC(CPU& cpu)
{
    printf("Enter a character: ");

    // Read character from console
    char c = getchar();
    // Output character to console
    putc(c, stdout);
    // Flush output buffer to ensure immediate display
    fflush(stdout);
    // Store ASCII value of character in register R0
    cpu.registers[Registers::R_0] = (uint16_t)c;
    // Update condition flags based on the result
    cpu.UpdateFlags(Registers::R_0);
}

/**
 * @brief Outputs a string stored in memory starting at the address in register R0 to the console.
 * This function outputs a null-terminated string stored in memory
 * starting at the address specified in register R0 to the console.
 * @param cpu Reference to the CPU object used to control the virtual machine's operation.
 */
void Trap::PUTSP(CPU& cpu)
{
    // Iterate through memory and output characters until null terminator is encountered
    uint16_t* c = cpu.memory + cpu.registers[Registers::R_0];
    while (*c)
    {
        // Extract lower 8 bits of the word
        char char1 = (*c) & 0x00FF;
        // Output lower byte to console
        putc(char1, stdout);
        // Extract upper 8 bits of the word
        char char2 = (*c) >> 8;
        // Output upper byte to console if not null
        if (char2) putc(char2, stdout);
        // Move to the next word in memory
        ++c;
    }

    // Flush output buffer to ensure immediate display
    fflush(stdout);
}

/**
 * @brief Halts the execution of the virtual machine.
 * This function prints "HALT" to the console and sets the 'running' flag of the CPU object to false,
 * indicating that the virtual machine should stop execution.
 * @param cpu Reference to the CPU object used to control the virtual machine's operation.
 */
void Trap::HALT(CPU& cpu)
{
    puts("HALT");
    // Flush output buffer to ensure immediate display
    fflush(stdout); 
    // Set 'running' flag to false to halt execution
    cpu.running = false;
}
