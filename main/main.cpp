// #include "bluetooth_hid/esp_hid_host.h"
#include "car_controller.hpp"

extern "C"
{
void app_main(void)
{
    init_bluetooth();
    init_motor_controls();
//     GBG::CarController car;
//     while(true)
//     {
//         car.drive();
//         ESP_LOGD("main", "post drive");
//         vTaskDelay(10);
//     }
}
}

