/*

  ========================================
  ||        COMPONENT       ||    PIN   ||
  ========================================
  ||  Sensor pH             ||    A0    ||
  ||  Water Level           ||    D7    ||
  ||  LED                   ||    D8    ||
  ||  Relay Valve 1 (INT4)  ||    D2    ||
  ||  Relay Valve 2 (INT3)  ||    D3    ||
  ||  Relay Motor   (INT2)  ||    D5    ||
  ||  Relay Pompa   (INT1)  ||    D6    ||
  ========================================

*/


#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd1(0x26, 20, 4);
//LiquidCrystal_I2C lcd2(0x27, 20, 4);
LiquidCrystal_I2C lcd1(0x23, 20, 4);
LiquidCrystal_I2C lcd2(0x26, 20, 4);

const char* ssid = "Rumah sakit";
const char* password = "k0stput1h";

// Initialize Telegram BOT
#define BOTtoken "6008536230:AAFpYLVJXl6neZpOjPcBv7k2_aQs-9Br60Q"
#define CHAT_ID "1726336699"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 0,5 second.
int botRequestDelay = 500;
unsigned long lastTimeBotRan;

// pH sensor
float calibration = 30.00; //21.42 - change this value to calibrate
const int analogInPin = A0; 
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;
float phValueFinal;

// Configuration pin
const int waterLevelSensorPin = D7;   // D7
const int ledPin = D8;              // D5
const int relayV1Pin = D3;            // D3
const int relayV2Pin = D4;            // D4
const int relayPoPin = D6;            // D6
const int relayMoPin = D5;          // D5
bool relayState = HIGH;

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n\n";
      welcome += "SYSTEM MENU. \n";
      welcome += "/relay_on to start the system \n";
      welcome += "/relay_off to shut down the system \n";
      welcome += "/pump_on to activate the pump \n\n\n";
      welcome += "NUTRITION MENU. \n";
      welcome += "/status to check the pH value \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/relay_on") {
      bot.sendMessage(chat_id, "System Activated", "");
      lcd2.clear();
      lcd2.setCursor(0, 0);
      lcd2.print("   Response State   ");
      lcd2.setCursor(0, 2);
      lcd2.print("  System is running ");
      relayState = LOW;
      digitalWrite(relayV1Pin, relayState);
      digitalWrite(relayV2Pin, relayState);
      delay(3000);
      digitalWrite(relayV1Pin, HIGH);
      digitalWrite(relayV2Pin, HIGH);
      digitalWrite(relayMoPin, relayState);
      //delay(30000);
      delay(3000);
      digitalWrite(relayMoPin, HIGH);
      lcd2.clear();
    }
    
    if (text == "/relay_off") {
      bot.sendMessage(chat_id, "System Shut down", "");
      lcd2.clear();
      lcd2.setCursor(0, 0);
      lcd2.print("   Response State   ");
      lcd2.setCursor(0, 2);
      lcd2.print("  System shut down  ");
      relayState = HIGH;
      digitalWrite(relayV1Pin, relayState);
      digitalWrite(relayV2Pin, relayState);
      digitalWrite(relayPoPin, relayState);
      digitalWrite(relayMoPin, relayState);
      delay(3000);
      lcd2.clear();
    }

    if (text == "/pump_on") {
      bot.sendMessage(chat_id, "Pump Activated", "");
      lcd2.clear();
      lcd2.setCursor(0, 0);
      lcd2.print("   Response State   ");
      lcd2.setCursor(0, 2);
      lcd2.print("   Pump Activated   ");
      relayState = LOW;
      digitalWrite(relayPoPin, relayState);
    }

    if (text == "/status") {
      String message = "pH Value Status\n\n";
      message += "pH Nutrisi: " + String(phValueFinal) + "\n";
      bot.sendMessage(CHAT_ID, message, "");
    }

  }
  
}

void ledTwinkle() {
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
}

void concatData() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}

void pHsensor() {
  for(int i=0;i<10;i++) 
 { 
 buf[i]=analogRead(analogInPin);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buf[i]>buf[j])
 {
 temp=buf[i];
 buf[i]=buf[j];
 buf[j]=temp;
 }
 }
   }
 avgValue=0;
 for(int i=2;i<8;i++)
 avgValue+=buf[i];
 float pHVol=(float)avgValue*5.0/1024/6;
 float phValue = -5.70 * pHVol + calibration;
 phValueFinal = phValue;
 Serial.print("pH: ");
 Serial.println(phValue);
 lcd1.setCursor(0, 2);
 lcd1.print("pH    : " + String(phValue));
}

/*
void pHsensor() {
  // Read the analog value from the pH sensor
  int pHValue = analogRead(pH_Sensor_Pin);

  // Convert the analog value to pH level
  float voltage = pHValue * (5.0 / 1023.0);
  // pH basa (pH up)
  float pHup = -5.70 * voltage + 20.56;
  // pH basa (pH up)
  float pHdown = -5.70 * voltage + 19.96;

  // Print the pH value to the serial monitor
  Serial.print("pH Up Value: ");
  Serial.println(pHup);
  Serial.print("pH Down Value: ");
  Serial.println(pHdown);

  // Wait for a moment
  delay(1000);
}
*/


void setup() {
  lcd1.begin();
  lcd2.begin();
  lcd1.backlight();
  lcd2.backlight();

  
  Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif

  pinMode(relayV1Pin, OUTPUT);
  pinMode(relayV2Pin, OUTPUT);
  pinMode(relayPoPin, OUTPUT);
  pinMode(relayMoPin, OUTPUT);
  pinMode(waterLevelSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  digitalWrite(relayV1Pin, relayState);
  digitalWrite(relayV2Pin, relayState);
  digitalWrite(relayPoPin, relayState);
  digitalWrite(relayMoPin, relayState);

  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
    lcd1.setCursor(0,1);
    lcd1.print("    Connecting...   ");
  }
  // Print Local IP Address
  Serial.println(WiFi.localIP());
  lcd1.clear();
  lcd1.setCursor(0,1);
  lcd1.print(" WiFi: " + String(ssid));
  lcd1.setCursor(0,2);
  lcd1.print("      ALL CLEAR     ");
  delay(2000);
  lcd1.clear();

 
}


void loop() {
  // LED
  ledTwinkle();
  
  int waterLevelSensorValue = digitalRead(waterLevelSensorPin);
  
  // Menu LCD 1
  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd1.print("    Sensor Value    ");

  // pH sensor
  pHsensor();

  if (waterLevelSensorValue == HIGH) {
    lcd1.setCursor(0, 3);
    lcd1.print("Tangki: Penuh       ");
    String message = "Tangki pengaduk telah mencapai batas maksimal.";
    bot.sendMessage(CHAT_ID, message, "");
  } else {
    lcd1.setCursor(0, 3);
    lcd1.print("Tangki: Kosong      ");
  }

  // Menu LCD 2
  lcd2.setCursor(0, 0);
  lcd2.print(" Nutritional Mixing ");
  lcd2.setCursor(0, 1);
  lcd2.print("  System with App   ");
  lcd2.setCursor(0, 2);
  lcd2.print("      Control       ");
  lcd2.setCursor(0, 3);
  lcd2.print("                    ");


  // Send data to Telegram
  concatData();
  
}
