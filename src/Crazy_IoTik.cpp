#include "Crazy_IoTik.h" // подключаем заголовок обязательно
#include <Wire.h>
/*void testClass::printer(printModes value) {
 switch (value) {
 case KEK: Serial.println("kek");
  break;
 case LOL: Serial.println("lol");
  break;
 case AZAZA: Serial.println("azaza");
  break;
 case HELLO: Serial.println("hello");
  break;
  
 }  
}*/
IoTik_motor::IoTik_motor(uint8_t addr) {
  _i2caddr = addr;

#if defined(ARDUINO_SAM_DUE)
  _i2c = &Wire1;
#else
  _i2c = &Wire;
#endif
}

IoTik_motor::IoTik_motor(TwoWire *i2c, uint8_t addr) {
  _i2c = i2c;
  _i2caddr = addr;
}

void IoTik_motor::begin(void) {
  _i2c->begin();
  reset();
  // set a default frequency
  setPWMFreq(1000);
  //pins[] = {8, 9, 10, 11};
  initPWM();
}

void IoTik_motor::begin(int scl, int sda) {
  _i2c->begin(scl, sda);
  reset();
  // set a default frequency
  setPWMFreq(1000);
  //pins[] = {8, 9, 10, 11};
  initPWM();
}

void IoTik_motor::initPWM() {
  setPWM(8,  0, 4096);
  setPWM(9,  0, 4096);
  setPWM(10, 0, 4096);
  setPWM(11, 0, 4096);
}

void IoTik_motor::motor_setpower(int motor, float pwr, bool invert)
{
  if (invert)
  {
    pwr = -pwr;
  }
  if (pwr < -100)
  {
    pwr = -100;
  }
  if (pwr > 100)
  {
    pwr = 100;
  }
  int pwmvalue = fabs(pwr) * 40.95;
  if (pwr < 0)
  {
    setPWM(pins[motor*2], 0, 4096);
    setPWM(pins[motor*2]+1, 0, pwmvalue);
  }
  else
  {
    setPWM(pins[motor*2]+1, 0, 4096);
    setPWM(pins[motor*2], 0, pwmvalue);
  }
}

void IoTik_motor::reset(void) {
  write8(PCA9685_MODE1, 0x80);
  delay(10);
}

void IoTik_motor::setPWMFreq(float freq) {
#ifdef ENABLE_DEBUG_OUTPUT
  Serial.print("Attempting to set freq ");
  Serial.println(freq);
#endif

  freq *= 0.9;  // Correct for overshoot in the frequency setting (see issue #11).
  float prescaleval = 25000000;
  prescaleval /= 4096;
  prescaleval /= freq;
  prescaleval -= 1;

#ifdef ENABLE_DEBUG_OUTPUT
  Serial.print("Estimated pre-scale: "); Serial.println(prescaleval);
#endif

  uint8_t prescale = floor(prescaleval + 0.5);
#ifdef ENABLE_DEBUG_OUTPUT
  Serial.print("Final pre-scale: "); Serial.println(prescale);
#endif
  
  uint8_t oldmode = read8(PCA9685_MODE1);
  uint8_t newmode = (oldmode&0x7F) | 0x10; // sleep
  write8(PCA9685_MODE1, newmode); // go to sleep
  write8(PCA9685_PRESCALE, prescale); // set the prescaler
  write8(PCA9685_MODE1, oldmode);
  delay(5);
  write8(PCA9685_MODE1, oldmode | 0xa0);  //  This sets the MODE1 register to turn on auto increment.

#ifdef ENABLE_DEBUG_OUTPUT
  Serial.print("Mode now 0x"); Serial.println(read8(PCA9685_MODE1), HEX);
#endif
}

void IoTik_motor::setPWM(uint8_t num, uint16_t on, uint16_t off) {
#ifdef ENABLE_DEBUG_OUTPUT
  Serial.print("Setting PWM "); Serial.print(num); Serial.print(": "); Serial.print(on); Serial.print("->"); Serial.println(off);
#endif

  _i2c->beginTransmission(_i2caddr);
  _i2c->write(LED0_ON_L+4*num);
  _i2c->write(on);
  _i2c->write(on>>8);
  _i2c->write(off);
  _i2c->write(off>>8);
  _i2c->endTransmission();
}

uint8_t IoTik_motor::read8(uint8_t addr) {
  _i2c->beginTransmission(_i2caddr);
  _i2c->write(addr);
  _i2c->endTransmission();

  _i2c->requestFrom((uint8_t)_i2caddr, (uint8_t)1);
  return _i2c->read();
}

void IoTik_motor::write8(uint8_t addr, uint8_t d) {
  _i2c->beginTransmission(_i2caddr);
  _i2c->write(addr);
  _i2c->write(d);
  _i2c->endTransmission();
}