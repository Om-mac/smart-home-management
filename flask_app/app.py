from flask import Flask, render_template, request, redirect, jsonify
import os
from twilio.rest import Client

app = Flask(__name__)

# === Store latest sensor values ===
sensor_data = {
    "temp": 0,
    "hum": 0,
    "mq2": 0,
    "mq6": 0,
    "flame": 1
}

# === Door command storage ===
door_command = "CLOSED"

# === Twilio WhatsApp Configuration ===
account_sid = os.environ.get("TWILIO_ACCOUNT_SID")
auth_token = os.environ.get("TWILIO_AUTH_TOKEN")
client = Client(account_sid, auth_token)

TWILIO_WHATSAPP_FROM = "whatsapp:+14155238886"   # Twilio sandbox number
TO_WHATSAPP_NUMBER = "whatsapp:+919420440361"    # Your verified WhatsApp number


# === Home page ===
@app.route('/')
def index():
    return render_template('index.html', data=sensor_data)


# === ESP32 updates sensor values here ===
@app.route('/update')
def update():
    global sensor_data

    # Get sensor readings from ESP32
    sensor_data['temp'] = request.args.get('temp', 0)
    sensor_data['hum'] = request.args.get('hum', 0)
    sensor_data['mq2'] = request.args.get('mq2', 0)
    sensor_data['mq6'] = request.args.get('mq6', 0)
    sensor_data['flame'] = int(request.args.get('flame', 1))

    # === Send WhatsApp Alert if Flame Detected ===
    if sensor_data['flame'] == 0:   # assuming 0 = flame detected
        try:
            message = client.messages.create(
                from_=TWILIO_WHATSAPP_FROM,
                to=TO_WHATSAPP_NUMBER,
                body="🚨 Urgent Alert! Flame detected in your home. Please check immediately."
            )
            print("🔥 WhatsApp Alert Sent! SID:", message.sid)
        except Exception as e:
            print("Error sending WhatsApp message:", e)

    return "OK"


# === Route for the dashboard JS to fetch live data ===
@app.route('/data')
def data():
    return jsonify(sensor_data)


# === Webpage button triggers door open ===
@app.route('/open-door')
def open_door():
    global door_command
    door_command = "OPEN"
    return redirect("/")


# === ESP32 checks this to see if door should open ===
@app.route('/door_status')
def door_status():
    return door_command


# === ESP32 resets door command after opening ===
@app.route('/door_status_reset')
def door_status_reset():
    global door_command
    door_command = "CLOSED"
    return "OK"


if __name__ == "__main__":
    # host="0.0.0.0" allows ESP32 and other LAN devices to connect
    app.run(host="0.0.0.0", port=5000, debug=True)