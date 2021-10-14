//#define ICM20948_ADDRESS (0x69)     // AD0 =1
#define ICM20948_ADDRESS (0x68)     // AD0 =0
#define Reset_Value                     0x00
#define READ                            0x80
#define WRITE                           0x00
/**************************************************************************//**
* @name Error Codes
* @{
******************************************************************************/
#define OK                                  0x0000                      /**< No errors          */
#define ERROR                               0x0001                      /**< Error              */
/**@}*/

/**************************************************************************//**
* @name ICM20948 register banks
* @{
******************************************************************************/
#define ICM20948_BANK_0                     0x00                    /**< Register bank 0    */
#define ICM20948_BANK_1                     0x10                    /**< Register bank 1    */
#define ICM20948_BANK_2                     0x20                    /**< Register bank 2    */
#define ICM20948_BANK_3                     0x30                    /**< Register bank 3    */
/**@}*/

/**************************************************************************//**
* @name Register and associated bit definitions
* @{
******************************************************************************/
/***********************/
/* Bank 0 register map */
/***********************/
#define ICM20948_REG_WHO_AM_I               (ICM20948_BANK_0 | 0x00)    /**< Device ID register                                     */

#define ICM20948_REG_USER_CTRL              (ICM20948_BANK_0 | 0x03)    /**< User control register                                  */
#define ICM20948_BIT_DMP_EN                 0x80                        /**< DMP enable bit                                         */
#define ICM20948_BIT_FIFO_EN                0x40                        /**< FIFO enable bit                                        */
#define ICM20948_BIT_I2C_MST_EN             0x20                        /**< I2C master I/F enable bit                              */
#define ICM20948_BIT_I2C_IF_DIS             0x10                        /**< Disable I2C, enable SPI bit                            */
#define ICM20948_BIT_DMP_RST                0x08                        /**< DMP module reset bit                                   */
#define ICM20948_BIT_DIAMOND_DMP_RST        0x04                        /**< SRAM module reset bit                                  */

#define ICM20948_REG_LP_CONFIG              (ICM20948_BANK_0 | 0x05)    /**< Low Power mode config register                         */
#define ICM20948_BIT_I2C_MST_CYCLE          0x40                        /**< I2C master cycle mode enable                           */
#define ICM20948_BIT_ACCEL_CYCLE            0x20                        /**< Accelerometer cycle mode enable                        */
#define ICM20948_BIT_GYRO_CYCLE             0x10                        /**< Gyroscope cycle mode enable                            */

#define ICM20948_REG_PWR_MGMT_1             (ICM20948_BANK_0 | 0x06)    /**< Power Management 1 register                            */
#define ICM20948_BIT_H_RESET                0x80                        /**< Device reset bit                                       */
#define ICM20948_BIT_SLEEP                  0x40                        /**< Sleep mode enable bit                                  */
#define ICM20948_BIT_LP_EN                  0x20                        /**< Low Power feature enable bit                           */
#define ICM20948_BIT_TEMP_DIS               0x08                        /**< Temperature sensor disable bit                         */
#define ICM20948_BIT_CLK_PLL                0x01                        /**< Auto clock source selection setting                    */

#define ICM20948_REG_PWR_MGMT_2             (ICM20948_BANK_0 | 0x07)    /**< Power Management 2 register                            */
#define ICM20948_BIT_PWR_ACCEL_STBY         0x38                        /**< Disable accelerometer                                  */
#define ICM20948_BIT_PWR_GYRO_STBY          0x07                        /**< Disable gyroscope                                      */
#define ICM20948_BIT_PWR_ALL_OFF            0x7F                        /**< Disable both accel and gyro                            */

#define ICM20948_REG_INT_PIN_CFG            (ICM20948_BANK_0 | 0x0F)    /**< Interrupt Pin Configuration register                   */
#define ICM20948_BIT_INT_ACTL               0x80                        /**< Active low setting bit                                 */
#define ICM20948_BIT_INT_OPEN               0x40                        /**< Open collector configuration bit                       */
#define ICM20948_BIT_INT_LATCH_EN           0x20                        /**< Latch enable bit                                       */

#define ICM20948_REG_INT_ENABLE             (ICM20948_BANK_0 | 0x10)    /**< Interrupt Enable register                              */
#define ICM20948_BIT_WOM_INT_EN             0x08                        /**< Wake-up On Motion enable bit                           */

