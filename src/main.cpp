#include "main.h"
#include <cmath>


    /**
    * A callback function for LLEMU's center button.
    *
    * When this callback is fired, it will toggle line 2 of the LCD text between
    * "I was pressed!" and nothing.
    */
    
    void on_center_button() {
        static bool pressed = false;
        pressed = !pressed;
        if (pressed) {
            pros::lcd::set_text(2, "I was pressed!");
        } else {
            pros::lcd::clear_line(2);
        }
    }

    /**
    * Runs initialization code. This occurs as soon as the program is started.
    *
    * All other competition modes are blocked by initialize; it is recommended
    * to keep execution time for this mode under a few seconds.
    */
    
    void initialize() {
        pros::lcd::initialize();
        pros::lcd::register_btn1_cb(on_center_button);

        pros::lcd::print(0, "initializing + calibrating");
        vertical_encoder.reset();
        vertical_encoder.set_reversed(true);
        chassis.calibrate();

        cascade.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

    }

    /**
    * Runs while the robot is in the disabled state of Field Management System or
    * the VEX Competition Switch, following either autonomous or opcontrol. aWhen
    * the robot is enabled, this task will exit.
    */
    
    void disabled() {}

    /**
    * Runs after initialize(), and before autonomous when connected to the Field
    * Management System or the VEX Competition Switch. This is intended for
    * competition-specific initialization routines, such as an autonomous selector
    * on the LCD.
    *
    * This task will exit when the robot is enabled and autonomous or opcontrol
    * starts.
    */

    void autonomous() {
        chassis.setPose(0, 0, 0);

        chassis.moveToPoint(0, 24, 4000);
        chassis.waitUntilDone();
    }

    // Note: opcontrol() is defined in src/robotcontrol/opcontrol.cpp