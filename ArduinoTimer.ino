/*
Arduino Timer 1.0 

By Affe130

Description:
A adjusteble timer that displays the time on a LCD, blinks with a LED and beeps using a piezo/speaker
*/

#include <LiquidCrystal.h> //include the LiquidCrystal library

//Config
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // initialize the LCD with the numbers of the interface pins
const unsigned int ledPin = 13; //The pin that is connected to the LED
const unsigned int piezoPin = 10; //The pin that is connected to the piezo/speaker
const unsigned int piezoPitch1 = 1100; //The piezoPitch that the piezo/speaker uses when button 1 is pressed during setTimer
const unsigned int piezoPitch2 = 1200; //The piezoPitch that the piezo/speaker uses when button 2 is pressed during setTimer
const unsigned int piezoPitch3 = 1400; //The piezoPitch that the piezo/speaker uses when time ticks
const bool silentMode = false; //If the piezo/speaker should be silent, true or false
const unsigned int switch1Pin = 7; //The pin that is connected to the 1 switch
const unsigned int switch2Pin = 8; //The pin that is connected to the 2 switch
const unsigned int switch3Pin = 9; //The pin that is connected to the 3 switch
//Config end, dont change the variables under this comment

unsigned int hours = 0;
unsigned int minutes = 0;
unsigned int seconds = 0;

bool switchState1;
bool switchState2;
bool switchState3;

unsigned long currentMillis;
unsigned long millisSinceTimeChange = 0;
unsigned long millisSinceLed = 0;

char lcdLine1[16];
char lcdLine2[16];

bool timerActive;

unsigned int cursorPosition;

void setup() { //Code that runs once
  pinMode(ledPin, OUTPUT);
  pinMode(piezoPin, OUTPUT);
  pinMode(switch1Pin, INPUT);
  pinMode(switch2Pin, INPUT);
  pinMode(switch3Pin, INPUT);

  lcd.begin(16, 2);

  startupMessage();
  startupMelody();
  delay(3000);

  setTimer();
}

void loop() { //Code that runs repeatedly:
  if (!timerActive)
  {
    timerFinishedMelody();
    delay(1000);
    setTimer();
  }

  currentMillis = millis();
  if ((currentMillis - millisSinceTimeChange >= 1000) && (timerActive)) {
    digitalWrite(ledPin, HIGH);
    if (!silentMode) {
      tone(piezoPin, piezoPitch3, 50);
    }
    millisSinceLed = currentMillis;
    millisSinceTimeChange = currentMillis;
    updateTimerValues();
    updateLcd();
  }

  if ((currentMillis - millisSinceLed >= 500) && (timerActive)) {
    digitalWrite(ledPin, LOW);
  }
}

