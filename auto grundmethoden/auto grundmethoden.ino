#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "HUSKYLENS.h"
HUSKYLENS huskylens;


#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Gleichstrommotor links

int GSM1 = D8;
int in1 = D3;
int in2 = D4;

// Gleichstrommotor rechts

int in3 = D7;
int in4 = D6;
int GSM2 = D5;

String command = "";
boolean drive = false;
int oledzähler = 0;
int tagzähler = 0;
void setup() {

  Serial.begin(115200);



  Wire.begin();

  while (!huskylens.begin(Wire))  //Gibt es eine hergestellte I2C Verbindung?
  {
    huskylens.begin(Wire);
    Serial.println("IC2 Verbindung fehlgeschlagen...");
  }
  Serial.println("");
  Serial.println("I2C Verbindung erfolgreich hergestellt.");
  Serial.println("");
  huskylens.writeAlgorithm(ALGORITHM_LINE_TRACKING);
  delay(100);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000);  // Pause for 1 second

  // Clear the buffer
  display.clearDisplay();
  display.display();

  display.setCursor(0, 0);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.clearDisplay();
  display.display();


  pinMode(GSM1, OUTPUT);
  pinMode(GSM2, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop() {

  if (huskylens.request()) {      //Besteht eine I2C Verbindung mit der Huskylens?
    if (huskylens.isLearned()) {  //Hat die Huskylens irgendwas gelernt / sich schon was gemerkt?

        while (huskylens.available() > 0) {  //Solange die Huskylens mehr als 0 Objekte vor sich erkennt...
          HUSKYLENSResult result = huskylens.read();
          if (result.xTarget <= 179 && result.xTarget >= 141) {
          digitalWrite(in1, 0);
          digitalWrite(in2, 1);
          digitalWrite(in3, 0);
          digitalWrite(in4, 1);

          analogWrite(GSM1, 100);
          analogWrite(GSM2, 100);          
          delay(10);
          command = "fahren";
          Serial.println("fahren");
          }
       
          else if (result.xTarget <=110 && result.xTarget >= 0) {
            Serial.println("links");
            command = "links";
            links();
          
          }

         else if (result.xTarget >= 210) {
           Serial.println("rechts");
            command = "rechts";
          rechts();
            
          }
          huskylens.customText(command, 20, 20);

        display.clearDisplay();
        display.setCursor(0,0);
        display.setTextSize(2);
        display.println("---AUTO---");
        display.println(command);
        display.display();
               
       
       
        }
      }
      //tag
      tagzähler++;
      if (tagzähler == 10) {
      huskylens.writeAlgorithm(ALGORITHM_TAG_RECOGNITION);
      if (huskylens.isLearned()) {
      while (huskylens.available() > 0) {
      HUSKYLENSResult resulttag = huskylens.read();
      motoroff(100);
      if (resulttag.ID == 1) {
      tank(100);

      }
      if (resulttag.ID == 2) {
      shake(100);
      }
      }
      }
      }
      huskylens.writeAlgorithm(ALGORITHM_LINE_TRACKING);
      motoroff(2000);
    }

    }
  

void motoroff(int time){
  
  digitalWrite(in1, 0);
  digitalWrite(in2, 0);
  analogWrite(GSM1, 0);

  digitalWrite(in3, 0);
  digitalWrite(in4, 0);
  analogWrite(GSM2, 0);
  delay(time);

}

void rechts(){
  digitalWrite(in1, 0);
  digitalWrite(in2, 1);
  
  analogWrite(GSM1, 125);

  digitalWrite(in3, 0);
  digitalWrite(in4, 0);

  analogWrite(GSM2, 125);
  
  delay(5);
}

void links(){
  digitalWrite(in1, 0);
  digitalWrite(in2, 0);
  
  analogWrite(GSM1, 125);

  digitalWrite(in3, 0);
  digitalWrite(in4, 1);

  analogWrite(GSM2, 125);
  
  delay(5);
}

void tank(int speed){
  digitalWrite(in1, 1);
  digitalWrite(in2, 0);
  digitalWrite(in3, 0);
  digitalWrite(in4, 1);
  analogWrite(GSM1, 200);
  analogWrite(GSM2, 200);
  delay(2000);

  digitalWrite(in1, 0);
  digitalWrite(in2, 1);
  digitalWrite(in3, 1);
  digitalWrite(in4, 0);

  delay(2000);


  digitalWrite(in1, 1);
  digitalWrite(in2, 0);
  digitalWrite(in3, 1);
  digitalWrite(in4, 0);
  analogWrite(GSM1, speed);
  analogWrite(GSM2, speed);  
}

void shake(int speed){
   digitalWrite(in1, 1);
  digitalWrite(in2, 0);
  digitalWrite(in3, 0);
  digitalWrite(in4, 1);
  analogWrite(GSM1, 200);
  analogWrite(GSM2, 200);
  delay(50);

  digitalWrite(in1, 0);
  digitalWrite(in2, 1);
  digitalWrite(in3, 1);
  digitalWrite(in4, 0);

  delay(50);

  digitalWrite(in1, 1);
  digitalWrite(in2, 0);
  digitalWrite(in3, 1);
  digitalWrite(in4, 0);
  analogWrite(GSM1, speed);
  analogWrite(GSM2, speed);  
}

  