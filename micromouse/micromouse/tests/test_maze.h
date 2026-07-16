#ifndef TEST_MAZE_H
#define TEST_MAZE_H

#include <stdio.h>
#include <assert.h>
#include "../src/core/maze.h"

void test_maze_init() {
    printf("Testing maze initialization...\n");
    
    Maze maze;
    maze_init(&maze);
    
    // Test start position
    assert(maze.start.x == 0 && maze.start.y == 0);
    
    // Test goal positions
    assert(maze.goal_count == 4);
    assert(maze.goal[0].x == 7 && maze.goal[0].y == 7);
    
    // Test outer walls
    assert(maze_has_wall(&maze, (Position){0, 0}, SOUTH));
    assert(maze_has_wall(&maze, (Position){0, 0}, WEST));
    assert(maze_has_wall(&maze, (Position){15, 15}, NORTH));
    assert(maze_has_wall(&maze, (Position){15, 15}, EAST));
    
    printf("  maze_init: PASSED\n");
}

void test_maze_walls() {
    printf("Testing maze wall operations...\n");
    
    Maze maze;
    maze_init(&maze);
    
    // Test setting walls
    Position pos = {5, 5};
    maze_set_wall(&maze, pos, NORTH, true);
    assert(maze_has_wall(&maze, pos, NORTH));
    
    // Test clearing walls
    maze_set_wall(&maze, pos, NORTH, false);
    assert(!maze_has_wall(&maze, pos, NORTH));
    
    // Test neighbor wall consistency
    maze_set_wall(&maze, pos, EAST, true);
    Position neighbor = maze_get_neighbor(pos, EAST);
    assert(maze_has_wall(&maze, neighbor, WEST));
    
    printf("  maze_walls: PASSED\n");
}

void test_maze_distance() {
    printf("Testing maze distance operations...\n");
    
    Maze maze;
    maze_init(&maze);
    
    // Test initial distances
    Position start = {0, 0};
    uint8_t dist = maze_get_distance(&maze, start);
    assert(dist == 14);  // Manhattan distance to (7,7)
    
    // Test setting distance
    maze_set_distance(&maze, start, 10);
    assert(maze_get_distance(&maze, start) == 10);
    
    printf("  maze_distance: PASSED\n");
}

void test_maze_manhattan() {
    printf("Testing Manhattan distance calculation...\n");
    
    Position a = {0, 0};
    Position b = {7, 7};
    assert(maze_manhattan(a, b) == 14);
    
    Position c = {3, 4};
    Position d = {7, 7};
    assert(maze_manhattan(c, d) == 7);
    
    printf("  maze_manhattan: PASSED\n");
}

void test_maze_valid() {
    printf("Testing maze validity check...\n");
    
    assert(maze_is_valid((Position){0, 0}));
    assert(maze_is_valid((Position){15, 15}));
    assert(!maze_is_valid((Position){16, 0}));
    assert(!maze_is_valid((Position){0, 16}));
    
    printf("  maze_valid: PASSED\n");
}

#endif // TEST_MAZE_H
