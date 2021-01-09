#include <Tone.h>

int* octave;
int* blackKeys;
int octave1[] = {NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5};
int octave2[] = {NOTE_C6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_G6, NOTE_A6, NOTE_B6}; 
int blackKeys1[] = {NOTE_CS5, NOTE_DS5, NOTE_FS5, NOTE_GS5, NOTE_AS5};
int blackKeys2[] = {NOTE_CS6, NOTE_DS6, NOTE_FS6, NOTE_GS6, NOTE_AS6};

int track1, track2, track3, track4;
int trackPins[] = {38, 39};
int buzzerPins[] = {40, 41, 42, 43};
int ledPins[] = {50, 51, 52, 53};
int octavePin = 30;
int recordPin = 32;
int pausePin = 33;


Tone tones[4];
bool finished[4] = {false, false, false, false};
Tone currentTone;
int currentToneIndex = 0;

struct note {
  unsigned short int freq;
  unsigned short int duration;
};

note firstTrackNotes[200];
note secondTrackNotes[200];
note thirdTrackNotes[200];
note fourthTrackNotes[200];
note* trackNotes;
int* trackIndex;
int lastNote = 0;
unsigned long startTime = 0;
unsigned long endTime = 0;
bool recording = false;
bool paused = false; 
bool playable[4] = {true, true, true, true};

unsigned long timers[4] = {0, 0, 0, 0};
unsigned int freq;
unsigned int duration;

int i1 = 0, i2 = 0, i3 = 0, i4 = 0;
int j = 0;
int trackIndexes[4] = {6, 7, 5, 4};


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


  firstTrackNotes[0].freq = octave1[0];
  firstTrackNotes[1].freq = octave1[1];  
  firstTrackNotes[2].freq = octave1[2];  
  firstTrackNotes[3].freq = octave1[3];  
  firstTrackNotes[4].freq = octave1[4];  
  firstTrackNotes[5].freq = 0;

  secondTrackNotes[0].freq = octave1[5];
  secondTrackNotes[1].freq = octave1[4];
  secondTrackNotes[2].freq = octave1[5];
  secondTrackNotes[3].freq = 0;
  secondTrackNotes[4].freq = octave1[4];
  secondTrackNotes[5].freq = octave1[3];
  secondTrackNotes[6].freq = 0;

  
  firstTrackNotes[0].duration = 700;
  firstTrackNotes[1].duration = 300;
  firstTrackNotes[2].duration = 600;
  firstTrackNotes[3].duration = 200;
  firstTrackNotes[4].duration = 700;
  firstTrackNotes[5].duration = 300;


  secondTrackNotes[0].duration = 700;
  secondTrackNotes[1].duration = 300;
  secondTrackNotes[2].duration = 200;
  secondTrackNotes[3].duration = 200;
  secondTrackNotes[4].duration = 700;
  secondTrackNotes[5].duration = 300;
  secondTrackNotes[6].duration = 200;

  thirdTrackNotes[0].freq = octave2[0];
  thirdTrackNotes[1].freq = 0;
  thirdTrackNotes[2].freq = octave2[1];
  thirdTrackNotes[3].freq = octave2[2];
  thirdTrackNotes[4].freq = 0;

  thirdTrackNotes[0].duration = 1600;
  thirdTrackNotes[1].duration = 1200;
  thirdTrackNotes[2].duration = 200;
  thirdTrackNotes[3].duration = 600;
  thirdTrackNotes[4].duration = 500;

  fourthTrackNotes[0].freq = 0;
  fourthTrackNotes[1].freq = octave1[2];
  fourthTrackNotes[2].freq = octave1[1];
  fourthTrackNotes[3].freq = 0;

  fourthTrackNotes[0].duration = 3000;
  fourthTrackNotes[1].duration = 600;
  fourthTrackNotes[2].duration = 300;
  fourthTrackNotes[3].duration = 500;
}

void loop() {
  pin_read();
  play();
}

void pin_config() {
  pinMode(trackPins[0], INPUT_PULLUP);
  pinMode(trackPins[1], INPUT_PULLUP);
  pinMode(trackPins[2], INPUT_PULLUP);
  pinMode(trackPins[3], INPUT_PULLUP);
  
  pinMode(ledPins[0], OUTPUT);
  pinMode(ledPins[1], OUTPUT);
  pinMode(ledPins[2], OUTPUT);
  pinMode(ledPins[3], OUTPUT);
}

