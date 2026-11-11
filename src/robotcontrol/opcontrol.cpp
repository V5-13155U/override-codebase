#include "main.h"
#include <cmath>

void run_custom_opcontrol() {
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
        int leftPower = curve(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));
        int rightPower = curve(controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));

        left_motors.move(leftPower);
        right_motors.move(rightPower);

        pros::delay(10);
    }
}