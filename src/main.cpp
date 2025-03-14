#include <Arduino.h>
#include "display_manager.h"
#include "touch_manager.h"
#include "ui.h"
#include <WiFi.h>
#include <Preferences.h>

Preferences preferences; // Criação do objeto

// Variáveis globais para os objetos LVGL
lv_obj_t *scr_login;
lv_obj_t *ssid_input;
lv_obj_t *password_input;
lv_obj_t *keyboard;
lv_obj_t *status_label; // mensagem de status

bool tryConnectToWiFi(const char *ssid, const char *password);
void saveCredentials(const char *ssid, const char *password);
void keyboard_event_cb(lv_event_t *e);
void showLoginScreen();
void tryConnectWithSavedCredentials();

void updateWiFiStatus(void *arg);
// Função para tentar conectar ao Wi-Fi
LV_FONT_DECLARE(lv_font_montserrat_20); // Fonte com suporte UTF-8
void textarea_event_cb(lv_event_t *e)
{
    lv_obj_t *textarea = lv_event_get_target(e);
    lv_keyboard_set_textarea(keyboard, textarea); // Associa o teclado ao campo de texto ativo
}
//

// Função para tentar conectar ao Wi-Fi
bool tryConnectToWiFi(const char *ssid, const char *password)
{

    WiFi.begin(ssid, password); // Inicia a conexão Wi-Fi

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000); // Aguarda 1 segundo
        Serial.print(".");
        attempts++;

        if (attempts >= 10)
        { // Timeout após 10 tentativas
            Serial.println("\nFalha na conexão ao Wi-Fi.");
            return false;
        }
    }

    Serial.println("\nConectado ao Wi-Fi!");
    return true;
}
// Função para salvar as credenciais via preferences
void saveCredentials(const char *ssid, const char *password)
{
    preferences.begin("wifi", false);
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    preferences.end();

    Serial.println("Credenciais salvas!");
}

// Função para exibir a tela de login
void showLoginScreen()
{
    lv_obj_clean(lv_scr_act()); // Limpa todos os objetos da tela atual

    // Adicionar um spinner para indicar carregamento
    lv_obj_t *spinner = lv_spinner_create(lv_scr_act(), 1000, 60); // Duração e largura da linha
    lv_obj_set_size(spinner, 50, 50);                              // Define o tamanho do spinner
    lv_obj_align(spinner, LV_ALIGN_TOP_MID, 0, 20);                // Posiciona o spinner no topo da tela

    // Campo para o SSID
    ssid_input = lv_textarea_create(lv_scr_act());
    lv_textarea_set_placeholder_text(ssid_input, "SSID");
    lv_obj_set_width(ssid_input, 300);
    lv_obj_set_height(ssid_input, 50);
    lv_obj_align(ssid_input, LV_ALIGN_CENTER, 0, -100);

    // Campo para a senha
    password_input = lv_textarea_create(lv_scr_act());
    lv_textarea_set_placeholder_text(password_input, "Senha");
    lv_textarea_set_password_mode(password_input, true); // Esconde a senha
    lv_obj_set_width(password_input, 300);
    lv_obj_set_height(password_input, 50);
    lv_obj_align(password_input, LV_ALIGN_CENTER, 0, -20);

    // Criar o teclado virtual e aumentar o tamanho
    keyboard = lv_keyboard_create(lv_scr_act());
    lv_keyboard_set_mode(keyboard, LV_KEYBOARD_MODE_TEXT_LOWER);
    lv_obj_set_size(keyboard, 460, 200); // Teclado maior
    lv_obj_align(keyboard, LV_ALIGN_BOTTOM_MID, 0, 0);

    // Associa o teclado às textareas
    lv_keyboard_set_textarea(keyboard, ssid_input); // O teclado começa com SSID ativo

    // Evento para alternar entre SSID e Senha
    lv_obj_add_event_cb(ssid_input, textarea_event_cb, LV_EVENT_FOCUSED, NULL);     // Foca no SSID
    lv_obj_add_event_cb(password_input, textarea_event_cb, LV_EVENT_FOCUSED, NULL); // Foca na Senha

    // Adicionar evento de "ENTER" para salvar as credenciais
    lv_obj_add_event_cb(keyboard, keyboard_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

// Callback para eventos do teclado
void keyboard_event_cb(lv_event_t *e)
{

    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    { // Evento disparado quando uma tecla é pressionada
        lv_obj_t *kb = lv_event_get_target(e);
        uint32_t key = lv_keyboard_get_selected_btn(kb); // Obtém a tecla pressionada

        // Depuração: Mostra o código da tecla pressionada
        // Serial.print("Tecla pressionada: ");
        // Serial.println(key);

        // Verifica se a tecla pressionada é o ENTER (39 no seu caso)
        if (key == 39)
        { // Use o valor 39 para a tecla ENTER
            const char *ssid = lv_textarea_get_text(ssid_input);
            const char *password = lv_textarea_get_text(password_input);

            if (ssid != NULL && password != NULL)
            {
                // Tenta conectar ao Wi-Fi
                bool connected = tryConnectToWiFi(ssid, password);
                if (connected)
                {
                    // Se a conexão for bem-sucedida, salva as credenciais
                    lv_scr_load(ui_Screen1); // Carrega a tela principal (ui_Screen1)
                    lv_task_handler();       // Força o LVGL a atualizar a tela
                    updateWiFiStatus(NULL);
                    Serial.println("Tela principal carregada.");
                }
            }
            else
            {
                Serial.println("Nenhum texto inserido.");
            }
        }
    }
}
//
void updateWiFiStatus(void *arg)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        lv_label_set_text(wifiStatus, LV_SYMBOL_WIFI);     // Exibe o símbolo Wi-Fi
        lv_label_set_text(ssidLabel, WiFi.SSID().c_str()); // Exibe o nome da rede conectada
    }
    else
    {
        lv_label_set_text(wifiStatus, LV_SYMBOL_WARNING); // Exibe símbolo de alerta
        lv_label_set_text(ssidLabel, "SEM WI-FI");        // Exibe "SEM WI-FI"
    }
}

// Função para tentar conectar com as credenciais salvas
void tryConnectWithSavedCredentials()
{
    preferences.begin("wifi", true); // Abre as preferences em modo leitura
    String ssid = preferences.getString("ssid", "");
    String password = preferences.getString("password", "");
    preferences.end();

    if (ssid != "" && password != "")
    {
        Serial.println("Tentando conectar com credenciais salvas...");
        bool connected = tryConnectToWiFi(ssid.c_str(), password.c_str());

        // Depuração: Verifique o valor de connected
        Serial.print("Valor de connected: ");
        Serial.println(connected ? "true" : "false");

        if (!connected)
        {
            Serial.println("Falha na conexão com credenciais salvas. Exibindo tela de login...");
            lv_obj_clean(lv_scr_act()); // Limpa todos os objetos da tela atual
            showLoginScreen();          // Exibe a tela de login
        }
    }
    else
    {
        Serial.println("Nenhuma credencial salva. Exibindo tela de login...");
        showLoginScreen();
    }
}

void setup()
{
    Serial.begin(115200);
    saveCredentials("false", "false");
    display_init();                   // Inicializa o display
    touch_init_lvgl();                // Inicializa o touch
    ui_init();                        // Inicializa a interface gráfica
    tryConnectWithSavedCredentials(); // Tenta conectar com as credenciais salvas
    Serial.println("Setup concluído.");
}

void loop()
{
    lv_task_handler();
    delay(5);
}