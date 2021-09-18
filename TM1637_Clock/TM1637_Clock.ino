#include <M5Atom.h>
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK 21
#define DIO 25

bool showSeconds = true;
bool use24Hour = true;

int h = 17;
int m = 25;
int s = 34;

// ------------------------------------------

TaskHandle_t inputTask;

static uint32_t lastTime, now = 0;

bool nowShowingSeconds = false;
int countDownMax = 7;
int showSecondsCountDown = countDownMax;
bool PM = h > 11;

const uint8_t digitToSegmentAMPM[] = {
  // XGFEDCBA
  0b01110111,    // A m
  0b01110011,    // P m
  0b10000000,    // Dots
  0b00000000     // Blank
};

TM1637Display display(CLK, DIO, 30);

void setup() {
  display.setBrightness(0);
  display.clear();
  xTaskCreatePinnedToCore(
    inputTaskFunction, /* Function to implement the task */
    "InputTask", /* Name of the task */
    10000,  /* Stack size in words */
    NULL,  /* Task input parameter */
    0,  /* Priority of the task */
    &inputTask,  /* Task handle. */
    0); /* Core where the task should run */
  showSecondsCountDown = countDownMax;
  now = millis();
}

void inputTaskFunction( void * parameter) {
  for (;;) {
    M5.update();
    if (M5.Btn.wasPressed()) use24Hour = !use24Hour;
    vTaskDelay(10);// Feed the watchdog timer.
  }
}

void timeTick() {
  s++;
  if (s == 60) {
    s = 0;
    m = m + 1;
  }
  if (m == 60) {
    m = 0;
    h = h + 1;
  }
  if (h == 24) {
    h = 0;
  }
  PM = (h > 11);
}

void showHoursMins() {
  uint8_t offsetHours = (h > 12 && !use24Hour) ? h - 12 : h;
  display.showNumberDecEx((offsetHours * 100 + m), 64);
  while ((now - lastTime) < 500) {
    now = millis();
  }
  lastTime = now;
  display.showNumberDecEx((offsetHours * 100) + m, 0);
  while ((now - lastTime) < 500) {
    now = millis();
  }
  lastTime = now;
}

void showAMPMSeconds() {
  if (!use24Hour)
    display.setSegments(digitToSegmentAMPM + (PM ? 1 : 0) , 1, 0); // AM/PM
  else
    display.setSegments(digitToSegmentAMPM + 3 , 1, 0); // Blank
  display.showNumberDecEx(s, 0, true, 2, 2); // Seconds
  display.setSegments(digitToSegmentAMPM + 2, 1, 1); // Dots (On char 2)
  while ((now - lastTime) < 500) {
    now = millis();
  }
  lastTime = now;
  if (!use24Hour)
    display.setSegments(digitToSegmentAMPM + (PM ? 1 : 0) , 1, 0); // AM/PM
  else
    display.setSegments(digitToSegmentAMPM + 3 , 1, 0); // Blank
  display.showNumberDecEx(s, 0, true, 2, 2); // Seconds
  display.setSegments(digitToSegmentAMPM + 3, 1, 1); // Dots off (On char 2)
  while ((now - lastTime) < 500) {
    now = millis();
  }
  lastTime = now;
}

void loop() {
  timeTick();
  if (showSeconds) {
    if (--showSecondsCountDown <= 0) {
      showSecondsCountDown = countDownMax;
      nowShowingSeconds = !nowShowingSeconds;
      display.clear();
    }
  }
  if (nowShowingSeconds) {
    showAMPMSeconds();
  } else {
    showHoursMins();
  }
}
