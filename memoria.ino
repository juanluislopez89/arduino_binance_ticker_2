void clearEEPROM() {
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0); // Llenar con ceros
  }
}

void writeDataToEEPROM(const char* ssid, const char* password, const char* tickers) {
  clearEEPROM();

  // Almacenar SSID
  for (int i = 0; i < 32; i++) {
    EEPROM.write(SSID_EEPROM_ADDR + i, ssid[i]);
  }

  // Almacenar Password
  for (int i = 0; i < 32; i++) {
    EEPROM.write(PASSWORD_EEPROM_ADDR + i, password[i]);
  }

  // Almacenar Tickers
  int tickersLength = strlen(tickers);
  int maxTickers = 120;
  for (int i = 0; i < maxTickers; i++) {
    if (i < tickersLength) {
      EEPROM.write(TICKERS_EEPROM_ADDR + i, tickers[i]);
    } else {
      EEPROM.write(TICKERS_EEPROM_ADDR + i, ' ');
    }
  }

  EEPROM.commit();
}

String read_eeprom_ssid() {
  char storedSSID[33]; // +1 for null terminator
  for (int i = 0; i < 32; i++) {
    storedSSID[i] = EEPROM.read(SSID_EEPROM_ADDR + i);
  }
  storedSSID[32] = '\0';

  return String(storedSSID);
}

String read_eeprom_password() {
  char storedPassword[33]; // +1 for null terminator
  for (int i = 0; i < 32; i++) {
    storedPassword[i] = EEPROM.read(PASSWORD_EEPROM_ADDR + i);
  }
  storedPassword[32] = '\0';

  return String(storedPassword);
}

bool isPrintableChar(char c) {
  return (c >= 32 && c <= 126); // Caracteres imprimibles en ASCII
}

String read_eeprom_tickers() {
  char storedTickers[121]; // +1 for null terminator
  for (int i = 0; i < 120; i++) {
    char tickerChar = EEPROM.read(TICKERS_EEPROM_ADDR + i);
    if (isPrintableChar(tickerChar)) {
      storedTickers[i] = tickerChar;
    } else {
      storedTickers[i] = ' '; // Reemplazar caracteres no imprimibles con espacio
    }
  }
  storedTickers[120] = '\0';

  return String(storedTickers);
}
