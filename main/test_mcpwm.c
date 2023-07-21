#include "mcpwm/mcpwm.h"

void app_main(void)
{
    printf("Testing brushed motor...\n");
    xTaskCreate(mcpwm_example_brushed_motor_control, "mcpwm_example_brushed_motor_control", 4096, NULL, 5, NULL);
}