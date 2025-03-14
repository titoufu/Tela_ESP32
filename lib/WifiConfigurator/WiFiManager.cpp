#include "WiFiManager.h"
#include "LoginScreen.h"
#include "ui.h"


Preferences preferences;
bool isConnecting = false; // Variável para rastrear o estado da conexão Wi-Fi

bool tryConnectToWiFi(const char *ssid, const char *password)
{
    WiFi.begin(ssid, password);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        attempts++;
        if (attempts >= 10) // Timeout após 10 tentativas
        {
            return false;
        }
    }
    return true;
}

void saveCredentials(const char *ssid, const char *password)
{
    preferences.begin("wifi", false);
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    preferences.end();
}

bool tryConnectWithSavedCredentials()
{
    preferences.begin("wifi", true);
    String ssid = preferences.getString("ssid", "");
    String password = preferences.getString("password", "");
    preferences.end();

    if (ssid != "" && password != "")
    {
        return tryConnectToWiFi(ssid.c_str(), password.c_str());
    }
    return false;
}
void connectToWiFi()
{

    if (!tryConnectWithSavedCredentials())
    {
        Serial.println("Falha na conexão. Exibindo tela de login...");
        showLoginScreen(); // Exibe a tela de login
    }

    while (WiFi.status() != WL_CONNECTED)
    {
        lv_task_handler(); // Processa eventos da interface gráfica
        delay(100);        // Pequeno atraso para evitar sobrecarga
    }

    Serial.println("Conexão Wi-Fi estabelecida!");
    saveCredentials(WiFi.SSID().c_str(), WiFi.psk().c_str()); // Salva as credenciais
    lv_scr_load(ui_Screen1);                                  // Carrega a tela principal

    // Atualiza o nome da rede na tela principal
    lv_label_set_text(ssidLabel, WiFi.SSID().c_str());
}
