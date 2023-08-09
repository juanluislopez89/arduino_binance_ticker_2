void start_marquee(){
    Serial.println("Inicializando matrices LED...");
    matrix.setIntensity(intensity);
    for(byte i = 0; i <= 7; i++){
       matrix.setRotation(i, 1);
    }
    Serial.println("Matrices LED inicialidadas!");
}

void marquee_scroll(String scroll_text) {
    for (int i = 0; i < width * scroll_text.length() + matrix.width() - 1 - spacer; i++) {
        matrix.fillScreen(LOW);
        int letter = i / width;
        int x = (matrix.width() - 1) - i % width;
        int y = (matrix.height() - 8) / 2;

        while (x + width - spacer >= 0 && letter >= 0) {
            if (letter < scroll_text.length()) {
                matrix.drawChar(x, y, scroll_text[letter], HIGH, LOW, 1);
            }
            letter--;
            x -= width;
        }
        
        matrix.write();
        delay(scroll_wait);
    }
}
