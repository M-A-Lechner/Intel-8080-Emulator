# Intel 8080 Emulator

## Project description

Program for emulating the Intel 8080 processor.\
The goal of this project is to have a working emulator for this processor, which behaves the same as the original processor.

Future goals would be to implement a compiler, so that assembly code can be written directly for the processor. A way to run CP/M operating systems, and ideally for this emulator to be used to emulate a real computer, like the Altair 8800.\
A version of this emulator for RaspberryPi Zero and ESP32 are also planned.

## How to build this program

Currently all code has only been built and tested in Linux, so I cannot guarantee that it will build on any other operating system.\
I plan, however to support Windows and MacOS in the future.

**This project will only build with versions >= C++20.**

There are no prebuilt options of this project available at this moment, so you will have to build the project yourself.\
This is possible with the included makefile or with GCC. Other ways of building have not been tested yet, so do so at your own risk!

### Build with Makefile

A Makefile has been included in the root directory. The Makefile will build the project automatically with the C++20 standard library. You can change this manually to a higher version if you so desire, however a lower version will break the code.

Simply open your console in the root directory of this project and enter `make`. At the moment make will automatically build a `test_runner` to check if everything is working correctly, then build the main program, and lastly delete all temporary files, including all `*.o` files. If you wish to only do one of these things, simply append `make` with `test_runner`, `main.out` or `clean`.

## Useage

After building the program, you can start it by typing `./main.out "{path to file}` in your terminal. The program will automatically read the specified binary file into memory, and will execute the instructions it finds within (obviously if you rename main.out, you will have to run the renamed file).

### Arguments

At the moment this program will recognize three arguments:

| Argument | Description |
|----------|-------------|
| -h --help | Shows some relevant information about the program. |
| -f --file \<file> | This argument is the same as just specifying a file when starting the program. Therefore it is only necessary to use this flag if other flags are used in the command. |
| -d --debug | Enables debug mode. This will enable you to see every instruction the emulator executed and what the state of the registers and flags were at each point. Can be exported out to a file, with "\>\> \<file>" (if using Linux cmd).

### Creating a binary file

The binary file must only contain instructions that the processor can read. There are many resources online which describe the operation codes that the processor will understand. The program will fill its internal memory, with an address space of two bytes, with these instructions. If more instructions are loaded than fit into this address space, the program will crash. All programs must end with a `halt` instruction, if you do not wish them to run forever.

If you wish to write a program for the emulator, please look for an editor and compiler which supports this version of assembly. I have not yet looked for any options, so I cannot recommend any.

For writing small programs you can just use a hex-editor, like Okteta. The following site will help you find the hexcodes for the instructions you want to use.

- [https://grantmestrength.github.io/RetroComputerInstructionManual/intel8080.html](https://grantmestrength.github.io/RetroComputerInstructionManual/intel8080.html)

In the future, I plan to implement a simple assembly interpreter, to more easily create programs for the processor.

## Implemented instructions

**~ 61%** of all 256 instructions have been implemented.

Please check this chapter to see which instructions are functional, even though an instruction might be listed in "opcodes.h", it may not be implemented yet.\
Instructions that have a "not tested" marker, may seem work as expected, however their behaviour regarding reading memory and updating flags have not been thoroughly tested.

- [x] MOV (not tested)
- [x] INR (not tested)
- [x] DCR (not tested)
- [x] NOP (not tested)
- [x] LDAX (partially tested)
- [x] STAX (not tested)
- [x] ADD (not tested)
- [x] ADC (not tested)
- [x] SUB (partially tested)
- [x] SBB (not tested)
- [x] ANA (partially tested)
- [x] XRA (not tested)
- [x] ORA (not tested)
- [x] CMP (not tested)
- [x] CMA (not tested)