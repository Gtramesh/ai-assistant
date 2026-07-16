#ifndef ROBOT_H
#define ROBOT_H

#include "maze.h"

typedef enum {
    STATE_INIT,
    STATE_EXPLORE,
    STATE_SOLVE,
    STATE_SPEED,
    STATE_IDLE
} RobotState;

typedef struct {
    Position position;
    Direction direction;
    RobotState state;
    bool maze_solved;
    uint16_t distance_traveled;
    uint8_t speed;
} Robot;

// Initialize robot
void robot_init(Robot* robot);

// Update robot state
void robot_update(Robot* robot, Maze* maze);

// Move robot forward one cell
bool robot_move_forward(Robot* robot, Maze* maze);

// Turn robot
void robot_turn(Robot* robot, Direction new_dir);

// Check if robot can move in direction
bool robot_can_move(Robot* robot, Maze* maze, Direction dir);

// Get current position
Position robot_get_position(Robot* robot);

// Get current direction
Direction robot_get_direction(Robot* robot);

// Set robot state
void robot_set_state(Robot* robot, RobotState state);

// Get robot state
RobotState robot_get_state(Robot* robot);

// Reset robot to start
void robot_reset(Robot* robot);

#endif // ROBOT_H
