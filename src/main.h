#ifndef _MAIN_H_
#define _MAIN_H_

#include <Arduino.h>
#include <PS4Controller.h>
#include <EEB.h>

#define LEFT_MOTOR 2
#define RIGHT_MOTOR 3

#define INTAKE_SERVO 1
#define SHOOTER 2
#define LIFT 3 

#define INTAKE_PULSE_1 1600
#define INTAKE_PULSE_0 1000

#define SHOOTER_PULSE_1 1700
#define SHOOTER_PULSE_0 1000

#define LIFT_PULSE_1 2000
#define LIFT_PULSE_0 1000

#define SPR(a) Serial.print(a);
#define SPRL(a) Serial.println(a);

struct entry_points {
    uint8_t low_forw_LY, high_forw_LY, low_back_LY, high_back_LY;
    uint8_t low_right_RX, high_right_RX, low_left_RX, high_left_RX;
} EP;

#endif