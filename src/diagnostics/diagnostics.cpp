    /* void run_sensor_diagnostic() {
    while (true) {
        // keep the drivetrain stopped the whole time
        left_motors.move(0);
        right_motors.move(0);
 
        // ---- IMU (port 13) ----
        double heading = imu.get_heading();
        double rotation = imu.get_rotation();
 
        // ---- Rotation sensors (raw position is in centidegrees) ----
        int32_t vert_raw = vertical_encoder.get_position();
        int32_t horiz_raw = horizontal_encoder.get_position();
 
        // ---- LemLib pose ----
        lemlib::Pose pose = chassis.getPose();
 
        pros::lcd::print(0, "SENSOR DIAGNOSTIC - motors stopped");
 
        if (std::isinf(heading) || std::isnan(heading)) {
            pros::lcd::print(1, "IMU p13: ERROR (inf) - check port/cable");
        } else {
            pros::lcd::print(1, "IMU p13 heading: %.2f", heading);
        }
        pros::lcd::print(2, "IMU rotation: %.2f  calibrating: %s",
                         rotation, imu.is_calibrating() ? "YES" : "no");
 
        if (vert_raw == PROS_ERR) {
            pros::lcd::print(3, "Vert p16: ERROR %d", static_cast<int>(vert_raw));
        } else {
            pros::lcd::print(3, "Vert p16: %.2f deg", vert_raw / 100.0);
        }
 
        if (horiz_raw == PROS_ERR) {
            pros::lcd::print(4, "Horiz p4: ERROR %d", static_cast<int>(horiz_raw));
        } else {
            pros::lcd::print(4, "Horiz p4: %.2f deg", horiz_raw / 100.0);
        }
 
        pros::lcd::print(5, "Pose X: %.2f  Y: %.2f", pose.x, pose.y);
        pros::lcd::print(6, "Pose Theta: %.2f", pose.theta);
        pros::lcd::print(7, "Spin tracking wheels by hand to test");
 
        pros::delay(50);
    }
} */
