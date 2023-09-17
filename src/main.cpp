#include "main.h"

uint16_t LIMIT_PWM = 4000;

uint16_t pwm_left, pwm_right; bool dir_left, dir_right;
int8_t LY, Rx;

DCMotor VSRC_Motor;
Servo_Motor VSRC_Servo;


void setup() {   
    Serial.begin(115200);
    VSRC_Motor.Init();
    VSRC_Servo.Init();
    PS4.begin("1a:3b:5c:7d:9e:ff");
    Serial.println("READY");

    VSRC_Servo.Pulse(INTAKE_PULSE_0, INTAKE_SERVO);
    VSRC_Servo.Pulse(SHOOTER_PULSE_0, SHOOTER);
    VSRC_Servo.Pulse(LIFT_PULSE_0, LIFT);

}

static bool status_INTAKE = 0;
static bool status_SHOOTER = 0;
static bool status_LIFT = 0;

void loop() {
    if (PS4.isConnected()) {
        LY = PS4.LStickY();
        Rx = PS4.RStickX();
        
        // CALC PWM
            if ((LY > -30 && LY < 30) && (Rx > -30 && Rx < 30)) {
                pwm_left = 0;
                pwm_right = 0;
                dir_left = 0; 
                dir_right = 0;
            } 
            if (LY >= 30) {
                dir_left = 0;
                dir_right = 0;
                pwm_left = map(LY, 30, 127, 0, LIMIT_PWM);
                pwm_right = map(LY, 30, 127, 0, LIMIT_PWM);
            } else if (LY <= -30) {
                dir_left = 1;
                dir_right = 1;
                pwm_left = map(LY, -128, -30, LIMIT_PWM, 0);
                pwm_right = map(LY, -128, -30, LIMIT_PWM, 0);
            } else if (Rx >= 30) {
                dir_left = 0;
                dir_right = 1;
                pwm_left = LIMIT_PWM;
                pwm_right = LIMIT_PWM;
            } else if (Rx <= -30) {
                dir_left = 1;
                dir_right = 0;
                pwm_left = LIMIT_PWM;
                pwm_right = LIMIT_PWM;
            }

        // intake - L1
        if (PS4.L1() == 1) {
            if (status_INTAKE == 0) {
                VSRC_Servo.Pulse(INTAKE_PULSE_1, INTAKE_SERVO);
            } else {
                VSRC_Servo.Pulse(INTAKE_PULSE_0, INTAKE_SERVO);
            } 
            status_INTAKE = !status_INTAKE;
        }
        
        // shooter - R1
        if (PS4.R1() == 1) {
            if (status_SHOOTER == 0) {
                VSRC_Servo.Pulse(SHOOTER_PULSE_1, SHOOTER);
            } else {
                VSRC_Servo.Pulse(SHOOTER_PULSE_0, SHOOTER);
            } 
            status_SHOOTER = !status_SHOOTER;
        }

        // lift - SQUARE
        if (PS4.Square() == 1) {
            // if (status_LIFT == 0) {
                VSRC_Servo.Pulse(LIFT_PULSE_1, LIFT);
                Serial.println("sqr 1");
        // }
        }

        // lift - SQUARE
        if (PS4.Circle() == 1) {
            // if (status_LIFT == 0) {
                VSRC_Servo.Pulse(LIFT_PULSE_0, LIFT);
                Serial.println("sqr 0");
        // }
        }
    }
            // } else {
            //     VSRC_Servo.Pulse(LIFT_PULSE_0, LIFT);
            //     Serial.println("sqr 0");
            // } 
            // status_LIFT = !status_LIFT;
    
    if (pwm_left > LIMIT_PWM) pwm_left = LIMIT_PWM;
    if (pwm_left < 0) pwm_left = 0;
    if (pwm_right > LIMIT_PWM) pwm_right = LIMIT_PWM;
    if (pwm_right < 0) pwm_right = 0;
 
    // Serial.printf("pwm L: %d pwm R: %d \n", pwm_left, pwm_right);
    VSRC_Motor.Run(LEFT_MOTOR, pwm_left, dir_left);
    VSRC_Motor.Run(RIGHT_MOTOR, pwm_right, dir_right);
    delay(5);

}