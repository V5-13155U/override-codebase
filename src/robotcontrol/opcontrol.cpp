#include "main.h"
#include <cmath>

void opcontrol() {
    pros::Controller controller(pros::E_CONTROLLER_MASTER);

    // Keep track of where the lifts should hold their positions against gravity
    double banshee_hold_position = bansheelift.get_position();
    double cascade_hold_position = cascade.get_position();

    // State flag for the continuous banshee spin-in loop
    bool banshee_auto_spin = false;

    // Joystick exponential curve function
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

        // Drivetrain control with exponential curves
        int leftPower = curve(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));
        int rightPower = curve(controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));

        left_motors.move(leftPower);
        right_motors.move(rightPower);

        // Intake control (R1 = forward, R2 = reverse)
        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            intake.move(127);
        } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            intake.move(-127);
        } else {
            intake.move(0);
        }

        // A Button Macro Sequence
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
            // Step 1: Move banshee lift to 800
            bansheelift.move_absolute(800, 100);
            banshee_hold_position = 800;
            pros::delay(800);

            // Step 2: Move cascade all the way down (0)
            cascade.move_absolute(0, 100);
            cascade_hold_position = 0;
            pros::delay(1000);

            // Step 3: Move cascade up half as much (1.5 inches / 540 degrees)
            cascade.move_absolute(1.5 * 360, 100);
            cascade_hold_position = 1.5 * 360;
            pros::delay(1000);

            // Step 4: Bring banshee lift all the way down to -400
            bansheelift.move_absolute(-400, 100);
            banshee_hold_position = -400;
            pros::delay(800);

            // Step 5: Start the continuous spin-in loop
            banshee_auto_spin = true;
        }

        // Cascade control with active position holding (L1 = up, L2 = down)
        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            cascade.move(127);
            cascade_hold_position = cascade.get_position();
        } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            cascade.move(-127);
            cascade_hold_position = cascade.get_position();
        } else {
            cascade.move_absolute(cascade_hold_position, 100);
        }

        // Banshee lift control with active position holding (Up/Down arrows)
        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
            bansheelift.move(127);
            banshee_hold_position = bansheelift.get_position();
        } 
        else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
            bansheelift.move(-127);
            banshee_hold_position = bansheelift.get_position();
        } 
        else {
            bansheelift.move_absolute(banshee_hold_position, 100);
        }

        // Banshee spinner control (X, Y, or B break the auto-spin loop)
        bool x_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_X);
        bool y_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y);
        bool b_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_B);

        if (x_pressed || y_pressed || b_pressed) {
            banshee_auto_spin = false; // Interrupted by user button press

            if (x_pressed) {
                bansheespin.move(-127); // spin out
            } else if (y_pressed) {
                bansheespin.move(127);  // spin in
            } else if (b_pressed) {
                bansheespin.move(127);  // spin in
            }
        } 
        else if (banshee_auto_spin) {
            bansheespin.move(127);      // keeps spinning in forever until X, Y, or B is pressed
        } 
        else {
            bansheespin.move(0);
        }

        pros::delay(10);
    }
}