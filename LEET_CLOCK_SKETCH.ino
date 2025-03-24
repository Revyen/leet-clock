#include <Adafruit_NeoPixel.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include "LETTER.h"
#include "animator.h"
#include "animation.h"
#include "sites.h"

#define PIN_WS2812B 2
#define NUM_PIXELS 41

String ap_ssid = "LEET_CLOCK_AP";
String ap_password = "leet1337";

IPAddress local_ip(192,168,4,1);
IPAddress gateway(192,168,4,254);
IPAddress netmask(255,255,255,0);

ESP8266WebServer server(80);

String ssid = "";
String password = "";



WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600, 600000);

Adafruit_NeoPixel strip(NUM_PIXELS, PIN_WS2812B, NEO_RGB + NEO_KHZ800);

LETTER one(0,7, strip);
LETTER three1(8,18, strip);
LETTER colon(19,20, strip);
LETTER three2(21,31, strip);
LETTER seven(32,40, strip);

LETTER letters[5] = { one, three1, colon, three2, seven };

Animator animator(strip, NUM_PIXELS, letters, 5);

String lastStartTime = "";
float highestRam = 0.0;
float lowestRam = 60.0;
String myIp = local_ip.toString();

void handleConnectToWifi() {
  server.send(200, "text/html", wifiCredentialsPage);
}

void handleSuccesfullyConnected() {
  String page = mainPage;
  String currentTime = timeClient.getFormattedTime();
  page.replace("%TIME%", currentTime);

  server.send(200, "text/html", page);
}

bool* checkForLeet(int currentHour, int currentMinute) {
  bool* booleanArray = new bool[4];
  booleanArray[0] = false;
  booleanArray[1] = false;
  booleanArray[2] = false;
  booleanArray[3] = false;
  // after 13:37 return early
  if (currentHour >= 13 && currentMinute > 37) {
    return booleanArray;
  }
  if (currentHour >= 10 && currentHour <= 13) {
    booleanArray[0] = true;
  }
  if (booleanArray[0] && currentHour == 13) {
    booleanArray[1] = true;
  }
  if (booleanArray[1] && currentMinute >= 30 && currentMinute <= 37) {
    booleanArray[2] = true;
  }
  if (booleanArray[2] && currentMinute == 37) {
    booleanArray[3] = true;
  }

  return booleanArray;
}


void handleTime() {
  timeClient.update();
  String currentTime = timeClient.getFormattedTime();
  server.send(200, "text/plain", currentTime);
}

void handleRamUsage() {
  float ramKB = float(ESP.getFreeHeap()) / float(1024);
  if (highestRam < ramKB) {
    highestRam = ramKB;
  }
  if (lowestRam > ramKB) {
    lowestRam = ramKB;
  }
  String ramUsage = 
    String(ramKB, 4) + " KB Freier Ram\n" + 
    String(lowestRam, 4) + " KB niedrigster Freier Ram\n" + 
    String(highestRam, 4) + " KB höchster Freier Ram\n" + 
    lastStartTime;
  server.send(200, "text/plain", ramUsage);
}

void handleSSIDSave() {
  ssid = server.arg("ssid");
  password = server.arg("password");

  WiFi.persistent(true);
  connectToWiFi();

  String page = connectingPage;
  page.replace("%IP%", myIp);

  server.send(200, "text/html", page);
  delay(2000);
}

void connectToWiFi() {
  if (ssid != "" && password != "" || !WiFi.SSID().isEmpty()) {
    if (ssid != "" && password != "") {
      WiFi.begin(ssid.c_str(), password.c_str());
    }
    if (!WiFi.SSID().isEmpty()) {
      WiFi.begin();
    }

    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
    animator.startAnimation(Animation(Animation::PULSE, RGB().deepRed(), 50));
    Serial.println("Connected!");
    Serial.println("IP Address: ");
    Serial.println(WiFi.localIP());
    myIp = WiFi.localIP().toString();
    timeClient.update();
    server.close();
    server.on("/", handleSuccesfullyConnected);
    server.on("/time", handleTime);
    server.on("/ram", handleRamUsage);
    server.begin();
  } else {
    Serial.println("No WiFi confiured...");
    Serial.println("Starting AP");
    WiFi.mode(WIFI_AP);
    startAP();
  }

}

