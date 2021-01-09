#include <Tone.h>
//#include "pitches.h"

int* octave; //holds the current octave notes
int* blackKeys; //holds the current black key notes (sharps)
int octave1[] = {NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5}; //first octave sample
int octave2[] = {NOTE_C6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_G6, NOTE_A6, NOTE_B6}; //second octave sample
int blackKeys1[] = {NOTE_CS5, NOTE_DS5, NOTE_FS5, NOTE_GS5, NOTE_AS5};
int blackKeys2[] = {NOTE_CS6, NOTE_DS6, NOTE_FS6, NOTE_GS6, NOTE_AS6};

int key1, key2, key3, key4, key5, key6, key7, track1, track2, track3, track4, octaveButton, recordButton, pauseButton; //button values
int black1, black2, black3, black4, black5; 
int trackPins[] = {34, 33, 38, 39};
int buttonPins[] = {22, 23, 24, 25, 26, 27, 28};
int blackKeyPins[] = {44, 45, 46, 47, 29};
int buzzerPins[] = {40, 41, 42, 43};
int ledPins[] = {50, 51, 52, 53};
int octavePin = 30;
int recordPin = 3;
int pausePin = 7;


Tone tones[4]; // Tone objects for buzzers
Tone currentTone; // where the current Tone object is stored (the keys are used to play on a single buzzer)
int currentToneIndex = 0; //vector index of the current Tone object

struct note { //struct declaration for a certain (recorded) note. the relevant info is: frequency & duration
  unsigned short int freq;
  unsigned long duration;
};

note firstTrackNotes[200];
note secondTrackNotes[200];
note thirdTrackNotes[200];
note fourthTrackNotes[200];
note* trackNotes;

int trackIndexes[4] = {0, 0, 0, 0};   //to keep track of where the last note on each track is (also the number of notes + pauses)
bool playable[4] = {false, false, false, false};  //true if there is a recorded sequence of noets on a track
bool finished[4] = {false, false, false, false};  //used in play(), true if a certain sequence has finished playing
int timers[4] = {0, 0, 0, 0};                     //used in play(), for counting the note lengths (in ms)
int* trackIndex = trackIndexes;                   //current track index

int i1 = 0, i2 = 0, i3 = 0, i4 = 0;               //indexes for trackNotes vectors, used in play()
unsigned int freq;                                //extracted frequency of the current note to be played
unsigned int duration;                            //extracted duration of the current note to be played

int lastNote = -1;                                //last played note. is assigned a valid note from the octaves after a 
                                                  //key has been pressed. after release, it is set back to 0.
unsigned long startTime = 0;                      //used for timing the notes/pauses in order to record them. expressed in ms
unsigned long endTime = 0;                        //same as above. 
bool recording = false;                           //true after the RECORD button has been pressed up until the user presses PAUSE
bool paused = false;                              //complementary to RECORD state

unsigned long int startTime_dbg = 0;              //for debug purposes
unsigned long int endTime_dbg = 0;                //for debug purposes

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
  pin_read();           //read values from digital pins
  track_set();          //change track, if needed
  octave_set();         //change octave, if needed
  process_rec_pause();  //change RECORD/PAUSED state, if needed
  processKeys();        //register pressed musical keys and play specific sound
  play();               //play recorded tracks
}

