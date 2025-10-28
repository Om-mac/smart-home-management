from flask import Flask, render_template, request, redirect, jsonify
import os
from twilio.rest import Client

app = Flask(__name__)

sensor_data = {
    "temp": 0,
    "hum": 0,
    "mq2": 0,
    "mq6": 0,
    "flame": 1
}


door_command = "CLOSED"


account_sid = os.environ.get("TWILIO_ACCOUNT_SID")
auth_token = os.environ.get("TWILIO_AUTH_TOKEN")
client = Client(account_sid, auth_token)

TWILIO_WHATSAPP_FROM = "whatsapp:+14155238886"  
TO_WHATSAPP_NUMBER = "whatsapp:+919420440361"    



@app.route('/')
def index():
    return render_template('index.html', data=sensor_data)



@app.route('/update')
def update():
    global sensor_data


    sensor_data['temp'] = request.args.get('temp', 0)
    sensor_data['hum'] = request.args.get('hum', 0)
    sensor_data['mq2'] = request.args.get('mq2', 0)
    sensor_data['mq6'] = request.args.get('mq6', 0)
    sensor_data['flame'] = int(request.args.get('flame', 1))


    if sensor_data['flame'] == 0:  
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



@app.route('/data')
def data():
    return jsonify(sensor_data)



@app.route('/open-door')
def open_door():
    global door_command
    door_command = "OPEN"
    return redirect("/")



@app.route('/door_status')
def door_status():
    return door_command



@app.route('/door_status_reset')
def door_status_reset():
    global door_command
    door_command = "CLOSED"
    return "OK"


if __name__ == "__main__":

    app.run(host="0.0.0.0", port=5000, debug=True)