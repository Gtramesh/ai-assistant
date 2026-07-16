# Micromouse Project

A complete open-source Micromouse maze-solving robot platform with full hardware, software, and algorithm documentation.

## Overview

Micromouse is an international robotics competition where autonomous robots navigate a 16x16 maze. This project provides everything needed to build, program, and compete with your own Micromouse robot.

## Project Structure

```
micromouse/
├── README.md                    # This file
├── LICENSE                      # Open source license
├── CMakeLists.txt              # Build system
├── requirements.txt            # Python dependencies
├── src/                        # Source code
│   ├── algorithms/            # Maze solving algorithms
│   │   ├── flood_fill.h/.cpp  # Flood fill algorithm
│   │   ├── astar.h/.cpp       # A* pathfinding
│   │   └── wall_follow.h/.cpp # Wall following
│   ├── core/                  # Core robot logic
│   │   ├── maze.h/.cpp        # Maze representation
│   │   ├── robot.h/.cpp       # Robot state machine
│   │   ├── sensor.h/.cpp      # Sensor management
│   │   └── motor.h/.cpp       # Motor control
│   ├── hardware/              # Hardware abstraction
│   │   ├── gpio.h/.cpp        # GPIO interface
│   │   ├── timer.h/.cpp       # Timer/PWM
│   │   └── adc.h/.cpp         # ADC interface
│   └── utils/                 # Utility functions
│       ├── pid.h/.cpp         # PID controller
│       └── config.h           # Configuration
├── docs/                      # Documentation
│   ├── ARCHITECTURE.md        # System architecture
│   ├── ALGORITHMS.md          # Algorithm details
│   ├── HARDWARE.md            # Hardware design
│   ├── BUILDING.md            # Build instructions
│   └── COMPETITION.md         # Competition rules
├── config/                    # Configuration files
│   ├── robot.json             # Robot parameters
│   └── maze.json              # Default maze
├── tests/                     # Unit tests
├── simulator/                 # Maze simulator
├── tools/                     # Development tools
└── firmware/                  # MCU firmware
```

## Quick Start

### Prerequisites

- **Hardware**: STM32F4xx or ESP32 microcontroller
- **Software**: GCC ARM toolchain, CMake 3.20+
- **Optional**: Python 3.8+ for simulation

### Build

```bash
mkdir build && cd build
cmake ..
make
```

### Flash

```bash
# For STM32
st-flash write firmware.bin 0x8000000

# For ESP32
esptool.py --port /dev/ttyUSB0 write_flash 0x0 firmware.bin
```

## Features

- **Maze Representation**: 16x16 grid with wall tracking
- **Algorithms**: Flood fill, A*, Wall following
- **Sensors**: IR distance, wheel encoders, IMU
- **Motor Control**: PID speed control, precise turning
- **Simulation**: Desktop maze simulator for testing
- **Logging**: SD card logging for post-run analysis

## Competition Rules Summary

- Maze: 16x16 cells (180mm x 180mm each)
- Start: Center cell
- Goal: Any 2x2 area in center quadrant
- Time limit: 5 minutes for exploration
- Scoring: Fastest time to solve maze

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

## License

MIT License - see [LICENSE](LICENSE)