void setTimer()
{
  lcd.clear();
  sprintf(lcdLine1, "Timer:");
  updateTimerValues();
  updateLcd();

  lcd.cursor();
  cursorPosition = 1;
  updateCursorPosition();

  while (true)
  {
    checkSwitches();

    if ((switchState2 == HIGH) && (cursorPosition == 1)) {
      if ((seconds < 59) || (minutes < 59) || (hours < 99)) {
        seconds++;
      }

      if (seconds >= 60)
      {
        seconds = 0;
        minutes++;
      }
      
      if (minutes >= 60)
      {
        minutes = 0;
        hours++;
      }

      sprintf(lcdLine2,"%0.2d:%0.2d:%0.2d", hours, minutes, seconds);
      updateLcd();
      updateCursorPosition();
      if (!silentMode) {
        tone(piezoPin, piezoPitch2, 100);
      }
      delay(250);
    }

    if ((switchState2 == HIGH) && (cursorPosition == 2)) {
      if ((seconds < 50) || (minutes < 59) || (hours < 99)) {
        seconds = seconds + 10;
      } 

      if (seconds >= 60)
      {
        seconds = 0;
        minutes++;
      }
      
      if (minutes >= 60)
      {
        minutes = 0;
        hours++;
      }

      sprintf(lcdLine2,"%0.2d:%0.2d:%0.2d", hours, minutes, seconds);
      updateLcd();
      updateCursorPosition();
      if (!silentMode) {
        tone(piezoPin, piezoPitch2, 100);
      }
      delay(250);
    }

    if ((switchState2 == HIGH) && (cursorPosition == 3)) {
      if ((minutes < 59) || (hours < 99)) {
        minutes++;
      }

    if (minutes >= 60)
    {
      minutes = 0;
      hours++;
    }

      sprintf(lcdLine2,"%0.2d:%0.2d:%0.2d", hours, minutes, seconds);
      updateLcd();
      updateCursorPosition();
      if (!silentMode) {
        tone(piezoPin, piezoPitch2, 100);
      }
      delay(250);
    }

    if ((switchState2 == HIGH) && (cursorPosition == 4)) {
      if ((minutes <= 50) || (hours < 99)) {
        minutes = minutes + 10;
      }

      if (minutes >= 60)
      {
        minutes = 0;
        hours++;
      }

      sprintf(lcdLine2,"%0.2d:%0.2d:%0.2d", hours, minutes, seconds);
      updateLcd();
      updateCursorPosition();
      if (!silentMode) {
        tone(piezoPin, piezoPitch2, 100);
      }
      delay(250);
    }

    if ((switchState2 == HIGH) && (cursorPosition == 5)) {
      if (hours < 99) {
        hours++;
      }

      sprintf(lcdLine2,"%0.2d:%0.2d:%0.2d", hours, minutes, seconds);
      updateLcd();
      updateCursorPosition();
      if (!silentMode) {
        tone(piezoPin, piezoPitch2, 100);
      }
      delay(250);
    }

    if ((switchState2 == HIGH) && (cursorPosition == 6)) {
      if (hours < 90) {
        hours = hours + 10;
      }

      sprintf(lcdLine2,"%0.2d:%0.2d:%0.2d", hours, minutes, seconds);
      updateLcd();
      updateCursorPosition();
      if (!silentMode) {
        tone(piezoPin, piezoPitch2, 100);
      }
      delay(250);
    }

    if (switchState1 == HIGH) {
      if (cursorPosition < 6)
      {
        cursorPosition++;
      }
      else {
        cursorPosition = 1;
      }

      updateCursorPosition();
      if (!silentMode) {
        tone(piezoPin, piezoPitch1, 100);
      }
      delay(250);   
    }

    if ((switchState3 == HIGH) && (seconds > 0)) {
      lcd.noCursor();
      sprintf(lcdLine1, "Detonation in:");
      updateLcd;
      timerActive = true;
      break;    
    }
  }
}

void startupMessage() {
  sprintf(lcdLine1, "Arduino Timer");
  sprintf(lcdLine2, "by Affe_130");
  updateLcd();
}

void startupMelody() { //plays the startup melody
  if (!silentMode) {
    tone(piezoPin, 1200, 150);
    delay(300);
    tone(piezoPin, 1600, 50);
    delay(150);
    tone(piezoPin, 1800, 75);
  }
}

void timerFinishedMelody() {
  if (!silentMode) {
    for (int i = 0; i <= 10; i++) {
      tone(piezoPin, 1400, 50);
      delay(100);
    }
  }
}

void updateCursorPosition() { //Updates the cursor to the current value
  if (cursorPosition == 1) {
    lcd.setCursor(7, 1);
  }

  if (cursorPosition == 2) {
    lcd.setCursor(6, 1);
  }

  if (cursorPosition == 3) {
    lcd.setCursor(4, 1);
  }

  if (cursorPosition == 4) {
    lcd.setCursor(3, 1);
  }

  if (cursorPosition == 5) {
    lcd.setCursor(1, 1);
  }

  if (cursorPosition == 6) {
    lcd.setCursor(0, 1);
  }
}

void updateLcd() { //Updates the LCD to the current values
  lcd.setCursor(0, 0);
  lcd.print(lcdLine1);
  lcd.setCursor(0, 1);
  lcd.print(lcdLine2);
}

void updateTimerValues() { //Updates the timer values
  if ((seconds <= 0) && (minutes <= 0) && (hours > 0)) {
    hours--;
    minutes = 60;
  }

  if ((seconds <= 0) && (minutes > 0)) {
    minutes--;
    seconds = 59;
  }

  if (seconds > 0) {
    seconds--;
  }

  if ((seconds <= 0) && (minutes <= 0) && (hours <= 0)) {
    timerActive = false;
  }

  sprintf(lcdLine2,"%0.2d:%0.2d:%0.2d", hours, minutes, seconds);
}

void checkSwitches() { //Checks the switches
  switchState1 = digitalRead(switch1Pin);
  switchState2 = digitalRead(switch2Pin);
  switchState3 = digitalRead(switch3Pin);
}
