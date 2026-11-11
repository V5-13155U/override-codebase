    #include "main.h"
    #include "lemlib/api.hpp"

    // Drivetrain Configuration 
    pros::MotorGroup left_motors({6, 7}, pros::MotorGearset::blue);
    pros::MotorGroup right_motors({5, 20}, pros::MotorGearset::blue);

    // Drivetrain Settings