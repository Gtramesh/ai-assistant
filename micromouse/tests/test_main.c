#include <stdio.h>
#include "test_maze.h"
#include "test_algorithm.h"

int main() {
    printf("=== Micromouse Test Suite ===\n\n");
    
    // Maze tests
    printf("--- Maze Tests ---\n");
    test_maze_init();
    test_maze_walls();
    test_maze_distance();
    test_maze_manhattan();
    test_maze_valid();
    printf("\n");
    
    // Algorithm tests
    printf("--- Algorithm Tests ---\n");
    test_queue_operations();
    test_flood_fill_init();
    test_flood_fill_update();
    test_flood_fill_get_direction();
    test_flood_fill_is_goal();
    test_flood_fill_path();
    printf("\n");
    
    printf("=== All Tests Passed ===\n");
    
    return 0;
}
