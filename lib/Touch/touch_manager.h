#ifndef TOUCH_MANAGER_H
#define TOUCH_MANAGER_H

#include <lvgl.h>

void touch_init_lvgl();
void touch_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
void touch_init();
bool touch_has_signal();
bool touch_touched();
bool touch_released();

#endif