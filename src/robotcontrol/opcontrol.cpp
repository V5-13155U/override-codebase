#include "main.h"
#include <cmath>

void opcontrol() {
    pros::Controller controller(pros::E_CONTROLLER_MASTER);

    // Keep track of where the lifts should hold their positions against gravity
    double banshee_hold_position = bansheelift.get_position();
    double cascade_hold_position = cascade.get_position();

    // Banshee spinner mode: 0 = stopped, 1 = spin in forever, -1 = spin out forever
    int banshee_spin_mode = 0;

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

        // A Button Macro Sequence (1.5 inches up)
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
            bansheelift.move_absolute(800, 100);
            banshee_hold_position = 800;
            pros::delay(800);

            cascade.move_absolute(0, 100);
            cascade_hold_position = 0;
            pros::delay(1000);

            cascade.move_absolute(1.5 * 360, 100);
            cascade_hold_position = 1.5 * 360;
            pros::delay(1000);

            bansheelift.move_absolute(-400, 100);
            banshee_hold_position = -400;
            pros::delay(800);

            banshee_spin_mode = 1; // Start continuous spin-in
        }

        // Right Arrow Macro Sequence (3 inches up)
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
            bansheelift.move_absolute(800, 100);
            banshee_hold_position = 800;
            pros::delay(800);

            cascade.move_absolute(0, 100);
            cascade_hold_position = 0;
            pros::delay(1000);

            cascade.move_absolute(3 * 360, 100);
            cascade_hold_position = 3 * 360;
            pros::delay(1000);

            bansheelift.move_absolute(-400, 100);
            banshee_hold_position = -400;
            pros::delay(800);

            banshee_spin_mode = 1; // Start continuous spin-in
        }

        // Left Arrow Macro Sequence (6 inches up)
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
            bansheelift.move_absolute(800, 100);
            banshee_hold_position = 800;
            pros::delay(800);

            cascade.move_absolute(0, 100);
            cascade_hold_position = 0;
            pros::delay(1000);

            cascade.move_absolute(6 * 360, 100);
            cascade_hold_position = 6 * 360;
            pros::delay(1000);

            bansheelift.move_absolute(-400, 100);
            banshee_hold_position = -400;
            pros::delay(800);

            banshee_spin_mode = 1; // Start continuous spin-in
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

        // Banshee spinner continuous control (X, Y, B start forever-spin modes)
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
            banshee_spin_mode = -1; // Spin out forever until another button is pressed
        } 
        else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
            banshee_spin_mode = 1;  // Spin in forever until another button is pressed
        } 
        else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
            banshee_spin_mode = 1;  // Spin in forever until another button is pressed
        }

        // Apply spinner movement based on the active mode
        if (banshee_spin_mode == 1) {
            bansheespin.move(-127); // Spinning in forever
        } 
        else if (banshee_spin_mode == -1) {
            bansheespin.move(127);  // Spinning out forever
        } 
        else {
            bansheespin.move(0);
        }

        pros::delay(10);
    }
}