#include <webots/robot.h>
#include <webots/distance_sensor.h>
#include <webots/motor.h>
#include <stdio.h>

#define TIME_STEP 64

void delay(int time_milisec) {
  double currentTime, initTime, TimeLeft;
  double timeValue = (double)time_milisec / 1000;

  initTime = wb_robot_get_time();
  TimeLeft = 0.0;

  while (TimeLeft < timeValue) {
    currentTime = wb_robot_get_time();
    TimeLeft = currentTime - initTime;
    wb_robot_step(TIME_STEP);
  }
}

int main() {
  wb_robot_init();

  // Motores
  WbDeviceTag left_motor_1 = wb_robot_get_device("front left wheel");
  WbDeviceTag right_motor_1 = wb_robot_get_device("front right wheel");
  WbDeviceTag left_motor_2 = wb_robot_get_device("back right wheel");
  WbDeviceTag right_motor_2 = wb_robot_get_device("back right wheel");

  // Sensor frontal
  WbDeviceTag sensf = wb_robot_get_device("so3");
  wb_distance_sensor_enable(sensf, TIME_STEP);

  // Configuração dos motores para modo velocidade
  wb_motor_set_position(left_motor_1, INFINITY);
  wb_motor_set_position(right_motor_1, INFINITY);
  wb_motor_set_position(left_motor_2, INFINITY);
  wb_motor_set_position(right_motor_2, INFINITY);

  double velocidade = 2.0;
  double sensf_value;

  while (wb_robot_step(TIME_STEP) != -1) {
    sensf_value = wb_distance_sensor_get_value(sensf);

    printf("Sensor frente: %.2f\n", sensf_value);
    fflush(stdout);

    // Se bater na parede (sensor dispara e está indo pra frente)
    if (sensf_value > 1000.0 && velocidade > 0) {
      // Parar
      wb_motor_set_velocity(left_motor_1, 0);
      wb_motor_set_velocity(left_motor_2, 0);
      wb_motor_set_velocity(right_motor_1, 0);
      wb_motor_set_velocity(right_motor_2, 0);
      delay(300);

      // Andar de ré
      velocidade = -2.0;
      wb_motor_set_velocity(left_motor_1, velocidade);
      wb_motor_set_velocity(left_motor_2, velocidade);
      wb_motor_set_velocity(right_motor_1, velocidade);
      wb_motor_set_velocity(right_motor_2, velocidade);
      delay(800);

      // Girar 180°
      wb_motor_set_velocity(left_motor_1, -2.0);
      wb_motor_set_velocity(left_motor_2, -2.0);
      wb_motor_set_velocity(right_motor_1, 2.0);
      wb_motor_set_velocity(right_motor_2, 2.0);
      delay(1000);

      // Voltar a andar pra frente
      velocidade = 2.0;
    }

    // Movimento padrão pra frente
    wb_motor_set_velocity(left_motor_1, velocidade);
    wb_motor_set_velocity(left_motor_2, velocidade);
    wb_motor_set_velocity(right_motor_1, velocidade);
    wb_motor_set_velocity(right_motor_2, velocidade);
  }

  wb_robot_cleanup();
  return 0;
}
