// https://github.com/weltamdraht/Berlin_Clock
  
#include <Wire.h>
#include <I2C_RTC.h>
#include <ShiftRegister74HC595.h>

ShiftRegister74HC595<3> sr(5, 3, 9);
static DS1307 RTC;

int setHrsPin = 11;
int setMinPin = 12;
bool hrsPress;
bool minPress;
int setHours[24] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };
int setMinutes[60] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59 };
int incH;
int incM;


int sekunde;
bool sekblink = true;
int stunde = 0;
int stunde5 = 0;
int stundemod = 0;
int minute = 0;
int minute5 = 0;
int minutemod = 0;
int rowstd5[5] = { 0, 1, 2, 3, 4 };
int rowstdmod[5] = { 0, 5, 6, 7, 8 };
int rowmin5[12] = { 0, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
int rowminmod[5] = { 0, 20, 21, 22, 23 };


void setup() {

  pinMode(setHrsPin, INPUT_PULLUP);
  pinMode(setMinPin, INPUT_PULLUP);

  RTC.begin();
  if (RTC.getHourMode() == CLOCK_H12) {
    RTC.setHourMode(CLOCK_H24);
  }
  sr.setAllHigh();
  delay(2000);
  sr.setAllLow();
}


void loop() {
  sekunde = RTC.getSeconds();
  stunde = RTC.getHours();
  minute = RTC.getMinutes();

  stunde5 = (stunde / 5);
  stundemod = (stunde % 5);
  minute5 = (minute / 5);
  minutemod = (minute % 5);

  setlamps(0, 0, 0, sekblink);
  setlamps(rowstd5[1], rowstd5[4], rowstd5[stunde5], stunde5);
  setlamps(rowstdmod[1], rowstdmod[4], rowstdmod[stundemod], stundemod);
  setlamps(rowmin5[1], rowmin5[11], rowmin5[minute5], minute5);
  setlamps(rowminmod[1], rowminmod[4], rowminmod[minutemod], minutemod);
  sr.updateRegisters();

  delay(1000);

  if (sekblink) {
    sekblink = false;
  } else {
    sekblink = true;
  }

  hrsPress = digitalRead(setHrsPin);
  minPress = digitalRead(setMinPin);

  if (hrsPress == LOW && minPress == LOW) {
    RTC.setSeconds(0);
    RTC.startClock();
    sr.setAllHigh();
    delay(800);
    sr.setAllLow();
    delay(400);
  }

  if (hrsPress == LOW && minPress == HIGH) {
    for (int i = 0; i < 24; i++) {
      hrsPress = digitalRead(setHrsPin);
      if (hrsPress == HIGH) {
        RTC.startClock();
        break;
      }
      RTC.setHours(setHours[i]);
      delay(100);
      stunde = RTC.getHours();
      minute = RTC.getMinutes();

      stunde5 = (stunde / 5);
      stundemod = (stunde % 5);
      minute5 = (minute / 5);
      minutemod = (minute % 5);

      setlamps(0, 0, 0, sekblink);
      setlamps(rowstd5[1], rowstd5[4], rowstd5[stunde5], stunde5);
      setlamps(rowstdmod[1], rowstdmod[4], rowstdmod[stundemod], stundemod);
      setlamps(rowmin5[1], rowmin5[11], rowmin5[minute5], minute5);
      setlamps(rowminmod[1], rowminmod[4], rowminmod[minutemod], minutemod);
      sr.updateRegisters();
      delay(500);
    }
  }
  if (minPress == LOW && hrsPress == HIGH) {
    for (int i = 0; i < 60; i++) {
      hrsPress = digitalRead(setMinPin);
      if (hrsPress == HIGH) {
        RTC.startClock();
        break;
      }
      RTC.setMinutes(setMinutes[i]);
      delay(100);
      stunde = RTC.getHours();
      minute = RTC.getMinutes();

      stunde5 = (stunde / 5);
      stundemod = (stunde % 5);
      minute5 = (minute / 5);
      minutemod = (minute % 5);

      setlamps(0, 0, 0, sekblink);
      setlamps(rowstd5[1], rowstd5[4], rowstd5[stunde5], stunde5);
      setlamps(rowstdmod[1], rowstdmod[4], rowstdmod[stundemod], stundemod);
      setlamps(rowmin5[1], rowmin5[11], rowmin5[minute5], minute5);
      setlamps(rowminmod[1], rowminmod[4], rowminmod[minutemod], minutemod);
      sr.updateRegisters();
      delay(500);
    }
  }
}



//function for lamp switching
void setlamps(int vonlampe, int bislampe, int bislampean, int alleaus) {
  for (int i = vonlampe; i <= bislampe; i++) {
    sr.setNoUpdate(i, LOW);
  }
  if (alleaus) {
    for (int i = vonlampe; i <= bislampean; i++) {
      sr.setNoUpdate(i, HIGH);
    }
  }
}
