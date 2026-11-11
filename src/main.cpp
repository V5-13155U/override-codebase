#include "main.h"
#include <cmath>

    void on_center_button() {
    static bool pressed = false;
    pressed = !pressed;

    if (pressed) {
        pros::lcd::set_text(2, "I was pressed!");
    } else {
        pros::lcd::clear_line(2);
    }
}

    void initialize() {
    pros::lcd::initialize();
    pros::lcd::register_btn1_cb(on_center_button);

    pros::lcd::print(0, "Initializing");

    vertical_encoder.reset();
    vertical_encoder.set_reversed(true);

    pros::Task telemetry_task([]() {
    while (true) {
    // 1. Grab LemLib Odometry Pose Variables
        lemlib::Pose pose = chassis.getPose();
        double x = pose.x;
        double y = pose.y;
        double theta = pose.theta;

        // 2. Grab Raw Sensor Variables
        double imu_heading = imu.get_heading();
        int vert_rot = vertical_encoder.get_position(); // Returns centidegrees (100 = 1 degree)
        int horiz_rot = horizontal_encoder.get_position();

        // 3. Print to fixed LCD lines (overwrites existing text cleanly without leaking memory)
        pros::lcd::print(1, "X: %.2f | Y: %.2f", x, y);
        pros::lcd::print(2, "Theta: %.2f | IMU: %.2f", theta, imu_heading);
        pros::lcd::print(3, "Vert: %d | Horiz: %d", vert_rot, horiz_rot);

        // 4. Safe 250ms delay (4 updates per second) prevents CPU/memory overload
        pros::delay(250);
        }
    });

    /* bansheelift setup + intitialization
    bansheelift.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    bansheelift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    bansheelift.move_velocity(60);
    int stall_timer = 0;
    int home_start = pros::millis();  // safety timeout so this can't loop forever

    while ((std::abs(bansheelift.get_actual_velocity()) > 5 || stall_timer < 20) &&
           pros::millis() - home_start < 3000) {
        pros::delay(10);
        if (std::abs(bansheelift.get_actual_velocity()) <= 5) {
            stall_timer++;
        } else {
            stall_timer = 0;
        }
    }

    bansheelift.move_velocity(0);
    bansheelift.tare_position(); 

    // 3. Move down to your default starting position (Change -30 to whatever you want)
    const int custom_start_angle = -30; 
    bansheelift.move_absolute(custom_start_angle, 100);

    // wait for the lift to stop moving before calibrating the IMU,
    // so vibration doesn't throw off the heading (max 1 second)
    int settle_start = pros::millis();
    while (std::fabs(bansheelift.get_position() - custom_start_angle) > 3 &&
           pros::millis() - settle_start < 1000) {
        pros::delay(10);
    }*/

    chassis.calibrate();
    cascade.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    }

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
    }

    // Note: opcontrol() is defined in src/robotcontrol/opcontrol.cpp