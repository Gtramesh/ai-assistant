#include "robot.h"
#include "../algorithms/flood_fill.h"

void robot_init(Robot* robot) {
    robot->position.x = 0;
    robot->position.y = 0;
    robot->direction = NORTH;
    robot->state = STATE_INIT;
    robot->maze_solved = false;
    robot->distance_traveled = 0;
    robot->speed = 0;
}

void robot_update(Robot* robot, Maze* maze) {
    switch (robot->state) {
        case STATE_INIT:
            // Initialize flood fill
            flood_fill_init(maze);
            robot->state = STATE_EXPLORE;
            break;
            
        case STATE_EXPLORE:
            // Explore maze using flood fill
            if (flood_fill_is_goal(maze, robot->position)) {
                robot->maze_solved = true;
                robot->state = STATE_SOLVE;
            } else {
                Direction next_dir = flood_fill_get_direction(
                    maze, robot->position, robot->direction
                );
                
                if (robot_can_move(robot, maze, next_dir)) {
                    robot_turn(robot, next_dir);
                    robot_move_forward(robot, maze);
                } else {
                    // Try other directions
                    for (int dir = NORTH; dir <= WEST; dir++) {
                        if (robot_can_move(robot, maze, dir)) {
                            robot_turn(robot, dir);
                            robot_move_forward(robot, maze);
                            break;
                        }
                    }
                }
            }
            break;
            
        case STATE_SOLVE:
            // Calculate optimal path
            flood_fill_propagate(maze);
            robot->state = STATE_SPEED;
            break;
            
        case STATE_SPEED:
            // Execute speed run
            if (flood_fill_is_goal(maze, robot->position)) {
                robot->state = STATE_IDLE;
            } else {
                Direction next_dir = flood_fill_get_direction(
                    maze, robot->position, robot->direction
                );
                robot_turn(robot, next_dir);
                robot_move_forward(robot, maze);
            }
            break;
            
        case STATE_IDLE:
            // Do nothing
            break;
    }
}

bool robot_move_forward(Robot* robot, Maze* maze) {
    if (!robot_can_move(robot, maze, robot->direction)) {
        return false;
    }
    
    robot->position = maze_get_neighbor(robot->position, robot->direction);
    robot->distance_traveled++;
    maze_mark_visited(maze, robot->position);
    
    return true;
}

void robot_turn(Robot* robot, Direction new_dir) {
    robot->direction = new_dir;
}

bool robot_can_move(Robot* robot, Maze* maze, Direction dir) {
    return !maze_has_wall(maze, robot->position, dir);
}

Position robot_get_position(Robot* robot) {
    return robot->position;
}

Direction robot_get_direction(Robot* robot) {
    return robot->direction;
}

void robot_set_state(Robot* robot, RobotState state) {
    robot->state = state;
}

RobotState robot_get_state(Robot* robot) {
    return robot->state;
}

void robot_reset(Robot* robot) {
    robot->position.x = 0;
    robot->position.y = 0;
    robot->direction = NORTH;
    robot->state = STATE_INIT;
    robot->distance_traveled = 0;
}
