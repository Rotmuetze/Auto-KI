#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "HUSKYLENS.h"
HUSKYLENS huskylens;


#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64  

#define OLED_RESET -1      
#define SCREEN_ADDRESS 0x3C 
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

//
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; 
  }


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
        if (result.xTarget <= 179 && result.xTarget >= 141) {         //geradeauss
          command = "fahren";
          Serial.println("fahren");
          fahren();
        }

        else if (result.xTarget <= 110 && result.xTarget >= 0) {      //links
          Serial.println("links");
          command = "links";
          links();

        }

        else if (result.xTarget >= 210) {                            //rechts
          Serial.println("rechts");
          command = "rechts";
          rechts();
        }
        huskylens.customText(command, 20, 20);

        display.clearDisplay();
        display.setCursor(0, 0);
        display.setTextSize(2);
        display.println("-DAS-AUTO-");
        display.println(command);
        display.display();
      }
    }
  }
}

void fahren(){        
               //fahren motorsteuerrung
 digitalWrite(in1, 0);
          digitalWrite(in2, 1);
          digitalWrite(in3, 0);
          digitalWrite(in4, 1);

          analogWrite(GSM1, 175);
          analogWrite(GSM2, 175);
          delay(1);
          
}
void rechts() {                      //rechts motorsteuerung
  digitalWrite(in1, 0);
  digitalWrite(in2, 1);

  analogWrite(GSM1, 150);

  digitalWrite(in3, 0);
  digitalWrite(in4, 0);

  analogWrite(GSM2, 150);

  delay(1);
}

void links() {                    //links motorsteuerung
  digitalWrite(in1, 0);
  digitalWrite(in2, 0);

  analogWrite(GSM1, 150);

  digitalWrite(in3, 0);
  digitalWrite(in4, 1);

  analogWrite(GSM2, 175);

  delay(1);
}
