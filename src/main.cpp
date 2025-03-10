#include <Arduino.h>
#include "display_manager.h"
#include "touch_manager.h"
#include "ui.h"
#include <WiFi.h>
#include "WiFiConfigurator.h"
#include <HTTPClient.h>

HTTPClient http;
WiFiConfigurator wifiConfigurator;

void setup()
{
    Serial.begin(115200);
    wifiConfigurator.begin(); // Conectando ao Wi-Fi
    display_init();
    touch_init_lvgl();
    ui_init();
}
bool reconnect()
{
    wifiConfigurator.handleClient();
    http.begin("http://www.google.com"); // URL de teste
    int httpCode = http.GET();
    return (httpCode == HTTP_CODE_OK) ? true : false;
}

void loop()
{
    while(!reconnect()){
        Serial.println("Reconectando...");
        delay(5000); // Reconectar a cada 5 segundos    
    }
    lv_timer_handler();
    delay(5);
}