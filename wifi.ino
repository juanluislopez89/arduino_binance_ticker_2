void start_access_point() {
    Serial.println("Inicializando Punto de acceso con SSID 'Configurator'...");
    WiFi.mode(WIFI_AP);
    WiFi.softAP("Configurator", "password");
    Serial.println("Punto de acceso inicializado!");
}

void connectToWiFi(const char* ssid, const char* password) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED && attempt < 10) {
        delay(1000);
        Serial.println("Conectando a " + String(ssid) + "...");
        attempt++;
    }
}
