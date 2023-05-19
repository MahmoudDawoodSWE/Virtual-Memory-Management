# Virtual Memory Management Simulator

## Description

The simulator implements a page table structure to manage virtual memory. Each entry in the page table contains information such as page validity, dirty status, permissions, frame number in RAM, and swap file index. The `swapPage()` function is used to manage the availability of pages in the swap file, marking them as either available or full.

## Background

Virtual Memory Management Simulator is a program that simulates virtual memory management and CPU read/write operations. It includes functionality for managing page tables, tracking page validity, dirty status, permissions, frame numbers in RAM, and swap file indices.

## Usage

To use the Virtual Memory Management Simulator, follow these steps:

1. Compile the program using the provided files:
   - `sim_mem.h`
   - `sim_mem.cpp`
   - `main.cpp`

2. Execute the compiled program.

3. Use the available functions and operations to simulate virtual memory management and CPU read/write operations.

## Program Files

- `sim_mem.h`: Header file for the Virtual Memory Management Simulator.
- `sim_mem.cpp`: Source file containing the implementation of the simulator.
- `main.cpp`: Source file with the main function to run the simulator.

