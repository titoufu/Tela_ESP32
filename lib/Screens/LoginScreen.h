#ifndef LOGIN_SCREEN_H
#define LOGIN_SCREEN_H

#include <lvgl.h> // Certifique-se de que o caminho está correto

void showLoginScreen();
void keyboard_event_cb(lv_event_t *e);
void textarea_event_cb(lv_event_t *e);

#endif // LOGIN_SCREEN_H