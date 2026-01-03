#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         // https://github.com/tzapu/WiFiManager
#include <EEPROM.h>
#include <ESP8266Ping.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

// Объявление номеров пинов для кнопок
const int buttonPin1 = D1;
const int buttonPin2 = D2;
const int buttonPin3 = D3;
const int buttonPin4 = D5;

// Переменные для хранения состояния кнопок
bool lastButtonState1 = HIGH;
bool lastButtonState2 = HIGH;
bool lastButtonState3 = HIGH;
bool lastButtonState4 = HIGH;

#include <Ticker.h>
Ticker flipper;
int count = 0;

#define led_wifi D4

const char* remote_host = "";
#define mqtt_server ""
#define mqtt_login ""
#define mqtt_password ""
#define mqtt_port 1883

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;

// подключение к mqtt брокеру
void reconnect() {
  if (Ping.ping(remote_host)) {
    Serial.println("ping: сервер mqtt доступен");
    while (!client.connected()) {// крутимся пока не подключемся.
      Serial.print("mqtt: попытка подключения...");
      // создаем случайный идентификатор клиента
      String clientId = "POST-";
      clientId += String(random(0xffff), HEX);
      // подключаемся, в client.connect передаем ID, логин и пасс
      if (client.connect(clientId.c_str(), mqtt_login, mqtt_password)) {
        digitalWrite(led_wifi, HIGH);
        Serial.println("подключились");// если подключились
      } else {
        digitalWrite(led_wifi, LOW);
        Serial.print("ошибка, rc=");
        Serial.print(client.state());
        Serial.println(" повтор через 5 секунд");
        // Wait 5 seconds before retrying
        delay(5000);
      }
    }
  } else {
    Serial.println("ping: сервер mqtt не доступен");
  }
}

void setup() {
  Serial.begin(115200);
  // Инициализация кнопок как входов с подтяжкой
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
  pinMode(led_wifi, OUTPUT);
  digitalWrite(led_wifi, LOW);
  client.setServer(mqtt_server, mqtt_port);
  WiFiManager wifiManager;
  wifiManager.setTimeout(180);
  if (!wifiManager.autoConnect("POST-1", "123qwe123")) {
    Serial.println("wifimanager: ошибка потключения");
    delay(3000);
    ESP.reset();
    delay(5000);
  }
}

void loop() {
  if (!client.connected()) {// проверяем подключение к брокеру
    reconnect();
  }
  client.loop();
  // Чтение состояния кнопок
  bool currentState1 = digitalRead(buttonPin1);
  bool currentState2 = digitalRead(buttonPin2);
  bool currentState3 = digitalRead(buttonPin3);
  bool currentState4 = digitalRead(buttonPin4);
  // Обработка нажатия кнопки 1
  if (lastButtonState1 == HIGH && currentState1 == LOW) {
    String message = "1";
    client.publish("sto/post1", message.c_str());
    Serial.println("Отправлено сообщение при нажатии кнопки 1");
  }
  lastButtonState1 = currentState1;

  // Обработка нажатия кнопки 2
  if (lastButtonState2 == HIGH && currentState2 == LOW) {
    String message = "2";
    client.publish("sto/post1", message.c_str());
    Serial.println("Отправлено сообщение при нажатии кнопки 2");
  }
  lastButtonState2 = currentState2;

  // Обработка нажатия кнопки 3
  if (lastButtonState3 == HIGH && currentState3 == LOW) {
    String message = "3";
    client.publish("sto/post2", message.c_str());
    Serial.println("Отправлено сообщение при нажатии кнопки 3");
  }
  lastButtonState3 = currentState3;

  // Обработка нажатия кнопки 4
  if (lastButtonState4 == HIGH && currentState4 == LOW) {
    String message = "4";
    client.publish("sto/post2", message.c_str());
    Serial.println("Отправлено сообщение при нажатии кнопки 4");
  }
  lastButtonState4 = currentState4;

  delay(50); // небольшая задержка для дебаунса
}