void startAP() {
  WiFi.softAPConfig(local_ip, gateway, netmask);
  WiFi.softAP(ap_ssid, ap_password);

  server.on("/", handleConnectToWifi);
  server.on("/save", HTTP_POST, handleSSIDSave);
  server.begin();
}

void initLed() {
  strip.begin();
}

void updateLed() {
  strip.show();
}

void setup() {
  Serial.begin(115200);
  initLed();
  randomSeed(analogRead(0));
  animator.startAnimation(Animation(Animation::HELLO, RGB().random12Color(), 0));
  connectToWiFi();
  ESP.wdtEnable(5000);
}

unsigned long previousSecondMillis = 0;
unsigned long previousAnimationMillis = 0;
unsigned long secondInterval = 1000;
unsigned long animationInterval = 100;

bool played = false;
bool isLeed = false;

void loop() {
  unsigned long currentMillis = millis();
  if (WiFi.status() != WL_CONNECTED && WiFi.getMode() != WIFI_AP_STA && WiFi.getMode() != WIFI_AP) {
    Serial.println("WiFi Disconnected...");
    Serial.println("Spawning AP for local connection");
    WiFi.mode(WIFI_AP_STA);
    startAP();
    Serial.println("AP Spawned:" + WiFi.softAPSSID());
  }
  if (WiFi.status() != WL_CONNECTED && !animator.getPlaying()) {
    animator.startAnimation(Animation(Animation::PULSE, RGB().yellow(), 50));
  }
  if (currentMillis - previousAnimationMillis >=animationInterval && animator.getPlaying()) {
    ESP.wdtFeed();
    animator.tick();
    if (isLeed && timeClient.getMinutes() >= 38) {
      animator.reset();
    }
    previousAnimationMillis = currentMillis;
  }
  if (currentMillis - previousSecondMillis >=secondInterval) {
    if (WiFi.status() == WL_CONNECTED && !animator.getPlaying()) {
      ESP.wdtFeed();
      handleTimeLed();
      handleDotSecondBlink();

      previousSecondMillis = currentMillis;
    }
  }
  updateLed();
  server.handleClient();
  if (ESP.getFreeHeap() < 5000) {
    Serial.println("Free heap below 5000 byte, restarting ESP...");
    ESP.restart();
  }
  // Satisfy ESP.wdt
  delay(50);
}

void handleDotSecondBlink() {
  bool isColonOn = colon.getState();
  if (isColonOn) {
    colon.turnOff();
  } else {
    colon.turnOn(RGB(100,100,100));
  }
};

void handleTimeLed() {
  const int currentHour = timeClient.getHours();
  const int currentMinute = timeClient.getMinutes();

  bool* check = checkForLeet(currentHour,currentMinute);

  for (int i = 0; i < 4; i++) {
    if (i == 0) {
      bool oneIsOn = one.getState();
      if (check[i] == 1 && !oneIsOn) {
        Serial.println("Triggering first letter");
        one.turnOn(RGB(100,100,100));
      }
      if (check[i] != 1 && oneIsOn) {
        one.turnOff();
      }
    }
    if (i == 1) {
      bool three1IsOn = three1.getState();
      if (check[i] == 1 && !three1IsOn) {
        Serial.println("Triggering second letter");
        three1.turnOn(RGB(100,100,100));
      }
      if (check[i] != 1 && three1IsOn) {
        three1.turnOff();
      }
    }
    if (i == 2) {
      bool three2IsOn = three2.getState();
      if (check[i] == 1 && !three2IsOn) {
        Serial.println("Triggering second letter");
        three2.turnOn(RGB(100,100,100));
      }
      if (check[i] != 1 && three2IsOn) {
        three2.turnOff();
      }
    }
    if (i == 3) {
      bool sevenIsOn = seven.getState();
      if (check[i] == 1 && !sevenIsOn) {
        Serial.println("Triggering second letter");
        seven.turnOn(RGB(100,100,100));
        animator.startAnimation(Animation(Animation::SPARKLE, RGB().random6Color(), 1), true);
        isLeed = true;
      }
      if (check[i] != 1 && sevenIsOn) {
        seven.turnOff();
      }
    }
  }

  delete[] check;
}
