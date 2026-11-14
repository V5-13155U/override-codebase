#include "main.h"

void autonomous() {
    chassis.setPose(0, 0, 0);

    // quickly stop drivetrain so quick-release mech falls down
    left_motors.move(-127);
    right_motors.move(-127);
    pros::delay(150);
    left_motors.brake();
    right_motors.brake();
    pros::delay(500);
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);

    // switches toggle twice
    chassis.moveToPoint(0, 1.5, 1000);
    chassis.moveToPoint(0, -5, 1000, {.forwards = false});
    chassis.moveToPoint(0, 1.5, 1000);

    // scoring preload
    chassis.moveToPoint(0, -14, 1000, {.forwards = false});
    chassis.turnToHeading(90, 1000, {}, true); bansheelift.move_absolute(600, 100);
    chassis.moveToPoint(-15, -14, 2000, {.forwards = false});
    pros::delay(1000);
    bansheespin.move(-100); pros::delay(1000); bansheespin.move(0);
    cascade.move_absolute(6 * 360, 100);
    
    // Move forward 24 inches asynchronously while moving cascade up 3 inches
    cascade.move_absolute(8 * 360, 100);
    chassis.moveToPoint(9, -14, 2000, {}, true);
    
    chassis.turnToHeading(45, 1000);
}
