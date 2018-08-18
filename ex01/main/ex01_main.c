#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

static const char* TAG = "ex01_module";

void gpio_task_example( void *pvParameters )
{
    uint8_t cnt=0;
    gpio_pad_select_gpio(GPIO_NUM_16);
    gpio_set_direction(GPIO_NUM_16, GPIO_MODE_OUTPUT);
    for( ;; )
    {
        gpio_set_level(GPIO_NUM_16, cnt%2);
        cnt++;
        printf("st=%d\n",cnt%2);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    vTaskDelete( NULL );
}

void print_chip_info()
{
    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");


}

void app_main()
{
    fflush(stdout);
    printf("Hello world!\n");
    print_chip_info();

    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);

}
