#include "main.h"

void autonomous() {
    chassis.setPose(0, 0, 0);
    
    // Spin the intake forward while driving reverse 5 inches
    chassis.moveToPoint(0, -5, 1000, {.forwards = false, .maxSpeed = 127});
    chassis.moveToPoint(0, 1.5, 1000);
    chassis.moveToPoint(0, -5, 1000, {.forwards = false});
    chassis.moveToPoint(0, 1.5, 1000);
    chassis.moveToPoint(0, -14, 1000, {.forwards = false});
    chassis.turnToHeading(90, 1000, {}, true); bansheelift.move_absolute(600, 100);
    chassis.moveToPoint(-15, -14, 2000, {.forwards = false});
    pros::delay(1000);
    bansheespin.move(-100); pros::delay(1000); bansheespin.move(0);
    cascade.move_absolute(6 * 360, 100);
    
    // Move forward 24 inches asynchronously while moving cascade up 3 inches
    chassis.moveToPoint(9, -14, 2000, {}, true);
    cascade.move_absolute(6 * 360, 100);
    chassis.turnToHeading(45, 1000);
}