#define ICM20948_REG_INT_ENABLE_1           (ICM20948_BANK_0 | 0x11)    /**< Interrupt Enable 1 register                            */
#define ICM20948_BIT_RAW_DATA_0_RDY_EN      0x01                        /**< Raw data ready interrupt enable bit                    */

#define ICM20948_REG_INT_ENABLE_2           (ICM20948_BANK_0 | 0x12)    /**< Interrupt Enable 2 register                            */
#define ICM20948_BIT_FIFO_OVERFLOW_EN_0     0x01                        /**< FIFO overflow interrupt enable bit                     */

#define ICM20948_REG_INT_ENABLE_3           (ICM20948_BANK_0 | 0x13)    /**< Interrupt Enable 2 register                            */

#define ICM20948_REG_INT_STATUS             (ICM20948_BANK_0 | 0x19)    /**< Interrupt Status register                              */
#define ICM20948_BIT_WOM_INT                0x08                        /**< Wake-up on motion interrupt bit                        */
#define ICM20948_BIT_PLL_RDY                0x04                        /**< PLL ready interrupt bit                                */

#define ICM20948_REG_INT_STATUS_1           (ICM20948_BANK_0 | 0x1A)    /**< Interrupt Status 1 register                            */
#define ICM20948_BIT_RAW_DATA_0_RDY_INT     0x01                        /**< Raw data ready interrupt bit                           */

#define ICM20948_REG_INT_STATUS_2           (ICM20948_BANK_0 | 0x1B)    /**< Interrupt Status 2 register                            */

#define ICM20948_REG_ACCEL_XOUT_H_SH        (ICM20948_BANK_0 | 0x2D)    /**< Accelerometer X-axis data high byte                    */
#define ICM20948_REG_ACCEL_XOUT_L_SH        (ICM20948_BANK_0 | 0x2E)    /**< Accelerometer X-axis data low byte                     */
#define ICM20948_REG_ACCEL_YOUT_H_SH        (ICM20948_BANK_0 | 0x2F)    /**< Accelerometer Y-axis data high byte                    */
#define ICM20948_REG_ACCEL_YOUT_L_SH        (ICM20948_BANK_0 | 0x30)    /**< Accelerometer Y-axis data low byte                     */
#define ICM20948_REG_ACCEL_ZOUT_H_SH        (ICM20948_BANK_0 | 0x31)    /**< Accelerometer Z-axis data high byte                    */
#define ICM20948_REG_ACCEL_ZOUT_L_SH        (ICM20948_BANK_0 | 0x32)    /**< Accelerometer Z-axis data low byte                     */

#define ICM20948_REG_GYRO_XOUT_H_SH         (ICM20948_BANK_0 | 0x33)    /**< Gyroscope X-axis data high byte                        */
#define ICM20948_REG_GYRO_XOUT_L_SH         (ICM20948_BANK_0 | 0x34)    /**< Gyroscope X-axis data low byte                         */
#define ICM20948_REG_GYRO_YOUT_H_SH         (ICM20948_BANK_0 | 0x35)    /**< Gyroscope Y-axis data high byte                        */
#define ICM20948_REG_GYRO_YOUT_L_SH         (ICM20948_BANK_0 | 0x36)    /**< Gyroscope Y-axis data low byte                         */
#define ICM20948_REG_GYRO_ZOUT_H_SH         (ICM20948_BANK_0 | 0x37)    /**< Gyroscope Z-axis data high byte                        */
#define ICM20948_REG_GYRO_ZOUT_L_SH         (ICM20948_BANK_0 | 0x38)    /**< Gyroscope Z-axis data low byte                         */

#define ICM20948_REG_TEMPERATURE_H          (ICM20948_BANK_0 | 0x39)    /**< Temperature data high byte                             */
#define ICM20948_REG_TEMPERATURE_L          (ICM20948_BANK_0 | 0x3A)    /**< Temperature data low byte                              */

#define ICM20948_REG_EXT_SLV_SENS_DATA_00   (ICM20948_BANK_0 | 0x3B)    /**< First sensor data byte read from external I2C devices through I2C master interface */

#define ICM20948_REG_TEMP_CONFIG            (ICM20948_BANK_0 | 0x53)    /**< Temperature Configuration register                     */

#define ICM20948_REG_FIFO_EN_1              (ICM20948_BANK_0 | 0x66)    /**< FIFO Enable 1 register                                 */

