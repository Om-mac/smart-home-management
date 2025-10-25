#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <ESP32Servo.h>
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

// === Pin Definitions ===
#define DHTPIN 32
#define DHTTYPE DHT11
#define FLAME_PIN 25
#define SERVO_PIN 27
#define BUZZER_PIN 26
#define MQ2_PIN 34   // Digital input
#define MQ6_PIN 35   // Digital input
#define FINGER_RX 16
#define FINGER_TX 17
#define SOLENOID_PIN 33  // Relay controlling 12V solenoid

// === Relay Config ===
#define RELAY_ACTIVE_HIGH true   // Change to false if your relay is active LOW

// Define readable solenoid states
const int SOLENOID_LOCKED_STATE   = (RELAY_ACTIVE_HIGH ? HIGH : LOW);
const int SOLENOID_UNLOCKED_STATE = (RELAY_ACTIVE_HIGH ? LOW  : HIGH);

DHT dht(DHTPIN, DHTTYPE);
Servo doorServo;
HardwareSerial fingerSerial(2);
Adafruit_Fingerprint finger(&fingerSerial);

// === WiFi credentials ===
const char* ssid = "DEAD";
const char* password = "12345678";

// === Flask server URLs ===
const char* serverURL = "http://10.71.7.64:5000/update";
const char* doorStatusURL = "http://10.71.7.64:5000/door_status";
const char* doorResetURL  = "http://10.71.7.64:5000/door_status_reset";

void setup() {
  Serial.begin(115200);
  Serial.println("======================================");
  Serial.println(" 🚀 SMART DOOR SYSTEM INITIALIZING...");
  Serial.println("======================================");

  dht.begin();
  pinMode(FLAME_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MQ2_PIN, INPUT);
  pinMode(MQ6_PIN, INPUT);
  pinMode(SOLENOID_PIN, OUTPUT);

  // === Ensure Solenoid Starts Locked ===
  digitalWrite(SOLENOID_PIN, SOLENOID_LOCKED_STATE);

  doorServo.attach(SERVO_PIN);
  doorServo.write(0); // Start closed

  // === Fingerprint Sensor Setup ===
  fingerSerial.begin(57600, SERIAL_8N1, FINGER_RX, FINGER_TX);
  finger.begin(57600);
  Serial.print("🔍 Checking fingerprint sensor... ");
  if (finger.verifyPassword()) {
    Serial.println("✅ Fingerprint sensor detected!");
  } else {
    Serial.println("❌ Fingerprint sensor not found! Check connections.");
    while (1);
  }

  // === WiFi Connection ===
  Serial.print("📡 Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 20) {
    delay(500);
    Serial.print(".");
    retry++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi connected successfully!");
    Serial.print("🌐 ESP32 IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n⚠️ WiFi connection failed. Restart ESP.");
  }

  Serial.println("======================================");
  Serial.println(" System Ready ✅");
  Serial.println("======================================\n");
}

void loop() {
  Serial.println("\n📊 Reading Sensors...");
  checkFingerprint();

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int flame = digitalRead(FLAME_PIN);
  int mq2Digital = digitalRead(MQ2_PIN);  // 1 = clean air, 0 = gas detected
  int mq6Digital = digitalRead(MQ6_PIN);  // 1 = clean air, 0 = gas detected

  // === Display sensor readings ===
  Serial.printf("🌡 Temp: %.2f°C | 💧 Humidity: %.2f%%\n", temp, hum);
  Serial.printf("🔥 Flame: %d\n🧪 MQ2 : %s\n🧪 MQ6 : %s\n", 
                flame, 
                mq2Digital ? "Not detected" : "Gas Detected", 
                mq6Digital ? "Not detected" : "Gas Detected");

  // === Fire detection logic ===
  if (flame == LOW) {  // LOW means flame detected (depending on module)
    Serial.println("🚨🚨🚨 FIRE DETECTED! 🚨🚨🚨");
    Serial.println("🔥🔥🔥 Triggering Alarm 🔥🔥🔥");
    beep(10, 200);  // Beep buzzer 10 times
  }

  sendDataToFlask(temp, hum, mq2Digital, mq6Digital, flame);
  checkDoorCommandFromFlask();

  Serial.println("--------------------------------------");
  delay(2000);
}

// === Fingerprint Handling ===
void checkFingerprint() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return;

  Serial.println("👉 Finger detected, processing...");
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) {
    Serial.println("❌ Fingerprint image error.");
    return;
  }

  p = finger.fingerFastSearch();

  if (p == FINGERPRINT_OK) {
    Serial.println("🔓 Fingerprint Matched! Door Opening...");
    smoothDoorCycle(130);
  } else {
    Serial.println("❌ Fingerprint Not Matched! Buzzer Alert!");
    beep(5, 200);
  }
}

// === Send Data to Flask Server ===
void sendDataToFlask(float temp, float hum, int mq2, int mq6, int flame) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("⚠️ WiFi not connected. Skipping data send.");
    return;
  }

  HTTPClient http;
  String url = String(serverURL) + "?temp=" + String(temp) + "&hum=" + String(hum)
               + "&mq2=" + String(mq2) + "&mq6=" + String(mq6)
               + "&flame=" + String(flame);

  Serial.println("🌐 Sending data to Flask server...");
  http.begin(url);
  int res = http.GET();
  if (res > 0) {
    Serial.print("✅ Flask Response: ");
    Serial.println(res);
  } else {
    Serial.println("❌ Error sending data to Flask.");
  }
  http.end();
}

// === Check Door Command from Flask ===
void checkDoorCommandFromFlask() {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  http.begin(doorStatusURL);
  int res = http.GET();
  if (res > 0) {
    String response = http.getString();
    Serial.print("🛰 Door Command from Flask: ");
    Serial.println(response);

    if (response == "OPEN") {
      Serial.println("🔓 Flask Command: Opening Door...");
      smoothDoorCycle(130);

      // Reset the door command
      HTTPClient http2;
      http2.begin(doorResetURL);
      http2.GET();
      http2.end();
    }
  } else {
    Serial.println("⚠️ Error checking door_status.");
  }
  http.end();
}

// === Door Cycle: Unlock Solenoid + Open/Close Servo ===
void smoothDoorCycle(int angle) {
  Serial.println("🔑 Unlocking Solenoid...");
  digitalWrite(SOLENOID_PIN, SOLENOID_UNLOCKED_STATE); // Unlock
  delay(1000);

  Serial.println("🚪 Opening Door...");
  for (int pos = 0; pos <= angle; pos += 5) {
    doorServo.write(pos);
    delay(30);
  }

  Serial.println("⏳ Door is open. Waiting 5 seconds...");
  delay(5000);

  Serial.println("🚪 Closing Door...");
  for (int pos = angle; pos >= 0; pos -= 5) {
    doorServo.write(pos);
    delay(30);
  }

  Serial.println("⏳ Door closed. Waiting 1 second...");
  delay(1000);

  Serial.println("🔒 Locking Solenoid...");
  digitalWrite(SOLENOID_PIN, SOLENOID_LOCKED_STATE); // Lock
  Serial.println("✅ Door cycle complete.");
}

// === Buzzer Beep Function ===
void beep(int times, int duration) {
  for (int i = 0; i < times; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(duration);
    digitalWrite(BUZZER_PIN, LOW);
    delay(duration);
  }
}