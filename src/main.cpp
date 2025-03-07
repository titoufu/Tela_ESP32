#include <Arduino_GFX_Library.h>
#include <lvgl.h>
#include "touch.h"  // Inclui a biblioteca do GT911

#define SCREEN_WIDTH  480
#define SCREEN_HEIGHT 480
#define GFX_BL        38  // Pino da luz de fundo

// Inicialização do Display
Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    39, 48, 47, 18, 17, 16, 21,
    11, 12, 13, 14, 0,
    8, 20, 3, 46, 9, 10,
    4, 5, 6, 7, 15
);

Arduino_ST7701_RGBPanel *gfx = new Arduino_ST7701_RGBPanel(
    bus, GFX_NOT_DEFINED, 0, true, SCREEN_WIDTH, SCREEN_HEIGHT,
    st7701_type1_init_operations, sizeof(st7701_type1_init_operations), true,
    10, 8, 50, 10, 8, 20
);

// Buffer para LVGL
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[SCREEN_WIDTH * 10];

// Função de atualização do display no LVGL
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    int32_t w = area->x2 - area->x1 + 1;
    int32_t h = area->y2 - area->y1 + 1;
    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)color_p, w, h);
    lv_disp_flush_ready(disp);
}

// Função para capturar os eventos do Touch GT911
void touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data) {
    if (touch_touched()) {
        data->point.x = touch_last_x;
        data->point.y = touch_last_y;
        data->state = LV_INDEV_STATE_PR;  // Pressionado
    } else {
        data->state = LV_INDEV_STATE_REL; // Solto
    }
}

// Callback do botão Wi-Fi
void wifi_btn_event_handler(lv_event_t * e) {
    Serial.println("Botão pressionado!");

    lv_obj_t *btn = lv_event_get_target(e);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x00FF00), LV_PART_MAIN);
    lv_label_set_text(lv_obj_get_child(btn, 0), "Conectando...");
}

// Criar interface gráfica
void create_ui() {
    lv_obj_t *label;

    // Definir fundo da tela (preto)
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), LV_PART_MAIN);

    // Criar botão
    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn, 200, 50);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 50);
    lv_obj_add_event_cb(btn, wifi_btn_event_handler, LV_EVENT_CLICKED, NULL);

    // Criar label dentro do botão
    label = lv_label_create(btn);
    lv_label_set_text(label, "Conectar ao Wi-Fi");
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_center(label);
}

void setup() {
    Serial.begin(115200);
    gfx->begin(16000000);
    gfx->fillScreen(WHITE);

    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);

    // Inicializar LVGL
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, SCREEN_WIDTH * 10);

    // Configuração do display no LVGL
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    // Inicializar o Touch GT911
    touch_init();

    // Configuração do driver de toque no LVGL
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    lv_indev_drv_register(&indev_drv);

    // Criar interface
    create_ui();
}

void loop() {
    lv_timer_handler();  // Mantém a interface atualizada
    delay(5);
}
