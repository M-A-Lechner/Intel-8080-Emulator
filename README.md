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

## Useage

After building the program, you can start it by typing `./{program_name}` in your terminal. The program will automatically look for a binary file to read instructions from, and will execute the instructions it finds within.

### Creating a binary file

The binary file must only contain instructions that the processor can read. There are many resources online which describe the operation codes that the processor will understand. At the moment, the binary file must have the filename `test_binary.bin`. The program will fill its internal memory, with an address space of two bytes, with these instructions. If more instructions are loaded than fit into this address space, the program will crash. All programs must end with a `halt`instruction.

In the future, I plan to implement a simple assembly interpreter, to more easily create programs for the processor.

## Implemented instructions

Please check this chapter to see which instructions are functional, even though an instruction might be listed in "opcodes.h", it may not be implemented yet.\
Instructions that have a "not tested" marker, may seem work as expected, however their behaviour regarding reading memory and updating flags have not been thoroughly tested.

- [x] MOV (not tested)
- [x] INR (not tested)
- [x] DCR (not tested)
- [x] NOP (not tested)
- [x] LDAX (not tested)
- [x] STAX (not tested)
- [x] ADD (not tested)
- [x] ADC (not tested)
- [x] SUB (partially tested)
- [x] SBB (not tested)
- [x] ANA (partially tested)
- [x] XRA (not tested)