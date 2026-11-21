#include "main.h"
#include "pros/rtos.hpp"
#include <cstdlib>

void autonomous() {
    // Keep track of where the lifts should hold their positions against gravity
    double banshee_hold_position = bansheelift.get_position();
    double cascade_hold_position = cascade.get_position();

    chassis.setPose(0, 0, 0);

    // Start spinning the banshee in continuously for the entire beginning portion
    bansheespin.move(127);

    // ==========================================
    // OPENING SEQUENCE
    // ==========================================
    // 1. Move cascade up 2 inches (approx 2 * 360 ticks)
    cascade.move_absolute(2.0 * 360, 100);
    cascade_hold_position = 2.0 * 360;
    pros::delay(600);

    // 2. Move banshee to 800
    bansheelift.move_absolute(800, 100);
    banshee_hold_position = 800;
    pros::delay(600);

    // 3. Move cascade back down 2 inches (back to 0)
    cascade.move_absolute(0, 100);
    cascade_hold_position = 0;
    pros::delay(600);

    // 4. Move banshee as low as it goes (-400)
    bansheelift.move_absolute(-400, 100);
    banshee_hold_position = -400;
    pros::delay(600);
    // ==========================================

    // quickly stop drivetrain so quick-release mech falls down
    chassis.moveToPoint(0, -7, 1000, {.forwards = false, .minSpeed = 127});
    chassis.moveToPoint(0, -6, 1000, {.minSpeed = 127});
    pros::delay(500);

    // ==========================================
    // SWITCH TOGGLING (RAMMING AT MAX SPEED)
    // ==========================================
    chassis.moveToPoint(0, 2, 600, {.maxSpeed = 127});
    chassis.moveToPoint(0, -10, 600, {.forwards = false, .maxSpeed = 127, .minSpeed = 127});
    chassis.moveToPoint(0, 2, 600, {.maxSpeed = 127});
    
    // ==========================================

    // scoring preload (Stop continuous spin right before scoring sequence)
    bansheespin.move(0); 

    chassis.moveToPoint(0, -14.5, 1000, {.forwards = false});
    chassis.turnToHeading(90, 1000, {}, true); 
    
    // Banshee raised 100 ticks higher than before for scoring preload
    bansheelift.move_absolute((2.3 * 360) + 100, 100);
    bansheelift.move_absolute((2.3 * 360) + 100, 100);
    
    
    chassis.moveToPoint(-16, -13, 2000, {.forwards = false});
    pros::delay(1000);
    bansheespin.move(-60); 
    pros::delay(1000); 
    bansheespin.move(0);
    cascade.move_absolute(2 * 360, 100);
    cascade_hold_position = 2 * 360;
    pros::delay (500);
    
    // collecting preload
    chassis.moveToPoint(1, -13, 2000, {}, true);
    chassis.turnToHeading(60, 1000);
}