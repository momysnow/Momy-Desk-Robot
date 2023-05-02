#include "WifiManager.h"

WifiManager::WifiManager() {}

bool WifiManager::connect_wifi(const char* ssid, const char* password) {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 15) {
    delay(500);
    Serial.print(".");
    retries++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    return true;
  } else {
    Serial.println("");
    Serial.println("WiFi connection failed");
    return false;
  }
}

void WifiManager::scan_wifi() {
  int n = WiFi.scanNetworks();
  Serial.println("Scan done");
  if (n == 0) {
    Serial.println("No networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println("");
      delay(10);
    }
  }
}

void WifiManager::start_ap() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Cubot", "Cubot1234");
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
}

void WifiManager::load_known_networks() {
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
  } else {
    File file = SPIFFS.open(known_networks_path, "r");
    if (!file) {
      Serial.println("Failed to open known networks file");
    } else {
      StaticJsonDocument<1024> doc;
      DeserializationError error = deserializeJson(doc, file);
      if (error) {
        Serial.println("Failed to parse known networks file");
      } else {
        num_known_networks = 0;
        for (JsonVariant network : doc.as<JsonArray>()) {
          if (num_known_networks < 10) {
            const char* ssid = network["ssid"];
            const char* password = network["password"];
            strncpy(known_networks[num_known_networks].ssid, ssid, sizeof(known_networks[num_known_networks].ssid));
            strncpy(known_networks[num_known_networks].password, password, sizeof(known_networks[num_known_networks].password));
            num_known_networks++;
          }
        }
      }
      file.close();
    }
  }
}

void WifiManager::add_known_network(const char* ssid, const char* password) {
  if (num_known_networks < 10) {
    strncpy(known_networks[num_known_networks].ssid, ssid, sizeof(known_networks[num_known_networks].ssid));
    strncpy(known_networks[num_known_networks].password, password, sizeof(known_networks[num_known_networks].password));
    num_known_networks++;

    if (!SPIFFS.begin()) {
      Serial.println("Failed to mount file system");
    } else {
      File file = SPIFFS.open(known_networks_path, "w");
      if (!file) {
        Serial.println("Failed to open known networks file");
      } else {
        StaticJsonDocument<1024> doc;
        JsonArray networks = doc.to<JsonArray>();
        for (int i = 0; i < num_known_networks; i++) {
          JsonObject network = networks.createNestedObject();
          network["ssid"] = known_networks[i].ssid;
          network["password"] = known_networks[i].password;
        }
        serializeJson(doc, file);
        file.close();
      }
    }
  } else {
    Serial.println("Cannot add more than 10 known networks");
  }
}

void WifiManager::create_web_server() {
  server.on("/", std::bind(&WifiManager::handle_root, this));
  server.on("/led_on", std::bind(&WifiManager::handle_led_on, this));
  server.on("/led_off", std::bind(&WifiManager::handle_led_off, this));
  server.on("/wifi", std::bind(&WifiManager::handle_connect_wifi, this));
  server.begin();
  Serial.println("Web server started");
}

void WifiManager::handle_root() {
  String html = "<html><body><h1>ESP32 Web Server</h1>";
  html += "<p>LED pin is now ";
  html += (digitalRead(led_pin) == HIGH) ? "on" : "off";
  html += ".</p>";
  html += "<p><a href=\"/led_on\">Turn LED on</a></p>";
  html += "<p><a href=\"/led_off\">Turn LED off</a></p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void WifiManager::handle_led_on() {
  digitalWrite(led_pin, HIGH);
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void WifiManager::handle_led_off() {
  digitalWrite(led_pin, LOW);
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void WifiManager::handle_connect_wifi() {
  String ssid = server.arg("ssid");
  String password = server.arg("password");
  add_known_network(ssid, password);
  connect_wifi(ssid, password);
}

void WifiManager::handleClient() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            client.println("<body><h1>ESP32 Web Server</h1>");
            client.println("<p>LED pin is now ");
            client.println((digitalRead(led_pin) == HIGH) ? "on" : "off");
            client.println(".</p>");
            client.println("<p><a href=\"/led_on\"><button class=\"button\">Turn LED on</button></a></p>");
            client.println("<p><a href=\"/led_off\"><button class=\"button button2\">Turn LED off</button></a></p>");
            client.println("<form method='POST' action='/wifi'>");
            client.println("<label for='ssid'>SSID:</label><br>");
            client.println("<input type='text' id='ssid' name='ssid'><br>");
            client.println("<label for='password'>Password:</label><br>");
            client.println("<input type='password' id='password' name='password'><br><br>");
            client.println("<input type='submit' value='Connect'>");
            client.println("</form>");
            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
}
