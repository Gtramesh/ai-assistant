#ifndef CONFIG_H
#define CONFIG_H

// Maze Configuration
#define MAZE_SIZE           16
#define CELL_SIZE_MM        180
#define WALL_HEIGHT_MM      50
#define WALL_THICKNESS_MM   12

// Goal Configuration
#define GOAL_X              7
#define GOAL_Y              7
#define GOAL_SIZE           2

// Robot Configuration
#define ROBOT_WIDTH_MM      90
#define ROBOT_LENGTH_MM     120
#define WHEEL_DIAMETER_MM   25
#define WHEELBASE_MM        80

// Motor Configuration
#define MOTOR_MAX_SPEED     1000
#define MOTOR_ACCELERATION  500
#define MOTOR_DECELERATION  800

// Sensor Configuration
#define SENSOR_FRONT_THRESHOLD      150
#define SENSOR_SIDE_THRESHOLD       100
#define SENSOR_DIAGONAL_THRESHOLD   120

// PID Configuration
#define PID_SPEED_KP        1.0f
#define PID_SPEED_KI        0.1f
#define PID_SPEED_KD        0.05f

#define PID_STEERING_KP     0.5f
#define PID_STEERING_KI     0.05f
#define PID_STEERING_KD     0.02f

// Algorithm Configuration
#define ALGORITHM_FLOOD_FILL    1
#define ALGORITHM_ASTAR         0
#define ALGORITHM_WALL_FOLLOW   0

// Timing Configuration
#define TIMER_MOTOR_FREQ     1000    // Hz
#define TIMER_SENSOR_FREQ    100     // Hz
#define TIMER_PID_FREQ       100     // Hz

// Communication Configuration
#define UART_BAUD_RATE       115200
#define SPI_CLOCK_SPEED      1000000 // Hz

// Debug Configuration
#define DEBUG_ENABLED        1
#define DEBUG_UART           1
#define DEBUG_SD_CARD        0

// Competition Configuration
#define MAX_EXPLORATION_TIME 300     // seconds
#define MAX_SPEED_RUN_TIME   60      // seconds

#endif // CONFIG_H
