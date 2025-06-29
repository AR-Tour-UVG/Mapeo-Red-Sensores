#include <Wire.h>
#include <U8g2lib.h>
#include <WiFi.h>

U8G2_SH1106_128X64_NONAME_F_SW_I2C oled(U8G2_R0, /* clock=*/9, /* data=*/8, /* reset=*/U8X8_PIN_NONE);

int currentPage = 0;
int networksPerPage = 6;
int totalPages = 1;
int n = 0;
unsigned long lastScan = 0;
const unsigned long scanInterval = 15000; // cada 15 segundos

void showWelcomeMessage() {
  oled.clearBuffer();
  oled.setFont(u8g2_font_6x10_tr);
  oled.drawStr(0, 20, "Bienvenido al Sniffer");
  oled.drawStr(0, 35, "AR UVG TOUR");
  oled.drawStr(0, 50, "ESP32 WiFi Scanner");
  oled.sendBuffer();
  delay(2500);
}

void scanNetworks() {
  WiFi.scanDelete(); // Borra resultados anteriores
  n = WiFi.scanNetworks(false, true); // Escaneo asíncrono
  totalPages = (n + networksPerPage - 1) / networksPerPage;
}

void updateScanIfNeeded() {
  if (millis() - lastScan > scanInterval) {
    scanNetworks();
    lastScan = millis();
  }
}

void setup() {
  Wire.begin(8, 9);
  oled.begin();
  oled.setFont(u8g2_font_6x10_tr);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  delay(1000);

  showWelcomeMessage();

  scanNetworks(); // Primer escaneo
  lastScan = millis();
}

void loop() {
  updateScanIfNeeded();

  oled.clearBuffer();

  if (n == 0) {
    oled.drawStr(0, 30, "No se detectaron redes");
  } else {
    int start = currentPage * networksPerPage;
    int end = min(start + networksPerPage, n);

    for (int i = start; i < end; i++) {
      String ssid = WiFi.SSID(i);
      int rssi = WiFi.RSSI(i);
      int channel = WiFi.channel(i);

      if (ssid.length() > 12) ssid = ssid.substring(0, 12) + "...";

      String line = ssid + " " + String(rssi) + "dBm Ch" + String(channel);
      oled.drawStr(0, 12 * (i - start) + 10, line.c_str());
    }

    String pg = "Pag " + String(currentPage + 1) + "/" + String(totalPages);
    oled.drawStr(0, 64 - 1, pg.c_str());
  }

  oled.sendBuffer();
  currentPage = (currentPage + 1) % ((n == 0) ? 1 : totalPages);
  delay(4000);
}
