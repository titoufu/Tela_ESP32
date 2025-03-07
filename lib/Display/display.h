#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino_GFX_Library.h>

#define GFX_BL 38

extern Arduino_ESP32RGBPanel *bus;
extern Arduino_ST7701_RGBPanel *gfx;

void initDisplay();

#endif
