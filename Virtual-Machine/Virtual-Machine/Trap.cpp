#include "Trap.h"
#include "CPU.h"


Trap::Trap(uint16_t* memory, uint16_t* registers, CPU* cpu)
{
    memoryPtr = memory;
    registersPtr = registers;
    cpuPtr = cpu;
}


/**
 * @brief Executes 16 bits of instruction by handling different trap vectors.
 * This function processes trap instructions by switching based on the trap vector
 * and invoking corresponding trap handling functions.
 * @param cpu: Reference to the CPU object used to control the virtual machine's operation.
 * @param instruction: The 16 bits of instruction to execute.
 */
void Trap::Proxy(uint16_t instruction)
{
    // Save the return address
    registersPtr[Registers::R_7] = registersPtr[Registers::R_PC];

    // Switch based on the trap vector
    switch (instruction & 0x00FF)
    {
    case TRAP_GETC:
        GETC(); // Handle GETC trap
        break;
    case TRAP_OUT:
        OUTC(); // Handle OUTC trap
        break;
    case TRAP_PUTS:
        PUTS(); // Handle PUTS trap
        break;
    case TRAP_IN:
        INC(); // Handle INC trap
        break;
    case TRAP_PUTSP:
        PUTSP(); // Handle PUTSP trap
        break;
    case TRAP_HALT:
        HALT(); // Handle HALT trap
        break;
    }
}

/**
 * @brief Reads a character from the console and stores it in register R0.
 * This function prompts the user to enter a character from the console
 * and stores the ASCII value of the character in register R0.
 * @param cpu: Reference to the CPU object used to control the virtual machine's operation.
 */
void Trap::GETC()
{
    // Read character from console
    registersPtr[Registers::R_0] = (uint16_t)getchar();
    // Update condition flags based on the result
    cpuPtr->UpdateFlags(Registers::R_0);
}

/**
 * @brief Outputs a character stored in register R0 to the console.
 * This function outputs the character stored in register R0 to the console.
 * @param cpu Reference to the CPU object used to control the virtual machine's operation.
 */
void Trap::OUTC()
{
    // Output character to console
    putc((char)registersPtr[Registers::R_0], stdout);
    // Flush output buffer to ensure immediate display
    fflush(stdout);
}

/**
 * @brief Outputs a string stored in memory starting at the address in register R0 to the console.
 * This function outputs a null-terminated string stored in memory
 * starting at the address specified in register R0 to the console.
 * @param cpu Reference to the CPU object used to control the virtual machine's operation.
 */
void Trap::PUTS()
{
    // Iterate through memory and output characters until null terminator is encountered
    uint16_t* c = memoryPtr + registersPtr[Registers::R_0];
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
void Trap::INC()
{
    printf("Enter a character: ");

    // Read character from console
    char c = getchar();
    // Output character to console
    putc(c, stdout);
    // Flush output buffer to ensure immediate display
    fflush(stdout);
    // Store ASCII value of character in register R0
    registersPtr[Registers::R_0] = (uint16_t)c;
    // Update condition flags based on the result
    cpuPtr->UpdateFlags(Registers::R_0);
}

/**
 * @brief Outputs a string stored in memory starting at the address in register R0 to the console.
 * This function outputs a null-terminated string stored in memory
 * starting at the address specified in register R0 to the console.
 * @param cpu Reference to the CPU object used to control the virtual machine's operation.
 */
void Trap::PUTSP()
{
    // Iterate through memory and output characters until null terminator is encountered
    uint16_t* c = memoryPtr + registersPtr[Registers::R_0];
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
void Trap::HALT()
{
    puts("HALT");
    // Flush output buffer to ensure immediate display
    fflush(stdout);
    // Set 'running' flag to false to halt execution
    cpuPtr->running = 0;
}
