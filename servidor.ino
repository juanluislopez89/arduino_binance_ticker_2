void start_server(){
    server.begin();
    // Handle AP mode configuration form
    server.on("/", HTTP_GET, http_get_index);
    server.on("/save_config", HTTP_POST, http_save_config);
}

void http_get_index() {
    String ssid = read_eeprom_ssid();
    String password = read_eeprom_password();
    String tickers = read_eeprom_tickers();

    Serial.print("SSID leída para formulario: "); Serial.println(ssid);
    Serial.print("Password leído para formulario: "); Serial.println(password);
    Serial.print("Tickers leídos para formulario: "); Serial.println(tickers);

    // Formatear tickers para mostrar en el formulario
    String formattedTickers;
    int tickersLength = tickers.length();
    for (int i = 0; i < tickersLength; i += 6) {
        formattedTickers += tickers.substring(i, i + 6) + ", ";
    }
    formattedTickers.trim(); // Eliminar espacios al final
    formattedTickers = formattedTickers.substring(0, formattedTickers.length() - 1); // Eliminar la coma final
    
    String html = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1.0, user-scalable=no'><title>Guardar</title></head><body style='background-color: #1E1E1E; color: #FFFFFF;'>";
    html += "<form method='post' action='save_config' style='background-color: #252526; padding: 20px; border-radius: 10px;'>";
    html += "<label for='ssid'>SSID:<label><br><input type='text' name='ssid' value='" + ssid + "' style='background-color: #2D2D30; color: #FFFFFF; border: none; padding: 5px; border-radius: 5px;'><br>";
    html += "<label for='ssid'>Password:</label><br><input type='password' name='password' value='" + password + "' style='background-color: #2D2D30; color: #FFFFFF; border: none; padding: 5px; border-radius: 5px;'><br>";
    html += "<label for='codes'>Tickers (separados por comas):</label><br><input type='text' name='tickers' value='" + formattedTickers + "' style='background-color: #2D2D30; color: #FFFFFF; border: none; padding: 5px; border-radius: 5px;'><br>";
    html += "<input type='submit' value='Submit' style='background-color: #0078D4; color: #FFFFFF; border: none; padding: 10px 15px; border-radius: 5px; cursor: pointer;'>";
    html += "</form>";
    html += "</body></html>";
    server.send(200, "text/html", html);
}

void http_save_config() {
   Serial.print("Grabando");
    String ssid = server.arg("ssid");
    String password = server.arg("password");
    String tickers = server.arg("tickers");

    // Eliminar espacios y la coma final de la lista de tickers
    tickers.replace(" ", ""); // Eliminar espacios interiores
    tickers.trim(); // Eliminar espacios al inicio y al final
    if (tickers.endsWith(",")) {
        tickers = tickers.substring(0, tickers.length() - 1); // Eliminar la coma final
    }

    // Separar tickers por comas y formatear cada ticker a 6 caracteres
    String formattedTickers;
    int startIdx = 0;
    int commaIdx = tickers.indexOf(',', startIdx);
    while (commaIdx != -1) {
        String ticker = tickers.substring(startIdx, commaIdx);
        while (ticker.length() < 6) {
            ticker += " ";
        }
        formattedTickers += ticker;
        startIdx = commaIdx + 1;
        commaIdx = tickers.indexOf(',', startIdx);
    }
    if (startIdx < tickers.length()) {
        String ticker = tickers.substring(startIdx);
        while (ticker.length() < 6) {
            ticker += " ";
        }
        formattedTickers += ticker;
    }
    
    Serial.print("Grabada SSID: "); Serial.println(ssid);
    Serial.print("Grabado Password: "); Serial.println(password);
    Serial.print("Grabados Tickers: "); Serial.println(formattedTickers);
    
    writeDataToEEPROM(ssid.c_str(), password.c_str() , formattedTickers.c_str());
    server.send(200, "text/plain", "Configuration saved.");
}
