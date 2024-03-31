#include "OS.h"

#include <cstdio>
#include <cstdint>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
// windows only
#include <Windows.h>
#include <conio.h>  // _kbhit


OS::OS()
{
    ;
}

/**
 * @brief Disables input buffering for console input.
 * This function modifies the console input mode to disable input echo and to return
 * when one or more characters are available.
 * Input buffering refers to the process of collecting input characters before
 * they are sent to the application, allowing for the processing of multiple characters
 * at once. Disabling buffering allows for immediate response to user input.
 */
void OS::DisableInputBuffering()
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
void OS::RestoreInputBuffering()
{
    SetConsoleMode(hStdin, fdwOldMode);
}


/**
 * @brief Checks if a key is pressed within a specified timeout.
 * @return True if a key is pressed within the timeout, false otherwise.
 */
uint16_t OS::CheckKey()
{
    return WaitForSingleObject(hStdin, 1000) == WAIT_OBJECT_0 && _kbhit();
}


/**
 * @brief Handles an interrupt signal.
 * This function restores the input buffering settings, prints a newline character,
 * and exits the program with a specific exit code (-2).
 */
void OS::HandleInterrupt(int signal)
{
    RestoreInputBuffering(); // Restore input buffering settings
    printf("\n"); // Print newline character
    exit(-2); // Exit program with exit code -2
}


// Static function to act as a wrapper
void OS::HandleInterruptWrapper(int signal)
{
    OS osInstance;
    osInstance.HandleInterrupt(signal);
}