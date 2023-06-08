#include <Crazy_IoTik.h>
IoTik_motor motors(0x70);

void setup() {
 motors.begin(100); // Инициализация драйвера
}

void loop() {
  for (int i = 0; i <= 100; i++) { // запустить плавно оба мотора в одну сторону (вперед)
    motors.motor_setpower(IoTik_motor_A, i, false);
    motors.motor_setpower(IoTik_motor_B, i, true);
    delay(25);
  }
  for (int i = 100; i >= -100; i--) { // реверс моторов (остановка и езда назад)
    motors.motor_setpower(IoTik_motor_A, i, false);
    motors.motor_setpower(IoTik_motor_B, i, true);
    delay(25);
  }
  for (int i = -100; i <= 0; i++) { // остановка моторов
    motors.motor_setpower(IoTik_motor_A, i, false);
    motors.motor_setpower(IoTik_motor_B, i, true);
    delay(25);
  }
  for (int i = 0; i <= 100; i++) { // поворот платформы вокруг себя
    motors.motor_setpower(IoTik_motor_A, i, false);
    motors.motor_setpower(IoTik_motor_B, i, false);
    delay(25);
  }
  for (int i = 100; i >= -100; i--) { // реверс до остановки плюс поворот в другую сторону
    motors.motor_setpower(IoTik_motor_A, i, false);
    motors.motor_setpower(IoTik_motor_B, i, false);
    delay(25);
  }
  for (int i = -100; i <= 0; i++) { // остановка платформы
    motors.motor_setpower(IoTik_motor_A, i, false);
    motors.motor_setpower(IoTik_motor_B, i, false);
    delay(25);
  }
}