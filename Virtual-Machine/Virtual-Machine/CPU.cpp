#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>


#include "Trap.h"
#include "MemoryIO.h"
#include "ArithmeticLogicUnit.h"
#include "CPU.h"


CPU::CPU()
{
	// Since exactly one condition flag is settled at any given time, set the Z flag.
	registers[Registers::R_COND] = ConditionFlags::FL_ZERO;

	// Set the PC to starting position
	// PC_START = 0x3000 is the default
	registers[Registers::R_PC] = 0x3000;
}
void CPU::RunVM(int argc, const char* argv[])
{
    // Check if at least one image file is provided as a command-line argument
    if (argc < 2)
    {
        // Display usage information and exit if no image files are provided
        printf("lc3 [image-file1] ...\n");
        exit(2);
    }

    // Iterate over command-line arguments (excluding the program name)
    for (int j = 1; j < argc; ++j)
    {
        // Attempt to read the image file specified by the current command-line argument
        if (!ReadImage(argv[j]))
        {
            // Print error message if image file cannot be loaded and exit with error code 1
            printf("failed to load image: %s\n", argv[j]);
            exit(1);
        }
    }

    // Set up a signal handler for interrupt signal (Ctrl+C)
    signal(SIGINT, HandleInterrupt);

    // Disable input buffering to allow direct console input
    DisableInputBuffering();

    while (this->running)
	{
		// Fetch Instruction. Read the memory location pointed by program counter.
		uint16_t instruction = memoryIO->Read(registers[Registers::R_PC]++);

        // Extract the opcode from the instruction by considering bits [15:12]
        uint16_t operation = (instruction >> 12);

		switch (operation)
		{
            case OP_ADD:
                arithmeticLogicUnit->ADD(instruction);
                break;
            case OP_AND:
                arithmeticLogicUnit->AND(instruction);
                break;
            case OP_NOT:
                arithmeticLogicUnit->NOT(instruction);
                break;
            case OP_BR:
                arithmeticLogicUnit->BR(instruction);
                break;
            case OP_JMP:
                arithmeticLogicUnit->JMP(instruction);
                break;
            case OP_JSR:
                arithmeticLogicUnit->JSR(instruction);
                break;
            case OP_LD:
                arithmeticLogicUnit->LD(instruction);
                break;
            case OP_LDI:
                arithmeticLogicUnit->LDI(instruction);
                break;
            case OP_LDR:
                arithmeticLogicUnit->LDR(instruction);
                break;
            case OP_LEA:
                arithmeticLogicUnit->LEA(instruction);
                break;
            case OP_ST:
                arithmeticLogicUnit->ST(instruction);
                break;
            case OP_STI:
                arithmeticLogicUnit->STI(instruction);
                break;
            case OP_STR:
                arithmeticLogicUnit->STR(instruction);
                break;
            case OP_TRAP:
                trap->Proxy(*this, instruction);
                break;
            case OP_RES:
            case OP_RTI:
            default:
                abort();
                break;
		}

        RestoreInputBuffering();
	}
}


/**
 * @brief Retrieves the value of the program counter (PC).
 * @return The value of the program counter.
 */
uint16_t CPU::GetProgramCounter() const
{
    return registers[Registers::R_PC];
}


/**
 * @brief Sign extends a given immediate number.
 * @param immNumber: The immediate number to sign extend.
 * @param immNumberLength: The length of the immediate number in bits.
 * @return The sign extended immediate number.
 */
uint16_t CPU::SignExtend(uint16_t immNumber, int immNumberLength) const
{
    // Determine the sign bit of the immediate number
    int signBit = (immNumber >> (immNumberLength - 1)) & 0x0001;

    // If the sign bit is set, perform sign extension
    if (signBit)
    {
        return immNumber | (0xFFFF << immNumberLength);
    }

    // If sign bit is zero (0), return the original number
    return immNumber;
}


/**
 * @brief Updates the condition flags based on the value in the specified register.
 * @param DR: The index of the destination register whose value is used to update the flags.
 */
void CPU::UpdateFlags(uint16_t DR)
{
    if (registers[DR] == 0)
    {
        // Set zero flag if the value in the register is zero
        registers[Registers::R_COND] = ConditionFlags::FL_ZERO;
    }
    // Check if the leftmost bit indicates negative for a 16-bit number
    else if ((registers[DR] >> 15) == 1)
    {
        // Set negative flag if the leftmost bit is 1
        registers[Registers::R_COND] = ConditionFlags::FL_NEGATIVE;
    }
    else
    {
        // Set positive flag if neither zero nor negative conditions are met
        registers[Registers::R_COND] = ConditionFlags::FL_POSITIVE;
    }
}


