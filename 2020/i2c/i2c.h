/*
 * i2c.h
 *
 */

void initI2C(void);
void readI2C(uint8_t slave_addr, uint8_t reg, int *data);
void I2C0_Send(uint8_t slave_addr, uint8_t num_of_args, ...);
void writeI2C(uint8_t slave_addr, uint8_t reg, uint8_t data);
void I2C0_Write (uint8_t addr, uint8_t N, ...);