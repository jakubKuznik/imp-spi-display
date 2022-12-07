// Solution for: VUT FIT IMP project: TFT display demonstration using WIFI 7.12.2022
// Authors:      Jakub Kuzník, 
// Organization: BUT FIT

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define WIFI_SSID "sietocky-lover"
#define WIFI_PASS "sietocky"

// Create a new web server
ESP8266WebServer webserver(80);

#define TFT_CS         5
#define TFT_RST        26
#define TFT_DC         25

#define DISPLAY_X_SIZE 128
#define DISPLAY_Y_SIZE 128

#define SAD 0
#define HAPPY 1


// For 1.44" and 1.8" TFT with ST7735 use:
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


float p = 3.1415926;

void loop() {
  // large block of text
  tft.fillScreen(ST77XX_BLACK);
  
  displayText("Hello world!", ST77XX_WHITE);
  displayFace(HAPPY);
  blinkLed(14, 500);
  displayFace(SAD);
  delay(500);  

  webserver.handleClient(); 


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

void setup(void) {
  Serial.begin(115200);

  // Display initialization 
  tft.initR(INITR_144GREENTAB);   // Init ST7735R chip, green tab
  tft.invertDisplay(false);

  // wifi indicator led 
  pinMode(14, OUTPUT);

  //Begin WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(100); }

  // WiFi Connected
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Start Web Server
  webserver.on("/", rootPage);
  webserver.onNotFound(notfoundPage);
  webserver.begin();

  Serial.println("Initialization done");
  delay(1000);
}

void displayText(char *text, uint16_t color){
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

// Handle Root
void rootPage() { 
  webserver.send(200, "text/plain", "Check out https://imp.com !"); 
}

// Handle 404
void notfoundPage(){ 
  webserver.send(404, "text/plain", "404: Not found"); 
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



