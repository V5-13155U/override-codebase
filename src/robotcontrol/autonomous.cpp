#include "main.h"
#include <cmath>

void autonomous() {
    chassis.setPose(0, 0, 0);
    
    // Spin the intake forward a little bit right at the start
    intake.move(127);
    pros::delay(300); 
    intake.move(0);

    // Fixed timeout from 0 to 4000 ms
    chassis.moveToPoint(0, -13, 4000, {.forwards = false}, true); 
    bansheelift.move_absolute(1000, 100);
    pros::delay(1000);
    bansheelift.move_absolute(600, 100);
    pros::delay(500);
    bansheespin.move(-127); 
    pros::delay(300);
    bansheelift.move_absolute(1000, 100);
    bansheespin.move(0);
    chassis.moveToPoint(0, 5, 4000);
    pros::delay(100);
    chassis.moveToPoint(0, -2, 4000, {.forwards = false}, true);
    chassis.moveToPoint(0,5,4000);
}