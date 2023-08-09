#pragma once

#include "MotorControl/MotorController.hpp"

using namespace GBG;

void drive_task(void *arg)
{
    CarController car;
    while(1)
    {
        car.drive();
        vTaskDelay(1);
    }
}

void init_motor_controls()
{
    xTaskCreate(drive_task, "drive_motor_task", 4096, NULL, 5, NULL);
}