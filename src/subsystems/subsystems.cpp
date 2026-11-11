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
        -1
    );