// Solution for: VUT FIT IMP project: TFT display demonstration using WIFI 7.12.2022
// Authors:      Jakub Kuzník, 
// Organization: BUT FIT

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <WiFi.h>
#include <WebServer.h>

void displayText(char *text, uint16_t color);
void displayFace(bool mood);
void blinkLed(uint8_t gpio, int time);

#define TFT_CS         5
#define TFT_RST        26
#define TFT_DC         25

#define DISPLAY_X_SIZE 128
#define DISPLAY_Y_SIZE 128

#define SAD 0
#define HAPPY 1

/* Put your SSID & Password */
const char* ssid = "sietocky_lover";  // Enter SSID here
const char* password = "sietocky";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);


// For 1.44" and 1.8" TFT with ST7735 use:
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


void loop() {
  // large block of text
  
  //displayText("Hello world!", ST77XX_WHITE);

  server.handleClient();
}


void setup(void) {
  Serial.begin(115200);

  // Display initialization 
  tft.initR(INITR_144GREENTAB);   // Init ST7735R chip, green tab
  tft.invertDisplay(false);

  // wifi indicator led 
  pinMode(14, OUTPUT);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  server.on("/", handleOnConnect);\
  server.on("/smile", handleSmile);
  server.on("/sad", handleSad);

  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");


  Serial.println("Initialization done");
  tft.fillScreen(ST77XX_BLACK);

  delay(1000);


}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

void handleOnConnect() {

  Serial.println("GPIO4 Status: OFF | GPIO5 Status: OFF");
  server.send(200, "text/html", generateHTML(1,2)); 
}

void handleSad() {
  displayFace(SAD);
}

void handleSmile() {
  displayFace(HAPPY);
}

/*
<!DOCTYPE html>
<html lang="en">
<head>

<meta charset="utf-8">
</head>
<body>
  <h2>IMP 2022</h2>

  <button type="button">Smile</button>
  <button type="button">Sad</button>
</body>
*/

String generateHTML(uint8_t led1stat,uint8_t led2stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<html lang=\"en\">\n";
  ptr +="<head>\n";
  ptr +="<meta charset=\"utf-8\">\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h2>IMP 2022</h2>\n";
  ptr +="<a href=\"/smile\">SMILE</a>\n";
  ptr +="<a href=\"/sad\">SAD</a>\n";
  ptr +="</body>\n";
  return ptr;
}


void displayText(char *text, uint16_t color){
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}


// mood can be SAD or HAPPY
void displayFace(bool mood) {
  // Head
  tft.fillCircle(DISPLAY_X_SIZE/2,DISPLAY_Y_SIZE/2, 40, ST7735_YELLOW);
  // left eye 
  tft.fillCircle(45, 50, 3, ST7735_BLACK);

  // right eye 65
  tft.fillCircle(80, 50, 3, ST7735_BLACK);

  // mounth
  tft.drawLine(55, 75, 75, 75, ST7735_BLACK); 

  // glasses 
  tft.drawCircle(45, 50, 10, ST7735_BLACK);
  tft.drawCircle(80, 50, 10, ST7735_BLACK);  
  tft.drawLine(55, 50, 70, 50, ST7735_BLACK);  
  tft.drawLine(35, 50, 10, 50, ST7735_BLACK);  
  tft.drawLine(90, 50, 120, 50, ST7735_BLACK); 

  if (mood == HAPPY){
    uint8_t y = 75;
    uint8_t x = 55;
    for (;x <= 75; x++){
      if (x % 2 == 0){
        if (x < 65) { y++; }
        else { y--; }
      }
      tft.drawPixel(x, y, ST7735_BLACK);
    }
  }
}

// blink led for time period 
void blinkLed(uint8_t gpio, int time){

  for (int x = 0; x < time/3; x++){
    digitalWrite(14, HIGH);
    delay(1);
    digitalWrite(14, LOW);
    delay(2);
  }
}



