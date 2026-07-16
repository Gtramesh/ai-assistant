#ifndef PID_H
#define PID_H

#include <stdint.h>

typedef struct {
    float kp;
    float ki;
    float kd;
    float setpoint;
    float integral;
    float prev_error;
    float output_min;
    float output_max;
} PIDController;

// Initialize PID controller
void pid_init(PIDController* pid, float kp, float ki, float kd);

// Set PID parameters
void pid_set_params(PIDController* pid, float kp, float ki, float kd);

// Set setpoint
void pid_set_setpoint(PIDController* pid, float setpoint);

// Set output limits
void pid_set_limits(PIDController* pid, float min, float max);

// Calculate PID output
float pid_calculate(PIDController* pid, float measurement);

// Reset PID controller
void pid_reset(PIDController* pid);

#endif // PID_H
