#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <vector> // Incluir la librería de vectores dinámicos
#include "config.h"


ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(80);

int pinCS = 2;
Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

bool configMode = true;

const int SSID_EEPROM_ADDR = 0;          // Dirección de inicio para almacenar SSID (32 bytes)
const int PASSWORD_EEPROM_ADDR = 32;     // Dirección de inicio para almacenar Password (32 bytes)
const int TICKERS_EEPROM_ADDR = 64;      // Dirección de inicio para almacenar Tickers (120 bytes)


void setup() {
    Serial.begin(115200);
    Serial.println("Inicializando EEPROM");
    EEPROM.begin(512);
    start_marquee();
    start_access_point();
    start_server();
}

void loop() {
  if(configMode == true){
      server.handleClient();
  }
  else{
    if (WiFi.status() == WL_CONNECTED) {   
        // Leer los tickers almacenados en la EEPROM
        String tickers = read_eeprom_tickers();
        
        // Bucle de la marquesina con los tickers
        for (int n = 0; n <= tickers.length() - 6; n += 6) {
            String currency_code = tickers.substring(n, n + 6);
            currency_code.trim();
            http_get_currency_price(currency_code);
        }
    }
    else
    {
        Serial.println("Wifi desconectado !");
        //si no estamos conectados, conectamos a la wifi almacenada
        String ssid = read_eeprom_ssid();
        String password = read_eeprom_password();
        connectToWiFi(ssid.c_str(), password.c_str());
    }
  }  
}

void http_get_currency_price(String currency_code) {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    HTTPClient https;

    if (https.begin(*client, binance_ticker_api_url + currency_code + fiat_currency)) { 
        int httpCode = https.GET();
        if (httpCode > 0) {
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                String json = https.getString();
                StaticJsonDocument<300> doc;
                DeserializationError error = deserializeJson(doc, json);
                if (!error) {
                    String price = doc["price"];
                    while (price.endsWith("0") || price.endsWith(".")){
                        price.remove(price.length() - 1);
                    }
                    marquee_scroll(currency_code + ": " + price);
                }
            }
        } 
        https.end();
    } 
}
