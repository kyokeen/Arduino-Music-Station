#include <Tone.h>
//#include "pitches.h"

int* octave;
int* blackKeys;
int octave1[] = {NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5};
int octave2[] = {NOTE_C6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_G6, NOTE_A6, NOTE_B6}; 
int blackKeys1[] = {NOTE_CS5, NOTE_DS5, NOTE_FS5, NOTE_GS5, NOTE_AS5};
int blackKeys2[] = {NOTE_CS6, NOTE_DS6, NOTE_FS6, NOTE_GS6, NOTE_AS6};

bool once = true;
int key1, key2, key3, key4, key5, key6, key7, key8, track1, track2, track3, track4, octaveButton, recordButton, pauseButton;
int black1, black2, black3, black4, black5;
int trackPins[] = {34, 33, 38, 39};
int buttonPins[] = {22, 23, 24, 25, 26, 27, 28};
int blackKeyPins[] = {44, 45, 46, 47, 29};
int buzzerPins[] = {40, 41, 42, 43};
int ledPins[] = {50, 51, 52, 53};
int octavePin = 30;
int recordPin = 3;
int pausePin = 7;


Tone tones[4];
Tone currentTone;
int currentToneIndex = 0;

struct note {
  unsigned short int freq;
  unsigned long duration;
};

note firstTrackNotes[200];
note secondTrackNotes[200];
note thirdTrackNotes[200];
note fourthTrackNotes[200];
note* trackNotes;

int trackIndexes[4] = {0, 0, 0, 0};
bool playable[4] = {false, false, false, false};
bool finished[4] = {false, false, false, false};
int timers[4] = {0, 0, 0, 0};
int* trackIndex = trackIndexes;

int i1 = 0, i2 = 0, i3 = 0, i4 = 0;
unsigned int freq;
unsigned int duration;

int lastNote = -1;
unsigned long startTime = 0;
unsigned long endTime = 0;
bool recording = false;
bool paused = false; 

void setup() {
  Serial.begin(9600);
  pin_config();

  tones[0].begin(buzzerPins[0]);
  tones[1].begin(buzzerPins[1]);
  tones[2].begin(buzzerPins[2]);
  tones[3].begin(buzzerPins[3]);
  
  currentTone = tones[0];
  digitalWrite(ledPins[0], HIGH);
  digitalWrite(ledPins[1], LOW);
  digitalWrite(ledPins[2], LOW);
  digitalWrite(ledPins[3], LOW);
}

void loop() {
  pin_read(); 
  track_set();
  octave_set();
  //Serial.print(track1);
  //Serial.print(track2);
  //Serial.println(currentToneIndex);
  //delay(100);
  process_rec_pause();
  play();
  processKeys();

}

