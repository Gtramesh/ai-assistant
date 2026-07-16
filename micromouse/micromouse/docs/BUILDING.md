# Build Instructions

## Prerequisites

### Development Environment

**Linux (Ubuntu/Debian):**
```bash
sudo apt update
sudo apt install -y \
    cmake \
    gcc-arm-none-eabi \
    libnewlib-arm-none-eabi \
    gdb-multiarch \
    openocd \
    python3-pip
```

**macOS:**
```bash
brew install cmake
brew install --cask gcc-arm-embedded
brew install openocd
pip3 install pyocd
```

**Windows:**
- Install [STM32CubeIDE](https://www.st.com/stm32cubeide)
- Or install [ARM GCC toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)
- Add to PATH

### Python Dependencies

```bash
pip install -r requirements.txt
```

## Building

### Option 1: CMake (Recommended)

```bash
# Create build directory
mkdir build
cd build

# Configure
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/arm-none-eabi.cmake ..

# Build
make -j4

# Output files
# firmware.elf  - Debug symbols
# firmware.bin  - Binary for flashing
# firmware.hex  - Intel HEX format
```

### Option 2: STM32CubeIDE

1. Import project: File → Import → Existing Projects
2. Select `micromouse/` directory
3. Build: Project → Build All

### Option 3: Makefile (Legacy)

```bash
make clean
make all
```

## Flashing

### Using ST-Link

```bash
# Install stlink tools
# Linux: sudo apt install stlink-tools
# macOS: brew install stlink

# Flash firmware
st-flash write build/firmware.bin 0x8000000

# Or using OpenOCD
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
    -c "program build/firmware.elf verify reset exit"
```

### Using DFU (USB Bootloader)

1. Set BOOT0 pin HIGH, reset MCU
2. Run:
```bash
dfu-util -a 0 -D build/firmware.bin
```

### Using Serial (UART)

```bash
# Using stm32flash
stm32flash -w build/firmware.bin -v /dev/ttyUSB0
```

## Configuration

### Robot Parameters

Edit `config/robot.json`:

```json
{
    "wheels": {
        "diameter": 25.0,
        "base": 80.0
    },
    "motors": {
        "max_speed": 1000,
        "acceleration": 500
    },
    "sensors": {
        "front_threshold": 150,
        "side_threshold": 100
    }
}
```

### Algorithm Selection

Edit `src/utils/config.h`:

```c
#define ALGORITHM_FLOOD_FILL    1
#define ALGORITHM_ASTAR         0
#define ALGORITHM_WALL_FOLLOW   0
```

## Testing

### Unit Tests

```bash
# Build tests
mkdir build_test
cd build_test
cmake -DTESTING=ON ..
make

# Run tests
./tests/test_maze
./tests/test_algorithm
```

### Hardware-in-the-Loop

```bash
# Connect MCU via ST-Link
make hil_test
```

### Simulation

```bash
cd simulator
python3 sim.py --maze ../config/maze.json --algorithm flood_fill
```

## Debugging

### GDB with OpenOCD

```bash
# Terminal 1: Start OpenOCD
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg

# Terminal 2: Connect GDB
arm-none-eabi-gdb build/firmware.elf
(gdb) target remote :3333
(gdb) load
(gdb) break main
(gdb) continue
```

### Serial Debug Output

```c
// In code
printf("Debug: x=%d, y=%d\n", x, y);

// Monitor
screen /dev/ttyUSB0 115200
# or
minicom -D /dev/ttyUSB0 -b 115200
```

## Troubleshooting

### Common Issues

| Issue | Solution |
|-------|----------|
| CMake can't find toolchain | Set `-DCMAKE_TOOLCHAIN_FILE` path |
| Flash fails | Check BOOT0 pin, try different interface |
| No serial output | Verify baud rate, check TX/RX wires |
| Motors don't move | Check driver connections, verify PWM |
| Sensors read 0 | Check power, verify analog connections |

### Build Errors

```
Error: arm-none-eabi-gcc not found
Solution: Add toolchain to PATH or set CMAKE_C_COMPILER

Error: undefined reference to `_sbrk'
Solution: Add --specs=nano.specs to linker flags

Error: section '.text' will not fit in region 'FLASH'
Solution: Reduce code size or use larger MCU
```

## Optimization

### Compile Optimization

```bash
# Debug build (for development)
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release build (for competition)
cmake -DCMAKE_BUILD_TYPE=Release ..
```

### Size Optimization

```c
// In CMakeLists.txt
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -ffunction-sections -fdata-sections")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")
```

## CI/CD

### GitHub Actions

```yaml
# .github/workflows/build.yml
name: Build
on: [push, pull_request]
jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - name: Install toolchain
        run: sudo apt install gcc-arm-none-eabi cmake
      - name: Build
        run: |
          mkdir build && cd build
          cmake ..
          make -j4
      - name: Run tests
        run: ./build/tests/test_all
```
