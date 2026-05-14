# S-RAQMS: Secure Real-Time Air Quality Monitoring and Analysis System

## Project Overview
**S-RAQMS** is an RTOS-based embedded system utilizing internal silicon thermal and voltage characteristics to simulate and analyze environmental air quality via hardware acceleration and secure data logging. 

This project is built using a **Layered Microkernel Architecture** to separate hardware-specific drivers from high-level application logic through a middle layer of hardware accelerators.

**Author:** Kit Sun Anson Lee

---

## Hardware & Software Stack
* **Microcontroller:** Texas Instruments LP-MSPM0G3507 (ARM Cortex-M0+)
* **Operating System:** FreeRTOS
* **Development Environment:** Code Composer Studio (CCS) / Eclipse Theia
* **Compiler:** TI Arm Clang
* **Hardware Accelerators:** MATHACL (Math Accelerator), AES-128, DMA Controller

---

## Key Features
* **Real-Time Sampling:** Captures environmental fluctuations by sampling the internal ADC12 at a rate of 100Hz, triggered deterministically by a Hardware Timer (TIMA0).
* **Hardware Offloading & Math Engine:** Implements a 10-point Moving Average Filter (MAF) using the MATHACL to stabilize raw sensor data, keeping CPU utilization below 30% without relying on a floating-point unit.
* **Concurrent Task Management:** Utilizes FreeRTOS to manage three concurrent tasks with fixed-priority preemption:
  1. **Processing Task (High Priority):** Handles DMA interrupts, watchdog feeding, and mathematical filtering.
  2. **Logging Task (Medium Priority):** Executes AES-128 encryption on aggregated data every 60 seconds.
  3. **UART Task (Low Priority):** Outputs a command-line interface with the Air Quality Index, CPU temperature, and system uptime.
* **Security & Integrity:** * Air-quality logs are encrypted using the onboard AES-128 hardware accelerator.
  * An onboard Memory Protection Unit (MPU) restricts AES encryption key access to "Privileged Access Only".
  * Implements CRC-32 checksums for every data packet to prevent memory corruption.
* **Reliability:** Integrates a Windowed Watchdog Timer (WWDT) for automatic recovery from main-loop hangs, and utilizes a Circular Buffer in the 32KB SRAM to manage data lifecycle safely.

---

## Architecture details

### FreeRTOS Task Structure
The task scheduling is strictly prioritized to meet real-time constraints:
* **High Priority (3):** Processing Task - wakes from Sleep Mode via Binary Semaphore triggered by DMA completion.
* **Medium Priority (2):** Logging Task - waits on a FreeRTOS queue/notification for filtered AQI data.
* **Low Priority (1):** UART Task - waits on a Message Queue for encrypted packets to transmit.

### Memory & State Management
* **Circular Buffer:** Stores encrypted logs (up to 1024 packets). Oldest records are safely overwritten when the buffer is full.
* **Dynamic Calibration:** Calculates average ambient temperature over 5 seconds upon startup for baseline AQI zeroing.

---

## Testing Scenarios
The system is validated against rigorous functional and exceptional corner cases:
1. **ADC Saturation (CC-01):** Fixed-point math constraints handle max ADC values (4095) without integer overflow.
2. **Buffer Wrap (CC-02):** Circular buffer pointers automatically reset and overwrite sequentially.
3. **Task Starvation (EX-01):** Validated WWDT reset mechanism during simulated CPU hangs.
4. **Memory Violation (EX-02):** HardFault_Handler successfully intercepts unauthorized memory read attempts from the UART task to the AES key region.
