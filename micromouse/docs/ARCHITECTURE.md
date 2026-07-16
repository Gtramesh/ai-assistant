# System Architecture

## High-Level Overview

The Micromouse system consists of four main layers:

```
┌─────────────────────────────────────────┐
│           APPLICATION LAYER             │
│   (Maze solving, Path planning)         │
├─────────────────────────────────────────┤
│           CONTROL LAYER                 │
│   (Robot state machine, PID control)    │
├─────────────────────────────────────────┤
│           HARDWARE ABSTRACTION          │
│   (Sensors, Motors, GPIO, Timers)       │
├─────────────────────────────────────────┤
│           HARDWARE LAYER                │
│   (MCU, Sensors, Motors, Battery)       │
└─────────────────────────────────────────┘
```

## Component Diagram

```
                    ┌─────────────┐
                    │   Main      │
                    │   Loop      │
                    └──────┬──────┘
                           │
              ┌────────────┼────────────┐
              │            │            │
              ▼            ▼            ▼
        ┌──────────┐ ┌──────────┐ ┌──────────┐
        │  Maze    │ │  Robot   │ │  Sensor  │
        │  Manager │ │  State   │ │  Manager │
        │          │ │  Machine │ │          │
        └────┬─────┘ └────┬─────┘ └────┬─────┘
             │            │            │
             ▼            ▼            ▼
        ┌──────────┐ ┌──────────┐ ┌──────────┐
        │  Flood   │ │  Motor   │ │  IR/ADC  │
        │  Fill    │ │  Control │ │  Encoder │
        └──────────┘ └──────────┘ └──────────┘
```

## State Machine

### Robot States

```
┌─────────┐
│  INIT   │
└────┬────┘
     │
     ▼
┌─────────┐
│ EXPLORE │◄─────────────────┐
└────┬────┘                  │
     │                       │
     ▼                       │
┌─────────┐                  │
│  SOLVE  │──────────────────┘
└────┬────┘
     │
     ▼
┌─────────┐
│  SPEED  │
└────┬────┘
     │
     ▼
┌─────────┐
│  IDLE   │
└─────────┘
```

### State Descriptions

1. **INIT**: Initialize all hardware and variables
2. **EXPLORE**: Navigate maze, map walls, build flood values
3. **SOLVE**: Calculate optimal path using discovered maze
4. **SPEED**: Execute fastest path with optimized movements
5. **IDLE**: Stop motors, wait for restart

## Data Flow

```
Sensors ──► Sensor Manager ──► Robot State Machine
                                    │
                                    ▼
                              Maze Manager
                                    │
                                    ▼
                            Algorithm (Flood Fill)
                                    │
                                    ▼
                              Motor Commands
                                    │
                                    ▼
                              Motor Drivers
```

## Timing Requirements

| Component | Frequency | Priority |
|-----------|-----------|----------|
| Motor Control | 1 kHz | Highest |
| Sensor Reading | 100 Hz | High |
| PID Controller | 100 Hz | High |
| State Machine | 50 Hz | Medium |
| Display Update | 10 Hz | Low |
| Logging | 1 Hz | Lowest |

## Memory Layout

```
RAM (192KB)
├── Stack           (8KB)
├── Global Variables (4KB)
├── Maze Data       (1KB)
├── Path Buffer     (1KB)
├── Sensor Buffer   (2KB)
└── Free            (176KB)

FLASH (1MB)
├── Bootloader      (64KB)
├── Application     (512KB)
├── Algorithm Code  (128KB)
└── Constants/Tables (256KB)
```
