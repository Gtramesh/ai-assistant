#include "pid.h"

void pid_init(PIDController* pid, float kp, float ki, float kd) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->setpoint = 0.0f;
    pid->integral = 0.0f;
    pid->prev_error = 0.0f;
    pid->output_min = -100.0f;
    pid->output_max = 100.0f;
}

void pid_set_params(PIDController* pid, float kp, float ki, float kd) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}

void pid_set_setpoint(PIDController* pid, float setpoint) {
    pid->setpoint = setpoint;
}

void pid_set_limits(PIDController* pid, float min, float max) {
    pid->output_min = min;
    pid->output_max = max;
}

float pid_calculate(PIDController* pid, float measurement) {
    float error = pid->setpoint - measurement;
    
    // Proportional term
    float p_term = pid->kp * error;
    
    // Integral term with anti-windup
    pid->integral += error;
    if (pid->integral > pid->output_max) {
        pid->integral = pid->output_max;
    } else if (pid->integral < pid->output_min) {
        pid->integral = pid->output_min;
    }
    float i_term = pid->ki * pid->integral;
    
    // Derivative term
    float derivative = error - pid->prev_error;
    float d_term = pid->kd * derivative;
    
    // Calculate output
    float output = p_term + i_term + d_term;
    
    // Clamp output
    if (output > pid->output_max) {
        output = pid->output_max;
    } else if (output < pid->output_min) {
        output = pid->output_min;
    }
    
    // Store error for next iteration
    pid->prev_error = error;
    
    return output;
}

void pid_reset(PIDController* pid) {
    pid->integral = 0.0f;
    pid->prev_error = 0.0f;
}