#define ICM20948_REG_FIFO_EN_2              (ICM20948_BANK_0 | 0x67)    /**< FIFO Enable 2 register                                 */
#define ICM20948_BIT_ACCEL_FIFO_EN          0x10                        /**< Enable writing acceleration data to FIFO bit           */
#define ICM20948_BITS_GYRO_FIFO_EN          0x0E                        /**< Enable writing gyroscope data to FIFO bit              */

#define ICM20948_REG_FIFO_RST               (ICM20948_BANK_0 | 0x68)    /**< FIFO Reset register                                    */
#define ICM20948_REG_FIFO_MODE              (ICM20948_BANK_0 | 0x69)    /**< FIFO Mode register                                     */

#define ICM20948_REG_FIFO_COUNT_H           (ICM20948_BANK_0 | 0x70)    /**< FIFO data count high byte                              */
#define ICM20948_REG_FIFO_COUNT_L           (ICM20948_BANK_0 | 0x71)    /**< FIFO data count low byte                               */
#define ICM20948_REG_FIFO_R_W               (ICM20948_BANK_0 | 0x72)    /**< FIFO Read/Write register                               */

#define ICM20948_REG_DATA_RDY_STATUS        (ICM20948_BANK_0 | 0x74)    /**< Data Ready Status register                             */
#define ICM20948_BIT_RAW_DATA_0_RDY         0x01                        /**< Raw Data Ready bit                                     */

#define ICM20948_REG_FIFO_CFG               (ICM20948_BANK_0 | 0x76)    /**< FIFO Configuration register                            */
#define ICM20948_BIT_MULTI_FIFO_CFG         0x01                        /**< Interrupt status for each sensor is required           */
#define ICM20948_BIT_SINGLE_FIFO_CFG        0x00                        /**< Interrupt status for only a single sensor is required  */

/***********************/
/* Bank 1 register map */
/***********************/
#define ICM20948_REG_XA_OFFSET_H            (ICM20948_BANK_1 | 0x14)    /**< Acceleration sensor X-axis offset cancellation high byte   */
#define ICM20948_REG_XA_OFFSET_L            (ICM20948_BANK_1 | 0x15)    /**< Acceleration sensor X-axis offset cancellation low byte    */
#define ICM20948_REG_YA_OFFSET_H            (ICM20948_BANK_1 | 0x17)    /**< Acceleration sensor Y-axis offset cancellation high byte   */
#define ICM20948_REG_YA_OFFSET_L            (ICM20948_BANK_1 | 0x18)    /**< Acceleration sensor Y-axis offset cancellation low byte    */
#define ICM20948_REG_ZA_OFFSET_H            (ICM20948_BANK_1 | 0x1A)    /**< Acceleration sensor Z-axis offset cancellation high byte   */
#define ICM20948_REG_ZA_OFFSET_L            (ICM20948_BANK_1 | 0x1B)    /**< Acceleration sensor Z-axis offset cancellation low byte    */

#define ICM20948_REG_TIMEBASE_CORR_PLL      (ICM20948_BANK_1 | 0x28)    /**< PLL Timebase Correction register                           */

/***********************/
/* Bank 2 register map */
/***********************/
#define ICM20948_REG_GYRO_SMPLRT_DIV        (ICM20948_BANK_2 | 0x00)    /**< Gyroscope Sample Rate Divider register     */

