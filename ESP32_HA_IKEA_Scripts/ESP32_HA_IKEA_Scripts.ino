#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

#include "secrets.h"

// Network credentials
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// Home Assistant
const char* haUrl = HA_URL;
const char* haToken = HA_TOKEN;

// Give up on a connection attempt after this long, then retry
const unsigned long connectTimeout = 20000;

// How often loop() checks that we are still connected
const unsigned long checkInterval = 5000;
unsigned long lastCheck = 0;

// Buttons wired between their pin and GND, using the internal pull-up
const unsigned long debounceDelay = 50;

struct Button {
  const char* name;
  int pin;
  const char* script;
  int lastReading;
  int state;
  unsigned long lastDebounce;
};

Button buttons[] = {
  { "btn1", 4, "script.desk_sit", HIGH, HIGH, 0 },
  { "btn2", 16, "script.desk_stand", HIGH, HIGH, 0 },
};
const int buttonCount = sizeof(buttons) / sizeof(buttons[0]);

void connectToWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to '" + String(ssid) + "'");

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < connectTimeout) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected");
    Serial.println("IP address: " + WiFi.localIP().toString());
    Serial.println("Signal: " + String(WiFi.RSSI()) + " dBm");
  } else {
    Serial.println("Failed to connect, will retry");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  for (int i = 0; i < buttonCount; i++) {
    pinMode(buttons[i].pin, INPUT_PULLUP);
  }
  connectToWiFi();
}

void runScript(const char* script) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Not connected, skipping " + String(script));
    return;
  }

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  http.begin(client, haUrl);
  http.addHeader("Authorization", "Bearer " + String(haToken));
  http.addHeader("Content-Type", "application/json");

  int code = http.POST("{\"entity_id\":\"" + String(script) + "\"}");
  if (code == 200) {
    Serial.println("Ran " + String(script));
  } else if (code > 0) {
    Serial.println("HA returned " + String(code) + " for " + String(script));
  } else {
    Serial.println("Request failed: " + http.errorToString(code));
  }

  http.end();
}

void checkButton(Button& b) {
  int reading = digitalRead(b.pin);

  // Any change restarts the settling window, so contact bounce is ignored
  if (reading != b.lastReading) {
    b.lastDebounce = millis();
  }

  if (millis() - b.lastDebounce > debounceDelay && reading != b.state) {
    b.state = reading;
    if (b.state == LOW) {
      Serial.println(String(b.name) + " pressed");
      runScript(b.script);
    }
  }

  b.lastReading = reading;
}

void loop() {
  for (int i = 0; i < buttonCount; i++) {
    checkButton(buttons[i]);
  }

  if (millis() - lastCheck >= checkInterval) {
    lastCheck = millis();

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Connection lost");
      WiFi.disconnect();
      connectToWiFi();
    }
  }
}
