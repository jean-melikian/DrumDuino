#define DO 523
#define RE 587
#define MI 657
#define SOL 783
#define LA 880

#define DURATION 200
const int buzz = 8;
//const int buzz2 = 12;
const int buzz2 = 3;
const int recordPin = 7;
const int playRecordedPin = 2;

const int freq1 = 523;
const int freq2 = 900;

int lastSensorValue = 0;
int lastSensorValue2 = 0;

int lastBuzzerPinToned = 0;

unsigned long previousMillis = 0;
const long intervalLoop = 2;

unsigned long previousMillisInterup = 0;
const long intervalInterupt = 50;
const long intervalPlayInterupt = 50;

unsigned long previousMillisRecord = 0;
const long intervalRecord = 100;

int recordsCount;
int records[100];
unsigned long recordsTimer[100];

int playPosition = 0;
unsigned long lastTimePlayedRecorded = 0;

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
  pinMode(playRecordedPin, INPUT_PULLUP);
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
  if (previousMillisInterup >= intervalPlayInterupt) {
    if (currentState == RECORD_PLAYING) {
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
      //Serial.print("[RECORDING] ");
      record();
      break;
    case FREE_PLAY:
      //Serial.print("[FREE_PLAY] ");
      freePlay(false);
      break;
    case RECORD_PLAYING:
      //Serial.print("[RECORD_PLAYING] ");
      freePlay(true);
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
}

void freePlay(bool isPlaying) {

  unsigned long currentMillis = millis();
  if (isPlaying) {

    if (playPosition == 0 || (recordsTimer[playPosition] - recordsTimer[playPosition - 1] < currentMillis - lastTimePlayedRecorded)) {
      if (records[playPosition] == buzz) {
        playBuzzer(records[playPosition], freq1, DURATION, currentMillis);
      } else if (records[playPosition] == buzz2) {
        playBuzzer(records[playPosition], freq2, DURATION, currentMillis);
      }

      int randomTone = random(5);
      int freq = 0;
      switch(randomTone) {
        case 0:
        freq = DO;
        break;
        case 1:
        freq = RE;
        break;
        case 2:
        freq = MI;
        break;
        case 3:
        freq = SOL;
        break;
        case 4:
        freq = LA;
        break;
      }
      
      unsigned long improDuration = 0;
      
      lastTimePlayedRecorded = currentMillis;
      if(playPosition >= recordsCount-1) {
        playPosition = 0;
      } else {
        improDuration = recordsTimer[playPosition + 1] - recordsTimer[playPosition] - DURATION - 50;
        playBuzzer(records[playPosition], freq, improDuration, currentMillis);
        playPosition++;
      }
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
  /*
    Serial.print(sensorValue);
    Serial.print("-");
    Serial.print(sensorValue2);
    Serial.print("\n");
    Serial.println(currentState);
  */
}

// -- CORE FUNCTIONS

void playBuzzer(int buzzerOutputPin, unsigned int frequency, unsigned long duration, unsigned long currentMillis) {
  if (lastBuzzerPinToned != 0) {
    noTone(lastBuzzerPinToned);
  }
  switch (currentState) {
    case RECORDING:
      if (previousMillisRecord >= intervalRecord) {
        records[recordsCount] = buzzerOutputPin;
        recordsTimer[recordsCount] = currentMillis;
        recordsCount++;
        previousMillisRecord = 0;
        Serial.print("[RECORDING]: ");
        Serial.println(buzzerOutputPin);
        for (int i = 0; i < recordsCount; i++) {
          Serial.print(records[i]);
          Serial.print(" - ");
        }
        Serial.println("__________");
      }
      break;
    case FREE_PLAY:
      Serial.print("Playing on buzzer pin output: ");
      Serial.println(buzzerOutputPin);
      break;
    case RECORD_PLAYING:
      Serial.print("Playing recorded: ");
      Serial.println(buzzerOutputPin);
      break;
    default:
      break;
  }




  tone(buzzerOutputPin, frequency, duration);
  lastBuzzerPinToned = buzzerOutputPin;
}

