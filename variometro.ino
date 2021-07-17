#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>

#include <SPI.h>
#include <Wire.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)
#define OLED_RESET 4
#define som 8

Adafruit_SSD1306 display(OLED_RESET);
Adafruit_BMP280 bmp; // I2C

int vario = 0;
double delay1 = 0.0;
float altitude = 0.0;
float altAnterior = 0.0;
float altPadrao = 0.0;

//------------------------------------------------------------------------------

void setup()   {                
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)

  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  altPadrao = bmp.readAltitude(1013.25);
}

//------------------------------------------------------------------------------

void loop() {
  if((millis() - delay1) > 1000){
    display.clearDisplay();
    
    printTemperatura();
    printPressao();
    printAltitude();
    printVari();
    
    display.display();
    delay1 = millis();
    Serial.println(delay1);
  }

  delay(100);
}

//------------------------------------------------------------------------------

void printTemperatura(){
  printText("T:", 3, 0, 1, false);
  const float temp = bmp.readTemperature();
  printFloat(temp, 15, 0, 1, false);
}

//------------------------------------------------------------------------------

void printAltitude(){
  printText("Alt(m): ", 4, 45, 1, false);
  const float alt = bmp.readAltitude(1013.25);
  Serial.println(">>");
  Serial.println(alt);
  Serial.println(altPadrao);
  altitude = alt - altPadrao;
  Serial.println(altitude);
  printFloat(altitude, 65, 45, 2, false);
}

//------------------------------------------------------------------------------

float variometro(){
  const float var = altitude - altAnterior;
  makeTone(var);
  //makeTone(10);
  altAnterior = altitude;
 
  return var;
}

//------------------------------------------------------------------------------

void printText(String text, int x, int y, int size, boolean atualizar) {
  display.setTextSize(size);
  display.setTextColor(WHITE);
  display.setCursor(x,y);
  display.println(text);
  
  if(atualizar){
    display.display();
  }
}

//------------------------------------------------------------------------------

void printFloat(float num, int x, int y,int size, boolean atualizar) {
  display.setTextSize(size);
  display.setTextColor(WHITE);
  display.setCursor(x,y);
  display.println(num);
  
  if(atualizar){
    display.display();
  }
}

//------------------------------------------------------------------------------

void printLong(long num, int x, int y,int size, boolean atualizar) {
  display.setTextSize(size);
  display.setTextColor(WHITE);
  display.setCursor(x,y);
  display.println(num);
  
  if(atualizar){
    display.display();
  }
}

//------------------------------------------------------------------------------

void printVari(){
  printText("Var: ", 3, 22, 1, false);
  const float var = variometro();
  printFloat(var, 35, 18, 3, false);
}

//------------------------------------------------------------------------------

void printPressao(){
  printText("P:", 60, 0, 1, false);
  const float temp = bmp.readPressure();
  printFloat(temp, 70, 0, 1, false);
}

//------------------------------------------------------------------------------

void makeTone(const int var){
  if(var != 0){
    if(var > 0){
      tone(som, 3000, 150);
      delay(150);
      if(var >= 3){
        tone(som, 3000, 150);
        delay(150);
      }
      if(var >= 6){
        tone(som, 3000, 150);
        delay(150);
      }
      if(var >= 9){
        tone(som, 3000, 200);
        delay(150);
      }
    }
    else{
      tone(som, 320, 200);
      delay(200);
      tone(som, 350, 200);
    }
  }
}
