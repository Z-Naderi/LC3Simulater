# LC3 Simulator Project

## Overview

This project provides a simulator for the LC3 (Little Computer 3) architecture, a 16-bit computer architecture used primarily for educational purposes. The simulator reads LC3 assembly language programs, converts them into machine code, and executes them while providing a detailed visualization of the process.

## Features

- **Full Instruction Set Support**: Supports all LC3 instructions including data movement, arithmetic, logic, and control flow instructions.
- **Memory and Register Visualization**: Visual tools for monitoring memory contents and register states.
- **Step-by-Step Execution**: Allows users to execute programs one instruction at a time, ideal for learning and debugging.

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

The LC3 simulator emulates the six stages of the instruction cycle:

1. **Fetch**: 
   - Load the next instruction from memory into the Instruction Register (IR).
   - Copy the address in the PC to the Memory Address Register (MAR).
   - Send a "read" signal to memory and load the data from the Memory Data Register (MDR) into the IR.
   - Increment the PC to point to the next instruction.

2. **Decode**:
   - Determine the opcode and identify other operands.

3. **Evaluate Address**:
   - Compute the memory address required for the instruction.

4. **Fetch Operands**:
   - Retrieve the necessary operands from memory or registers.

5. **Execute**:
   - Perform the operation specified by the instruction.

6. **Store**:
   - Write the result to the destination register or memory location.

### Usage Instructions

1. **Upload Assembly File**:
   - Press the "Upload" button to load your LC3 assembly file.

2. **Assemble Code**:
   - Press the "Assemble" button to convert the assembly code into machine code. A success message will be displayed if the assembly is successful.

3. **Execute Instructions**:
   - Use the "Next Step" button to proceed through each phase of the instruction cycle for each command, observing the detailed changes in memory and register states.

### Visualization

The simulator provides a detailed visualization interface that allows users to see:
- **Current Instruction**: The instruction currently being executed.
- **Register States**: The contents of all registers (R0-R7, PC, IR, CC).
- **Memory Content**: The contents of the memory locations being accessed.

### Example Workflow

1. Load your LC3 assembly program by pressing the "Upload" button.
2. Press the "Assemble" button to translate the assembly code into binary machine code.
3. Begin execution by pressing the "Next Step" button. Each press will advance the execution by one phase of the instruction cycle, allowing you to monitor the state changes in detail.

### Screenshot

![LC3](https://github.com/Z-Naderi/LC3Simulater/blob/main/LC3.png)

