#include "main.h"
#include <cmath>

void autonomous() {
    chassis.setPose(0, 0, 0);
    
    // Spin the intake forward at speed 100 right at the start
    intake.move(100);
    pros::delay(300); 
    intake.move(0);

    // Back up to (0, -14) asynchronously while moving cascade up a quarter inch and banshee to 500
    chassis.moveToPoint(0, -14, 2000, {.forwards = false}, true); 
    bansheelift.move_absolute(500, 100);
    cascade.move_absolute(0.25 * 360, 100); 
    pros::delay(1000);
    bansheespin.move(-100); 
    pros::delay(500);       
    bansheelift.move_absolute(1000, 100);
    bansheespin.move(0);
    
    // Turn right to an absolute heading of 45 degrees
    chassis.turnToHeading(45, 1000);
    
    // Drive forward 10 inches at that 45-degree angle
    chassis.moveToPoint(7.1, -1.9, 1500);
    
    // Turn left 90 degrees (-45 absolute heading)
    chassis.turnToHeading(-45, 1000);
    
    // Turn slightly (-43 absolute heading) and move forward 5 inches more
    chassis.turnToHeading(-43, 1000);
    chassis.moveToPoint(-2.5, 7.3, 500);

    // Move backwards a greater distance for the final sweep
    chassis.moveToPoint(6.5, -2.0, 2000, {.forwards = false});

    // Move forward back to position
    chassis.moveToPoint(-2.5, 7.3, 500);

    // Drive reverse straight back 27 inches from current position
    lemlib::Pose current_pose = chassis.getPose();
    double rad = current_pose.theta * M_PI / 180.0;
    double target_x = current_pose.x - 27 * std::sin(rad);
    double target_y = current_pose.y - 27 * std::cos(rad);

    chassis.moveToPoint(target_x, target_y, 3000, {.forwards = false});
    chassis.waitUntilDone();

    // Turn right 40 degrees asynchronously while moving cascade up 2 inches, 
    // setting banshee lift to 500, and spinning the banshee in continuously
    lemlib::Pose turn_pose = chassis.getPose();
    chassis.turnToHeading(turn_pose.theta + 40, 1000, {}, true);
    cascade.move_absolute(2 * 360, 100);
    bansheelift.move_absolute(500, 100);
    bansheespin.move(100);

    chassis.waitUntilDone();

    // Drive reverse straight back 9 inches from current position
    lemlib::Pose final_pose = chassis.getPose();
    double final_rad = final_pose.theta * M_PI / 180.0;
    double final_target_x = final_pose.x - 9 * std::sin(final_rad);
    double final_target_y = final_pose.y - 9 * std::cos(final_rad);

    chassis.moveToPoint(final_target_x, final_target_y, 2000, {.forwards = false});
    chassis.waitUntilDone();

    // Move cascade down 3 inches and delay so it has time to finish moving
    cascade.move_relative(-3 * 360, 100);
    pros::delay(500);
}