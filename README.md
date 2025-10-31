# 🏠 Smart Door & Home Safety System  

> **IoT + Flask + ESP32 Project** for Smart Home Automation, Door Access Control, and Fire/Gas Safety Monitoring.  
> Integrates real-time sensor data, fingerprint-based security, and WhatsApp alerts via Twilio.

---

## ⚙️ Tech Stack

| Software | Hardware |
|-----------|-----------|
| 🐍 Flask (Backend) | ⚡ ESP32 |
| 🌐 Flask-CORS | 🔒 R307s Fingerprint Sensor |
| 💬 Twilio API (WhatsApp Alerts) | 🌡️ DHT11 Temperature & Humidity |
| 🧠 Arduino IDE (ESP32 code) | 🔥 Flame Sensor |
| | 💨 MQ2 & MQ6 Gas Sensors |
| | 🔩 Solenoid + Relay |
| | ⚙️ Servo Motor |
| | 🔔 Buzzer |

---

## 🧰 Hardware Requirements

| Component | Description |
|------------|-------------|
| 🟦 **ESP32** | Main microcontroller |
| 🔒 **R307s** | Fingerprint sensor (image-based) |
| 🌡 **DHT11** | Temperature & humidity sensor |
| 🔥 **Flame Sensor** | Fire detection |
| 💨 **MQ2** | Gas sensor (Methane/Alcohol/LPG) |
| 💥 **MQ6** | LPG & Methane gas sensor |
| 🔩 **Solenoid + Relay** | Door/valve locking system |
| ⚙️ **Servo Motor** | Door movement control |
| 🔔 **Buzzer** | Alert signaling |
| ⚡ **12 V Adapter** | Power for solenoid |
| 🪡 **Jumpers & Rainbow Wires** | Circuit connections |

---

## 🔌 Pin Connections

| Component | ESP32 Pin |
|------------|------------|
| 🌡️ DHT11 | `GPIO 32` |
| 🔥 Flame Sensor | `GPIO 25` |
| ⚙️ Servo Motor | `GPIO 27` |
| 🔔 Buzzer | `GPIO 26` |
| 💨 MQ2 | `GPIO 34` |
| 💥 MQ6 | `GPIO 35` |
| 🔒 Fingerprint RX | `GPIO 16` |
| 🔓 Fingerprint TX | `GPIO 17` |
| 🔩 Solenoid | `GPIO 33` |

---

## ⚡ Power Setup

| Connection | Description |
|-------------|-------------|
| 🔌 **All VCC → 5 V** | Power for most modules |
| ⚙️ **Relay VCC → 3.3 V** | ESP32-compatible relay |
| ⚫ **All GND → GND** | Common ground |
| ⚡ **Adapter VCC → Relay COM** | Power routing |
| 🔄 **Relay NO → Solenoid VCC** | Solenoid control path |

---

## 🚀 How to Run

<details>
<summary>🔹 1️⃣ Configure & Run Flask Server</summary>

1. Install dependencies  
   ```bash
   pip install flask twilio
2.	Set your Twilio credentials as environment variables
   export TWILIO_ACCOUNT_SID="your_sid"
   export TWILIO_AUTH_TOKEN="your_token"
3.	Run the Flask server
   python app.py
   •	Copy your system’s local IP address (shown in terminal).
	 •	Use it in your ESP32 code (e.g., http://<your-ip>:5000/update).
</details>
<details>
<summary>🔹 2️⃣ Flash ESP32 Code</summary>
	1.	Open code.ino in Arduino IDE.
	2.	Replace the Wi-Fi credentials:
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
  3.	Update the Flask server URLs:
   const char* serverURL = "http://YOUR_PC_IP:5000/update";
   const char* doorStatusURL = "http://YOUR_PC_IP:5000/door_status";
   const char* doorResetURL  = "http://YOUR_PC_IP:5000/door_status_reset";
  4.	Select ESP32 Dev Module board → Upload.
   </details>
<details>
<summary>🔹 3️⃣ Verify & Operate</summary>
	•	Open Serial Monitor (115200 baud).
	•	Check Wi-Fi connection logs.
	•	Place a registered fingerprint → door unlocks automatically.
	•	On fire detection, WhatsApp alert is sent via Twilio.
	•	Visit
  http://localhost:5000

  to view live sensor readings.

  </details>


🧑‍💻 Author

Om Tapdiya
💡 Smart Home & Door Automation Project
📫 IoT | Embedded Systems | Flask | ESP32

⸻

💖 Support & Contribute

If you like this project:
⭐ Star the repo • 🛠 Fork it • 💬 Share feedback
