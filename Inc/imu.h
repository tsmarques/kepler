#ifndef __CTB__IMU_H__
#define __CTB__IMU_H__

#include "../config.h"

typedef struct imu_obj
{
  char device_name[IMU_DEVICE_NAME_SIZE];
  //! Read this device's ID
  uint8_t (* imu_get_device_id)();

  //! Initialize IMU's registers
  void (* imu_initialize)();

  //! Read xyz accelerations in g's
  void (* imu_get_accelerations)(double*, double*, double*);

  //! Read xyz angular velocities deg/s
  void (* imu_get_angular_velocities)(double*, double*, double*);

  //! Calibrate this device and acquire the bias
  void (* icm_calibrate)(float*, float*);
} imu_t;

#endif