/**
 * @brief Disables input buffering for console input.
 * This function modifies the console input mode to disable input echo and to return
 * when one or more characters are available.
 * Input buffering refers to the process of collecting input characters before
 * they are sent to the application, allowing for the processing of multiple characters
 * at once. Disabling buffering allows for immediate response to user input.
 */
void CPU::DisableInputBuffering()
{
    // Get the handle for standard input
    hStdin = GetStdHandle(STD_INPUT_HANDLE);

    // Save the current console input mode
    GetConsoleMode(hStdin, &fdwOldMode);

    // Calculate the new console input mode by toggling flags
    // Disabling ENABLE_ECHO_INPUT prevents characters from being echoed (reflecting) back to the screen
    // Disabling ENABLE_LINE_INPUT allows input functions to return immediately when
    // characters are available for reading, rather than waiting for the Enter key.
    //
    // Example: In a game where ENABLE_LINE_INPUT is used, pressing 'A' by the player
    // triggers an immediate action such as moving the character to the left.
    fdwMode = fdwOldMode
        ^ ENABLE_ECHO_INPUT  // Disable input echo
        ^ ENABLE_LINE_INPUT; // Enable immediate return when characters are available

    // Apply the new console input mode
    SetConsoleMode(hStdin, fdwMode);

    // Clear the input buffer to discard any pending input
    // This ensures that any characters typed before changing the mode
    // are not processed by the application
    FlushConsoleInputBuffer(hStdin);
}


/**
 * @brief Restores the previous input buffering settings.
 * This function sets the console input mode back to the old mode.
 */
void CPU::RestoreInputBuffering()
{
    SetConsoleMode(hStdin, fdwOldMode);
}


/**
 * @brief Checks if a key is pressed within a specified timeout.
 * @return True if a key is pressed within the timeout, false otherwise.
 */
uint16_t CPU::CheckKey()
{
    return WaitForSingleObject(hStdin, 1000) == WAIT_OBJECT_0 && _kbhit();
}


/**
 * @brief Handles an interrupt signal.
 * This function restores the input buffering settings, prints a newline character,
 * and exits the program with a specific exit code (-2).
 */
static void CPU::HandleInterrupt(int signal)
{
    RestoreInputBuffering(); // Restore input buffering settings
    printf("\n"); // Print newline character
    exit(-2); // Exit program with exit code -2
}


/**
 * @brief Swaps the byte order of a 16-bit value.
 * This function reverses the byte order of the input 16-bit value.
 * @param number: The 16-bit value to be swapped.
 * @return The 16-bit value with its byte order swapped.
 */
uint16_t CPU::Swap16(uint16_t number)
{
    return (number << 8) | (number >> 8);
}


/**
 * @brief Reads the contents of an image file and stores contents in the memory array.
 * This function reads the contents of the image file pointed to by the given file pointer
 * and stores them in memory, starting from the specified origin.
 * @param file: Pointer to the image file to be read.
 */
void CPU::ReadImageFile(FILE* file)
{
    // Declare a variable to store the origin, which indicates the starting address in memory to store the image
    uint16_t origin;

    // Read the origin value from the image file into the variable "origin"
    fread(&origin, sizeof(origin), 1, file);

    // Convert origin to little endian format
    origin = Swap16(origin);

    // The maximum file size is known, allowing for a single fread operation.
    // With the predetermined limit, the file size will not exceed this value.
    // Thus, a single file read operation is sufficient.
    uint16_t maxRead = MEMORY_MAX - origin;

    // Pointer to the memory location to start reading from
    uint16_t* p = memory + origin; 

    // Read the contents of the file into memory
    size_t read = fread(p, sizeof(uint16_t), maxRead, file);

    while (read-- > 0)
    {
        // Convert each read value to little endian format
        *p = Swap16(*p);
        ; // Move to the next memory location
        ++p;
    }
}


/**
 * @brief Opens the specified image file and reads its contents into memory.
 * @param imagePath: The path to the image file to be read.
 * @return Returns 1 if the image file was successfully read into memory, 0 otherwise.
 */
int CPU::ReadImage(const char* imagePath)
{
    // Open the image file specified by the imagePath in binary read mode ("rb") and store the file pointer in "file"
    FILE* file = fopen(imagePath, "rb");

    // Check if the file was successfully opened. If not, return 0 to indicate failure.
    if (!file) { return 0; };

    // Call the ReadImageFile function to read the contents of the image file into memory
    ReadImageFile(file);

    // Close the file stream to release resources
    fclose(file);

    // Return 1 to indicate that the image file was successfully read into memory
    return 1;

}