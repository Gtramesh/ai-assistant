#include <stdio.h>
#include "core/maze.h"
#include "core/robot.h"
#include "core/sensor.h"
#include "core/motor.h"
#include "algorithms/flood_fill.h"
#include "utils/config.h"

int main(void) {
    // Initialize components
    Maze maze;
    Robot robot;
    SensorManager sensor;
    MotorControl motor;
    
    maze_init(&maze);
    robot_init(&robot);
    sensor_init(&sensor);
    motor_init(&motor);
    
    // Enable motors
    motor_enable(&motor);
    
    printf("Micromouse initialized\n");
    printf("Starting exploration...\n");
    
    // Main loop
    while (robot_get_state(&robot) != STATE_IDLE) {
        // Read sensors
        sensor_read_ir(&sensor);
        sensor_read_encoders(&sensor);
        
        // Update robot state
        robot_update(&robot, &maze);
        
        // Update motors
        motor_update(&motor, 
            (float)sensor.encoders.left_count,
            (float)sensor.encoders.right_count);
        
        // Debug output
        #if DEBUG_ENABLED
        printf("Position: (%d, %d) Dir: %d State: %d Dist: %d\n",
            robot_get_position(&robot).x,
            robot_get_position(&robot).y,
            robot_get_direction(&robot),
            robot_get_state(&robot),
            maze_get_distance(&maze, robot_get_position(&robot)));
        #endif
    }
    
    printf("Maze solved!\n");
    printf("Total distance: %d cells\n", robot.distance_traveled);
    
    // Disable motors
    motor_disable(&motor);
    
    return 0;
}
