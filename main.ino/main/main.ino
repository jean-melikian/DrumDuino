#define DURATION 200
const int buzz = 8;
const int buzz2 = 12;
const int recordPin = 7;
const int playRecordedPin = 4;

const int freq1 = 523;
const int freq2 = 900;

int lastSensorValue = 0;
int lastSensorValue2 = 0;

int lastBuzzerPinToned = 0;

unsigned long previousMillis = 0;
const long intervalLoop = 2;

unsigned long previousMillisInterup = 0;
const long intervalInterupt = 50;

unsigned long previousMillisRecord = 0;
const long intervalRecord = 100;

int recordsCount;
int records[100];
unsigned long recordsTimer[100];
int playPosition = 0;

enum state {
  RECORDING,
  RECORD_PLAYING,
  FREE_PLAY,
};

int currentState = FREE_PLAY;


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(recordPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(recordPin), changeState, LOW);
  attachInterrupt(digitalPinToInterrupt(playRecordedPin), togglePlayRecorded, LOW);
}


void changeState() {
  if (previousMillisInterup >= intervalInterupt) {
    if (currentState == RECORDING) {
      currentState = FREE_PLAY;
      Serial.println("Switch recording OFF");
    } else {
      currentState = RECORDING;
      Serial.println("Switch recording ON");
    }
  }
  previousMillisInterup = 0;
}


void togglePlayRecorded() {
  if (previousMillisInterup >= intervalInterupt) {
    if (currentState = RECORD_PLAYING) {
      currentState = FREE_PLAY;
      Serial.println("Switch stopped playing recorded track");
    } else {
      currentState = RECORD_PLAYING;
      Serial.println("Switch started playing recorded track");
    }
  }
  previousMillisInterup = 0;
}



// the loop routine runs over and over again forever:
void loop() {

  previousMillisInterup = previousMillisInterup  + 1;
  switch (currentState) {
    case RECORDING:
      record();
      break;
    case FREE_PLAY:
    case RECORD_PLAYING:
      freePlay(currentState == RECORD_PLAYING);
      break;
  }

}

void record() {
  unsigned long currentMillis = millis();
  previousMillisRecord ++;
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  int sensorValue2 = analogRead(A1);
  if (currentMillis - previousMillis >= intervalLoop) {

    // save the last time you blinked the LED
    previousMillis = currentMillis;

    if (sensorValue >= lastSensorValue + 70) {
      playBuzzer(buzz, freq1, DURATION, currentMillis);
    }
    else if (sensorValue2 >= lastSensorValue2 + 70) {
      playBuzzer(buzz2, freq2, DURATION, currentMillis);
    }

  }
  lastSensorValue = sensorValue;
  lastSensorValue2 = sensorValue2;
  for (int i = 0; i < recordsCount; i++) {
    Serial.print(records[i]);
    Serial.print(" - ");
  }
  Serial.println("__________");
}

void freePlay(bool isPlaying) {

  unsigned long currentMillis = millis();
  if (isPlaying) {
    if(records[playPosition] == buzz) {
      playBuzzer(records[playPosition], freq1, DURATION, currentMillis);      
    } else if(records[playPosition] == buzz2) {
      playBuzzer(records[playPosition], freq2, DURATION, currentMillis);      
    }
    
  }

  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  int sensorValue2 = analogRead(A1);
  if (currentMillis - previousMillis >= intervalLoop) {
    previousMillis = currentMillis;

    // save the last time you blinked the LED
    previousMillis = currentMillis;
    if (sensorValue >= lastSensorValue + 50) {
      playBuzzer(buzz, freq1, DURATION, currentMillis);
    } else if (sensorValue2 >= lastSensorValue2 + 50) {
      playBuzzer(buzz2, freq2, DURATION, currentMillis);
    }
  }
  lastSensorValue = sensorValue;
  lastSensorValue2 = sensorValue2;
  //print out the value you read:
  Serial.print(sensorValue);
  Serial.print("-");
  Serial.print(sensorValue2);
  Serial.print("\n");
  Serial.println(currentState);

}

// -- CORE FUNCTIONS

void playBuzzer(int buzzerOutputPin, unsigned int frequency, unsigned long duration, unsigned long currentMillis) {
  if (lastBuzzerPinToned != 0) {
    noTone(lastBuzzerPinToned);
  }
  Serial.print("Playing on buzzer pin output: ");
  Serial.print(buzzerOutputPin);
  if (currentState == RECORDING && previousMillisRecord >= intervalRecord) {
    records[recordsCount] = buzzerOutputPin;
    recordsTimer[recordsCount] = currentMillis;
    recordsCount++;
    previousMillisRecord = 0;
    Serial.print(" [RECORDED]");
  }
  Serial.println("");

  tone(buzzerOutputPin, frequency, duration);
  lastBuzzerPinToned = buzzerOutputPin;
}

