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

void decrementProgram() {
  if (program == 0) {
    program = 127;
  } else {
    program--;
  }
}

void incrementProgram() {
  if (program == 127) {
    program = 0;
  } else {
    program++;
  }
}

void updateDebouncers() {
  nextDebouncer.update();
  previousDebouncer.update();
}

void loop() {
  bool queueProgramChange = false;

  time = millis();

  updateDebouncers();

  if (nextDebouncer.fell()) {
    timeNextPressed = time;
    queueProgramChange = true;

    incrementProgram()
  }

  if (nextDebouncer.read() == LOW) {
    durationNextHeld = time - timeNextPressed;

    if (durationNextHeld > 500) {
      digitalWrite(LED_PIN, HIGH);
      program = 0;
      queueProgramChange = true;
    }
  } else {
    durationNextHeld = 0;

    digitalWrite(LED_PIN, LOW);
  }

  if (previousDebouncer.fell()) {
    queueProgramChange = true;

    decrementProgram()
  }

  if (queueProgramChange) {
    MIDI.sendProgramChange(program, channel);
  }
}
