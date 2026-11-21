#include "main.h"
#include <cmath>

void opcontrol() {
    pros::Controller controller(pros::E_CONTROLLER_MASTER);

    // Keep track of where the lifts should hold their positions against gravity
    double banshee_hold_position = bansheelift.get_position();
    double cascade_hold_position = cascade.get_position();

    // Banshee spinner mode: 0 = stopped, 1 = spin in forever, -1 = spin out forever
    int banshee_spin_mode = 0;

    // Drivetrain speed toggle state (false = 100% speed, true = 40% slow mode)
    bool low_speed_mode = false;

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

        // Toggle drivetrain speed between 40% and 100% with each press of the Right Arrow
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
            low_speed_mode = !low_speed_mode; // Flips between true and false
        }

        // Apply multiplier based on the current toggle state
        double drive_speed_multiplier = low_speed_mode ? 0.4 : 1.0;

        // Drivetrain control with exponential curves and speed scaling applied
        int leftPower = curve(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)) * drive_speed_multiplier;
        int rightPower = curve(controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)) * drive_speed_multiplier;

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

        // Y Button: Move banshee claw out to 800 AND start spinning IN forever
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
            bansheelift.move_absolute(800, 100);
            banshee_hold_position = 800;
            banshee_spin_mode = 1; // Start spinning in forever
        }

        // Left Arrow: Move banshee claw out to 950, start spinning IN, and set drivetrain to 40%
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
            bansheelift.move_absolute(950, 100);
            banshee_hold_position = 950; 
            banshee_spin_mode = 1;      // Start spinning in forever
            low_speed_mode = true;      // Set drivetrain speed to 40%
        }

        // Cascade control: Default is 100% speed going up (L1) and 40% speed going down (L2)
        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            cascade.move(127); // 100% speed going up
            cascade_hold_position = cascade.get_position();
        } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            cascade.move(-127 * 0.4); // 40% speed going down by default
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

        // Banshee spinner persistent controls (X = spin in, B = spin out, A = stop + reset drivetrain speed to 100%)
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
            banshee_spin_mode = 1;  // Spin in forever
        } 
        else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
            banshee_spin_mode = -1; // Spin out forever
        } 
        else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
            banshee_spin_mode = 0;   // Stop spinner
            low_speed_mode = false;  // Reset drivetrain speed to 100%
        }

        // Apply spinner movement based on the active mode (127 = spin in, -127 = spin out)
        if (banshee_spin_mode == 1) {
            bansheespin.move(127);  // Spinning in
        } 
        else if (banshee_spin_mode == -1) {
            bansheespin.move(-127); // Spinning out
        } 
        else {
            bansheespin.move(0);    // Stopped
        }

        pros::delay(10);
    }
}