void track_set() {
  if (track1 == LOW) {
    digitalWrite(ledPins[currentToneIndex], LOW);
    currentTone.stop();
    currentToneIndex = 0;
    currentTone = tones[currentToneIndex];
    digitalWrite(ledPins[currentToneIndex], HIGH);

    trackNotes = firstTrackNotes;
    trackIndex = trackIndexes;
    recording = false;
    paused = true;
  }
  if (track2 == LOW) {
    digitalWrite(ledPins[currentToneIndex], LOW);
    currentTone.stop();
    currentToneIndex = 1;
    currentTone = tones[currentToneIndex];
    digitalWrite(ledPins[currentToneIndex], HIGH);

    trackNotes = secondTrackNotes;
    trackIndex = trackIndexes + 1;
    recording = false;
    paused = true;
  }
  if (track3 == LOW) {
    digitalWrite(ledPins[currentToneIndex], LOW);
    currentTone.stop();
    currentToneIndex = 2;
    currentTone = tones[currentToneIndex];
    digitalWrite(ledPins[currentToneIndex], HIGH);

    trackNotes = thirdTrackNotes;
    trackIndex = trackIndexes + 2;
    recording = false;
    paused = true;
  }
  if (track4 == LOW) {
    digitalWrite(ledPins[currentToneIndex], LOW);
    currentTone.stop();
    currentToneIndex = 3;
    currentTone = tones[currentToneIndex];
    digitalWrite(ledPins[currentToneIndex], HIGH);

    trackNotes = fourthTrackNotes;
    trackIndex = trackIndexes + 4;
    recording = false;
    paused = true;
  }
}
void octave_set() {
  if(octaveButton == LOW) {
    octave = octave2;
    blackKeys = blackKeys2;
  } else {
    octave = octave1;
    blackKeys = blackKeys1;
  }
}
void pin_config() {
  pinMode(buttonPins[0], INPUT_PULLUP);
  pinMode(buttonPins[1], INPUT_PULLUP);
  pinMode(buttonPins[2], INPUT_PULLUP);
  pinMode(buttonPins[3], INPUT_PULLUP);
  pinMode(buttonPins[4], INPUT_PULLUP);
  pinMode(buttonPins[5], INPUT_PULLUP);
  pinMode(buttonPins[6], INPUT_PULLUP);

  pinMode(blackKeyPins[0], INPUT_PULLUP);
  pinMode(blackKeyPins[1], INPUT_PULLUP);
  pinMode(blackKeyPins[2], INPUT_PULLUP);
  pinMode(blackKeyPins[3], INPUT_PULLUP);
  pinMode(blackKeyPins[4], INPUT_PULLUP);
  
  pinMode(trackPins[0], INPUT_PULLUP);
  pinMode(trackPins[1], INPUT_PULLUP);
  pinMode(trackPins[2], INPUT_PULLUP);
  pinMode(trackPins[3], INPUT_PULLUP);
  
  pinMode(ledPins[0], OUTPUT);
  pinMode(ledPins[1], OUTPUT);
  pinMode(ledPins[2], OUTPUT);
  pinMode(ledPins[3], OUTPUT);

  pinMode(octavePin, INPUT_PULLUP);
  pinMode(recordPin, INPUT_PULLUP);
  pinMode(pausePin, INPUT_PULLUP);
}
void pin_read() {
  key1 = digitalRead(buttonPins[0]);
  key2 = digitalRead(buttonPins[1]);
  key3 = digitalRead(buttonPins[2]);
  key4 = digitalRead(buttonPins[3]);
  key5 = digitalRead(buttonPins[4]);
  key6 = digitalRead(buttonPins[5]);
  key7 = digitalRead(buttonPins[6]);
  key8 = digitalRead(buttonPins[7]);

  black1 = digitalRead(blackKeyPins[0]);
  black2 = digitalRead(blackKeyPins[1]);
  black3 = digitalRead(blackKeyPins[2]);
  black4 = digitalRead(blackKeyPins[3]);
  black5 = digitalRead(blackKeyPins[4]);
  
  track1 = digitalRead(trackPins[0]);
  track2 = digitalRead(trackPins[1]);
  track3 = digitalRead(trackPins[2]);
  track4 = digitalRead(trackPins[3]);

  octaveButton = digitalRead(octavePin);
  recordButton = digitalRead(recordPin);
  pauseButton = digitalRead(pausePin);
}

void play() {
  if(playable[0] && !(still_playing() && finished[0])) {
    duration = firstTrackNotes[i1].duration;
    freq = firstTrackNotes[i1].freq;
    if (freq) tones[0].play(freq);

    if(millis() > timers[0] + duration) {
      timers[0] = millis();
      tones[0].stop();
      i1 = (i1 + 1) % trackIndexes[0];

      Serial.print("Playing ");
      Serial.print(freq);
      Serial.print(" for ");
      Serial.println(duration);

      if (i1 == 0) finished[0] = true;
    }
  }
  /*
  if(! (still_playing() && finished[1])) {
    duration = secondTrackNotes[i2].duration;
    freq = secondTrackNotes[i2].freq;
    if(freq) tones[i2].play(freq);

    if(millis() > timers[1] + duration) {
      timers[1] = millis();
      tones[1].stop();
      i2 = (i2 + 1) % trackIndexes[1];
      if (i2 == 0) finished[1] = true;
    }
  }

  if(! (still_playing() && finished[2])) {
    duration = thirdTrackNotes[i3].duration;
    freq = thirdTrackNotes[i3].freq;
    if(freq) tones[i3].play(freq);

    if(millis() > timers[2] + duration) {
      timers[2] = millis();
      tones[2].stop();
      i3 = (i3 + 1) % trackIndexes[2];
      if (i3 == 0) finished[2] = true;
    }
  }

  if(! (still_playing() && finished[3])) {
    duration = fourthTrackNotes[i4].duration;
    freq = fourthTrackNotes[i4].freq;
    if(freq) tones[i4].play(freq);

    if(millis() > timers[3] + duration) {
      timers[3] = millis();
      tones[3].stop();
      i4 = (i4 + 1) % trackIndexes[3];
      if (i4 == 0) finished[3] = true;
    }
  }
  */
  if(all_finished()) {
    start_all();
  }
}
bool still_playing() {
  return !(finished[0] && finished[1] && finished[2] && finished[3]);
}
bool all_finished() {
  return finished[0] && finished[1] && finished[2] && finished[3];
}
bool start_all() {
  i1 = 0; i2 = 0; i3 = 0; i4 = 0;
  finished[0] = false;
  finished[1] = false;
  finished[2] = false;
  finished[3] = false;
}

