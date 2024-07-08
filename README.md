# LC3 Simulator Project

## Overview

This project is an LC3 (Little Computer 3) simulator that takes an assembly file as input. Initially, it parses through the code to identify labels and their corresponding addresses. Then, it translates instructions based on their opcodes into binary machine code and writes them to a binary file. The simulator then reads from this binary file during simulation to populate memory.

## Features

- **Label and Address Parsing**: Parses through assembly code to identify labels and their respective addresses.
- **Instruction Translation**: Translates instructions into binary machine code based on their opcodes.
- **Binary File Output**: Writes the translated machine code into a binary file for simulation.
- **Memory Population**: Reads from the binary file to populate memory during simulation.

## LC3 Architecture

### Components

- **Registers**: Eight general-purpose registers (R0-R7), each 16 bits wide.
- **Memory**: 65,536 memory locations, each storing a 16-bit word.
- **Program Counter (PC)**: Holds the address of the next instruction to be executed.
- **Instruction Register (IR)**: Stores the currently executing instruction.
- **Condition Codes (CC)**: Reflects the result of the last operation (negative, zero, or positive).

### Supported Instructions

- **Data Movement**: LD, LDI, LDR, LEA, ST, STR, STI
- **Arithmetic and Logic**: ADD, AND, NOT
- **Control Flow**: BR, JMP, JSR, JSRR, RET

## Instruction Cycle

The LC3 simulator follows the standard six-phase instruction cycle:

1. **Fetch**:
   - Loads the next instruction from memory into the Instruction Register (IR).
   - Copies the address in the Program Counter (PC) to the Memory Address Register (MAR).
   - Sends a "read" signal to memory and loads the data from the Memory Data Register (MDR) into the IR.
   - Increments the PC to point to the next instruction.

2. **Decode**:
   - Determines the opcode and identifies other operands.

3. **Evaluate Address**:
   - Computes the memory address required for the instruction.

4. **Fetch Operands**:
   - Retrieves the necessary operands from memory or registers.

5. **Execute**:
   - Performs the operation specified by the instruction.

6. **Store**:
   - Writes the result to the destination register or memory location.

### Usage Instructions

1. **Upload Assembly File**:
   - Press the "Upload" button to load your LC3 assembly file.

2. **Execute Instructions**:
   - Use the "Next Phase" button to proceed through each phase of the instruction cycle for each command, observing the detailed changes in memory and register states.

### Visualization

The simulator provides a detailed visualization interface that allows users to see:
- **Current Instruction**: The instruction currently being executed.
- **Register States**: The contents of all registers (R0-R7, PC, IR, CC).
- **Memory Content**: The contents of the memory locations being accessed.

### Screenshot

![LC3 Simulator](https://github.com/Z-Naderi/LC3Simulater/blob/main/LC3.png)

