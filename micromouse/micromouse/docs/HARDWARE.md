# Hardware Design

## System Specifications

### Physical Dimensions

| Parameter | Value |
|-----------|-------|
| Length | 100-150 mm |
| Width | 90-100 mm |
| Height | 50-70 mm |
| Weight | 150-300 g |
| Wheel Diameter | 20-30 mm |
| Wheelbase | 70-90 mm |

### Electrical Specifications

| Parameter | Value |
|-----------|-------|
| Battery | 2S LiPo (7.4V) |
| Motor Voltage | 3.3-5V |
| Logic Voltage | 3.3V |
| Current (peak) | 2A |
| Current (typical) | 200mA |

## Block Diagram

```
                    ┌─────────────────┐
                    │   Battery       │
                    │   (2S LiPo)     │
                    └────────┬────────┘
                             │
                    ┌────────▼────────┐
                    │   Power         │
                    │   Management    │
                    └────────┬────────┘
                             │
        ┌────────────────────┼────────────────────┐
        │                    │                    │
        ▼                    ▼                    ▼
┌───────────────┐    ┌───────────────┐    ┌───────────────┐
│   Motors      │    │   MCU         │    │   Sensors     │
│   (2x DC)     │    │   (STM32F4)   │    │   (IR/Encoder)│
└───────┬───────┘    └───────┬───────┘    └───────┬───────┘
        │                    │                    │
        │              ┌─────┴─────┐              │
        │              │           │              │
        ▼              ▼           ▼              ▼
┌───────────────┐ ┌─────────┐ ┌─────────┐ ┌───────────────┐
│   Motor       │ │   USB   │ │   SD    │ │   Display     │
│   Drivers     │ │   Port  │ │   Card  │ │   (OLED)      │
└───────────────┘ └─────────┘ └─────────┘ └───────────────┘
```

## Component List

### Microcontroller

**STM32F405RG** (recommended)
- ARM Cortex-M4 @ 168MHz
- 192KB RAM, 1MB Flash
- Multiple timers, ADCs, UARTs

**Alternative: ESP32**
- Dual-core Xtensa @ 240MHz
- 520KB SRAM, 4MB Flash
- Built-in WiFi/BT

### Motors

**N20 Gear Motors** (recommended)
- Voltage: 3-6V
- RPM: 100-300 (no load)
- Gear ratio: 1:30 to 1:100
- Encoder: 6/12 CPR

### Sensors

**IR Distance Sensors** (x5-8)
- Type: Sharp GP2Y0A21 or similar
- Range: 10-80cm
- Output: Analog voltage
- Placement: Front, Left, Right, Diagonal

**Wheel Encoders** (x2)
- Type: Optical quadrature
- Resolution: 12-24 CPR
- Purpose: Odometry, distance measurement

**IMU** (optional)
- MPU6050/MPU9250
- Purpose: Orientation, acceleration

### Motor Drivers

**DRV8833** or **TB6612FNG**
- Dual H-bridge
- PWM control
- Current limiting

### Power

**2S LiPo Battery**
- Voltage: 7.4V nominal
- Capacity: 300-500mAh
- Discharge rate: 25C+

**Voltage Regulators**
- 5V for motors (if needed)
- 3.3V for MCU and sensors

## Schematic Overview

### Power Section

```
Battery+ ──┬──► [5V Reg] ──► Motors
           │
           └──► [3.3V Reg] ──► MCU, Sensors
```

### Motor Driver Section

```
MCU PWM1 ──► [DRV8833] ──► Left Motor
MCU PWM2 ──►      │       ──► Right Motor
MCU DIR1 ──►      │
MCU DIR2 ──►      │
```

### Sensor Section

```
IR Sensor 1 ──► [ADC1] ──► MCU
IR Sensor 2 ──► [ADC2] ──► MCU
IR Sensor 3 ──► [ADC3] ──► MCU
Encoder L  ──► [TIM2] ──► MCU
Encoder R  ──► [TIM3] ──► MCU
```

## PCB Layout Guidelines

### Component Placement

1. Center: MCU (short traces to peripherals)
2. Front: IR sensors (closest to maze walls)
3. Sides: Motor drivers (close to motors)
4. Rear: Battery connector, power section

### Trace Widths

| Signal Type | Width |
|-------------|-------|
| Power (main) | 20-30 mil |
| Motor current | 15-20 mil |
| Signal | 8-10 mil |
| Analog (sensors) | 10-12 mil |

### Ground Plane

- Use solid ground plane on bottom layer
- Separate analog and digital ground
- Connect at single point near MCU

## Mechanical Design

### Chassis

**Materials:**
- Carbon fiber plate (1-2mm)
- 3D printed PLA/PETG
- Aluminum (for rigidity)

**Design Considerations:**
- Low center of gravity
- Minimal overhang (wall contact)
- Sensor mounting points
- Easy battery access

### Wheel Design

```
        ┌─────────┐
        │ Encoder │
        │  Gear   │
        └────┬────┘
             │
    ┌────────┴────────┐
    │                 │
    │   Wheel Hub     │
    │                 │
    └────────┬────────┘
             │
    ┌────────┴────────┐
    │                 │
    │  Rubber Tire    │
    │  (high grip)    │
    │                 │
    └─────────────────┘
```

### Sensor Mounting

```
Top View:
        ┌───────────────┐
        │      F        │  F = Front sensor
        │  FL  │  FR    │  FL = Front-Left
   ┌────┤     MCU      ├────┐
   │ L  │             │  R │  L = Left, R = Right
   │    │             │    │
   └────┴─────────────┴────┘
```

## Bill of Materials (BOM)

| Qty | Component | Package | Cost |
|-----|-----------|---------|------|
| 1 | STM32F405RG | LQFP64 | $8 |
| 2 | N20 Motor | - | $6 |
| 2 | Encoder | - | $4 |
| 5 | IR Sensor | Module | $10 |
| 1 | DRV8833 | MSOP | $3 |
| 1 | 2S LiPo | 300mAh | $12 |
| 1 | OLED Display | 0.96" | $5 |
| 1 | SD Card Slot | MicroSD | $2 |
| - | Passive Components | Various | $10 |
| - | Connectors | Various | $5 |
| - | PCB | 2-layer | $15 |
| | **Total** | | **~$80** |

## Assembly Tips

1. **Solder Order**: Passives → ICs → Connectors → Mechanical
2. **Test Points**: Add test points for all signals
3. **Strain Relief**: Secure battery wires
4. **Sensor Calibration**: Mount sensors before calibration
5. **Weight Distribution**: Keep center balanced
