#include "maze.h"
#include <string.h>

// Goal positions (center 4 cells)
static const Position default_goals[GOAL_SIZE] = {
    {7, 7}, {7, 8}, {8, 7}, {8, 8}
};

void maze_init(Maze* maze) {
    memset(maze, 0, sizeof(Maze));
    
    // Set start position
    maze->start.x = 0;
    maze->start.y = 0;
    
    // Set goal positions
    maze->goal_count = GOAL_SIZE;
    for (int i = 0; i < GOAL_SIZE; i++) {
        maze->goal[i] = default_goals[i];
    }
    
    // Initialize outer walls
    for (int x = 0; x < MAZE_SIZE; x++) {
        maze->cells[x][0].walls |= WALL_SOUTH;
        maze->cells[x][MAZE_SIZE - 1].walls |= WALL_NORTH;
    }
    for (int y = 0; y < MAZE_SIZE; y++) {
        maze->cells[0][y].walls |= WALL_WEST;
        maze->cells[MAZE_SIZE - 1][y].walls |= WALL_EAST;
    }
    
    // Set initial distances (Manhattan to nearest goal)
    for (int x = 0; x < MAZE_SIZE; x++) {
        for (int y = 0; y < MAZE_SIZE; y++) {
            Position pos = {x, y};
            uint8_t min_dist = 255;
            for (int i = 0; i < GOAL_SIZE; i++) {
                uint8_t dist = maze_manhattan(pos, default_goals[i]);
                if (dist < min_dist) {
                    min_dist = dist;
                }
            }
            maze->cells[x][y].distance = min_dist;
        }
    }
    
    maze->explored = false;
}

void maze_set_wall(Maze* maze, Position pos, Direction dir, bool present) {
    if (!maze_is_valid(pos)) return;
    
    uint8_t wall_bit;
    switch (dir) {
        case NORTH: wall_bit = WALL_NORTH; break;
        case EAST:  wall_bit = WALL_EAST;  break;
        case SOUTH: wall_bit = WALL_SOUTH; break;
        case WEST:  wall_bit = WALL_WEST;  break;
        default: return;
    }
    
    if (present) {
        maze->cells[pos.x][pos.y].walls |= wall_bit;
    } else {
        maze->cells[pos.x][pos.y].walls &= ~wall_bit;
    }
    
    // Set opposite wall on neighbor
    Position neighbor = maze_get_neighbor(pos, dir);
    if (maze_is_valid(neighbor)) {
        Direction opposite = (dir + 2) % 4;
        maze_set_wall(maze, neighbor, opposite, present);
    }
}

bool maze_has_wall(Maze* maze, Position pos, Direction dir) {
    if (!maze_is_valid(pos)) return true;
    
    uint8_t wall_bit;
    switch (dir) {
        case NORTH: wall_bit = WALL_NORTH; break;
        case EAST:  wall_bit = WALL_EAST;  break;
        case SOUTH: wall_bit = WALL_SOUTH; break;
        case WEST:  wall_bit = WALL_WEST;  break;
        default: return true;
    }
    
    return (maze->cells[pos.x][pos.y].walls & wall_bit) != 0;
}

Position maze_get_neighbor(Position pos, Direction dir) {
    Position neighbor = pos;
    
    switch (dir) {
        case NORTH: neighbor.y++; break;
        case SOUTH: neighbor.y--; break;
        case EAST:  neighbor.x++; break;
        case WEST:  neighbor.x--; break;
    }
    
    return neighbor;
}

bool maze_is_valid(Position pos) {
    return pos.x < MAZE_SIZE && pos.y < MAZE_SIZE;
}

uint8_t maze_manhattan(Position a, Position b) {
    int dx = (a.x > b.x) ? (a.x - b.x) : (b.x - a.x);
    int dy = (a.y > b.y) ? (a.y - b.y) : (b.y - a.y);
    return (uint8_t)(dx + dy);
}

void maze_set_distance(Maze* maze, Position pos, uint8_t distance) {
    if (maze_is_valid(pos)) {
        maze->cells[pos.x][pos.y].distance = distance;
    }
}

uint8_t maze_get_distance(Maze* maze, Position pos) {
    if (maze_is_valid(pos)) {
        return maze->cells[pos.x][pos.y].distance;
    }
    return 255;
}

void maze_mark_visited(Maze* maze, Position pos) {
    if (maze_is_valid(pos)) {
        maze->cells[pos.x][pos.y].visited = 1;
    }
}

bool maze_is_visited(Maze* maze, Position pos) {
    if (maze_is_valid(pos)) {
        return maze->cells[pos.x][pos.y].visited != 0;
    }
    return false;
}

void maze_reset(Maze* maze) {
    for (int x = 0; x < MAZE_SIZE; x++) {
        for (int y = 0; y < MAZE_SIZE; y++) {
            maze->cells[x][y].distance = maze_manhattan((Position){x, y}, maze->goal[0]);
            maze->cells[x][y].visited = 0;
        }
    }
}