void track_set() {
  if (track1 == LOW) {
    digitalWrite(ledPins[currentToneIndex], LOW);     //disable the previous LED
    currentTone.stop();                               //stop playing on previous buzzer
    currentToneIndex = 0;                            
    currentTone = tones[currentToneIndex];            //setup buzzer 0 as the current buzzer
    digitalWrite(ledPins[currentToneIndex], HIGH);    //light up the corresponding LED

    trackNotes = firstTrackNotes;                     //setup track recording vector
    trackIndex = trackIndexes;
    recording = false;                                //initial state when changing the track: not recording
    paused = true;                                    //and paused
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
    trackIndex = trackIndexes + 3;
    recording = false;
    paused = true;
  }
}
void octave_set() {
  if(octaveButton == LOW) { //octave setting works via holding down the octave button
    octave = octave2;
    blackKeys = blackKeys2;
  } else {
    octave = octave1;
    blackKeys = blackKeys1;
  }
}
void pin_config() { //pin configuration. used in setup(). all buttons are setup with pull-up resistors.
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
void pin_read() {   //pin reading
  key1 = digitalRead(buttonPins[0]);
  key2 = digitalRead(buttonPins[1]);
  key3 = digitalRead(buttonPins[2]);
  key4 = digitalRead(buttonPins[3]);
  key5 = digitalRead(buttonPins[4]);
  key6 = digitalRead(buttonPins[5]);
  key7 = digitalRead(buttonPins[6]);

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

void play() { //play playable tracks in a loop
  for(int i = 0; i < 4; i++){
    if(!playable[i]) finished[i] = true;    //any track that is unplayable is FINISHED (we should not wait after it)
  }  
  
  if(playable[0] && !(still_playing() && finished[0])) {  //only start timers for a track that is PLAYABLE, NOT FINISHED and 
                                                          //and AFTER other tracks have finished playing (still_playing() checks this)
    duration = firstTrackNotes[i1].duration;              //extract note duration
    freq = firstTrackNotes[i1].freq;                      //extract note frequency / pause
    if (freq) tones[0].play(freq);                        //freq = 0 means there is a break/pause. we do not play anything

    if(millis() > timers[0] + duration) {                 //this is similar to delay(duration), but it is NON BLOCKING (important)
                                                          //we check if <<duration>> ms have passed since the last note
      tones[0].stop();                                    //stop playing the note (the time has passed)
      timers[0] = millis();                               //start timer for current buzzer, for the next note
      i1 = (i1 + 1) % trackIndexes[0];                    //go to the next note 
      if (i1 == 0) finished[0] = true;                    //if it is the last one, we mark this track as FINISHED by setting the flag

      Serial.print("Playing ");                           //DEBUG PRINTS
      Serial.print(freq);
      Serial.print(" for ");
      Serial.print(duration);
      Serial.print(" index ");
      Serial.print(i1);
      Serial.print(" out of ");
      Serial.println(trackIndexes[0]);
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
  if(all_finished()) {      //if all tracks have finished their melody, we will loop again.
    start_all();
  }
}
bool still_playing() { //check if any track is still playing its loop
  return !(finished[0] && finished[1] && finished[2] && finished[3]);
}
bool start_all() { //start by initialising indexes and clearing the FINISHED flags
  i1 = 0; i2 = 0; i3 = 0; i4 = 0;
  finished[0] = false || !playable[0];
  finished[1] = false || !playable[1];
  finished[2] = false || !playable[2];
  finished[3] = false || !playable[3];
  timers[0] = millis();
  timers[1] = millis();
  timers[2] = millis();
  timers[3] = millis();
}
bool all_finished() { //all tracks have finished their loop; we can start looping again
  return finished[0] && finished[1] && finished[2] && finished[3];
}

void process_rec_pause() {
  if (recordButton == LOW) { //setup recording state
    delay(500);                   //wait for half a second
    Serial.println("RECORDING");  //debug purposes
    digitalWrite(ledPins[(currentToneIndex + 2) % 4], HIGH); //light up a LED 2 spaces away (from anothe buzzer) to signal recording state
    recording = true;   //set recording flag
    paused = false;     //clear paused flag
    playable[currentToneIndex] = false; //current track is unplayable, as it is currently being recorded
    trackIndexes[currentToneIndex] = 0; //start from index 0
    lastNote = 0;
    startTime = millis(); //start recording
  }
  if (pauseButton == LOW) { //register last delay
    endTime = millis();
    Serial.println("PAUSED");   //debug purposes
    delay(500);                 //simulate "processing" state
    digitalWrite(ledPins[(currentToneIndex + 2) % 4], LOW); //turn off the lit up led from the recording state
    if(endTime - startTime > 100) {
      trackNotes[*trackIndex].freq = 0;     //register last delay
      trackNotes[*trackIndex].duration = endTime - startTime;
      (*trackIndex)++;
    }
    recording = false;    //clear recording flag
    paused = true;        //set paused flag

    playable[currentToneIndex] = true;  //mark current track as playable. a recording just finished.
    timers[0] = millis();
    timers[1] = millis();
    timers[2] = millis();
    timers[3] = millis(); ///initialise timers
  }
}
void processKeys() {
  if(recording && pressed_key()) {  //record pauses
    endTime = millis(); 
    if(endTime - startTime > 100) { //filter out short pauses
      
        trackNotes[*trackIndex].freq = 0;                       //pauses have frequency 0
        trackNotes[*trackIndex].duration = endTime - startTime; //duration

        Serial.print(" Paused for ");         //debug purposes
        Serial.print(endTime - startTime);  //debug purposes

        lastNote = 0;
        (*trackIndex)++;                    //increment the index 
        Serial.print("Track index: ");       //debug purposes
        Serial.print(*trackIndex);           //debug purposes
      }
  }
  if (key1 == LOW) {
    currentTone.play(octave[0]);
    if(lastNote == 0) {               //this is the first time passing after the key has been pressed
      startTime = millis();           //retain when the key has first been pressed, with millis()
      lastNote = octave[0];
      Serial.print("Begin note: ");   //for debug purposes
      Serial.print(octave[0]);        //for debug purposes
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
      lastNote = octave[5];
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
    if(recording && lastNote) {   //if we are recording and we just stopped playing a note (released a key)
      endTime = millis();         //retain the release time

      if(endTime - startTime > 100) { //filter out short notes
      
        Serial.print("End note: ");   //debug purposes
        Serial.print(lastNote);       //debug purposes
      
        trackNotes[*trackIndex].freq = lastNote;                //retain the released note
        trackNotes[*trackIndex].duration = endTime - startTime; //and the duration

        Serial.print(" Duration ");         //debug purposes
        Serial.print(endTime - startTime);  //debug purposes

        lastNote = 0;
        (*trackIndex)++;                    //increment the index 
        Serial.print("Track index: ");       //debug purposes
        Serial.print(*trackIndex);           //debug purposes
      }
    }
  }
}
bool pressed_key() {
  return !(key1 && key2 && key3 && key4 && key5 && key6 && key7 && black1 && black2 && black3 && black4 && black5);
}
