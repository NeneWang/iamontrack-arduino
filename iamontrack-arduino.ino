#include <ArduinoJson.h>




#include  <Wire.h>;
#include <LiquidCrystal_I2C.h>
//#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Construct an LCD object and pass it the
// I2C address, width (in characters) and
// height (in characters). Depending on the
// Actual device, the IC2 address may change.
const char* ssid = "FiOS-OMJJ7";
const char* password = "1010101010";
LiquidCrystal_I2C lcd(0x27, 16, 2);
StaticJsonDocument<128> doc;



void setup() {

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(1000);
    Serial.println("Connecting..");

  }


  // The begin call takes the width and height. This
  // Should match the number provided to the constructor.
  //  lcd.begin(16,2);
  lcd.init();

  // Turn on the backlight.
  lcd.backlight();

  // Move the cursor characters to the right and
  // zero characters down (line 1).
  lcd.setCursor(0, 0);
  

  String getPostObjectives = getPayloadFrom("http://iamontrack.wangnelson.xyz/public/api/get-objectives-names");

  DeserializationError error = deserializeJson(doc, getPostObjectives);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }


}

void loop() {
  String daysLeftString = "Days Left: " + getPayloadFrom("http://iamontrack.wangnelson.xyz/public/api/get-days-left");
  String streakMessage = "Week Streak: " + getPayloadFrom("http://iamontrack.wangnelson.xyz/public/api/get-current-streak");

  lcd.clear();
  lcd.print(daysLeftString);
  lcd.setCursor(0, 1);
  lcd.print(streakMessage);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Objectives: ");

  for (int i = 0; i < 5; i++) {
    lcd.setCursor(0, 1);
    const char* words = doc[i];
    lcd.print(words);
    Serial.println(words);
    delay(1000);
    lcd.clear();
  }

}

String getPayloadFrom(String url) {
  String payload = "";
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;


    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) { //Check the returning code

      payload = http.getString();   //Get the request response payload
      //      Serial.println("This is the payload");
      Serial.print(payload);

      //      getFirstCardIndex(doc);


    }

    http.end();   //Close connection

  }
  return payload;
}
