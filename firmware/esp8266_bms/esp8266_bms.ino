#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "config.h"

/* -------------------- Objects -------------------- */
Ticker socTimer;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

/* -------------------- Pins -------------------- */
#define ONE_WIRE_BUS D4
#define INTERRUPT_PIN D2
#define STOP_INPUT D5
#define LED_PIN D7
#define BUZZER_PIN D1

/* -------------------- Sensors -------------------- */
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

/* -------------------- Globals -------------------- */
volatile bool interruptFired = false;
unsigned long pulseCount = 0;
unsigned long tickCount = 0;

int soc = 100;
int sameCounter = 0;
bool startProcess = false;

/* Temperature */
const float TEMP_THRESHOLD = 30.0;

/* -------------------- ISR -------------------- */
ICACHE_RAM_ATTR void handleInterrupt() {
  interruptFired = true;
}

/* -------------------- WiFi -------------------- */
void connectWiFi() {
  Serial.print("[WiFi] Connecting...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n[WiFi] Connected");
  Serial.print("[WiFi] IP: ");
  Serial.println(WiFi.localIP());
}

/* -------------------- MQTT -------------------- */
void connectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("[MQTT] Connecting...");
    if (mqttClient.connect("bms_soc_node")) {
      Serial.println("connected");
    } else {
      Serial.println("failed, retrying...");
      delay(2000);
    }
  }
}

/* -------------------- Setup -------------------- */
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n[BMS] Firmware starting...");

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(STOP_INPUT, INPUT);
  pinMode(INTERRUPT_PIN, INPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  sensors.begin();

  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), handleInterrupt, CHANGE);

  connectWiFi();
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  connectMQTT();

  /* SoC calculation every 1 second */
  socTimer.attach_ms(1000, calculateSoC);
}

/* -------------------- Loop -------------------- */
void loop() {
  if (!mqttClient.connected()) {
    connectMQTT();
  }
  mqttClient.loop();

  /* Count pulses from interrupt */
  if (interruptFired) {
    pulseCount++;
    if (pulseCount >= 1000) {
      tickCount++;
      pulseCount = 0;
    }
    interruptFired = false;
  }

  /* Temperature monitoring */
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  Serial.print("[TEMP] ");
  Serial.print(tempC);
  Serial.println(" C");

  if (tempC > TEMP_THRESHOLD) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("[ALERT] High temperature!");
  } else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(1000);
}

/* -------------------- SoC Logic -------------------- */
void calculateSoC() {
  bool stopPressed = digitalRead(STOP_INPUT);

  if (stopPressed && tickCount > 0) {
    Serial.print("[SoC] Tick count: ");
    Serial.println(tickCount);

    /* Simple heuristic-based SoC adjustment */
    if (tickCount > 105 || tickCount < 95) {
      soc--;
      Serial.println("[SoC] Decreased due to variation");
    } else {
      sameCounter++;
      if (sameCounter > 3) {
        soc--;
        sameCounter = 0;
        Serial.println("[SoC] Decreased after repeated same cycles");
      }
    }

    if (soc < 0) soc = 0;

    Serial.print("[SoC] Current SoC: ");
    Serial.print(soc);
    Serial.println(" %");

    char socStr[8];
    itoa(soc, socStr, 10);
    mqttClient.publish(MQTT_SOC_TOPIC, socStr);

    tickCount = 0;
  }
}

