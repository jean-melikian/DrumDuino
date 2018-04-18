#include <Bounce2.h>

const int buzz = 8;
const int buzz2 = 9;
const int recordPin = 7;

int lastSensorValue = 0;
int lastSensorValue2 = 0;

unsigned long previousMillis = 0;
const long intervalLoop = 2;

unsigned long previousMillisInterup = 0;
const long intervalInterupt = 10000;

int recordCounts;
int records[1000];
unsigned long recordsTimer[1000];

enum state {
  RECORDING,
  RECORD_PLAYING,
  FREE_PLAY,
};

int currentState = RECORDING;


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(recordPin, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(recordPin), changeState, LOW);
}


void changeState() {
  detachInterrupt(recordPin);
  previousMillisInterup = 0;

  if (currentState == RECORDING) {
    currentState = FREE_PLAY;
  } else {
    currentState = RECORDING;
  }
  Serial.println("Switch");

}


// the loop routine runs over and over again forever:
void loop() {

  previousMillisInterup += 1;
  if (previousMillisInterup >= intervalInterupt) {
    attachInterrupt(digitalPinToInterrupt(recordPin), changeState, LOW);
  }

  switch (currentState) {
    case RECORDING:
      record();
      break;
    case FREE_PLAY:
      freePlay();
      break;
  }

  Serial.println(previousMillisInterup);


}


void record() {
  unsigned long currentMillis = millis();

  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  int sensorValue2 = analogRead(A1);
  if (currentMillis - previousMillis >= intervalLoop) {

    previousMillis = currentMillis;
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    if (sensorValue >= lastSensorValue + 20) {
      noTone(buzz2);
      Serial.println("dafuck");
      tone(buzz, 523, 200);
    }

    if (sensorValue2 >= lastSensorValue2 + 20) {
      noTone(buzz);
      Serial.println("dafuck22");
      tone(buzz2, 900, 200);
    }
  }
  lastSensorValue = sensorValue;
  lastSensorValue2 = sensorValue2;
  //print out the value you read:
}

void freePlay() {

  unsigned long currentMillis = millis();

  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  int sensorValue2 = analogRead(A1);
  if (currentMillis - previousMillis >= intervalLoop) {
    previousMillis = currentMillis;

    // save the last time you blinked the LED
    previousMillis = currentMillis;
    if (sensorValue >= lastSensorValue + 20) {
      noTone(buzz2);
      Serial.println("dafuck");
      tone(buzz, 523, 200);
    }

    if (sensorValue2 >= lastSensorValue2 + 20) {
      noTone(buzz);
      Serial.println("dafuck22");
      tone(buzz2, 900, 200);
    }

  }
  lastSensorValue = sensorValue;
  lastSensorValue2 = sensorValue2;
  //print out the value you read:

}
