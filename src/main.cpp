#include <Arduino.h>
#include <WiFi.h>
#include "WiFiManager.h"
#include "ui.h"
#include "display_manager.h"
#include "touch_manager.h"
#include "LoginScreen.h"

void setup()
{
    Serial.begin(115200);
    display_init();    // Inicializa o display
    touch_init_lvgl(); // Inicializa o touch
    ui_init();         // Inicializa a interface gráfica

    saveCredentials("false", "false"); // Teste: força credenciais inválidas
    connectToWiFi(); // Conecta ao Wi-Fi usando a função do WiFiManager
}

void loop()
{
    lv_task_handler();
    if (WiFi.status() != WL_CONNECTED)
    {
        connectToWiFi();
    }
    delay(1000);
}