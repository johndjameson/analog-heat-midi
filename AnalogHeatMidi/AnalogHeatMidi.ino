#include <Bounce2.h>
#include <MIDI.h>

#define NEXT_PIN 8
#define PREVIOUS_PIN 7

int channel = 1;
int program = 0;

Bounce nextDebouncer = Bounce();
Bounce previousDebouncer = Bounce();

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
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

  updateDebouncers();

  if (nextDebouncer.fell()) {
    queueProgramChange = true;

    incrementProgram();
  }

  if (previousDebouncer.fell()) {
    queueProgramChange = true;

    decrementProgram();
  }

  if (queueProgramChange) {
    MIDI.sendProgramChange(program, channel);
  }
}