#define ICM20948_REG_GYRO_CONFIG_1          (ICM20948_BANK_2 | 0x01)    /**< Gyroscope Configuration 1 register         */
#define ICM20948_BIT_GYRO_FCHOICE           0x01                        /**< Gyro Digital Low-Pass Filter enable bit    */
#define ICM20948_SHIFT_GYRO_FS_SEL          0x01                        /**< Gyro Full Scale Select bit shift           */
#define ICM20948_SHIFT_GYRO_DLPCFG          0x03                        /**< Gyro DLPF Config bit shift                 */
#define ICM20948_MASK_GYRO_FULLSCALE        0x06                        /**< Gyro Full Scale Select bit mask            */
#define ICM20948_MASK_GYRO_BW               0x39                        /**< Gyro Bandwidth Select bit mask             */
#define ICM20948_GYRO_FULLSCALE_250DPS      (0x00 << ICM20948_SHIFT_GYRO_FS_SEL)    /**< Gyro Full Scale = 250 deg/s  */
#define ICM20948_GYRO_FULLSCALE_500DPS      (0x01 << ICM20948_SHIFT_GYRO_FS_SEL)    /**< Gyro Full Scale = 500 deg/s  */
#define ICM20948_GYRO_FULLSCALE_1000DPS     (0x02 << ICM20948_SHIFT_GYRO_FS_SEL)    /**< Gyro Full Scale = 1000 deg/s */
#define ICM20948_GYRO_FULLSCALE_2000DPS     (0x03 << ICM20948_SHIFT_GYRO_FS_SEL)    /**< Gyro Full Scale = 2000 deg/s */
#define ICM20948_GYRO_BW_12100HZ            (0x00 << ICM20948_SHIFT_GYRO_DLPCFG)                                    /**< Gyro Bandwidth = 12100 Hz */
#define ICM20948_GYRO_BW_360HZ              ( (0x07 << ICM20948_SHIFT_GYRO_DLPCFG) | ICM20948_BIT_GYRO_FCHOICE)     /**< Gyro Bandwidth = 360 Hz   */
#define ICM20948_GYRO_BW_200HZ              ( (0x00 << ICM20948_SHIFT_GYRO_DLPCFG) | ICM20948_BIT_GYRO_FCHOICE)     /**< Gyro Bandwidth = 200 Hz   */
#define ICM20948_GYRO_BW_150HZ              ( (0x01 << ICM20948_SHIFT_GYRO_DLPCFG) | ICM20948_BIT_GYRO_FCHOICE)     /**< Gyro Bandwidth = 150 Hz   */
#define ICM20948_GYRO_BW_120HZ              ( (0x02 << ICM20948_SHIFT_GYRO_DLPCFG) | ICM20948_BIT_GYRO_FCHOICE)     /**< Gyro Bandwidth = 120 Hz   */
#define ICM20948_GYRO_BW_51HZ               ( (0x03 << ICM20948_SHIFT_GYRO_DLPCFG) | ICM20948_BIT_GYRO_FCHOICE)     /**< Gyro Bandwidth = 51 Hz    */
#define ICM20948_GYRO_BW_24HZ               ( (0x04 << ICM20948_SHIFT_GYRO_DLPCFG) | ICM20948_BIT_GYRO_FCHOICE)     /**< Gyro Bandwidth = 24 Hz    */
#define ICM20948_GYRO_BW_12HZ               ( (0x05 << ICM20948_SHIFT_GYRO_DLPCFG) | ICM20948_BIT_GYRO_FCHOICE)     /**< Gyro Bandwidth = 12 Hz    */
#define ICM20948_GYRO_BW_6HZ                ( (0x06 << ICM20948_SHIFT_GYRO_DLPCFG) | ICM20948_BIT_GYRO_FCHOICE)     /**< Gyro Bandwidth = 6 Hz     */

#define ICM20948_REG_GYRO_CONFIG_2          (ICM20948_BANK_2 | 0x02)    /**< Gyroscope Configuration 2 register                     */
#define ICM20948_BIT_GYRO_CTEN              0x38                        /**< Gyroscope Self-Test Enable bits                        */

#define ICM20948_REG_XG_OFFS_USRH           (ICM20948_BANK_2 | 0x03)    /**< Gyroscope sensor X-axis offset cancellation high byte  */
#define ICM20948_REG_XG_OFFS_USRL           (ICM20948_BANK_2 | 0x04)    /**< Gyroscope sensor X-axis offset cancellation low byte   */
#define ICM20948_REG_YG_OFFS_USRH           (ICM20948_BANK_2 | 0x05)    /**< Gyroscope sensor Y-axis offset cancellation high byte  */
#define ICM20948_REG_YG_OFFS_USRL           (ICM20948_BANK_2 | 0x06)    /**< Gyroscope sensor Y-axis offset cancellation low byte   */
#define ICM20948_REG_ZG_OFFS_USRH           (ICM20948_BANK_2 | 0x07)    /**< Gyroscope sensor Z-axis offset cancellation high byte  */
#define ICM20948_REG_ZG_OFFS_USRL           (ICM20948_BANK_2 | 0x08)    /**< Gyroscope sensor Z-axis offset cancellation low byte   */

#define ICM20948_REG_ODR_ALIGN_EN           (ICM20948_BANK_2 | 0x09)    /**< Output Data Rate start time alignment                  */

