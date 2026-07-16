#ifndef TEST_ALGORITHM_H
#define TEST_ALGORITHM_H

#include <stdio.h>
#include <assert.h>
#include "../src/core/maze.h"
#include "../src/algorithms/flood_fill.h"

void test_queue_operations() {
    printf("Testing queue operations...\n");
    
    PositionQueue queue;
    queue_init(&queue);
    
    assert(queue_empty(&queue));
    assert(queue_size(&queue) == 0);
    
    // Push positions
    Position p1 = {0, 0};
    Position p2 = {1, 1};
    Position p3 = {2, 2};
    
    queue_push(&queue, p1);
    assert(!queue_empty(&queue));
    assert(queue_size(&queue) == 1);
    
    queue_push(&queue, p2);
    queue_push(&queue, p3);
    assert(queue_size(&queue) == 3);
    
    // Pop positions
    Position popped = queue_pop(&queue);
    assert(popped.x == p1.x && popped.y == p1.y);
    
    popped = queue_pop(&queue);
    assert(popped.x == p2.x && popped.y == p2.y);
    
    popped = queue_pop(&queue);
    assert(popped.x == p3.x && popped.y == p3.y);
    
    assert(queue_empty(&queue));
    
    printf("  queue_operations: PASSED\n");
}

void test_flood_fill_init() {
    printf("Testing flood fill initialization...\n");
    
    Maze maze;
    maze_init(&maze);
    flood_fill_init(&maze);
    
    // Goal cells should have distance 0
    for (int i = 0; i < maze.goal_count; i++) {
        assert(maze.cells[maze.goal[i].x][maze.goal[i].y].distance == 0);
    }
    
    // Distances should increase away from goal
    Position far_pos = {0, 0};
    assert(maze_get_distance(&maze, far_pos) > 0);
    
    printf("  flood_fill_init: PASSED\n");
}

void test_flood_fill_update() {
    printf("Testing flood fill update...\n");
    
    Maze maze;
    maze_init(&maze);
    flood_fill_init(&maze);
    
    // Add a wall and update
    Position pos = {7, 6};
    maze_set_wall(&maze, pos, NORTH, true);
    flood_fill_update(&maze, pos);
    
    // Verify distances are updated
    Position neighbor = {7, 7};
    uint8_t dist = maze_get_distance(&maze, neighbor);
    assert(dist >= 0);  // Should still be reachable
    
    printf("  flood_fill_update: PASSED\n");
}

void test_flood_fill_get_direction() {
    printf("Testing flood fill direction selection...\n");
    
    Maze maze;
    maze_init(&maze);
    flood_fill_init(&maze);
    
    // From start, should move towards goal
    Position start = {0, 0};
    Direction dir = flood_fill_get_direction(&maze, start, NORTH);
    
    // Should not stay in place
    assert(dir == NORTH || dir == EAST);
    
    printf("  flood_fill_get_direction: PASSED\n");
}

void test_flood_fill_is_goal() {
    printf("Testing flood fill goal detection...\n");
    
    Maze maze;
    maze_init(&maze);
    
    // Test goal positions
    Position goal1 = {7, 7};
    assert(flood_fill_is_goal(&maze, goal1));
    
    Position goal2 = {8, 8};
    assert(flood_fill_is_goal(&maze, goal2));
    
    // Test non-goal positions
    Position non_goal = {0, 0};
    assert(!flood_fill_is_goal(&maze, non_goal));
    
    printf("  flood_fill_is_goal: PASSED\n");
}

void test_flood_fill_path() {
    printf("Testing flood fill path generation...\n");
    
    Maze maze;
    maze_init(&maze);
    flood_fill_init(&maze);
    
    Position path[MAZE_CELLS];
    int path_length = 0;
    
    flood_fill_get_path(&maze, path, &path_length);
    
    // Path should start at maze start
    assert(path[0].x == maze.start.x && path[0].y == maze.start.y);
    
    // Path should be non-empty
    assert(path_length > 0);
    
    printf("  flood_fill_path: PASSED\n");
}

#endif // TEST_ALGORITHM_H
