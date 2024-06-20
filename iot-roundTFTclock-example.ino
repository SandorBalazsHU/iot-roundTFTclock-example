#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_GC9A01A.h"

#define TFT_DC  7
#define TFT_CS 8

// TFT kijelző inicializálása
Adafruit_GC9A01A tft(TFT_CS, TFT_DC);

unsigned long previousMillis = 0;
const long interval = 1000;
int seconds = 0;
int minutes = 23;
int hours = 15;

void setup() {
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(GC9A01A_BLACK);
  drawClockFace(GC9A01A_WHITE);
}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    drawClock(hours, minutes, seconds, GC9A01A_BLACK, GC9A01A_BLACK);

    previousMillis = currentMillis;

    // Frissítjük az időt
    seconds++;
    if (seconds >= 60) {
      seconds = 0;
      minutes++;
      if (minutes >= 60) {
        minutes = 0;
        hours++;
        if (hours >= 24) {
          hours = 0;
        }
      }
    }

    drawClock(hours, minutes, seconds, GC9A01A_WHITE, GC9A01A_RED);
  }
}

void drawClockFace(uint16_t colorMain){
  int centerX = tft.width() / 2;
  int centerY = tft.height() / 2;
  int radius = min(centerX, centerY) - 2;

  // Draw clock face
  tft.drawCircle(centerX, centerY, radius, colorMain);

  // Draw hour marks and numbers
  for (int i = 0; i < 12; i++) {
    float angle = (i * 30 - 90); // Elforgatva 90 fokkal balra
    float radian = angle * PI / 180;
    int x1 = centerX + (radius - 10) * cos(radian);
    int y1 = centerY + (radius - 10) * sin(radian);
    int x2 = centerX + radius * cos(radian);
    int y2 = centerY + radius * sin(radian);
    tft.drawLine(x1, y1, x2, y2, colorMain);

    // Számok rajzolása
    int numX = centerX + (radius - 20) * cos(radian);
    int numY = centerY + (radius - 20) * sin(radian);
    tft.setCursor(numX - 5, numY - 5);
    tft.setTextColor(GC9A01A_WHITE);
    tft.setTextSize(1);
    int hourNum = i == 0 ? 12 : i;
    tft.print(hourNum);
  }
}

void drawClock(int hour, int minute, int second, uint16_t colorMain, uint16_t colorSecondary) {

  int centerX = tft.width() / 2;
  int centerY = tft.height() / 2;
  int radius = min(centerX, centerY) - 35;

  // Draw hour hand
  float hourAngle = ((hour % 12) + minute / 60.0) * 30 - 90;
  float hourRadian = hourAngle * PI / 180;
  int hourX = centerX + (radius - 20) * cos(hourRadian);
  int hourY = centerY + (radius - 20) * sin(hourRadian);
  tft.drawLine(centerX, centerY, hourX, hourY, colorMain);

  // Draw minute hand
  float minuteAngle = (minute + second / 60.0) * 6 - 90;
  float minuteRadian = minuteAngle * PI / 180;
  int minuteX = centerX + (radius - 5) * cos(minuteRadian);
  int minuteY = centerY + (radius - 5) * sin(minuteRadian);
  tft.drawLine(centerX, centerY, minuteX, minuteY, colorMain);

  // Draw second hand
  float secondAngle = second * 6 - 90;
  float secondRadian = secondAngle * PI / 180;
  int secondX = centerX + radius * cos(secondRadian);
  int secondY = centerY + radius * sin(secondRadian);
  tft.drawLine(centerX, centerY, secondX, secondY, colorSecondary);
}