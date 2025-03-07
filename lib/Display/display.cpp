#include "display.h"

// Configuração do barramento RGB
Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    39, 48, 47, 18, 17, 16, 21, 
    11, 12, 13, 14, 0, 8, 20, 3, 
    46, 9, 10, 4, 5, 6, 7, 15
);

// Configuração do driver ST7701
Arduino_ST7701_RGBPanel *gfx = new Arduino_ST7701_RGBPanel(
    bus, GFX_NOT_DEFINED, 0, true, 480, 480,
    st7701_type1_init_operations, sizeof(st7701_type1_init_operations), true,
    10, 8, 50, 10, 8, 20
);

void initDisplay() {
    gfx->begin();
    gfx->fillScreen(BLACK);
}
