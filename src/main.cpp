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

    pros::lcd::clear_line(0);
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

        // BANSHEE LIFT HOMING

        bansheelift.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
        bansheelift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

        // Reset any previous encoder reference
        bansheelift.tare_position();

        // Move toward the maximum raised position
        bansheelift.move_velocity(60);

        int stall_timer = 0;
        int home_start = pros::millis();

        while (pros::millis() - home_start < 5000) {

            pros::delay(10);

            if (std::abs(bansheelift.get_actual_velocity()) < 5) {
                stall_timer++;
            } else {
                stall_timer = 0;
            }

            // Motor has remained stalled for 200ms
            if (stall_timer >= 20) {
                break;
            }
        }

        // Stop the motor
        bansheelift.move_velocity(0);
        pros::delay(100);

        // Establish fully raised position as 0 degrees
        bansheelift.tare_position();

        // Move to autonomous starting position
        const double custom_start_angle = -30;

        bansheelift.move_absolute(custom_start_angle, 100);

        // Wait for the lift to reach its starting position
        int settle_start = pros::millis();

        while (std::fabs(bansheelift.get_position() - custom_start_angle) > 3 &&
            pros::millis() - settle_start < 1500) {

            pros::delay(10);
        }

        bansheelift.move(0);

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

    // Note: autonomous() is defined in src/robotcontrol/autonomous.cpp

    // Note: opcontrol() is defined in src/robotcontrol/opcontrol.cpp