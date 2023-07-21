#include "MotorControl/MotorController.hpp"

using namespace GBG;

void drive_motor_task(void *arg)
{
    MotorController drive({MCPWM_UNIT_0, MCPWM0A, MCPWM_TIMER_0}, 15, 16);
    while(1)
    {
        
    }
}

// void turn_motor_task()
// {

// }

void init_motor_controls()
{
    xTaskCreate(drive_motor_task, "drive_motor_task", 4096, NULL, 5, NULL);
}