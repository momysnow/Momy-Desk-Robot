#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <FS.h>
#include <ArduinoJson.h>

// JSON file path for storing known networks
const char* known_networks_path = "/known_networks.json";

// Initialize known networks JSON object
DynamicJsonDocument known_networks(1024);

// Connect to WiFi network
bool connect_wifi(String ssid, String password) {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid.c_str(), password.c_str());

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

// Disconnect from WiFi network
void disconnect_wifi() {
  WiFi.disconnect();
}

// Get IP address
String get_ip_address() {
  return WiFi.localIP().toString();
}

// Check if internet is connected
bool is_internet_connected() {
  WiFiClient client;
  if (!client.connect("www.google.com", 80)) {
    return false;
  } else {
    return true;
  }
}

// Scan WiFi networks
void scan_wifi() {
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

// Initialize access point
void start_ap() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP('Cubot', 'Cubot1234');
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
}

// Disconnect access point
void disconnect_ap() {
  WiFi.softAPdisconnect();
}

// Handle API command
void handle_API_command() {
  String message;
  String ssid;
  String ip_address;
  String response;

  if (server.args() == 0) {
    message = "Invalid request";
    response = "{\"message\":\"" + message + "\"}";
    server.send(400, "application/json", response);
  } else {
    String action = server.argName(0);
    if (action == "wifi_setup") {
      ssid = server.arg("ssid");
      String password = server.arg("password");
      if (connect_wifi(ssid, password)) {
        message = "WiFi setup completed";
        ip_address = get_ip_address();
        // Store credentials in known networks JSON object
        known_networks[ssid] = password;
        File file = SPIFFS.open(known_networks_path, "w");
        if (!file) {
          Serial.println("Failed to open known networks file");
        } else {
          serializeJson(known_networks, file);
          file.close();
        }
        response = "{\"message\":\"" + message + "\",\"ssid\":\"" + ssid + "\",\"ip_address\":\"" + ip_address + "\"}";
        server.send(200, "application/json", response);
      } else {
        message = "WiFi setup failed";
        response = "{\"message\":\"" + message + "\"}";
        server.send(400, "application/json", response);
      }
    } else if (action == "led_on") {
      message = "LED turned on";
      response = "{\"message\":\"" + message + "\"}";
      server.send(200, "application/json", response);
    } else if (action == "led_off") {
      message = "LED turned off";
      response = "{\"message\":\"" + message + "\"}";
      server.send(200, "application/json", response);
    } else {
      message = "Invalid request";
      response = "{\"message\":\"" + message + "\"}";
      server.send(400, "application/json", response);
    }
  }
}

// Initialize known networks JSON object
void init_known_networks() {
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
  } else {
    // Load known networks from JSON file
    File file = SPIFFS.open(known_networks_path, "r");
    if (!file) {
      Serial.println("Failed to open known networks file");
    } else {
      deserializeJson(known_networks, file);
      file.close();
    }
  }
}