#define ICM20948_REG_ACCEL_SMPLRT_DIV_1     (ICM20948_BANK_2 | 0x10)    /**< Acceleration Sensor Sample Rate Divider 1 register     */
#define ICM20948_REG_ACCEL_SMPLRT_DIV_2     (ICM20948_BANK_2 | 0x11)    /**< Acceleration Sensor Sample Rate Divider 2 register     */

#define ICM20948_REG_ACCEL_INTEL_CTRL       (ICM20948_BANK_2 | 0x12)    /**< Accelerometer Hardware Intelligence Control register   */
#define ICM20948_BIT_ACCEL_INTEL_EN         0x02                        /**< Wake-up On Motion enable bit                           */
#define ICM20948_BIT_ACCEL_INTEL_MODE       0x01                        /**< WOM algorithm selection bit                            */

#define ICM20948_REG_ACCEL_WOM_THR          (ICM20948_BANK_2 | 0x13)    /**< Wake-up On Motion Threshold register                   */

#define ICM20948_REG_ACCEL_CONFIG           (ICM20948_BANK_2 | 0x14)    /**< Accelerometer Configuration register                   */
#define ICM20948_BIT_ACCEL_FCHOICE          0x01                        /**< Accel Digital Low-Pass Filter enable bit               */
#define ICM20948_SHIFT_ACCEL_FS             0x01                        /**< Accel Full Scale Select bit shift                      */
#define ICM20948_SHIFT_ACCEL_DLPCFG         0x03                        /**< Accel DLPF Config bit shift                            */
#define ICM20948_MASK_ACCEL_FULLSCALE       0x06                        /**< Accel Full Scale Select bit mask                       */
#define ICM20948_MASK_ACCEL_BW              0x39                        /**< Accel Bandwidth Select bit mask                        */
#define ICM20948_ACCEL_FULLSCALE_2G         (0x00 << ICM20948_SHIFT_ACCEL_FS)   /**< Accel Full Scale = 2 g     */
#define ICM20948_ACCEL_FULLSCALE_4G         (0x01 << ICM20948_SHIFT_ACCEL_FS)   /**< Accel Full Scale = 4 g     */
#define ICM20948_ACCEL_FULLSCALE_8G         (0x02 << ICM20948_SHIFT_ACCEL_FS)   /**< Accel Full Scale = 8 g     */
#define ICM20948_ACCEL_FULLSCALE_16G        (0x03 << ICM20948_SHIFT_ACCEL_FS)   /**< Accel Full Scale = 16 g    */
#define ICM20948_ACCEL_BW_1210HZ            (0x00 << ICM20948_SHIFT_ACCEL_DLPCFG)                                    /**< Accel Bandwidth = 1210 Hz  */
#define ICM20948_ACCEL_BW_470HZ             ( (0x07 << ICM20948_SHIFT_ACCEL_DLPCFG) | ICM20948_BIT_ACCEL_FCHOICE)    /**< Accel Bandwidth = 470 Hz   */
#define ICM20948_ACCEL_BW_246HZ             ( (0x00 << ICM20948_SHIFT_ACCEL_DLPCFG) | ICM20948_BIT_ACCEL_FCHOICE)    /**< Accel Bandwidth = 246 Hz   */
#define ICM20948_ACCEL_BW_111HZ             ( (0x02 << ICM20948_SHIFT_ACCEL_DLPCFG) | ICM20948_BIT_ACCEL_FCHOICE)    /**< Accel Bandwidth = 111 Hz   */
#define ICM20948_ACCEL_BW_50HZ              ( (0x03 << ICM20948_SHIFT_ACCEL_DLPCFG) | ICM20948_BIT_ACCEL_FCHOICE)    /**< Accel Bandwidth = 50 Hz    */
#define ICM20948_ACCEL_BW_24HZ              ( (0x04 << ICM20948_SHIFT_ACCEL_DLPCFG) | ICM20948_BIT_ACCEL_FCHOICE)    /**< Accel Bandwidth = 24 Hz    */
#define ICM20948_ACCEL_BW_12HZ              ( (0x05 << ICM20948_SHIFT_ACCEL_DLPCFG) | ICM20948_BIT_ACCEL_FCHOICE)    /**< Accel Bandwidth = 12 Hz    */
#define ICM20948_ACCEL_BW_6HZ               ( (0x06 << ICM20948_SHIFT_ACCEL_DLPCFG) | ICM20948_BIT_ACCEL_FCHOICE)    /**< Accel Bandwidth = 6 Hz     */

