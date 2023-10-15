#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <SD.h>
#include <ESPAsyncWiFiManager.h>
#include "FS.h"
#include "SPI.h"
#include <ESPAsyncWebServer.h>

const char* ssid = "BeOurGuest";
const char* password = "GleamingEat44";
const char* ap_ssid = "ESP8266-Hotspot";
const char* ap_password = "Passwort123";

AsyncWebServer server(80);

// Global variable
File currentFile;


const int chipSelect = D8;

bool isImageFile(String filename) {
  String fileNameForCheck = String(filename);
  fileNameForCheck.toLowerCase();
  return fileNameForCheck.endsWith(".jpeg") || fileNameForCheck.endsWith(".jpg") || fileNameForCheck.endsWith(".png") || fileNameForCheck.endsWith(".raw") || fileNameForCheck.endsWith(".gif");
}
void handleRoot(AsyncWebServerRequest* request) {
  String html = "<html><head><title>Galerie</title></head><body><h1>Galerie</h1>";

  File root = SD.open("/");
  if (root) {
    html += "<ul>";
    while (File file = root.openNextFile()) {
      if (file.isDirectory()) {
        html += "<li>Verzeichnis: ";
        html += file.name();
        html += "</li>";
      } else {
        String filename = file.name();
        if (isImageFile(filename)) {
          // html += "<p>Datei: ";
          // html += filename;
          html += "<li><a href='/download?file=";
          html += filename;
          html += "'>";
          html += filename;
          html += "</a></li>";
          // html += " target='_self'><!--img src='/download?file=";
          //html += filename;
          //html += "' alt='Bild aus dem ESP8266 Flashspeicher' width='350' height='100'--></a></p>";
        }
      }
      file.close();
    }
    root.close();
  } else {
    html += "<p>Fehler beim Öffnen des Verzeichnisses</p>";
  }

  html += "</body></html>";
  request->send(200, "text/html", html);
}

size_t sendChunk(uint8_t* buffer, size_t maxLen, size_t index) {
  if (currentFile && currentFile.available()) {
    size_t thisChunk = currentFile.read(buffer, maxLen);
    return thisChunk;
  } else {
    if (currentFile) {
      currentFile.close();  // Close the file
    }
    return 0;
  }
}

const int MAX_BUFFER_SIZE = 1024;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Verbindung zu WLAN wird hergestellt...");
  }

  if (SD.begin(chipSelect)) {
    Serial.println("SD-Karte erkannt und gemounted.");
  } else {
    Serial.println("Fehler beim Erkennen und Mounten der SD-Karte.");
  }

  server.on("/", HTTP_GET, handleRoot);

  server.on("/download", HTTP_GET, [](AsyncWebServerRequest* request) {
    String fileName = request->arg("file");
    currentFile = SD.open("/" + fileName, "r");
    if (currentFile) {
      AsyncWebServerResponse* response = request->beginChunkedResponse("image/jpeg", sendChunk);
      response->addHeader("Content-Disposition", "attachment; filename=" + fileName);
      request->send(response);
    } else
      request->send(404, "text/plain", "File not found.");
  });


  /*
  server.on("/download", HTTP_GET, [](AsyncWebServerRequest* request) {
    if (request->hasParam("file")) {
      AsyncWebParameter* fileParam = request->getParam("file");
      String filename = fileParam->value();
      Serial.print("Download File:");
      Serial.println(filename);
      if (SD.exists(filename)) {
        File file = SD.open(filename);
        size_t fileSize = file.size();
        if (fileSize) {
          AsyncWebServerResponse* response = request->beginResponse(file, filename, "immage/jpeg", true);
          request->send(response);
        }
        file.close();
      }
    }
  });
  */

  server.begin();

  MDNS.begin("esp8266");

  WiFi.softAP(ap_ssid, ap_password);

  Serial.println("Hotspot IP-Adresse: " + WiFi.softAPIP().toString());
  Serial.println("Verbinden Sie sich mit Ihrem Heimnetzwerk unter der IP: " + WiFi.localIP().toString());
}

void loop() {
}
