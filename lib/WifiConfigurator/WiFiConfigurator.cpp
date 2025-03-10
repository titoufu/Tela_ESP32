#include "WiFiConfigurator.h"  // Inclui o cabeçalho da classe WiFiConfigurator

// Construtor da classe WiFiConfigurator
WiFiConfigurator::WiFiConfigurator()
  : server(80), preferences() {}

// Função para iniciar a configuração da rede WiFi
void WiFiConfigurator::begin() {
  Serial.begin(115200);  // Inicializa a comunicação serial com baud rate de 115200
  setupWiFi();           // Chama a função para configurar a rede WiFi
}

// Função para tratar as solicitações do cliente
void WiFiConfigurator::handleClient() {
  server.handleClient();  // Trata as solicitações do cliente no servidor web
}

// Função para configurar a rede WiFi
void WiFiConfigurator::setupWiFi() {
  // Inicializa as preferências na EEPROM
  preferences.begin("wifi-cred", false);
  String stored_ssid = preferences.getString("ssid", "");          // Obtém o SSID armazenado na EEPROM
  String stored_password = preferences.getString("password", "");  // Obtém a senha armazenada na EEPROM
  preferences.end();                                               // Encerra o acesso às preferências na EEPROM
  Serial.println("entrando no modo AP");
  // Configura o modo do WiFi para estação (modo cliente)
  WiFi.mode(WIFI_STA);
  // Inicia a conexão com a rede WiFi utilizando as credenciais obtidas da EEPROM
  WiFi.begin(stored_ssid.c_str(), stored_password.c_str());

  // Tempo de início da tentativa de conexão
  unsigned long startAttemptTime = millis();
  const unsigned long maxAttemptDuration = 15000;  // 15 segundos
  // Loop para aguardar até que a conexão seja estabelecida ou o tempo máximo de tentativa seja atingido
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < maxAttemptDuration) {
    delay(500);
    Serial.print(".");
  }

  // Verifica se a conexão foi estabelecida com sucesso
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi conectado");  // Mensagem indicando que o WiFi foi conectado com sucesso
    Serial.println("Endereço IP (redeLocal): ");
    Serial.println(WiFi.localIP());  // Imprime o endereço IP atribuído ao ESP32 na rede local
    // Configura o tratamento da raiz do servidor web
    server.on("/", HTTP_GET, std::bind(&WiFiConfigurator::handleRoot, this));
    // Inicia o servidor web
    server.begin();
    Serial.println("Servidor iniciado");  // Mensagem indicando que o servidor web foi iniciado
  } else {
    Serial.println("");
    Serial.println("Falha ao se conectar à rede WiFi. Iniciando modo AP...");  // Mensagem indicando falha na conexão WiFi
    // Configura o ESP32 para operar como ponto de acesso WiFi (modo AP)
    WiFi.mode(WIFI_AP);
    // Cria uma rede WiFi com SSID e senha específicos
    WiFi.softAP("Horta_Malob", "12345678");
    // Obtém e imprime o endereço IP atribuído ao ponto de acesso WiFi
    IPAddress IP = WiFi.softAPIP();
    Serial.print("Endereço IP do AP (HortaMalob): ");
    Serial.println(IP);
    // Configura o tratamento da raiz do servidor web
    server.on("/", HTTP_GET, std::bind(&WiFiConfigurator::handleRoot, this));
    // Inicia o servidor web
    server.begin();
    Serial.println("Servidor iniciado");  // Mensagem indicando que o servidor web foi iniciado
  }
}

// Função para tratar a raiz do servidor web
void WiFiConfigurator::handleRoot() {
  // Verifica se foram fornecidas credenciais de rede WiFi
  if (server.hasArg("ssid") && server.hasArg("password")) {
    String ssid = server.arg("ssid");          // Obtém o SSID fornecido pelo usuário
    String password = server.arg("password");  // Obtém a senha fornecida pelo usuário

    // Armazena as credenciais na EEPROM
    preferences.begin("wifi-cred", false);
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    preferences.end();  // Encerra o acesso às preferências na EEPROM

    // Cria uma mensagem confirmando o recebimento das credenciais
    //String message = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1.0'></head><body><h2 style='text-align:center;color:#007bff;'>Dados recebidos</h2><p style='text-align:center;'>Nova Rede WiFi configurada: <strong>" + ssid + "</strong></p></body></html>";
    String message = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1.0'><style>body {display: flex;justify-content: center;align-items: center;height: 100vh;margin: 0;}.content {text-align: center;}</style></head><body><div class='content'><h2 style='color:#007bff;'>Dados recebidos</h2><p>Nova Rede WiFi configurada: <strong>" + ssid + "</strong></p></div></body></html>";
    // Envia a mensagem como resposta para o cliente
    server.send(200, "text/html", message);
    delay(2000);
    ESP.restart();  // Reinicia o ESP32 para aplicar as novas configurações de rede.
  } else {
    // Se não foram fornecidas credenciais, envia a página HTML de configuração
    server.send(200, "text/html", getPage());
  }
}

// Função para retornar a página HTML de configuração da rede WiFi
String WiFiConfigurator::getPage() {
  String message = "<!DOCTYPE html><html lang='pt-BR'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>CONEXÃO DO MEDIDOR</title><style>body {font-family: Arial, sans-serif;margin: 0;padding: 20px;background-color: #f4f4f4;display: flex;justify-content: center;align-items: center;height: 100vh;} .container {max-width: 400px;background-color: #fff;padding: 20px;border-radius: 10px;box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);} h1 {text-align: center;color: #333;} p {color: #666;} input[type='text'], input[type='password'] {width: calc(100% - 20px);padding: 10px;margin-bottom: 10px;border: 1px solid #ccc;border-radius: 5px;font-size: 16px;} input[type='submit'] {width: 100%;background-color: #007bff;color: #fff;border: none;padding: 10px;border-radius: 5px;cursor: pointer;font-size: 16px;} input[type='submit']:hover {background-color: #0056b3;}</style></head><body><div class='container'><h1>CONEXÃO DO MEDIDOR</h1><p>Entre com as credenciais da rede WiFi:</p><form action='/' method='get'><input type='text' name='ssid' placeholder='Nome da Rede'><br><input type='password' name='password' placeholder='Senha'><br><input type='submit' value='Enviar'></form></div></body></html>";
  return message;
}