#include <WiFi.h>
#include <SPIFFS.h>
#include <WebServer.h>
#include "esp_wifi.h"

WebServer server(80);

// ------------------- UTILIDADES -------------------

String getPacketType(uint8_t type) {
  switch (type) {
    case 0x08: return "Data";
    case 0x40: return "Probe Request";
    case 0x50: return "Probe Response";
    case 0x80: return "Beacon";
    default: return "Otro";
  }
}

String getMAC(const uint8_t *addr) {
  char macStr[18];
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X",
          addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
  return String(macStr);
}

// ------------------- CALLBACK PROMISCUO -------------------

void snifferCallback(void* buf, wifi_promiscuous_pkt_type_t type) {
  const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t*)buf;
  const uint8_t *payload = pkt->payload;
  uint8_t packetType = payload[0];

  String tipo = getPacketType(packetType);

  if (tipo != "Otro") {
    String macOrigen = getMAC(&payload[10]);
    String macDestino = getMAC(&payload[4]);
    int rssi = pkt->rx_ctrl.rssi;
    int canal = pkt->rx_ctrl.channel;

    String logLine = tipo + "," + macOrigen + "," + macDestino + "," + rssi + "," + canal + "\n";
    Serial.print("[" + tipo + "] ");
    Serial.print("Origen: " + macOrigen);
    Serial.print(" | Destino: " + macDestino);
    Serial.print(" | RSSI: " + String(rssi));
    Serial.println(" | Canal: " + String(canal));

    File f = SPIFFS.open("/sniffer_log.txt", FILE_APPEND);
    if (f) {
      f.print(logLine);
      f.close();
    }
  }
}

// ------------------- SETUP -------------------

void setup() {
  Serial.begin(115200);
  delay(1000);

  if (!SPIFFS.begin(true)) {
    Serial.println("Error montando SPIFFS");
  }

  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP32_Sniffer", "12345678");
  Serial.println("Access Point creado: ESP32_Sniffer");
  Serial.println("IP: " + WiFi.softAPIP().toString());

  server.on("/sniffer_log.txt", HTTP_GET, []() {
    File f = SPIFFS.open("/sniffer_log.txt", FILE_READ);
    if (!f) {
      server.send(404, "text/plain", "Archivo no encontrado");
      return;
    }

    server.streamFile(f, "text/plain");
    f.close();
  });

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", "<h2>Sniffer ESP32</h2><a href='/sniffer_log.txt'>Descargar log</a>");
  });

  server.begin();
  Serial.println("Servidor HTTP iniciado");

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);
  esp_wifi_set_storage(WIFI_STORAGE_RAM);
  esp_wifi_set_mode(WIFI_MODE_NULL);
  esp_wifi_start();

  esp_wifi_set_promiscuous_rx_cb(snifferCallback);
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(6, WIFI_SECOND_CHAN_NONE);

  Serial.println("Modo promiscuo activado en canal 6");
}

// ------------------- LOOP -------------------

void loop() {
  server.handleClient();
  delay(10);
}
