/*
@author - xhico
@github - https://github.com/xhico

This code effectively turns an ESP8266 device into a smart doorbell notifier,
notifying users via Telegram when the doorbell is rung during specified hours
while conserving power during inactive periods.
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <TimeLib.h>

// Check for Interrupt
volatile boolean interrupt_occurred = false;

// Set Microphone PIN
int sound_digital = 14;

// Network credentials
const char* ssid = "ssid";
const char* password = "password";

// Telegram BOT Token
#define BOT_TOKEN "BOT_TOKEN"
#define CHAT_ID "CHAT_ID"

// Set Telegram SSL Cert
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

// Set Interrupt ISR
ICACHE_RAM_ATTR void ISR() {
  interrupt_occurred = true;
}

// Clear Serial Console
void clear() {
  for (int i = 0; i < 100; i++) Serial.println("");
  Serial.println("----------------");
}

// Function to Connect to Wi-Fi
void connect_wifi() {
  secured_client.setTrustAnchors(&cert);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to '" + String(ssid) + "'");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(2500);
  }
  Serial.print("connected | IP address: ");
  Serial.println(WiFi.localIP());
}

// Function to Light Sleep
void light_sleep() {
  Serial.println("Sleep");
  wifi_set_opmode_current(NULL_MODE);
  wifi_fpm_set_sleep_type(MODEM_SLEEP_T);
  wifi_fpm_open();
  wifi_fpm_do_sleep(0xFFFFFFF);
  delay(10);
}

void setup() {
  Serial.begin(9600);
  pinMode(sound_digital, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(sound_digital), ISR, RISING);
  light_sleep();
}

void loop() {
  if (interrupt_occurred == true) {

    // Clear Serial Console
    clear();

    // Connect to Wi-Fi
    connect_wifi();

    // Get Current Time
    configTime(3600, 0, "pool.ntp.org");
    time(nullptr);

    // Send Telegram Message
    Serial.println("Send Telegram Message");
    bot.sendMessage(CHAT_ID, "Doorbell", "");

    // Reset Interrupt Flag
    interrupt_occurred = false;

    // Sleep
    light_sleep();
  }
}
