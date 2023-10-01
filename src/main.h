#ifndef _MAIN_H_
#define _MAIN_H_

#include <Arduino.h>
#include <EEB.h>
// #include <PS4Controller.h>
// #include <ESP32_Servo.h>

// DC MOTORS
#define INTAKE 1        
#define LEFT_MOTOR 2
#define RIGHT_MOTOR 3

// SERVO MOTORS
#define SHOOTER 2
#define LIFT 3 
#define BALL1 4
#define BALL2 5

#define INTAKE_PULSE_1 1600
#define INTAKE_PULSE_0 1000

#define SHOOTER_PULSE_1 1700
#define SHOOTER_PULSE_0 1000

#define LIFT_PULSE_1 2500
#define LIFT_PULSE_0 500

#define BALL_INIT_ANGLE 0
#define BALL_GOAL_ANGLE 90 

#define CROSS 11
#define CIRCLE 10
#define SQUARE 8
#define TRIANGLE 9

#define L3 12
#define R3 13


#define SPR(a) Serial.print(a);
#define SPRL(a) Serial.println(a);

#endif