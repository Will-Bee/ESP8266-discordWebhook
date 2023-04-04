#include <Arduino.h>
#include <string.h>

#include <LiquidCrystal_I2C.h>

#include <WiFimanager.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>



// JSON:
// {
// "content": "@everyone",
// "username": "OFD-Device",
// "embeds": [
//     {
//         "title": "Status: {Status_kabelu}",
//         "description": "OPTICAL FIBER DETECTOR"
//     }
// ]
// }



WiFiManager wifiManager;
WiFiClientSecure client;
HTTPClient http;
String JSONDetected;
String JSONEmpty;
String JSON;
StaticJsonDocument<220> messageDetected;
StaticJsonDocument<220> messageEmpty;
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define SENSOR_PIN D8

int notifState;
int lastTime = 2;
int httpCode;
bool state = false;
String webhookLink = "";
String payload;
String yourApi = "https://www.bartosek.cz/shared/OFD/api/?secret=heslo";


void messageBuild(){

  messageDetected["content"] = "@everyone";
  messageDetected["username"] = "OFD-Device";
  messageDetected["embeds"][0]["description"] = "OPTICAL FIBER DETECTOR";

  messageEmpty["content"] = "@everyone";
  messageEmpty["username"] = "OFD-Device";
  messageEmpty["embeds"][0]["description"] = "OPTICAL FIBER DETECTOR";

  messageDetected["embeds"][0]["title"] = "Status: DETECTED";
  messageDetected["embeds"][0]["color"] = "5763719";

  messageEmpty["embeds"][0]["title"] = "Status: EMPTY";
  messageEmpty["embeds"][0]["color"] = "15548997";

  serializeJson(messageDetected, JSONDetected);
  serializeJson(messageEmpty, JSONEmpty);

}




int notification(bool status){
  // Send POST request to Discord webhook

  http.addHeader("Content-Type", "application/json");

  if (status) {
      httpCode = http.POST(JSONDetected);
  } else {
      httpCode = http.POST(JSONEmpty);
  }

  // Check if device is connected to internet
  if (WiFi.status() != WL_CONNECTED) {
    lcd.setCursor(0,1);
    lcd.print("No internet");
    // end program if not connected
    return 0;
  }

  return 1;
}

bool getWebhookLink() {
  // Get webhook link from user
  // url of api: https://www.bartosek.cz/shared/OFD/api/?secret=heslo
  // Json will be like this:
  // {
  //   "webhook": "url of webhook"
  // }



  // Get webhook link from user
  Serial.println("Getting webhook link...");
  Serial.print("Getting webhook link...");



  // Get webhook link from user
  client.connect("www.bartosek.cz", 443);
  http.begin(client, yourApi);
  httpCode = http.GET();



  // Print HTTP response
  if (httpCode > 0) {
    String payload = http.getString();
    client.stop();

    // All "/" are \/ in response, so we need to replace them with /
    payload.replace("\\/", "/");

    // Example of payload:
    // {"URL":"https://discord.com/api/webhooks/123456789/123456789","text":"test"}

    // Parse JSON without using ArduinoJson
    // Get webhook link
    int start = payload.indexOf("https://discord.com/api/webhooks/");
    int end = payload.indexOf("\",\"text\"");
    webhookLink = payload.substring(start, end);

    // Cut this part from webhook: ","text":"...
    webhookLink = webhookLink.substring(0, webhookLink.indexOf("\",\"text\""));

    Serial.println("Done");
    Serial.print("Webhook link: ");
    Serial.println(webhookLink);

    return 1;






  } else {
    Serial.println("Error on HTTP request");
    return 0;
  }
  return 0;
}

void setup() {

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Starting...");
  lcd.setCursor(0,1);
  lcd.print("github/Will-Bee");

  // autoconnect to wifi
  wifiManager.autoConnect("AutoConnectAP");

  Serial.begin(9600);
  Serial.println("Connected to WiFi");

  client.setInsecure();

  getWebhookLink();

  client.connect(webhookLink, 443);
  http.begin(client, webhookLink);

  messageBuild();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Status: ");

}



void loop() {

  // From now

  state = digitalRead(SENSOR_PIN);

  if (state != lastTime) {
    lastTime = state;

    lcd.setCursor(0,1);
    lcd.print("                ");

    lcd.setCursor(0,1);
    lcd.print(state);

    notifState = notification(state);

    if (notifState == 0) {
      return;
    }

    lcd.setCursor(2,1);
    lcd.print("Sent:");
    lcd.setCursor(8,1);

    if (httpCode < 0)
    {
      httpCode = 204;
    }
    lcd.print(httpCode);

  }

}