#define ICM20948_REG_ACCEL_CONFIG_2         (ICM20948_BANK_2 | 0x15)    /**< Accelerometer Configuration 2 register             */
#define ICM20948_BIT_ACCEL_CTEN             0x1C                        /**< Accelerometer Self-Test Enable bits                */

/***********************/
/* Bank 3 register map */
/***********************/
#define ICM20948_REG_I2C_MST_ODR_CONFIG     (ICM20948_BANK_3 | 0x00)    /**< I2C Master Output Data Rate Configuration register */

#define ICM20948_REG_I2C_MST_CTRL           (ICM20948_BANK_3 | 0x01)    /**< I2C Master Control register                        */
#define ICM20948_BIT_I2C_MST_P_NSR          0x10                        /**< Stop between reads enabling bit                    */
#define ICM20948_I2C_MST_CTRL_CLK_400KHZ    0x07                        /**< I2C_MST_CLK = 345.6 kHz (for 400 kHz Max)          */

#define ICM20948_REG_I2C_MST_DELAY_CTRL     (ICM20948_BANK_3 | 0x02)    /**< I2C Master Delay Control register                  */
#define ICM20948_BIT_SLV0_DLY_EN            0x01                        /**< I2C Slave0 Delay Enable bit                        */
#define ICM20948_BIT_SLV1_DLY_EN            0x02                        /**< I2C Slave1 Delay Enable bit                        */
#define ICM20948_BIT_SLV2_DLY_EN            0x04                        /**< I2C Slave2 Delay Enable bit                        */
#define ICM20948_BIT_SLV3_DLY_EN            0x08                        /**< I2C Slave3 Delay Enable bit                        */

#define ICM20948_REG_I2C_SLV0_ADDR          (ICM20948_BANK_3 | 0x03)    /**< I2C Slave0 Physical Address register               */
#define ICM20948_REG_I2C_SLV0_REG           (ICM20948_BANK_3 | 0x04)    /**< I2C Slave0 Register Address register               */
#define ICM20948_REG_I2C_SLV0_CTRL          (ICM20948_BANK_3 | 0x05)    /**< I2C Slave0 Control register                        */
#define ICM20948_REG_I2C_SLV0_DO            (ICM20948_BANK_3 | 0x06)    /**< I2C Slave0 Data Out register                       */

#define ICM20948_REG_I2C_SLV1_ADDR          (ICM20948_BANK_3 | 0x07)    /**< I2C Slave1 Physical Address register               */
#define ICM20948_REG_I2C_SLV1_REG           (ICM20948_BANK_3 | 0x08)    /**< I2C Slave1 Register Address register               */
#define ICM20948_REG_I2C_SLV1_CTRL          (ICM20948_BANK_3 | 0x09)    /**< I2C Slave1 Control register                        */
#define ICM20948_REG_I2C_SLV1_DO            (ICM20948_BANK_3 | 0x0A)    /**< I2C Slave1 Data Out register                       */

#define ICM20948_REG_I2C_SLV2_ADDR          (ICM20948_BANK_3 | 0x0B)    /**< I2C Slave2 Physical Address register               */
#define ICM20948_REG_I2C_SLV2_REG           (ICM20948_BANK_3 | 0x0C)    /**< I2C Slave2 Register Address register               */
#define ICM20948_REG_I2C_SLV2_CTRL          (ICM20948_BANK_3 | 0x0D)    /**< I2C Slave2 Control register                        */
#define ICM20948_REG_I2C_SLV2_DO            (ICM20948_BANK_3 | 0x0E)    /**< I2C Slave2 Data Out register                       */

#define ICM20948_REG_I2C_SLV3_ADDR          (ICM20948_BANK_3 | 0x0F)    /**< I2C Slave3 Physical Address register               */
#define ICM20948_REG_I2C_SLV3_REG           (ICM20948_BANK_3 | 0x10)    /**< I2C Slave3 Register Address register               */
#define ICM20948_REG_I2C_SLV3_CTRL          (ICM20948_BANK_3 | 0x11)    /**< I2C Slave3 Control register                        */
#define ICM20948_REG_I2C_SLV3_DO            (ICM20948_BANK_3 | 0x12)    /**< I2C Slave3 Data Out register                       */

