#ifndef FLOOD_FILL_H
#define FLOOD_FILL_H

#include "maze.h"

#define MAX_QUEUE_SIZE (MAZE_CELLS)

typedef struct {
    Position items[MAX_QUEUE_SIZE];
    int front;
    int rear;
    int count;
} PositionQueue;

// Initialize queue
void queue_init(PositionQueue* queue);

// Check if queue is empty
bool queue_empty(PositionQueue* queue);

// Check if queue is full
bool queue_full(PositionQueue* queue);

// Push position to queue
void queue_push(PositionQueue* queue, Position pos);

// Pop position from queue
Position queue_pop(PositionQueue* queue);

// Get queue size
int queue_size(PositionQueue* queue);

// Initialize flood fill distances
void flood_fill_init(Maze* maze);

// Update flood fill from a cell
void flood_fill_update(Maze* maze, Position cell);

// Get next direction to move (lowest distance)
Direction flood_fill_get_direction(Maze* maze, Position pos, Direction current_dir);

// Check if current position is goal
bool flood_fill_is_goal(Maze* maze, Position pos);

// Get path from start to goal
void flood_fill_get_path(Maze* maze, Position* path, int* path_length);

// Full flood fill propagation
void flood_fill_propagate(Maze* maze);

#endif // FLOOD_FILL_H
