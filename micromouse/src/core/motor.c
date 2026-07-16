#include "motor.h"

void motor_init(MotorControl* motor) {
    motor->left_speed = 0;
    motor->right_speed = 0;
    motor->max_speed = 1000;
    motor->enabled = false;
    
    // Initialize PID controllers
    pid_init(&motor->left_pid, 1.0f, 0.1f, 0.05f);
    pid_init(&motor->right_pid, 1.0f, 0.1f, 0.05f);
    
    // Set output limits
    pid_set_limits(&motor->left_pid, -motor->max_speed, motor->max_speed);
    pid_set_limits(&motor->right_pid, -motor->max_speed, motor->max_speed);
}

void motor_set_speeds(MotorControl* motor, int16_t left, int16_t right) {
    if (!motor->enabled) return;
    
    // Clamp speeds
    if (left > motor->max_speed) left = motor->max_speed;
    if (left < -motor->max_speed) left = -motor->max_speed;
    if (right > motor->max_speed) right = motor->max_speed;
    if (right < -motor->max_speed) right = -motor->max_speed;
    
    motor->left_speed = left;
    motor->right_speed = right;
    
    // Set PID setpoints
    pid_set_setpoint(&motor->left_pid, (float)left);
    pid_set_setpoint(&motor->right_pid, (float)right);
    
    // Here you would send PWM signals to motor drivers
    // Example for STM32:
    // TIM_SetCompare1(TIM3, left + motor->max_speed);
    // TIM_SetCompare2(TIM3, right + motor->max_speed);
}

void motor_set_left(MotorControl* motor, int16_t speed) {
    motor_set_speeds(motor, speed, motor->right_speed);
}

void motor_set_right(MotorControl* motor, int16_t speed) {
    motor_set_speeds(motor, motor->left_speed, speed);
}

void motor_stop(MotorControl* motor) {
    motor_set_speeds(motor, 0, 0);
    pid_reset(&motor->left_pid);
    pid_reset(&motor->right_pid);
}

void motor_enable(MotorControl* motor) {
    motor->enabled = true;
}

void motor_disable(MotorControl* motor) {
    motor->enabled = false;
    motor_stop(motor);
}

void motor_update(MotorControl* motor, float left_encoder, float right_encoder) {
    if (!motor->enabled) return;
    
    // Calculate PID outputs
    float left_output = pid_calculate(&motor->left_pid, left_encoder);
    float right_output = pid_calculate(&motor->right_pid, right_encoder);
    
    // Apply to motors (hardware-specific)
    // This would update the actual PWM values
}

void motor_set_pid(MotorControl* motor, float kp, float ki, float kd) {
    pid_set_params(&motor->left_pid, kp, ki, kd);
    pid_set_params(&motor->right_pid, kp, ki, kd);
}

void motor_get_speeds(MotorControl* motor, int16_t* left, int16_t* right) {
    *left = motor->left_speed;
    *right = motor->right_speed;
}
