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

    // bansheelift setup + intitialization
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
    }

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

// ===========================================================================
// AUTONOMOUS HELPERS
// ===========================================================================
 
// Shows which auton step is running on the Brain screen, so if the robot
// does something wrong you can see exactly which step it was on.
void show_step(const char* text) {
    pros::lcd::print(3, "AUTON: %s", text);
}
 
// ---------------------------------------------------------------------------
// MECHANISM SETTINGS - all TUNE values. If a motor spins the wrong way,
// flip the sign of its speed here instead of changing anything else.
// ---------------------------------------------------------------------------
const int INTAKE_IN_SPEED  = 127;   // TUNE: flip to -127 if the intake spits out
const int INTAKE_OUT_SPEED = -127;  // TUNE: flip to 127 if it pulls in
 
const int LIFT_SPEED       = 100;   // TUNE: lift speed (max 200 on green)
const double LIFT_UP_DEG   = -5;   // TUNE: how far up the lift goes (degrees)
const double LIFT_DOWN_DEG = -300;     // TUNE: lift down position (0 = start spot)
                                    // if the lift goes DOWN first, make
                                    // LIFT_UP_DEG negative
 
// Moves the banshee lift to a position and waits until it gets there
// (or gives up after timeout_ms so auton never gets stuck).
void lift_to(double target_deg, int timeout_ms) {
    bansheelift.move_absolute(target_deg, LIFT_SPEED);
    int start = pros::millis();
    while (std::fabs(bansheelift.get_position() - target_deg) > 5 &&
           pros::millis() - start < timeout_ms) {
        pros::delay(10);
    }
}
 
// ---------------------------------------------------------------------------
// MECHANISM ACTIONS
// ---------------------------------------------------------------------------
void score_preload() {
    show_step("score preload");
    intake.move(INTAKE_OUT_SPEED);   // push the preload cone out
    pros::delay(600);                // TUNE: how long to spit it out
    intake.move(0);
}
 
// Banshee claw: lift goes up, then comes back down to catch the pin
void grab_goal() {
    show_step("grab goal - lift up");
    lift_to(LIFT_UP_DEG, 1500);
    show_step("grab goal - lift down");
    lift_to(LIFT_DOWN_DEG, 1500);
}
 
void pick_up_cone() {
    show_step("pick up cone");
    intake.move(INTAKE_IN_SPEED);    // keep pulling the cone in
    pros::delay(800);                // TUNE: how long to intake
    intake.move(0);
}
 
void stack_cone() {
    show_step("stack cone (placeholder)");
    pros::delay(500);
}
 
// ===========================================================================
// AUTONOMOUS ROUTINE
// Coordinates are in inches. The robot starts at (0, 0) facing forward (0 deg).
// +Y = straight ahead from the start, +X = to the robot's right.
// Every number marked TUNE is an estimate from the video. Measure on the
// field and adjust.
// ===========================================================================
void autonomous() {
    chassis.setPose(0, 0, 0);
 
    // STEP 1: drive forward to the red base and score the preload cone
    show_step("1 drive to red base");
    chassis.moveToPoint(0, 18, 2000);           // TUNE: distance to red base
    chassis.waitUntilDone();
    score_preload();
 
    // STEP 2: back up away from the red base
    show_step("2 back up");
    chassis.moveToPoint(0, 6, 1500, {.forwards = false});  // TUNE
    chassis.waitUntilDone();
 
    // STEP 3: turn so the BACK of the robot (banshee claw) faces the goal
    // 225 = 45 + 180, so the back points toward the goal at 45 degrees
    show_step("3 turn back to goal");
    chassis.turnToHeading(225, 1500);           // TUNE: angle (keep +180)
    chassis.waitUntilDone();
 
    // STEP 4: drive BACKWARDS into the goal and grab it with the banshee
    show_step("4 back into goal");
    chassis.moveToPoint(24, 30, 3000, {.forwards = false});  // TUNE: goal position
    chassis.waitUntilDone();
    grab_goal();
 
    // STEP 5: pick up the yellow cone next to the goal
    show_step("5 pick up cone");
    pick_up_cone();
 
    // STEP 6: drive FORWARD back toward the starting corner, pulling the goal
    // behind the robot on the banshee
    show_step("6 return to corner");
    chassis.moveToPoint(6, 8, 3000);            // TUNE
    chassis.waitUntilDone();
 
    // STEP 7: stack the cone on the goal
    show_step("7 stack cone");
    stack_cone();
 
    show_step("DONE");
}
