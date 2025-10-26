Flask
Flask-Cors
'
h/w requirements : 
  ESP32
  solenoid 
  servo
  dht11
  12volt adapter(for solenoid)
  mq2 methane alcohol
  mq2 methane alchol lpg 
  R307s fingerprint sensor(image based)
  flame sensor
  jumpers and rainbow wires
  ....

connections :
     DHTPIN 32
     DHTTYPE DHT11
     FLAME_PIN 25
     SERVO_PIN 27
     BUZZER_PIN 26
     MQ2_PIN 34   
     MQ6_PIN 35   
     FINGER_RX 16
     FINGER_TX 17
     SOLENOID_PIN 33 

     ALL VCC -> 5V 
     EXCEPT RELAY 
     RELAY VCC -> 3.3V

     ALL GND -> GND

     ADAPTER VCC -> RELAY Com
     RELAY NO -> SOLENOID VCC

STEPS TO RUN THE CODE , FIRST CHANGE THE WIFI CREDENTAILS IN THE 
CODE.INO FILE AND THEN CHANGE IT'S IP ACCORDING TO YOUR IP ADDRESS 

1.RUN THE FLASK/APP.PY AND COPY THE PUBLIC IP FROM TERMINAL AND THEN 
PASTE THE IP TO CODE.INO IP VARIABLES 
2.UPLOAD THE CODE.INO INO YOUR ESP USING ARDUNIO IDE
3.LOOK AT THE SERIAL MONITOR ,WEATHER THE WIFI IS CONNECTED OR NOT 
3.1 .IF NOT CONNECTED THEN TROUBLESHOOT
4. OPEN YOU LOCALHOST AND CHEAK THE DASHBOARD READINGS
  


-----------------------------------------------------------------------------------------------------
-- OM TAPDIYA
-- VISHWAKARMA INSTITUTE OF TECHNOLOGY
-- DEPT CS(DATA SCIENCE)

LINKED-IN : www.linkedin.com/in/omtapdiya
GIT-HUB : https://github.com/Om-mac
EMAIL : omtapdiya75@gmail.com

