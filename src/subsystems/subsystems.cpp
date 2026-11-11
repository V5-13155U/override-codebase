#include "main.h"
#include "lemlib/api.hpp"

// drivetrain configuration
pros::MotorGroup left_motors({5, 20}, pros::MotorGearset::blue);
pros::MotorGroup right_motors({-6, -7}, pros::MotorGearset::blue);

// drivetrain settings
lemlib::Drivetrain drivetrain(
    &left_motors,
    &right_motors,
    11,
    lemlib::Omniwheel::NEW_325,
    360,
    2
);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(
    10,   // kP
    0,    // kI
    3,    // kD - CHANGED from 12 to 3 to reduce shaking at the end of moves
    3,    // anti windup
    1,    // small error range (inches)
    100,  // small error range timeout (ms)
    3,    // large error range (inches)
    200,  // large error range timeout (ms)
    20    // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(
    0.90,
    0,
    0,
    0,
    2,
    100,
    5,
    300,
    10
);

// imu setup
pros::Imu imu(13);

// tracking wheel encoder setup
pros::Rotation vertical_encoder(16);
pros::Rotation horizontal_encoder(4);

lemlib::TrackingWheel vertical_tracking_wheel(
    &vertical_encoder,
    2.125,
    0.55
);

lemlib::TrackingWheel horizontal_tracking_wheel(
    &horizontal_encoder,
    2.125,
    0.55
);

// odometry settings
lemlib::OdomSensors sensors(
    &vertical_tracking_wheel,
    nullptr,
    &horizontal_tracking_wheel,
    nullptr,
    &imu
);

// chassis
lemlib::Chassis chassis(
    drivetrain,
    lateral_controller,
    angular_controller,
    sensors
);

// intake
pros::Motor intake(2, pros::MotorGearset::green);

// banshee subsystem
pros::Motor bansheespin(9, pros::MotorGearset::green);
pros::Motor bansheelift(1, pros::MotorGearset::green);

// cascade
pros::MotorGroup cascade({-18, 10}, pros::MotorGearset::green);