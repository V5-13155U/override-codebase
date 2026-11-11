    #include "main.h"
    #include "lemlib/api.hpp"

    // Drivetrain Configuration 
    pros::MotorGroup left_motors({6, 7}, pros::MotorGearset::blue);
    pros::MotorGroup right_motors({5, 20}, pros::MotorGearset::blue);

    // Drivetrain Settings
        lemlib::Drivetrain drivetrain(
		& left_motors,
        & right_motors,
        11,
        lemlib::Omniwheel::NEW_325,
        360,
        2
    );

    // lateral PID controller
    lemlib::ControllerSettings lateral_controller
        (10, // proportional gain (kP)
        0, // integral gain (kI)
        12, // derivative gain (kD)
        3, // anti windup
        1, // small error range, in inches
        100, // small error range timeout, in milliseconds
        3, // large error range, in inches
        200, // large error range timeout, in milliseconds
        20 // maximum acceleration (slew)
        );

    // angular PID controller   
    lemlib::ControllerSettings angular_controller(
        0.90,   // kP slightly lower than original 8
        0,
        0.5,  // kD slightly higher than original 40
        0,
        2,          
        100,
        5,
        300,
        10
    );

    // imu (inertial)
    pros::Imu imu(12);

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
