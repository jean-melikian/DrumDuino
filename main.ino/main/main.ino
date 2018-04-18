const int buzz = 8;
const int buzz2 = 9;
const int recordPin = 7;

int lastSensorValue = 0;
int lastSensorValue2 = 0;

unsigned long previousMillis = 0;
const long intervalLoop = 2;

unsigned long previousMillisInterup = 0;
const long intervalInterupt = 50;

unsigned long previousMillisRecord = 0;
const long intervalRecord = 100;

int recordsCount;
int records[100];
unsigned long recordsTimer[100];

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
}


void changeState() {
  if (previousMillisInterup >= intervalInterupt) {
    if (currentState == RECORDING) {
    currentState = FREE_PLAY;
    } else {
      currentState = RECORDING;
    }
    Serial.println("Switch");
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
      freePlay();
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

    previousMillis = currentMillis;
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    
    if (sensorValue >= lastSensorValue + 70) {
      noTone(buzz2);
      Serial.println("dafuck");
      if(previousMillisRecord >= intervalRecord){
        records[recordsCount] = buzz;
        recordsTimer[recordsCount] = currentMillis;
        recordsCount++;
        previousMillisRecord = 0;
      }
      tone(buzz, 523, 200);
    }
    else if (sensorValue2 >= lastSensorValue2 + 70) {
      noTone(buzz);
      Serial.println("dafuck22");
      if(previousMillisRecord >= intervalRecord){
        records[recordsCount] = buzz2;
        recordsTimer[recordsCount] = currentMillis;
        recordsCount++; 
        previousMillisRecord = 0;
      }
      tone(buzz2, 900, 200);
    }
    
  }
  lastSensorValue = sensorValue;
  lastSensorValue2 = sensorValue2;
  for (int i = 0;i<recordsCount;i++){
    Serial.print(records[i]);  
    Serial.print(" - ");  
  }
  Serial.println("__________");
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
    if (sensorValue >= lastSensorValue + 50) {
      noTone(buzz2);
      Serial.println("dafuck");
      tone(buzz, 523, 200);
    }else if (sensorValue2 >= lastSensorValue2 + 50) {
      noTone(buzz);
      Serial.println("dafuck22");
      tone(buzz2, 900, 200);
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
