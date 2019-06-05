#ifndef __RPI_I2C_H__
#define __RPI_I2C_H__

int i2c_write(unsigned addr, uint8_t data[], unsigned nbytes);
int i2c_read(unsigned addr, uint8_t data[], unsigned nbytes);
void i2c_init(void);

int i2c_initialized(void);

// call N times, runs i2c_init() once.
void i2c_init_once(void);

#endif
