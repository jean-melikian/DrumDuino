# DrumDuino

## Description
DrumDuino est drumkit à base de piezos permettant de créer un rythme et de l'enregister sur un Arduino Leonardo. Sur ce rythme sera ensuite jouer une suite de note en improvisation.

Il est composé de deux piezos en entré qui lors d'un contact joue un son sur les deux autres piezos. Il est ensuite possible via un bouton de passer en mode enregistrement. Avec un autre bouton il est possible d'écouter son enregistrement avec une mélodie improvisée sur une gamme de note définie en avance.

## Schéma Circuit
![schema.png](schema.png)

## Code Arduino
```C
void changeState()
```
Cette fonction est appellé 





## Optimisation possible
Utilisation de la librairies [Tonal](https://github.com/danigb/tonal) pour créer des improvisation sur de nombreuses gammes différentes.
