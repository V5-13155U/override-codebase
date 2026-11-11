
#include "main.h"
#include <cmath>

/**
 * LCD center button callback.
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
 * Initialization.
 */
void initialize() {
    pros::lcd::initialize();
    pros::lcd::register_btn1_cb(on_center_button);

    pros::lcd::print(0, "Initializing");

    vertical_encoder.reset();
    vertical_encoder.set_reversed(true);

    chassis.calibrate();

    pros::lcd::print(0, "Calibration complete");
}

/**
 * Disabled.
 */
void disabled() {}

/**
 * Autonomous - Sensor diagnostic.
 */
void autonomous() {
    left_motors.move(0);
    right_motors.move(0);

    while (true) {
        double heading = imu.get_heading();

        pros::lcd::print(0, "SENSOR DIAGNOSTICS");

        pros::lcd::print(1, "IMU: %.2f", heading);

        pros::lcd::print(
            2, "Vertical: %d",
            (int)vertical_encoder.get_position()
        );

        pros::lcd::print(
            3, "Horizontal: %d",
            (int)horizontal_encoder.get_position()
        );

        pros::delay(100);
    }
}

/**
 * Driver control.
 */
void opcontrol() {
    pros::Controller controller(pros::E_CONTROLLER_MASTER);

    auto curve = [](int input) -> int {
        const int deadband = 5;
        const double expo = 0.45;

        if (std::abs(input) < deadband)
            return 0;

        double x = input / 127.0;

        x = expo * x * x * x + (1.0 - expo) * x;

        return static_cast<int>(x * 127.0);
    };

    while (true) {
        int leftPower = curve(
            controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)
        );

        int rightPower = curve(
            controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)
        );

        left_motors.move(leftPower);
        right_motors.move(rightPower);

        pros::delay(10);
    }
}
