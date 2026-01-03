# rfid-authorization
Authorization and change of user on the website.
<script src="https://unpkg.com/mqtt/dist/mqtt.min.js"></script> - JS MQTT client
<script src="https://cdn.tailwindcss.com"></script> - css

You need to install mosquitto on the server and configure websockets. I chose port 9001.

A sketch is provided for testing. However, pressing the button sends a message to the desired channel. The MFRC522 will be used later.
