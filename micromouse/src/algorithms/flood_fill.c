#include "flood_fill.h"
#include <string.h>

void queue_init(PositionQueue* queue) {
    queue->front = 0;
    queue->rear = -1;
    queue->count = 0;
}

bool queue_empty(PositionQueue* queue) {
    return queue->count == 0;
}

bool queue_full(PositionQueue* queue) {
    return queue->count >= MAX_QUEUE_SIZE;
}

void queue_push(PositionQueue* queue, Position pos) {
    if (queue_full(queue)) return;
    
    queue->rear = (queue->rear + 1) % MAX_QUEUE_SIZE;
    queue->items[queue->rear] = pos;
    queue->count++;
}

Position queue_pop(PositionQueue* queue) {
    Position empty = {0, 0};
    if (queue_empty(queue)) return empty;
    
    Position pos = queue->items[queue->front];
    queue->front = (queue->front + 1) % MAX_QUEUE_SIZE;
    queue->count--;
    
    return pos;
}

int queue_size(PositionQueue* queue) {
    return queue->count;
}

void flood_fill_init(Maze* maze) {
    PositionQueue queue;
    queue_init(&queue);
    
    // Reset all distances to max
    for (int x = 0; x < MAZE_SIZE; x++) {
        for (int y = 0; y < MAZE_SIZE; y++) {
            maze->cells[x][y].distance = 255;
        }
    }
    
    // Set goal cells to 0 and enqueue them
    for (int i = 0; i < maze->goal_count; i++) {
        Position goal = maze->goal[i];
        maze->cells[goal.x][goal.y].distance = 0;
        queue_push(&queue, goal);
    }
    
    // Propagate distances
    while (!queue_empty(&queue)) {
        Position current = queue_pop(&queue);
        uint8_t current_dist = maze->cells[current.x][current.y].distance;
        
        for (int dir = NORTH; dir <= WEST; dir++) {
            if (!maze_has_wall(maze, current, dir)) {
                Position neighbor = maze_get_neighbor(current, dir);
                if (maze_is_valid(neighbor)) {
                    uint8_t new_dist = current_dist + 1;
                    if (maze->cells[neighbor.x][neighbor.y].distance > new_dist) {
                        maze->cells[neighbor.x][neighbor.y].distance = new_dist;
                        queue_push(&queue, neighbor);
                    }
                }
            }
        }
    }
}

void flood_fill_update(Maze* maze, Position cell) {
    PositionQueue queue;
    queue_init(&queue);
    
    queue_push(&queue, cell);
    
    while (!queue_empty(&queue)) {
        Position current = queue_pop(&queue);
        uint8_t current_dist = maze->cells[current.x][current.y].distance;
        
        // Find minimum neighbor distance
        uint8_t min_neighbor = 255;
        for (int dir = NORTH; dir <= WEST; dir++) {
            if (!maze_has_wall(maze, current, dir)) {
                Position neighbor = maze_get_neighbor(current, dir);
                if (maze_is_valid(neighbor)) {
                    uint8_t n_dist = maze->cells[neighbor.x][neighbor.y].distance;
                    if (n_dist < min_neighbor) {
                        min_neighbor = n_dist;
                    }
                }
            }
        }
        
        // Update current cell if needed
        if (current_dist != min_neighbor + 1) {
            maze->cells[current.x][current.y].distance = min_neighbor + 1;
            
            // Propagate to neighbors
            for (int dir = NORTH; dir <= WEST; dir++) {
                if (!maze_has_wall(maze, current, dir)) {
                    Position neighbor = maze_get_neighbor(current, dir);
                    if (maze_is_valid(neighbor)) {
                        queue_push(&queue, neighbor);
                    }
                }
            }
        }
    }
}

Direction flood_fill_get_direction(Maze* maze, Position pos, Direction current_dir) {
    uint8_t min_dist = maze->cells[pos.x][pos.y].distance;
    Direction best_dir = current_dir;
    
    // Priority: current direction, then left, right, back
    for (int offset = 0; offset < 4; offset++) {
        Direction check_dir = (current_dir + offset) % 4;
        
        if (!maze_has_wall(maze, pos, check_dir)) {
            Position neighbor = maze_get_neighbor(pos, check_dir);
            if (maze_is_valid(neighbor)) {
                uint8_t n_dist = maze->cells[neighbor.x][neighbor.y].distance;
                if (n_dist < min_dist) {
                    min_dist = n_dist;
                    best_dir = check_dir;
                }
            }
        }
    }
    
    return best_dir;
}

bool flood_fill_is_goal(Maze* maze, Position pos) {
    for (int i = 0; i < maze->goal_count; i++) {
        if (pos.x == maze->goal[i].x && pos.y == maze->goal[i].y) {
            return true;
        }
    }
    return false;
}

void flood_fill_get_path(Maze* maze, Position* path, int* path_length) {
    Position current = maze->start;
    *path_length = 0;
    
    while (!flood_fill_is_goal(maze, current) && *path_length < MAZE_CELLS) {
        path[*path_length] = current;
        (*path_length)++;
        
        Direction next_dir = flood_fill_get_direction(maze, current, NORTH);
        current = maze_get_neighbor(current, next_dir);
    }
    
    // Add final goal position
    if (flood_fill_is_goal(maze, current)) {
        path[*path_length] = current;
        (*path_length)++;
    }
}

void flood_fill_propagate(Maze* maze) {
    PositionQueue queue;
    queue_init(&queue);
    
    // Enqueue all goal cells
    for (int i = 0; i < maze->goal_count; i++) {
        queue_push(&queue, maze->goal[i]);
    }
    
    // Full propagation
    while (!queue_empty(&queue)) {
        Position current = queue_pop(&queue);
        uint8_t current_dist = maze->cells[current.x][current.y].distance;
        
        for (int dir = NORTH; dir <= WEST; dir++) {
            if (!maze_has_wall(maze, current, dir)) {
                Position neighbor = maze_get_neighbor(current, dir);
                if (maze_is_valid(neighbor)) {
                    uint8_t new_dist = current_dist + 1;
                    if (maze->cells[neighbor.x][neighbor.y].distance > new_dist) {
                        maze->cells[neighbor.x][neighbor.y].distance = new_dist;
                        queue_push(&queue, neighbor);
                    }
                }
            }
        }
    }
}
