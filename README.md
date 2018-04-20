# DrumDuino

## Description
DrumDuino est drumkit à base de piezos permettant de créer un rythme et de l'enregister sur un Arduino Leonardo. Sur ce rythme sera ensuite jouer une suite de note en improvisation.

Il est composé de deux piezos en entré qui lors d'un contact joue un son sur les deux autres piezos. Il est ensuite possible via un bouton de passer en mode enregistrement. Avec un autre bouton il est possible d'écouter son enregistrement avec une mélodie improvisée sur une gamme de note définie en avance.

## Schéma Circuit
![schema.png](schema.png)

## Code Arduino
```C
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
```
Cette fonction est appellé par l'intéruption suivante, appelée au moment du clic sur le bouton record mode
```C
  attachInterrupt(digitalPinToInterrupt(recordPin), changeState, LOW);
```

```C
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
```
Cette fonction est appellée par l'intéruption suivante, appelée au moment du clic sur le bouton play record mode

```C
  attachInterrupt(digitalPinToInterrupt(recordPin), changeState, LOW);
```


Ces deux fonctions de changement d'état permettent ensuite au code de gérer les différents cas comme dans la fonction play buzzer 

```C
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
```








## Optimisation possible
Utilisation de la librairies [Tonal](https://github.com/danigb/tonal) pour créer des improvisation sur de nombreuses gammes différentes.
