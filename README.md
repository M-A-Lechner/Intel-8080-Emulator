# Intel 8080 Emulator

## Project description

Program for emulating the Intel 8080 processor.\
The goal of this project is to have a working emulator for this processor, which can take compiled code and output a result, same as the original processor.

Future goals would be to implement a compiler, so that assembly code can be written directly for the processor. A way to run CP/M operating systems, and ideally for this emulator to be used to emulate a real computer, like the Altair 8800.

## How to build this program

Currently all code has only been built and tested in Linux, so I cannot guarantee that it will build on any other operating system.\
It is planned however that support for Windows and MacOS will be added.

There are no prebuilt options of this project available at this moment, so you will have to build the project yourself. This is possible with the included makefile or with GCC. Other ways of building have not been tested yet, so do so at your own risk!

### Build with Makefile

A Makefile has been included in the root directory. Simply open your console in the root directory and enter "make". To clean any residual files from the building process enter "make clean" (this will remove all "*.cpp" and "*.o" files from the root directory, so make sure you do not have any code there you do not want to lose!).

## Implemented instructions

Please check this chapter to see which instructions are functional, even though an instruction might be listed in "opcodes.h", it may not be implemented yet.

All MOV and INR instructions have been implemented, but not thoroughly tested.

Whilst other instructions have also in some form been implemented, they are at this point not useable, since all the instructions are currently being migrated out of the processor struct.
