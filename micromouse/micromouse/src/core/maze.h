#ifndef MAZE_H
#define MAZE_H

#include <stdint.h>
#include <stdbool.h>

#define MAZE_SIZE 16
#define MAZE_CELLS (MAZE_SIZE * MAZE_SIZE)
#define GOAL_SIZE 4

typedef enum {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
} Direction;

typedef struct {
    uint8_t x;
    uint8_t y;
} Position;

typedef struct {
    uint8_t distance;
    uint8_t walls;      // Bit field: N=0x01, E=0x02, S=0x04, W=0x08
    uint8_t visited;
} Cell;

typedef struct {
    Cell cells[MAZE_SIZE][MAZE_SIZE];
    Position start;
    Position goal[GOAL_SIZE];
    uint8_t goal_count;
    bool explored;
} Maze;

// Wall bit masks
#define WALL_NORTH  0x01
#define WALL_EAST   0x02
#define WALL_SOUTH  0x04
#define WALL_WEST   0x08
#define WALL_ALL    0x0F

// Initialize maze with default values
void maze_init(Maze* maze);

// Set wall between two adjacent cells
void maze_set_wall(Maze* maze, Position pos, Direction dir, bool present);

// Check if wall exists
bool maze_has_wall(Maze* maze, Position pos, Direction dir);

// Get neighboring position
Position maze_get_neighbor(Position pos, Direction dir);

// Check if position is valid
bool maze_is_valid(Position pos);

// Get Manhattan distance
uint8_t maze_manhattan(Position a, Position b);

// Set distance value
void maze_set_distance(Maze* maze, Position pos, uint8_t distance);

// Get distance value
uint8_t maze_get_distance(Maze* maze, Position pos);

// Mark cell as visited
void maze_mark_visited(Maze* maze, Position pos);

// Check if cell is visited
bool maze_is_visited(Maze* maze, Position pos);

// Reset maze for new run
void maze_reset(Maze* maze);

#endif // MAZE_H
