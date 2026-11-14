#include "main.h"
#include <cmath>

void opcontrol() {
    pros::Controller controller(pros::E_CONTROLLER_MASTER);

    // Keep track of where the lifts should hold their positions against gravity
    double banshee_hold_position = bansheelift.get_position();
    double cascade_hold_position = cascade.get_position();

    // Banshee spinner mode: 0 = stopped, 1 = spin in forever, -1 = spin out forever
    int banshee_spin_mode = 0;

    // Drivetrain speed scale multiplier (1.0 = 100% full speed, 0.1 = 10% slow mode)
    double drive_speed_multiplier = 1.0;

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

        // Drivetrain speed adjustment via Left and Right arrows
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
            drive_speed_multiplier = 1.0; // 100% max speed
        } 
        else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
            drive_speed_multiplier = 0.1; // 10% slow mode
        }

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

        // A Button Macro Sequence:
        // 1. Banshee out to 800
        // 2. Cascade all the way to max height (5.0 * 360)
        // 3. Cascade comes down to 2.0 * 360
        // 4. Banshee goes all the way back in to -400
        // 5. Start spinning in forever
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
            // Step 1: Banshee out
            bansheelift.move_absolute(800, 100);
            banshee_hold_position = 800;
            pros::delay(800);

            // Step 2: Cascade all the way to max height
            cascade.move_absolute(5.0 * 360, 100);
            cascade_hold_position = 5.0 * 360;
            pros::delay(1200);

            // Step 3: Cascade comes down to 2
            cascade.move_absolute(2.0 * 360, 100);
            cascade_hold_position = 2.0 * 360;
            pros::delay(1000);

            // Step 4: Banshee goes all the way back in
            bansheelift.move_absolute(-400, 100);
            banshee_hold_position = -400;
            pros::delay(800);

            // Step 5: Start continuous spin-in
            banshee_spin_mode = 1; 
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

        // Banshee spinner persistent controls (X = spin out, B = spin in, Y = stop)
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
            banshee_spin_mode = -1; // Spin out forever
        } 
        else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
            banshee_spin_mode = 1;  // Spin in forever
        } 
        else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
            banshee_spin_mode = 0;  // Stop spinner
        }

        // Apply spinner movement based on the active mode
        if (banshee_spin_mode == 1) {
            bansheespin.move(-127); // Spinning in
        } 
        else if (banshee_spin_mode == -1) {
            bansheespin.move(127);  // Spinning out
        } 
        else {
            bansheespin.move(0);    // Stopped
        }

        pros::delay(10);
    }
}