void process_rec_pause() {
  if (recordButton == LOW) { //setup recording state
    delay(500);
    digitalWrite(ledPins[(currentToneIndex + 2) % 4], HIGH);
    recording = true;
    paused = false;
    playable[currentToneIndex] = false;
    trackIndexes[currentToneIndex] = 0;
    lastNote = 0;
    startTime = millis();
  }
  if (pauseButton == LOW) { //register last delay
    endTime = millis();
    digitalWrite(ledPins[(currentToneIndex + 2) % 4], LOW);

    trackNotes[*trackIndex].freq = 0;
    trackNotes[*trackIndex].duration = startTime - endTime;
    (*trackIndex)++;

    recording = false;
    paused = true;

    playable[currentToneIndex] = true;
  }
}
void processKeys() {
  if (key1 == LOW) {
    currentTone.play(octave[0]);
    if(lastNote == 0) {
      startTime = millis();
      lastNote = octave[0];
      Serial.print("Begin note: ");
      Serial.print(octave[0]);
    }
  }
  else if (key2 == LOW) {
    currentTone.play(octave[1]);
    if(lastNote == 0) {
      startTime = millis();
      lastNote = octave[1];
      Serial.print("Begin note: ");
      Serial.println(octave[1]);
    }
  }
  else if (key3 == LOW) {
    currentTone.play(octave[2]);
    if(lastNote == 0) {
      startTime = millis();
      lastNote = octave[2];
    }
  }
  else if (key4 == LOW) {
    currentTone.play(octave[3]);
    if(lastNote == 0) {
      startTime = millis();
      lastNote = octave[3];
    }
  }
  else if (key5 == LOW) {
    currentTone.play(octave[4]);
    if(lastNote == 0) {
      startTime = millis();
      lastNote = octave[4];
    }
  }
  else if (key6 == LOW) {
    currentTone.play(octave[5]);
    if(lastNote == 0) {
      startTime = millis();
      if(lastNote == 0) {
      startTime = millis();
      lastNote = octave[5];
    }
    }
  }
  else if (key7 == LOW) {
    currentTone.play(octave[6]);
    if(lastNote == 0) {
      startTime = millis();
      lastNote = octave[6];
    }
  }
  else if (black1 == LOW) {
    currentTone.play(blackKeys[0]);
    if(lastNote == 0) {
      startTime = millis();
      lastNote = blackKeys[0];
    }
  }
  else if (black2 == LOW) {
    currentTone.play(blackKeys[1]);
    if(lastNote == 0) {
      startTime = millis();
      lastNote = blackKeys[1];
    }
  }
  else if (black3 == LOW) {
    currentTone.play(blackKeys[2]);
    if(lastNote == 0) {
      startTime = millis();
      lastNote = blackKeys[2];
    }
  }
  else if (black4 == LOW) {
    currentTone.play(blackKeys[3]);
    if(lastNote == 0) {
      startTime = millis();
      lastNote = blackKeys[3];
    }
  }
  else if (black5 == LOW) {
    currentTone.play(blackKeys[4]);
    if(lastNote == 0) {
      startTime = millis();
      lastNote = blackKeys[4];
    }
  }
  else  { //process pressed note, if any
    currentTone.stop();
    if(recording && lastNote) {

      endTime = millis();

      Serial.print("End note: ");
      Serial.print(lastNote);

      trackNotes[*trackIndex].freq = lastNote;
      trackNotes[*trackIndex].duration = endTime - startTime;

      Serial.print(" Duration ");
      Serial.println(endTime - startTime);

      lastNote = 0;
      (*trackIndex)++;
      Serial.print("Track index: ");
      Serial.print(*trackIndex);
      Serial.println(trackIndexes[0]);
    }
  }
}
