#ifndef OS_H
#define OS_H


#include "CPU.h"


class OS : public CPU
{
	public: 
        OS();
        void DisableInputBuffering();
        void RestoreInputBuffering();
        uint16_t CheckKey();
        void HandleInterrupt(int signal);
        static void HandleInterruptWrapper(int signal);
};
#endif