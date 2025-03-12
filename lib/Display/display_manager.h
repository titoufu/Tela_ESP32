#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino_GFX_Library.h>
#include <lvgl.h>

#define SCREEN_WIDTH  480
#define SCREEN_HEIGHT 480
#define GFX_BL        38  // Pino da luz de fundo

extern Arduino_DataBus *bus;
extern Arduino_RGB_Display *gfx;

void display_init();
void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

#endif