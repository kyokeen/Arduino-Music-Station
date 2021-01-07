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
int recordPin = 32;
int pausePin = 33;


Tone tones[4];
Tone currentTone;
int currentToneIndex = 0;

struct note {
  unsigned char freq;
  unsigned short int duration;
};

note firstTrackNotes[200];
note secondTrackNotes[200];
note* trackNotes;
int firstTrackIndex = 0;
int secondTrackIndex = 0;
int* trackIndex;
int lastNote = 0;
unsigned long startTime = 0;
unsigned long endTime = 0;
bool recording = false;
bool paused = false; 
bool firstPlayable = false;
bool secondPlayable = false;

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
  // citire stare butoane
  pin_read(); 
  track_set();
  
  
  if(octaveButton == LOW) {
    octave = octave2;
    blackKeys = blackKeys2;
  } else {
    octave = octave1;
    blackKeys = blackKeys1;
  }
  
  /*
  if (recordButton == LOW) { //setup recording state
    delay(500);
    digitalWrite(ledPins[3], HIGH);
    recording = true;
    paused = false;
    startTime = millis();
  }
  if (pauseButton == LOW) { //register last pause (?) and play recorded tunes
    endTime = millis();
    trackNotes[*trackIndex].freq = 0;
    trackNotes[*trackIndex].duration = startTime - endTime;
    (*trackIndex)++;

    digitalWrite(ledPins[3], LOW);
    recording = false;
    paused = true;

    if (trackNotes == firstTrackNotes) {
      firstPlayable = true;
    }
    else if (trackNotes == secondTrackNotes) {
      secondPlayable = true;
    }
  }

  if (firstPlayable || secondPlayable) {
    int i = 0, j = 0;
    while(i < firstTrackIndex && j < secondTrackIndex) {
      i++; j++;
    }
  }
  */
  //Serial.print(track1);
  //Serial.print(track2);
  //Serial.println(currentToneIndex);
  //delay(100);

  if (key1 == LOW) {
    currentTone.play(octave[0]);
  }
  else if (key2 == LOW) {
    currentTone.play(octave[1]);
  }
  else if (key3 == LOW) {
    currentTone.play(octave[2]);
  }
  else if (key4 == LOW) {
    currentTone.play(octave[3]);
  }
  else if (key5 == LOW) {
    currentTone.play(octave[4]);
  }
  else if (key6 == LOW) {
    currentTone.play(octave[5]);
  }
  else if (key7 == LOW) {
    currentTone.play(octave[6]);
  }
  else if (black1 == LOW) {
    currentTone.play(blackKeys[0]);
  }
  else if (black2 == LOW) {
    currentTone.play(blackKeys[1]);
  }
  else if (black3 == LOW) {
    currentTone.play(blackKeys[2]);
  }
  else if (black4 == LOW) {
    currentTone.play(blackKeys[3]);
  }
  else if (black5 == LOW) {
    currentTone.play(blackKeys[4]);
  }
  else  { //process pressed note, if any
    currentTone.stop();

    if(lastNote != 0) {
      endTime = millis();
      trackNotes[*trackIndex].freq = lastNote;
      trackNotes[*trackIndex].duration = startTime - endTime;

      lastNote = 0;
      (*trackIndex)++;
    }
  }
  

  
}

void track_set() {
  if (track1 == LOW) {
    digitalWrite(ledPins[currentToneIndex], LOW);
    currentTone.stop();
    currentToneIndex = 0;
    currentTone = tones[currentToneIndex];
    digitalWrite(ledPins[currentToneIndex], HIGH);

    trackNotes = firstTrackNotes;
    trackIndex = &firstTrackIndex;
    recording = false;
    paused = true;
  }
  if (track2 == LOW) {
    digitalWrite(ledPins[currentToneIndex], LOW);
    currentTone.stop();
    currentToneIndex = 1;
    currentTone = tones[currentToneIndex];
    digitalWrite(ledPins[currentToneIndex], HIGH);

    //trackNotes = secondTrackNotes;
    //trackIndex = &secondTrackIndex;
    //recording = false;
    //paused = true;
  }
  if (track3 == LOW) {
    digitalWrite(ledPins[currentToneIndex], LOW);
    currentTone.stop();
    currentToneIndex = 2;
    currentTone = tones[currentToneIndex];
    digitalWrite(ledPins[currentToneIndex], HIGH);

    //trackNotes = secondTrackNotes;
    //trackIndex = &secondTrackIndex;
    //recording = false;
    //paused = true;
  }
  if (track4 == LOW) {
    digitalWrite(ledPins[currentToneIndex], LOW);
    currentTone.stop();
    currentToneIndex = 3;
    currentTone = tones[currentToneIndex];
    digitalWrite(ledPins[currentToneIndex], HIGH);

    //trackNotes = secondTrackNotes;
    //trackIndex = &secondTrackIndex;
    //recording = false;
    //paused = true;
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
