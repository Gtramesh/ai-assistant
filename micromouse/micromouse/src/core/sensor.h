#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include <stdbool.h>

#define NUM_IR_SENSORS 5
#define NUM_ENCODERS 2

typedef enum {
    SENSOR_FRONT = 0,
    SENSOR_FRONT_LEFT = 1,
    SENSOR_FRONT_RIGHT = 2,
    SENSOR_LEFT = 3,
    SENSOR_RIGHT = 4
} SensorIndex;

typedef struct {
    uint16_t raw_values[NUM_IR_SENSORS];
    uint16_t calibrated_values[NUM_IR_SENSORS];
    uint16_t min_values[NUM_IR_SENSORS];
    uint16_t max_values[NUM_IR_SENSORS];
    bool calibrated;
} IRSensors;

typedef struct {
    int32_t left_count;
    int32_t right_count;
    float left_distance;
    float right_distance;
    float wheel_diameter;
    int32_t counts_per_revolution;
} Encoders;

typedef struct {
    IRSensors ir;
    Encoders encoders;
    float imu_x;
    float imu_y;
    float imu_z;
} SensorManager;

// Initialize sensors
void sensor_init(SensorManager* sensor);

// Read IR sensor values
void sensor_read_ir(SensorManager* sensor);

// Read encoder values
void sensor_read_encoders(SensorManager* sensor);

// Read IMU
void sensor_read_imu(SensorManager* sensor);

// Calibrate sensors
void sensor_calibrate(SensorManager* sensor);

// Get wall presence
bool sensor_has_wall(SensorManager* sensor, SensorIndex index, uint16_t threshold);

// Get distance value
uint16_t sensor_get_distance(SensorManager* sensor, SensorIndex index);

// Reset encoders
void sensor_reset_encoders(SensorManager* sensor);

// Get encoder counts
void sensor_get_encoder_counts(SensorManager* sensor, int32_t* left, int32_t* right);

#endif // SENSOR_H
