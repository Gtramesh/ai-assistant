#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pid.h"

typedef struct {
    int16_t left_speed;
    int16_t right_speed;
    int16_t max_speed;
    PIDController left_pid;
    PIDController right_pid;
    bool enabled;
} MotorControl;

// Initialize motor control
void motor_init(MotorControl* motor);

// Set motor speeds
void motor_set_speeds(MotorControl* motor, int16_t left, int16_t right);

// Set left motor speed
void motor_set_left(MotorControl* motor, int16_t speed);

// Set right motor speed
void motor_set_right(MotorControl* motor, int16_t speed);

// Stop motors
void motor_stop(MotorControl* motor);

// Enable motors
void motor_enable(MotorControl* motor);

// Disable motors
void motor_disable(MotorControl* motor);

// Update motor PID control
void motor_update(MotorControl* motor, float left_encoder, float right_encoder);

// Set PID parameters
void motor_set_pid(MotorControl* motor, float kp, float ki, float kd);

// Get current speeds
void motor_get_speeds(MotorControl* motor, int16_t* left, int16_t* right);

#endif // MOTOR_H
