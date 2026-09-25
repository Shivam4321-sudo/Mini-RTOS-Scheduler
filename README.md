# Mini RTOS Scheduler

A C++ based simulation of basic RTOS scheduling and synchronization concepts.

## About

I built this project to understand how an RTOS manages multiple tasks, allocates CPU time, handles task states, and manages shared resources.

The project simulates task scheduling, timer ticks, context switching, Mutex, Semaphore, and basic scheduler performance metrics.

> This is a software simulation for learning RTOS concepts. It is not a real RTOS kernel.

---

## Features

- Task creation and management
- Task states
- Ready Queue
- Round Robin Scheduling
- Priority Scheduling
- Time Quantum
- Timer Tick Simulation
- Context Switching Simulation
- Mutex
- Semaphore
- Resource Ownership
- BLOCKED State
- Task Wake-up
- Scheduling Performance Analysis

---

## How to Schedule task

```text
Tasks
  |
  v
Ready Queue
  |
  v
Scheduler
  |
  +------------------+
  |                  |
  v                  v
Round Robin       Priority
  |                  |
  +--------+---------+
           |
           v
      Task Execution
           |
           v
      Timer Tick
           |
           v
   Context Switching
