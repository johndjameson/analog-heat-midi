#include <Bounce2.h>
#include <MIDI.h>

#define LED_PIN 13
#define NEXT_PIN 8
#define PREVIOUS_PIN 7

int channel = 1;
int program = 0;
unsigned long durationNextHeld;
unsigned long durationPreviousHeld;
unsigned long time;
unsigned long timeNextPressed;
unsigned long timePreviousPressed;

Bounce nextDebouncer = Bounce();
Bounce previousDebouncer = Bounce();

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(NEXT_PIN, INPUT_PULLUP);
  pinMode(PREVIOUS_PIN, INPUT_PULLUP);

  nextDebouncer.attach(NEXT_PIN);
  nextDebouncer.interval(5);

  previousDebouncer.attach(PREVIOUS_PIN);
  previousDebouncer.interval(5);

  MIDI.begin(MIDI_CHANNEL_OFF);
}

void loop() {
  bool queueProgramChange = false;

  time = millis();

  nextDebouncer.update();
  previousDebouncer.update();

  if (nextDebouncer.fell()) {
    if (program == 127) {
      program = 0;
    } else {
      program++;
    }

    timeNextPressed = time;
    queueProgramChange = true;
  }

  if (nextDebouncer.rose()) {
    durationNextHeld = time - timeNextPressed;

    if (durationNextHeld > 500) {
      durationNextHeld = 0;
      program = 0;
      queueProgramChange = true;
    }
  }

  if (previousDebouncer.fell()) {
    if (program == 0) {
      program = 127;
    } else {
      program--;
    }

    queueProgramChange = true;
  }

  if (queueProgramChange) {
    MIDI.sendProgramChange(program, channel);
  }
}
