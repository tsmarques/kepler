#ifndef __KEPLER__IMU_H__
#define __KEPLER__IMU_H__

#include <stdint.h>

#include <config.h>

typedef struct imu_obj
{
  char device_name[IMU_DEVICE_NAME_SIZE];
  //! Read this device's ID
  uint8_t (* get_device_id)();

  //! Initialize IMU's registers
  void (* initialize)();

  //! Read xyz accelerations in g's
  void (* get_accelerations)(double*, double*, double*);

  //! Read xyz angular velocities deg/s
  void (* get_angular_velocities)(double*, double*, double*);

  //! Calibrate this device and acquire the bias
  void (* calibrate)(float*, float*);
} imu_t;

#endif
