#include "sensor.h"
#include <string.h>

void sensor_init(SensorManager* sensor) {
    memset(sensor, 0, sizeof(SensorManager));
    
    sensor->ir.calibrated = false;
    sensor->encoders.wheel_diameter = 25.0f;  // mm
    sensor->encoders.counts_per_revolution = 12;
}

void sensor_read_ir(SensorManager* sensor) {
    // Hardware-specific implementation
    // Example for STM32 ADC:
    // for (int i = 0; i < NUM_IR_SENSORS; i++) {
    //     sensor->raw_values[i] = ADC_GetConversion(ADC1, i);
    // }
    
    // For simulation/testing, use dummy values
    for (int i = 0; i < NUM_IR_SENSORS; i++) {
        sensor->raw_values[i] = 0;
    }
}

void sensor_read_encoders(SensorManager* sensor) {
    // Hardware-specific implementation
    // Example for STM32 timers:
    // sensor->encoders.left_count = TIM_GetCounter(TIM2);
    // sensor->encoders.right_count = TIM_GetCounter(TIM3);
}

void sensor_read_imu(SensorManager* sensor) {
    // Hardware-specific implementation
    // Example for MPU6050:
    // MPU6050_GetAccel(&sensor->imu_x, &sensor->imu_y, &sensor->imu_z);
}

void sensor_calibrate(SensorManager* sensor) {
    // Read raw values
    sensor_read_ir(sensor);
    
    // Initialize min/max
    for (int i = 0; i < NUM_IR_SENSORS; i++) {
        sensor->min_values[i] = sensor->raw_values[i];
        sensor->max_values[i] = sensor->raw_values[i];
    }
    
    // Calibrate by rotating robot
    for (int sample = 0; sample < 100; sample++) {
        sensor_read_ir(sensor);
        
        for (int i = 0; i < NUM_IR_SENSORS; i++) {
            if (sensor->raw_values[i] < sensor->min_values[i]) {
                sensor->min_values[i] = sensor->raw_values[i];
            }
            if (sensor->raw_values[i] > sensor->max_values[i]) {
                sensor->max_values[i] = sensor->raw_values[i];
            }
        }
    }
    
    sensor->ir.calibrated = true;
}

bool sensor_has_wall(SensorManager* sensor, SensorIndex index, uint16_t threshold) {
    if (index >= NUM_IR_SENSORS) return false;
    
    uint16_t value;
    
    if (sensor->ir.calibrated) {
        // Normalize to 0-1000 range
        uint16_t range = sensor->ir.max_values[index] - sensor->ir.min_values[index];
        if (range == 0) return false;
        
        value = ((sensor->raw_values[index] - sensor->ir.min_values[index]) * 1000) / range;
    } else {
        value = sensor->raw_values[index];
    }
    
    return value > threshold;
}

uint16_t sensor_get_distance(SensorManager* sensor, SensorIndex index) {
    if (index >= NUM_IR_SENSORS) return 0;
    return sensor->raw_values[index];
}

void sensor_reset_encoders(SensorManager* sensor) {
    sensor->encoders.left_count = 0;
    sensor->encoders.right_count = 0;
    sensor->encoders.left_distance = 0.0f;
    sensor->encoders.right_distance = 0.0f;
}

void sensor_get_encoder_counts(SensorManager* sensor, int32_t* left, int32_t* right) {
    *left = sensor->encoders.left_count;
    *right = sensor->encoders.right_count;
    
    // Calculate distance
    float circumference = 3.14159f * sensor->encoders.wheel_diameter;
    sensor->encoders.left_distance = 
        (float)sensor->encoders.left_count * circumference / sensor->encoders.counts_per_revolution;
    sensor->encoders.right_distance = 
        (float)sensor->encoders.right_count * circumference / sensor->encoders.counts_per_revolution;
}
