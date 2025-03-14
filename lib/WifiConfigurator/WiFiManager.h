#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H
#include <Arduino.h>
#include <WiFi.h> // Certifique-se de incluir WiFi.h aqui
#include <Preferences.h>

// Declaração das funções
bool tryConnectWithSavedCredentials();
bool tryConnectToWiFi(const char *ssid, const char *password);
void saveCredentials(const char *ssid, const char *password);
void connectToWiFi();

#endif // WIFI_MANAGER_H