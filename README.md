# Virtual Memory Management Simulator

## Background

Virtual Memory Management Simulator is a program that simulates virtual memory management and CPU read/write operations. It provides a mechanism for managing memory using a page table and simulating the movement of pages between RAM and a swap file. The simulator helps in understanding the concepts and algorithms involved in virtual memory management.

## Description

The Virtual Memory Management Simulator is designed to provide a practical understanding of virtual memory management concepts. It simulates a page table structure to manage virtual memory. Each entry in the page table contains information such as page validity, dirty status, permissions, frame number in RAM, and swap file index.
## Usage

To use the Virtual Memory Management Simulator, follow these steps:

1. Compile the program using the provided files:
   - `sim_mem.h`
   - `sim_mem.cpp`
   - `main.cpp`

2. Execute the compiled program.

3. Use the available functions and operations to simulate virtual memory management and CPU read/write operations. These functions include:
   - `swapPage()`: This function allows you to manage the availability of pages in the swap file. It takes an array of values (0 or 1) with a length equal to the number of pages in the swap file. It keeps track of available places in the swap file, marking them as either empty or full.

The simulator allows you to explore the movement of pages between RAM and the swap file. You can track the status of pages, identify which pages are currently in RAM and which are swapped out to the swap file. This can help in visualizing and understanding how virtual memory management algorithms work.

## Program Files

- `sim_mem.h`: This header file contains the necessary declarations and definitions for the Virtual Memory Management Simulator.
- `sim_mem.cpp`: This source file contains the implementation of the Virtual Memory Management Simulator functions.
- `main.cpp`: This source file includes the main function to run the Virtual Memory Management Simulator.


