#include <string.h>

#include "../config.h"
#include <spi.h>
#include <icm20948.h>

uint8_t accel_scale = AFS_8G;
uint8_t gyro_scale = GFS_250DPS;

#define SPI_SELECT() HAL_GPIO_WritePin(IMU_PORT, IMU_CS_Pin, GPIO_PIN_RESET)
#define SPI_DESELECT() HAL_GPIO_WritePin(IMU_PORT, IMU_CS_Pin, GPIO_PIN_SET)

SPI_HandleTypeDef hspi2;

/* SPI2 init function */
void MX_SPI2_Init(void)
{

  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void write_byte(uint8_t reg, uint8_t Data) // ***
{
  reg = reg & 0x7F;
  SPI_SELECT();
  HAL_SPI_Transmit(&hspi2, &reg, 1, 0xFFFF);
  HAL_SPI_Transmit(&hspi2, &Data, 1, 0xFFFF);
  SPI_DESELECT();
}

static inline void select_bank(uint8_t bank)
{
  write_byte(USER_BANK_SEL, bank);
}


static void read_bytes(uint8_t reg, uint8_t *pData, uint16_t Size) // ***
{
  reg = reg | REG_READ_FLAG;
  SPI_SELECT();
  HAL_SPI_Transmit(&hspi2, &reg, 1, 0xFFFF);
  HAL_SPI_Receive(&hspi2, pData, Size, 0xFFFF);
  SPI_DESELECT();
}

static void write_bytes(uint8_t reg, uint8_t *pData, uint16_t Size) // ***
{
  reg = reg & 0x7F;
  SPI_SELECT();
  HAL_SPI_Transmit(&hspi2, &reg, 1, 0xFFFF);
  HAL_SPI_Transmit(&hspi2, pData, Size, 0xFFFF);
  SPI_DESELECT();
}

static void read_byte(uint8_t reg, uint8_t* pData) // ***
{
  reg = reg | REG_READ_FLAG;
  SPI_SELECT();
  HAL_SPI_Transmit(&hspi2, &reg, 1, 0xFFFF);
  HAL_SPI_Receive(&hspi2, pData, 1, 0xFFFF);
  SPI_DESELECT();
}

static float
acceleration_resolution(uint8_t accel_scale)
{
  float accel_res = 0;
  switch (accel_scale)
  {
    case AFS_2G:
      accel_res = 2.0f / 32768.0f;
      break;

    case AFS_4G:
      accel_res = 4.0f / 32768.0f;
      break;

    case AFS_8G:
      accel_res = 8.0f / 32768.0f;
      break;

    case AFS_16G:
      accel_res = 16.0f / 32768.0f;
      break;

    default:
      break;
  }
  return accel_res;
}

static float
gyroscope_resolution(uint8_t gyro_scale)
{
  float gyro_res = 0;
  switch (gyro_scale)
  {
    case GFS_250DPS:
      gyro_res = 250.0 / 32768.0;
      break;
    case GFS_500DPS:
      gyro_res = 500.0 / 32768.0;
      break;
    case GFS_1000DPS:
      gyro_res = 1000.0 / 32768.0;
      break;
    case GFS_2000DPS:
      gyro_res = 2000.0 / 32768.0;
      break;

    default:
      break;
  }
  return gyro_res;
}

void imu_initialize(void)
{
    select_bank(USER_BANK_0);
    HAL_Delay(10);

  // Get stable time source
  // Auto select clock source to be PLL gyroscope reference if ready else
  write_byte(PWR_MGMT_1, 0x01);
  HAL_Delay(200);

  // Switch to user bank 2
  select_bank(USER_BANK_2);

  // Configure Gyro and Thermometer
  // Disable FSYNC and set gyro bandwidth to 51.2 Hz,
  // respectively;
  // minimum delay time for this setting is 5.9 ms, which means sensor fusion
  // update rates cannot be higher than 1 / 0.0059 = 170 Hz
  // DLPF_CFG = bits 2:0 = 011; this limits the sample rate to 1000 Hz for both
  // With the ICM20948, it is possible to get gyro sample rates of 32 kHz (!),
  // 8 kHz, or 1 kHz
  // Set gyroscope full scale range to 250 dps
  write_byte(GYRO_CONFIG_1, 0x19);
  write_byte(TEMP_CONFIG, 0x03);

  // Set sample rate = gyroscope output rate/(1 + GYRO_SMPLRT_DIV)
  // Use a 220 Hz rate; a rate consistent with the filter update rate
  // determined inset in CONFIG above.

  write_byte(GYRO_SMPLRT_DIV, 0x04);

  // Set gyroscope full scale range
  // Range selects FS_SEL and AFS_SEL are 0 - 3, so 2-bit values are
  // left-shifted into positions 4:3

  // Set accelerometer full-scale range configuration
  // Get current ACCEL_CONFIG register value
  uint8_t c;
  read_byte(ACCEL_CONFIG, &c);
  // c = c & ~0xE0; // Clear self-test bits [7:5]
  c = c & ~0x06;  // Clear AFS bits [4:3]
  c = c | accel_scale << 1; // Set full scale range for the accelerometer
  c = c | 0x01; // Set enable accel DLPF for the accelerometer
  c = c | 0x18; // and set DLFPFCFG to 50.4 hz
  // Write new ACCEL_CONFIG register value
  write_byte(ACCEL_CONFIG, c);

  // Set accelerometer sample rate configuration
  // It is possible to get a 4 kHz sample rate from the accelerometer by
  // choosing 1 for accel_fchoice_b bit [3]; in this case the bandwidth is
  // 1.13 kHz
  write_byte(ACCEL_SMPLRT_DIV_2, 0x04);
  // The accelerometer, gyro, and thermometer are set to 1 kHz sample rates,
  // but all these rates are further reduced by a factor of 5 to 200 Hz because
  // of the GYRO_SMPLRT_DIV setting

  // Switch to user bank 0
  select_bank(USER_BANK_0);

  // Configure Interrupts and Bypass Enable
  // Set interrupt pin active high, push-pull, hold interrupt pin level HIGH
  // until interrupt cleared, clear on read of INT_STATUS, and enable
  // I2C_BYPASS_EN so additional chips can join the I2C bus and all can be
  // controlled by the Arduino as master.
  write_byte(INT_PIN_CFG, 0x22);
  // Enable data ready (bit 0) interrupt
  write_byte(INT_ENABLE_1, 0x01);
}

void
imu_get_accelerations(float* ax, float* ay, float* az)
{
  select_bank(USER_BANK_0);
  HAL_Delay(1);

  uint8_t raw_data[6];
  read_bytes(ACCEL_XOUT_H, raw_data, 6);

  int16_t accel[3];
  accel[0] = ((int16_t) raw_data[0] << 8) | raw_data[1];
  accel[1] = ((int16_t) raw_data[2] << 8) | raw_data[3];
  accel[2] = ((int16_t) raw_data[4] << 8) | raw_data[5];

  float accel_res = acceleration_resolution(accel_scale);
  *ax = (float)accel[0] * accel_res;
  *ay = (float)accel[1] * accel_res;
  *az = (float)accel[2] * accel_res;
}

void
imu_get_angular_velocities(float* gx, float* gy, float *gz)
{
  select_bank(USER_BANK_0);
  HAL_Delay(1);

  uint8_t raw_data[6];
  read_bytes(GYRO_XOUT_H, raw_data, 6);

  int16_t gyro[3];
  gyro[0] = ((int16_t)raw_data[0] << 8) | raw_data[1];
  gyro[1] = ((int16_t)raw_data[2] << 8) | raw_data[3];
  gyro[2] = ((int16_t)raw_data[4] << 8) | raw_data[5];

  float gyro_res = gyroscope_resolution(gyro_scale);
  *gx = (float)gyro[0] * gyro_res;
  *gy = (float)gyro[1] * gyro_res;
  *gz = (float)gyro[2] * gyro_res;
}

uint8_t imu_get_device_id(void)
{
  select_bank(USER_BANK_0);
  HAL_Delay(10);

  uint8_t spiData = 0x01;
  read_byte(0x00, &spiData);
  return spiData;
}

void
imu_calibrate(float * gyroBias, float * accelBias)
{
  uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
  uint16_t ii, packet_count, fifo_count;
  int32_t gyro_bias[3]  = {0, 0, 0}, accel_bias[3] = {0, 0, 0};

  // reset device
  // Write a one to bit 7 reset bit; toggle reset device
  write_byte(PWR_MGMT_1, 0x80);
  HAL_Delay(200);

  // get stable time source; Auto select clock source to be PLL gyroscope
  // reference if ready else use the internal oscillator, bits 2:0 = 001
  write_byte(PWR_MGMT_1, 0x01);
  write_byte(PWR_MGMT_2, 0x00);
  HAL_Delay(200);

  // Configure device for bias calculation
  // Disable all interrupts
  write_byte(INT_ENABLE, 0x00);
  // Disable FIFO
  write_byte(FIFO_EN_1, 0x00);
  write_byte(FIFO_EN_2, 0x00);
  // Disable FIFO and I2C master modes
  write_byte(USER_CTRL, 0x00);
  // Reset FIFO and DMP
  write_byte(USER_CTRL, 0x08);
  write_byte(FIFO_RST, 0x1F);
  HAL_Delay(10);
  write_byte(FIFO_RST, 0x00);
  HAL_Delay(15);

  // Set FIFO mode to snapshot
  write_byte(FIFO_MODE, 0x1F);
  // Switch to user bank 2
  select_bank(USER_BANK_2);
  // Configure ICM20948 gyro and accelerometer for bias calculation
  // Set low-pass filter to 188 Hz
  write_byte(GYRO_CONFIG_1, 0x01);
  // Set sample rate to 1 kHz
  write_byte(GYRO_SMPLRT_DIV, 0x00);
  // Set gyro full-scale to 250 degrees per second, maximum sensitivity
  write_byte(GYRO_CONFIG_1, 0x00);
  // Set accelerometer full-scale to 2 g, maximum sensitivity
  write_byte(ACCEL_CONFIG, 0x00);

  uint16_t  gyrosensitivity  = 131;   // = 131 LSB/degrees/sec
  uint16_t  accelsensitivity = 16384; // = 16384 LSB/g

  // Switch to user bank 0
  select_bank(USER_BANK_0);
  // Configure FIFO to capture accelerometer and gyro data for bias calculation
  write_byte(USER_CTRL, 0x40);  // Enable FIFO
  // Enable gyro and accelerometer sensors for FIFO  (max size 512 bytes in
  // ICM20948)
  write_byte(FIFO_EN_2, 0x1E);
  HAL_Delay(40);  // accumulate 40 samples in 40 milliseconds = 480 bytes

  // At end of sample accumulation, turn off FIFO sensor read
  // Disable gyro and accelerometer sensors for FIFO
  write_byte(FIFO_EN_2, 0x00);
  // Read FIFO sample count
  read_bytes(FIFO_COUNTH, &data[0], 2);
  fifo_count = ((uint16_t)data[0] << 8) | data[1];
  // How many sets of full gyro and accelerometer data for averaging
  packet_count = fifo_count/12;

  for (ii = 0; ii < packet_count; ii++)
  {
    int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};
    // Read data for averaging
    read_bytes(FIFO_R_W, &data[0], 12);
    // Form signed 16-bit integer for each sample in FIFO
    accel_temp[0] = (int16_t) (((int16_t)data[0] << 8) | data[1]  );
    accel_temp[1] = (int16_t) (((int16_t)data[2] << 8) | data[3]  );
    accel_temp[2] = (int16_t) (((int16_t)data[4] << 8) | data[5]  );
    gyro_temp[0]  = (int16_t) (((int16_t)data[6] << 8) | data[7]  );
    gyro_temp[1]  = (int16_t) (((int16_t)data[8] << 8) | data[9]  );
    gyro_temp[2]  = (int16_t) (((int16_t)data[10] << 8) | data[11]);

    // Sum individual signed 16-bit biases to get accumulated signed 32-bit
    // biases.
    accel_bias[0] += (int32_t) accel_temp[0];
    accel_bias[1] += (int32_t) accel_temp[1];
    accel_bias[2] += (int32_t) accel_temp[2];
    gyro_bias[0]  += (int32_t) gyro_temp[0];
    gyro_bias[1]  += (int32_t) gyro_temp[1];
    gyro_bias[2]  += (int32_t) gyro_temp[2];
  }
  // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
  accel_bias[0] /= (int32_t) packet_count;
  accel_bias[1] /= (int32_t) packet_count;
  accel_bias[2] /= (int32_t) packet_count;
  gyro_bias[0]  /= (int32_t) packet_count;
  gyro_bias[1]  /= (int32_t) packet_count;
  gyro_bias[2]  /= (int32_t) packet_count;

  // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
  if (accel_bias[2] > 0L)
  {
    accel_bias[2] -= (int32_t) accelsensitivity;
  }
  else
  {
    accel_bias[2] += (int32_t) accelsensitivity;
  }

  // Construct the gyro biases for push to the hardware gyro bias registers,
  // which are reset to zero upon device startup.
  // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input
  // format.
  data[0] = (-gyro_bias[0]/4  >> 8) & 0xFF;
  // Biases are additive, so change sign on calculated average gyro biases
  data[1] = (-gyro_bias[0]/4)       & 0xFF;
  data[2] = (-gyro_bias[1]/4  >> 8) & 0xFF;
  data[3] = (-gyro_bias[1]/4)       & 0xFF;
  data[4] = (-gyro_bias[2]/4  >> 8) & 0xFF;
  data[5] = (-gyro_bias[2]/4)       & 0xFF;

  // Switch to user bank 2
  select_bank(USER_BANK_2);

  // Push gyro biases to hardware registers
  write_byte(XG_OFFSET_H, data[0]);
  write_byte(XG_OFFSET_L, data[1]);
  write_byte(YG_OFFSET_H, data[2]);
  write_byte(YG_OFFSET_L, data[3]);
  write_byte(ZG_OFFSET_H, data[4]);
  write_byte(ZG_OFFSET_L, data[5]);

  // Output scaled gyro biases for display in the main program
  gyroBias[0] = (float) gyro_bias[0]/(float) gyrosensitivity;
  gyroBias[1] = (float) gyro_bias[1]/(float) gyrosensitivity;
  gyroBias[2] = (float) gyro_bias[2]/(float) gyrosensitivity;

  // Construct the accelerometer biases for push to the hardware accelerometer
  // bias registers. These registers contain factory trim values which must be
  // added to the calculated accelerometer biases; on boot up these registers
  // will hold non-zero values. In addition, bit 0 of the lower byte must be
  // preserved since it is used for temperature compensation calculations.
  // Accelerometer bias registers expect bias input as 2048 LSB per g, so that
  // the accelerometer biases calculated above must be divided by 8.

  // Switch to user bank 1
  select_bank(USER_BANK_1);
  // A place to hold the factory accelerometer trim biases
  int32_t accel_bias_reg[3] = {0, 0, 0};
  // Read factory accelerometer trim values
  read_bytes(XA_OFFSET_H, &data[0], 2);
  accel_bias_reg[0] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
  read_bytes(YA_OFFSET_H, &data[0], 2);
  accel_bias_reg[1] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
  read_bytes(ZA_OFFSET_H, &data[0], 2);
  accel_bias_reg[2] = (int32_t) (((int16_t)data[0] << 8) | data[1]);

  // Define mask for temperature compensation bit 0 of lower byte of
  // accelerometer bias registers
  uint32_t mask = 1uL;
  // Define array to hold mask bit for each accelerometer bias axis
  uint8_t mask_bit[3] = {0, 0, 0};

  for (ii = 0; ii < 3; ii++)
  {
    // If temperature compensation bit is set, record that fact in mask_bit
    if ((accel_bias_reg[ii] & mask))
    {
      mask_bit[ii] = 0x01;
    }
  }

  // Construct total accelerometer bias, including calculated average
  // accelerometer bias from above
  // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g
  // (16 g full scale)
  accel_bias_reg[0] -= (accel_bias[0]/8);
  accel_bias_reg[1] -= (accel_bias[1]/8);
  accel_bias_reg[2] -= (accel_bias[2]/8);

  data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
  data[1] = (accel_bias_reg[0])      & 0xFF;
  // preserve temperature compensation bit when writing back to accelerometer
  // bias registers
  data[1] = data[1] | mask_bit[0];
  data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
  data[3] = (accel_bias_reg[1])      & 0xFF;
  // Preserve temperature compensation bit when writing back to accelerometer
  // bias registers
  data[3] = data[3] | mask_bit[1];
  data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
  data[5] = (accel_bias_reg[2])      & 0xFF;
  // Preserve temperature compensation bit when writing back to accelerometer
  // bias registers
  data[5] = data[5] | mask_bit[2];

  // Apparently this is not working for the acceleration biases in the ICM-20948
  // Are we handling the temperature correction bit properly?
  // Push accelerometer biases to hardware registers
  /* write_byte(XA_OFFSET_H, data[0]); */
  /* write_byte(XA_OFFSET_L, data[1]); */
  /* write_byte(YA_OFFSET_H, data[2]); */
  /* write_byte(YA_OFFSET_L, data[3]); */
  /* write_byte(ZA_OFFSET_H, data[4]); */
  /* write_byte(ZA_OFFSET_L, data[5]); */

  // Output scaled accelerometer biases for display in the main program
  accelBias[0] = (float)accel_bias[0]/(float)accelsensitivity;
  accelBias[1] = (float)accel_bias[1]/(float)accelsensitivity;
  accelBias[2] = (float)accel_bias[2]/(float)accelsensitivity;
  // Switch to user bank 0
  select_bank(USER_BANK_0);
}
