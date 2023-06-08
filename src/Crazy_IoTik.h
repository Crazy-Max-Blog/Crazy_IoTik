#ifndef Crazy_IoTik_h
#define Crazy_IoTik_h
#include <Arduino.h>

#include <Wire.h>

#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD

#define IoTik_motor_A 0
#define IoTik_motor_B 1


class IoTik_motor {
 public:
  uint8_t _i2caddr;
  
  TwoWire *_i2c;

  int pins[4] = {8, 9, 10, 11};
  IoTik_motor(uint8_t addr = 0x40);
  IoTik_motor(TwoWire *I2C, uint8_t addr = 0x40);
  void begin(void);
  void begin(int scl, int sda);
  void motor_setpower(int motor, float pwr, bool invert);
  void reset(void);
  void setPWMFreq(float freq);
  void setPWM(uint8_t num, uint16_t on, uint16_t off);
  void setPin(uint8_t num, uint16_t val, bool invert=false);

 private:

  uint8_t read8(uint8_t addr);
  void write8(uint8_t addr, uint8_t d);
  void initPWM();
};

#endif
