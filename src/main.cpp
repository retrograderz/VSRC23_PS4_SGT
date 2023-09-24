#include "BluetoothSerial.h"
#include "main.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to and enable it
#endif

DCMotor VSRC_Motor;
Servo_Motor SERVO;
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  VSRC_Motor.Init();
  SERVO.Init();
  SERVO.Pulse(LIFT_PULSE_0, LIFT);
  SERVO.Pulse(SHOOTER_PULSE_0, SHOOTER);
  SERVO.Pulse(INTAKE_PULSE_0, INTAKE);
  SERVO.Angle(BALL_INIT_ANGLE, BALL1);
  SERVO.Angle(BALL_INIT_ANGLE, BALL2);
  SerialBT.begin("VSRC - SGT"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

uint8_t byte_cnt=0;
uint8_t in_byte = 0, array_index=0;
uint8_t data_in[7]={0,0,0,0,0,0,0};
// 1: button, 2: LX, 3: LY, 4: RX, 5: RY;

uint8_t button, LY, Rx, last_button=0;

uint16_t LIMIT_PWM = 4000;

int16_t pwm_left=0, pwm_right=0;
int16_t pwm_shake_left = 300, pwm_shake_right = 300; 
int16_t vel_left, vel_right;
bool dir_left, dir_right;

static bool status_INTAKE, last_stt_INTAKE = 0;
static bool status_SHOOTER,last_stt_SHOOTER = 0;
static bool status_LIFT, last_stt_LIFT = 0;
static bool status_BALL, last_stt_BALL = 0;
static bool status_SHAKE, last_stt_SHAKE = 0;
uint32_t check_button_time;

#define ROTATE_SCALE 0.5
#define LINEAR_SCALE 1.5

void cal_pwm_motor(){
  	if(LY<=128){
			vel_right = (128-LY)*LINEAR_SCALE+(128-Rx);
			vel_left = (128-LY)*LINEAR_SCALE-(128-Rx);
	}
	else{
			vel_right = (128-LY)*LINEAR_SCALE-(128-Rx);
			vel_left = (128-LY)*LINEAR_SCALE+(128-Rx);
	}

  pwm_left = map(vel_left, -255, 255, -LIMIT_PWM, LIMIT_PWM);
  pwm_right = map(vel_right, -255, 255, -LIMIT_PWM, LIMIT_PWM);
  Serial.printf("PWM left: %d  PWM right %d \n",pwm_left, pwm_right);
  if(pwm_left>0){
    dir_left = 0;
  }
  else{
    dir_left = 1;
    pwm_left = -pwm_left;
  }

  if(pwm_right>0){
    dir_right = 0;
  }
  else{
    dir_right = 1;
    pwm_right = -pwm_right;
  }
}

void loop() {
  if (SerialBT.available()>0) {
    in_byte = SerialBT.read();

    // Serial.print(SerialBT.read());
    if(in_byte == 170){
      array_index = 0;
    }
    data_in[array_index] = in_byte;
    //Serial.print(data_in[array_index]);
    array_index ++;

    LY = data_in[3];
    Rx = data_in[4];
    cal_pwm_motor();
  }

  // ====== HANDLE BUTTONS ======

  if(millis()-check_button_time > 50 ){
    button = data_in[1];
    check_button_time = millis();
  }

  switch (button) {
    case L3: // lift
    if(status_LIFT == last_stt_LIFT){
      status_LIFT = !status_LIFT;
        // lift - can gat - L3
      if (status_LIFT == 1) {
              Serial.println("lift_on");
              SERVO.Pulse(LIFT_PULSE_1, LIFT);
              SERVO.Pulse(INTAKE_PULSE_0, INTAKE);
              SERVO.Pulse(SHOOTER_PULSE_0, SHOOTER);
      } 
      else {
              Serial.println("lift_off");
              SERVO.Pulse(LIFT_PULSE_0, LIFT);
              SERVO.Pulse(INTAKE_PULSE_0, INTAKE);
              SERVO.Pulse(SHOOTER_PULSE_0, SHOOTER);
      }
    }
    break;

    case SQUARE:
    if(status_INTAKE == last_stt_INTAKE){
      status_INTAKE = !status_INTAKE;
      // intake - SQUARE
      if (status_INTAKE == 1) {
              Serial.println("intake_on");
              SERVO.Pulse(INTAKE_PULSE_0, INTAKE);
              SERVO.Pulse(SHOOTER_PULSE_0, SHOOTER);
      } else {
              Serial.println("intake_off");
              SERVO.Pulse(INTAKE_PULSE_1, INTAKE);
              SERVO.Pulse(SHOOTER_PULSE_0, SHOOTER);
      }
    }
    break;

    case CROSS:
    if(status_SHOOTER == last_stt_SHOOTER){
      status_SHOOTER = !status_SHOOTER;
          // shooter - CROSS
      if (status_SHOOTER == 1) {
              Serial.println("shooter_on");
              SERVO.Pulse(SHOOTER_PULSE_1, SHOOTER);
              SERVO.Pulse(INTAKE_PULSE_0, INTAKE);
      } else {
              Serial.println("shooter_off");
              SERVO.Pulse(SHOOTER_PULSE_0, SHOOTER);
              SERVO.Pulse(INTAKE_PULSE_0, INTAKE);
      }
    }
    break;

    case TRIANGLE:
    if(status_BALL == last_stt_BALL){
      status_BALL = !status_BALL;
        // nap bong - TRIANGLE
      if (status_BALL == 1) {
              Serial.println("ball_on");
              SERVO.Angle(BALL_GOAL_ANGLE, BALL1);
              SERVO.Angle(BALL_GOAL_ANGLE, BALL2);
      } else {
              Serial.println("ball_off");
              SERVO.Angle(BALL_INIT_ANGLE, BALL1);
              SERVO.Angle(BALL_INIT_ANGLE, BALL2);
      }
    }
    break;

    case R3:
    if(status_SHAKE == last_stt_SHAKE){
      status_SHAKE = !status_SHAKE;
      if (status_SHAKE == 1) {
              Serial.println("shake"); // trai qua phai 
              VSRC_Motor.Run(LEFT_MOTOR, pwm_shake_left, 1);
              VSRC_Motor.Run(RIGHT_MOTOR, pwm_shake_right, 0);
              delay(100);
              VSRC_Motor.Run(LEFT_MOTOR, pwm_shake_left, 0);
              VSRC_Motor.Run(RIGHT_MOTOR, pwm_shake_right, 1);
              
      } else {
              Serial.println("shake"); // phai qua trai
              VSRC_Motor.Run(LEFT_MOTOR, pwm_shake_left, 0);
              VSRC_Motor.Run(RIGHT_MOTOR, pwm_shake_right, 1);
              delay(100);
              VSRC_Motor.Run(LEFT_MOTOR, pwm_shake_left, 1);
              VSRC_Motor.Run(RIGHT_MOTOR, pwm_shake_right, 0);
      }
    }
    break;

    default: case 0:
      //Serial.println("no button pressed");
      last_stt_LIFT = status_LIFT;
      last_stt_INTAKE = status_INTAKE;
      last_stt_SHOOTER = status_SHOOTER;
      last_stt_BALL = status_BALL;
      last_stt_SHAKE = status_SHAKE;
      break;
    }

  VSRC_Motor.Run(LEFT_MOTOR, pwm_left, dir_left);
  VSRC_Motor.Run(RIGHT_MOTOR, pwm_right, dir_right);
}