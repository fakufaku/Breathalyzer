
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

#define BM_P 0b01110011
#define BM_U 0b00111110
#define BM_S 0b01101101
#define BM_H 0b01110110

#define STATE_PUSHED 0
#define STATE_DISPLAY 1
#define STATE_IDLE 2
#define STATE_SEND 3

int state = STATE_IDLE;

const int breathPin = 0;    // the pin that the potentiometer is attached to
const int button = 2;

Adafruit_7segment matrix = Adafruit_7segment();

void buttonPushed()
{
  if (state == STATE_IDLE)
    state = STATE_PUSHED;
  else if (state == STATE_DISPLAY)
    state = STATE_SEND;
}

void setup() {
  Serial.begin(9600);
  matrix.begin(0x70);
  
  pinMode(button, INPUT);
  digitalWrite(button, HIGH);   // configure pull-up resistor
  attachInterrupt(0, buttonPushed, FALLING);
  
  printPush();
}

void loop() {

  if (state == STATE_PUSHED)
  {
    // start doing things
    int breath = readBreath();
    Serial.println(breath);
    
    //breath -= 20;
    //if (breath < 0)
      //breath = 0;
      
    matrix.print(breath, DEC);
    matrix.writeDisplay();
    
    state = STATE_DISPLAY;
  }
  
  if (state == STATE_SEND)
  {
    // push over to twitter 
    
    // next state is idle
    printPush();
    delay(1000);
    state = STATE_IDLE;
  }

}


void printPush()
{
  matrix.writeDigitRaw(0, BM_P);
  matrix.writeDigitRaw(1, BM_U);
  matrix.writeDigitRaw(3, BM_S);
  matrix.writeDigitRaw(4, BM_H);
  matrix.writeDisplay();
}

int readBreath()
{
  int oter = 1000;
  int iter = 10;
  unsigned long sensorReading = 0;
  for (int i = 0 ; i < oter ; i++)
  {
    unsigned long s = 0;
    for (int j = 0 ; j < iter ; j++)
      s += analogRead(breathPin);
    s = 1023 - s/iter;
    if (s > sensorReading)
      sensorReading = s;
    rotationPattern();
  }
  return sensorReading;
}

void rotationPattern()
{
  static int cur = 0;
  
  if (cur == 0)
  {
    matrix.writeDigitRaw(0, 0b00001000);
    matrix.writeDigitRaw(1, 0);
    matrix.writeDigitRaw(3, 0);
    matrix.writeDigitRaw(4, 0);
  }
  else if (cur == 1)
  {
    matrix.writeDigitRaw(1, 0b00001000);
    matrix.writeDigitRaw(0, 0);
    matrix.writeDigitRaw(3, 0);
    matrix.writeDigitRaw(4, 0);
  }
  else if (cur == 2)
  {
    matrix.writeDigitRaw(3, 0b00001000);
    matrix.writeDigitRaw(1, 0);
    matrix.writeDigitRaw(0, 0);
    matrix.writeDigitRaw(4, 0);
  }
  else if (cur == 3)
  {
    matrix.writeDigitRaw(4, 0b00001000);
    matrix.writeDigitRaw(1, 0);
    matrix.writeDigitRaw(3, 0);
    matrix.writeDigitRaw(0, 0);
  }
  else if (cur == 4)
  {
    matrix.writeDigitRaw(4, 0b00000100);
    matrix.writeDigitRaw(1, 0);
    matrix.writeDigitRaw(3, 0);
    matrix.writeDigitRaw(0, 0);
  }
  else if (cur == 5)
  {
    matrix.writeDigitRaw(4, 0b00000010);
    matrix.writeDigitRaw(1, 0);
    matrix.writeDigitRaw(3, 0);
    matrix.writeDigitRaw(0, 0);
  }
  else if (cur == 6)
  {
    matrix.writeDigitRaw(4, 0b00000001);
    matrix.writeDigitRaw(1, 0);
    matrix.writeDigitRaw(3, 0);
    matrix.writeDigitRaw(0, 0);
  }
  else if (cur == 7)
  {
    matrix.writeDigitRaw(3, 0b00000001);
    matrix.writeDigitRaw(1, 0);
    matrix.writeDigitRaw(0, 0);
    matrix.writeDigitRaw(4, 0);
  }
  else if (cur == 8)
  {
    matrix.writeDigitRaw(1, 0b00000001);
    matrix.writeDigitRaw(0, 0);
    matrix.writeDigitRaw(3, 0);
    matrix.writeDigitRaw(4, 0);
  }
  else if (cur == 9)
  {
    matrix.writeDigitRaw(0, 0b00000001);
    matrix.writeDigitRaw(1, 0);
    matrix.writeDigitRaw(3, 0);
    matrix.writeDigitRaw(4, 0);
  }
  else if (cur == 10)
  {
    matrix.writeDigitRaw(0, 0b00100000);
    matrix.writeDigitRaw(1, 0);
    matrix.writeDigitRaw(3, 0);
    matrix.writeDigitRaw(4, 0);
  }
  else if (cur == 11)
  {
    matrix.writeDigitRaw(0, 0b00010000);
    matrix.writeDigitRaw(1, 0);
    matrix.writeDigitRaw(3, 0);
    matrix.writeDigitRaw(4, 0);
  }
  matrix.writeDisplay();
  cur = (cur+1)%12;
}
