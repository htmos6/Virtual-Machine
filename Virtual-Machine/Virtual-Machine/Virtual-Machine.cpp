// Virtual-Machine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "CPU.h"

int main(int argc, const char* argv[])
{
    CPU cpu;
    cpu.memory[0x3000] = 0x15FB;
    cpu.registers[R_7] = 8;

    // cpu.memory[0x3000] = 0x15FB; ADD RO, R4, R5
    // cpu.memory[0x3000] = 0x1105; ADD R2, R7, -5

    cpu.RunVM();
}
















// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
