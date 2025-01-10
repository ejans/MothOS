//IMPORTANT: COMMENT OUT ISOLED BELOW IF NOT USING ACTUAL OLED
#define ISOLED
#define LED 48

#include <Arduino.h>

//MothSynth led manager init
#include "LedManager.h"
//LedManager ledManager = LedManager(LED, LED, LED, LED);
LedManager ledManager = LedManager(49, 49, 49, 49);


//MothSynth input manager init
#include "InputManager.h"
InputManager inputManager = InputManager();

//tracker init
#include "Tracker.h"
Tracker tracker = Tracker();

//keypad setup
#include <Keypad.h>
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  { 'M', 'N', 'O', 'P' },
  { 'I', 'J', 'K', 'L' },
  { 'E', 'F', 'G', 'H' },
  { 'A', 'B', 'C', 'D' }
};

byte rowPins[ROWS] = { 11, 10, 9, 8 };
byte colPins[COLS] = { 6, 7, 13, 12 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

//i2s sound
#include <ESP_I2S.h>
I2SClass i2s;
const int sampleRate = 44100;  // sample rate in Hz

//OLED init
#include "ScreenManager.h"
#include <U8g2lib.h>
#include <Wire.h>


//U8G2_SSD1306_128X64_NONAME_1_HW_I2C screen = U8G2_SSD1306_128X64_NONAME_1_HW_I2C(U8G2_R0, /* reset=*/U8X8_PIN_NONE, /* clock=*/5, /* data=*/4);
U8G2_SSD1306_128X64_NONAME_1_HW_I2C screen = U8G2_SSD1306_128X64_NONAME_1_HW_I2C(U8G2_R2, /* reset=*/U8X8_PIN_NONE, /* clock=*/5, /* data=*/4);
ScreenManager screenManager = ScreenManager();

//OLED Helers
TaskHandle_t Task2;  //OLED requires second core
char ledCommandOLED;
String noteChars[12];
int volumeBars[4];

void setup() {

  //Disable red led
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  noteChars[0] = String("C_");
  noteChars[1] = String("C#");
  noteChars[2] = String("D_");
  noteChars[3] = String("D#");
  noteChars[4] = String("E_");
  noteChars[5] = String("F_");
  noteChars[6] = String("F#");
  noteChars[7] = String("G_");
  noteChars[8] = String("G#");
  noteChars[9] = String("A_");
  noteChars[10] = String("A#");
  noteChars[11] = String("B");

  #ifdef ISOLED
  xTaskCreatePinnedToCore(
    Task2Loop,
    "Core 0 task",
    10000,
    NULL,
    1,
    &Task2,
    0);
  #endif
  keypad.setDebounceTime(0);

  //i2s.setPins(9, 10, 11);
  //i2s.setPins(1, 2, 3); // BCLK, LCK, DOUT
  i2s.setPins(3, 1, 2); // BCK, LCK, DIN 
  if (!i2s.begin(I2S_MODE_STD, sampleRate, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO)) {
    while (1) {};
  }

  screen.begin();
}

void loop() {

  inputManager.UpdateInput(keypad.getKey());
  char note = inputManager.note;
  char trackCommand = inputManager.trackCommand;
  int trackCommandArgument = inputManager.trackCommandArgument;
  char ledCommand = inputManager.ledCommand;

  if (ledCommand != ' ') {
    ledCommandOLED = ledCommand;
    ledManager.SetCommand(ledCommand);
  }

  if (trackCommand != ' ') {
    tracker.SetCommand(trackCommand, trackCommandArgument);
  }

  int sample = tracker.UpdateTracker();

  // New i2s only wants to write bytes out, so we need to split the sample before writing
  // Copy the high and low bytes of our 16bit sample into a buffer and write that
  byte outbuf[2];
  //outbuf[0] = lowByte(tracker.sample);
  //outbuf[1] = highByte(tracker.sample);
  outbuf[0] = lowByte(tracker.sample/2);
  outbuf[1] = highByte(tracker.sample/2);
  i2s.write(outbuf, 2);

  int tempoBlink = tracker.tempoBlink;
  if (tempoBlink > 0)
    ledManager.SetLit(tempoBlink, tracker.selectedTrack);

  ledManager.SetPattern(tracker.allPatternPlay, tracker.currentPattern);
  ledManager.UpdateLed();
  inputManager.EndFrame();
}

void Task2Loop(void *parameter) {
  for (;;) {
    screen.firstPage();
    do {
      screenManager.Update(tracker, screen, ledCommandOLED, volumeBars, noteChars);
    } while (screen.nextPage());
  }
}
