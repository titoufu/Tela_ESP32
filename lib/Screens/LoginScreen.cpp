#include "LoginScreen.h"
#include "WiFiManager.h"
#include "ui.h"

lv_obj_t *ssid_input;
lv_obj_t *password_input;
lv_obj_t *keyboard;

void textarea_event_cb(lv_event_t *e)
{
    lv_obj_t *textarea = lv_event_get_target(e);
    lv_keyboard_set_textarea(keyboard, textarea);
}

void keyboard_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        lv_obj_t *kb = lv_event_get_target(e);
        uint32_t key = lv_keyboard_get_selected_btn(kb);

        if (key == 39) // ENTER key
        {
            const char *ssid = lv_textarea_get_text(ssid_input);
            const char *password = lv_textarea_get_text(password_input);

            if (ssid != NULL && password != NULL)
            {
                bool connected = tryConnectToWiFi(ssid, password);
                if (connected)
                {
                    saveCredentials(ssid, password);
                    lv_scr_load(ui_Screen1); // Carrega a tela principal
                }
            }
        }
    }
}

void showLoginScreen()
{
    lv_obj_clean(lv_scr_act());

    // Adicionar um spinner para indicar carregamento
    lv_obj_t *spinner = lv_spinner_create(lv_scr_act(), 1000, 60); // Duração e largura da linha
    lv_obj_set_size(spinner, 50, 50);                              // Define o tamanho do spinner
    lv_obj_align(spinner, LV_ALIGN_TOP_MID, 0, 20);                // Posiciona o spinner no topo da tela

    ssid_input = lv_textarea_create(lv_scr_act());
    lv_textarea_set_placeholder_text(ssid_input, "SSID");
    lv_obj_set_width(ssid_input, 300);
    lv_obj_set_height(ssid_input, 50);
    lv_obj_align(ssid_input, LV_ALIGN_CENTER, 0, -100);

    password_input = lv_textarea_create(lv_scr_act());
    lv_textarea_set_placeholder_text(password_input, "Senha");
    lv_textarea_set_password_mode(password_input, true);
    lv_obj_set_width(password_input, 300);
    lv_obj_set_height(password_input, 50);
    lv_obj_align(password_input, LV_ALIGN_CENTER, 0, -20);

    keyboard = lv_keyboard_create(lv_scr_act());
    lv_keyboard_set_mode(keyboard, LV_KEYBOARD_MODE_TEXT_LOWER);
    lv_obj_set_size(keyboard, 460, 200);
    lv_obj_align(keyboard, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_keyboard_set_textarea(keyboard, ssid_input);

    lv_obj_add_event_cb(ssid_input, textarea_event_cb, LV_EVENT_FOCUSED, NULL);
    lv_obj_add_event_cb(password_input, textarea_event_cb, LV_EVENT_FOCUSED, NULL);
    lv_obj_add_event_cb(keyboard, keyboard_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
}