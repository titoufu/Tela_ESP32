#include <Arduino.h>
#include "ui_manager.h"

// Callback do botão Wi-Fi
void wifi_btn_event_handler(lv_event_t * e) {
    Serial.println("Botão pressionado!");
    

    lv_obj_t *btn = lv_event_get_target(e);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x00FF00), LV_PART_MAIN);
    lv_label_set_text(lv_obj_get_child(btn, 0), "Conectando...");
}

void ui_init() {
    lv_obj_t *label;

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), LV_PART_MAIN);

    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn, 200, 50);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 50);
    lv_obj_add_event_cb(btn, wifi_btn_event_handler, LV_EVENT_CLICKED, NULL);

    label = lv_label_create(btn);
    lv_label_set_text(label, "Conectar ao Wi-Fi");
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_center(label);
}