#define ICM20948_REG_I2C_SLV4_ADDR          (ICM20948_BANK_3 | 0x13)    /**< I2C Slave4 Physical Address register               */
#define ICM20948_REG_I2C_SLV4_REG           (ICM20948_BANK_3 | 0x14)    /**< I2C Slave4 Register Address register               */
#define ICM20948_REG_I2C_SLV4_CTRL          (ICM20948_BANK_3 | 0x15)    /**< I2C Slave4 Control register                        */
#define ICM20948_REG_I2C_SLV4_DO            (ICM20948_BANK_3 | 0x16)    /**< I2C Slave4 Data Out register                       */
#define ICM20948_REG_I2C_SLV4_DI            (ICM20948_BANK_3 | 0x17)    /**< I2C Slave4 Data In register                        */

#define ICM20948_BIT_I2C_SLV_READ           0x80                        /**< I2C Slave Read bit                                 */
#define ICM20948_BIT_I2C_SLV_EN             0x80                        /**< I2C Slave Enable bit                               */
#define ICM20948_BIT_I2C_BYTE_SW            0x40                        /**< I2C Slave Byte Swap enable bit                     */
#define ICM20948_BIT_I2C_REG_DIS            0x20                        /**< I2C Slave Do Not Write Register Value bit          */
#define ICM20948_BIT_I2C_GRP                0x10                        /**< I2C Slave Group bit                                */
#define ICM20948_BIT_I2C_READ               0x80                        /**< I2C Slave R/W bit                                  */

/* Register common for all banks */
#define ICM20948_REG_BANK_SEL               0x7F                        /**< Bank Select register                               */

#define ICM20948_DEVICE_ID                  0xEA                        /**< Device ID value                                    */

/*****************************/
/* AK09916 register map */
/*****************************/
#define AK09916_REG_WHO_AM_I                0x01                        /**< AK09916 Device ID register             */
#define AK09916_DEVICE_ID                   0x09                        /**< AK09916 Device ID value                */

#define AK09916_REG_STATUS_1                0x10                        /**< Status 1 register                      */
#define AK09916_BIT_DRDY                    0x01                        /**< Data Ready bit                         */
#define AK09916_BIT_DOR                     0x02                        /**< Data Overrun bit                       */

#define AK09916_REG_HXL                     0x11                        /**< Magnetometer X-axis data lower byte    */
#define AK09916_REG_HXH                     0x12                        /**< Magnetometer X-axis data higher byte   */
#define AK09916_REG_HYL                     0x13                        /**< Magnetometer Y-axis data lower byte    */
#define AK09916_REG_HYH                     0x14                        /**< Magnetometer Y-axis data higher byte   */
#define AK09916_REG_HZL                     0x15                        /**< Magnetometer Z-axis data lower byte    */
#define AK09916_REG_HZH                     0x16                        /**< Magnetometer Z-axis data higher byte   */

#define AK09916_REG_STATUS_2                0x18                        /**< Status 2 register                      */

#define AK09916_REG_CONTROL_2               0x31                        /**< Control 2 register                     */
#define AK09916_BIT_MODE_POWER_DOWN         0x00                        /**< Power-down                             */
#define AK09916_MODE_SINGLE                 0x01                        /**< Magnetometer takes one measurement     */
#define AK09916_MODE_10HZ                   0x02                        /**< Magnetometer Measurement Rate = 10HZ   */
#define AK09916_MODE_20HZ                   0x04                        /**< Magnetometer Measurement Rate = 20HZ   */
#define AK09916_MODE_50HZ                   0x06                        /**< Magnetometer Measurement Rate = 50HZ   */
#define AK09916_MODE_100HZ                  0x08                        /**< Magnetometer Measurement Rate = 100HZ  */
#define AK09916_MODE_ST                     0x16                        /**< Self-test                              */

#define AK09916_REG_CONTROL_3               0x32                        /**< Control 3 register                     */
#define AK09916_BIT_SRST                    0x01                        /**< Soft Reset bit                         */

#define AK09916_REG_WHO_AM_I                0x01                        /**< AK09916 Device ID register             */
#define AK09916_BIT_I2C_SLV_ADDR            0x0C                        /**< AK09916 I2C Slave Address              */
/**@}*/

/** @endcond */
