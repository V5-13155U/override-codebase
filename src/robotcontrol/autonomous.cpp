#include "main.h"
#include "pros/rtos.hpp"

void autonomous() {
    // Keep track of where the lifts should hold their positions against gravity
    double banshee_hold_position = bansheelift.get_position();
    double cascade_hold_position = cascade.get_position();

    chassis.setPose(0, 0, 0);

    // quickly stop drivetrain so quick-release mech falls down
    //left_motors.move(-127);
    //right_motors.move(-127);
    chassis.moveToPoint(0, -5, 1000, {.forwards = false, .minSpeed = 127});
    //pros::delay(150);
    //left_motors.brake();
    //right_motors.brake();
    //pros::delay(500);
    //chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);

    // switches toggle twice
    chassis.moveToPoint(0, 1.5, 1000, {.minSpeed = 60});
    chassis.moveToPoint(0, -5, 1000, {.forwards = false, .minSpeed = 60});
    chassis.moveToPoint(0, 1.5, 1000, {.minSpeed = 60});

    // scoring preload
    chassis.moveToPoint(0, -13.7, 1000, {.forwards = false});
    chassis.turnToHeading(90, 1000, {}, true); 
    bansheelift.move_absolute(625, 100);
    cascade.move_absolute(0.5 * 360, 100);
    cascade_hold_position = 0.5 * 360;
    chassis.moveToPoint(-14.5, -13.7, 2000, {.forwards = false});
    pros::delay(500);
    bansheespin.move(-100); 
    pros::delay(1000); 
    bansheespin.move(0);
    cascade.move_absolute(4 * 360, 100);
    cascade_hold_position = 4 * 360;
    pros::delay (500);
    
    // collecting preload
    cascade.move_absolute(8 * 360, 100);
    cascade_hold_position = 8 * 360;
    chassis.moveToPoint(9, -14.7, 2000, {}, true);
    chassis.turnToHeading(45, 1000);
}
