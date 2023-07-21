#include "bluetooth_hid/esp_hid_host.h"
#include "car_controller.hpp"

static

extern "C"
{
void app_main(void)
{
    init_bluetooth();
    init_motor_controls();
}
}

