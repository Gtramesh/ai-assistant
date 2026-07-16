# Competition Rules

## Overview

Micromouse is an international robotics competition where autonomous robots navigate a complex maze. This document covers the official rules and strategies.

## Maze Specifications

### Dimensions

```
16 x 16 Grid

Each cell: 180mm x 180mm
Wall height: 50mm
Wall thickness: 12mm

Total maze size: 2.88m x 2.88m
```

### Layout

```
┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
│   │   │   │   │   │   │   │   │   │   │   │   │   │   │   │   │
├───┼─┐ ├───┴─┐ ├───┴─┐ ├───┴─┐ ├───┴─┐ ├───┴─┐ ├───┴─┐ ├───┴─┐ │
│   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │
├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ │
│   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │
├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ │
│   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │
├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ │
│   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │
├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ │
│   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │
├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ │
│   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │
├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ │
│     │ │     │ │     │ │     │ │     │ │     │ │     │ │     │ │
├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ │
│   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │
├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ ├───┤ │ │
│   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │
├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ │
│     │ │     │ │     │ │     │ │     │ │     │ │     │ │     │ │
├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ │
│   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │
├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ │
│     │ │     │ │     │ │     │ │     │ │     │ │     │ │     │ │
├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ │
│   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │
├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ │
│     │ │     │ │     │ │     │ │     │ │     │ │     │ │     │ │
├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ │
│   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │
├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ │
│     │ │     │ │     │ │     │ │     │ │     │ │     │ │     │ │
├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ │
│   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │
├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ │
│     │ │     │ │     │ │     │ │     │ │     │ │     │ │     │ │
├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ ├───┐ │ │
│   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │   │ │ │
├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ ├───┘ │ │
│     │ │     │ │     │ │     │ │     │ │     │ │     │ │     │ │
└─────┘ └─────┘ └─────┘ └─────┘ └─────┘ └─────┘ └─────┘ └─────┘ │
│                                                                   │
└───────────────────────────────────────────────────────────────────┘
```

### Goal Area

- **Location**: 4 center cells (2x2 square)
- **Goal**: Reach any of the 4 center cells
- **Start**: Bottom-left corner (0,0)

## Run Rules

### Exploration Phase

| Rule | Description |
|------|-------------|
| Time Limit | 5 minutes maximum |
| Wall Contact | Robot may touch walls |
| Restart | Unlimited restarts allowed |
| Manual Stop | Operator may stop robot |

### Speed Run Phase

| Rule | Description |
|------|-------------|
| Time Limit | Per-run limit (varies by competition) |
| Wall Contact | Results in disqualification |
| Path | Must solve maze first |

### Scoring

```
Score = Fastest valid speed run time

Lower is better!
```

## Competition Categories

### Standard Micromouse

- Maze size: 16x16
- Cell size: 180mm
- Wall height: 50mm

### MegaMouse (Large maze)

- Maze size: 32x32
- Cell size: 120mm
- Wall height: 50mm

### Pro Mouse

- Open class
- Custom maze sizes
- Advanced challenges

## Robot Categories

### Weight Classes

| Class | Max Weight |
|-------|------------|
| Standard | 500g |
| Lightweight | 300g |
| Ultralight | 150g |

### Size Constraints

| Dimension | Max Value |
|-----------|-----------|
| Length | 250mm |
| Width | 250mm |
| Height | 250mm |

## Strategy Guide

### Exploration Strategy

1. **Start Conservative**
   - Move slowly initially
   - Build confidence in sensor readings
   - Establish baseline wall detection

2. **Use Flood Fill**
   - Update distances as you discover walls
   - Always move to lowest distance neighbor
   - Mark visited cells

3. **Handle Dead Ends**
   - When reaching a dead end, backtrack
   - Update flood values
   - Continue exploration

4. **Goal Detection**
   - Recognize center cells
   - Store path to goal
   - Prepare for speed run

### Speed Run Strategy

1. **Path Optimization**
   - Remove unnecessary turns
   - Merge straight segments
   - Calculate diagonal moves

2. **Speed Control**
   - Accelerate on straights
   - Decelerate before turns
   - Maintain momentum through curves

3. **Turn Execution**
   - Pre-plan turn radius
   - Adjust speed for turn angle
   - Use wheel encoders for precision

### Common Pitfalls

| Issue | Solution |
|-------|----------|
| Sensor noise | Filter readings, use multiple samples |
| Motor drift | Regular calibration, encoder feedback |
| Dead reckoning error | Use wall following to correct |
| Memory overflow | Optimize data structures |

## Competition Tips

### Before Competition

- [ ] Test all sensors
- [ ] Calibrate motor speeds
- [ ] Verify battery voltage
- [ ] Practice maze navigation
- [ ] Test speed runs

### During Competition

- [ ] Check sensor readings before run
- [ ] Verify battery level
- [ ] Warm up motors
- [ ] Stay calm, trust your code

### Troubleshooting

- **Robot stops**: Check battery, sensors
- **Robot crashes**: Reduce speed, check sensors
- **Inconsistent runs**: Calibrate, check connections

## Official Resources

- [International Micromouse Competition](http://www.micromouseusa.com/)
- [Japanese Micromouse](http://www.micromouse.jp/)
- [UK Micromouse](http://micromouse.org/)
