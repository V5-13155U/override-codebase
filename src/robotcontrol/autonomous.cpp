#include "main.h"
#include <cmath>

    void autonomous() {
        chassis.setPose(0, 0, 0);
        chassis.moveToPoint(0,48,4000);
        chassis.turnToHeading(90, 4000);
    }