void pin_read() {
  track1 = digitalRead(trackPins[0]);
  track2 = digitalRead(trackPins[1]);
  track3 = digitalRead(trackPins[2]);
  track4 = digitalRead(trackPins[3]);
}

void play() {
  for(int i = 0; i < 4; i++){
    if(!playable[i]) finished[i] = true;
  }  
  
  if(playable[0] && !(still_playing() && finished[0])) {
    duration = firstTrackNotes[i1].duration;
    freq = firstTrackNotes[i1].freq;
    if (freq) tones[0].play(freq);

    if(millis() > timers[0] + duration) {
      timers[0] = millis();
      tones[0].stop();
      i1 = (i1 + 1) % trackIndexes[0];
      if (i1 == 0) finished[0] = true;

      Serial.print("Playing ");
      Serial.print(freq);
      Serial.print(" for ");
      Serial.println(duration);
    }
  }
  if(playable[1] && !(still_playing() && finished[1])) {
    duration = secondTrackNotes[i2].duration;
    freq = secondTrackNotes[i2].freq;
    if(freq) tones[1].play(freq);

    if(millis() > timers[1] + duration) {
      timers[1] = millis();
      tones[1].stop();
      i2 = (i2 + 1) % trackIndexes[1];
      if (i2 == 0) finished[1] = true;
    }
  }

  if(playable[2] &&  !(still_playing() && finished[2])) {
    duration = thirdTrackNotes[i3].duration;
    freq = thirdTrackNotes[i3].freq;
    if(freq) tones[2].play(freq);

    if(millis() > timers[2] + duration) {
      timers[2] = millis();
      tones[2].stop();
      i3 = (i3 + 1) % trackIndexes[2];
      if (i3 == 0) finished[2] = true;
    }
  }

  if(playable[3] && !(still_playing() && finished[3])) {
    duration = fourthTrackNotes[i4].duration;
    freq = fourthTrackNotes[i4].freq;
    if(freq) tones[3].play(freq);

    if(millis() > timers[3] + duration) {
      timers[3] = millis();
      tones[3].stop();
      i4 = (i4 + 1) % trackIndexes[3];
      if (i4 == 0) finished[3] = true;
    }
  }
  
  if(all_finished()) {
    start_all();
  }
}

bool still_playing() {
  return !(finished[0] && finished[1] && finished[2] && finished[3]);
}


bool start_all() {
  i1 = 0; i2 = 0; i3 = 0; i4 = 0;
  finished[0] = false || !playable[0];
  finished[1] = false || !playable[1];
  finished[2] = false || !playable[2];
  finished[3] = false || !playable[3];
}
/*
bool still_playing() {
  if(!(playable[0] && playable[1] && playable[2] && playable[3])) return false;

  bool playing = true;
  for(int i = 0; i < 4; i++) {
    playing = playing && (!playable[i] || playable[i] && !finished[i]);
  }
}*/

bool all_finished() {
  return finished[0] && finished[1] && finished[2] && finished[3];
}
/*
bool start_all() {
  i1=0; i2=0;
  finished[0] = false;
  finished[1] = false;
}*/

void track_set() {
  if (track1 == LOW) {
    digitalWrite(ledPins[currentToneIndex], LOW);
    currentTone.stop();
    currentToneIndex = 0;
    currentTone = tones[currentToneIndex];
    digitalWrite(ledPins[currentToneIndex], HIGH);
  }
  if (track2 == LOW) {
    digitalWrite(ledPins[currentToneIndex], LOW);
    currentTone.stop();
    currentToneIndex = 1;
    currentTone = tones[currentToneIndex];
    digitalWrite(ledPins[currentToneIndex], HIGH);
  }
  if (track3 == LOW) {
    digitalWrite(ledPins[currentToneIndex], LOW);
    currentTone.stop();
    currentToneIndex = 2;
    currentTone = tones[currentToneIndex];
    digitalWrite(ledPins[currentToneIndex], HIGH);
  }
  if (track4 == LOW) {
    digitalWrite(ledPins[currentToneIndex], LOW);
    currentTone.stop();
    currentToneIndex = 3;
    currentTone = tones[currentToneIndex];
    digitalWrite(ledPins[currentToneIndex], HIGH);
  }
}
