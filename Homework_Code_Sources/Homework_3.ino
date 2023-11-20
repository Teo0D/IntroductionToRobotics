/*
c4 e4 g4 c5
b4 d5 c5 a4
*/
//// song for opening door
int melody1[] = {262, 330, 392, 523};
int noteDurations1[] = {4, 4, 4, 2};

//// song for closing door
int melody2[] = {494, 587, 523, 440};
int noteDurations2[] = {4, 4, 4, 2};

int currNote = -1;
unsigned long long startNote = 0;
byte song1 = LOW;
byte song2 = LOW;

const int musicNorm = 500;
const int pauseFactor = 1.3;
const int maxLed = 255;
const int minLed = 0;
const int firstLevel = 1;
const int lastLevel = 3;

const int arrayButtonPin[] = {-1, 11, 6, 7}; /// probleme cu 11 ?
const int arrayLedPinLev[] = {-1, 4, 8, 12};
const int arrayLedPinDoor[] = {-1, 9, 5, 10};
const int ledPinMove = 13;
const int ledPinQueue = 2;
const int buzzerPin = 3;
int buzzerTone = 2000;

byte arrayReading[] = {LOW, LOW, LOW, LOW};
byte arrayLastReading[] = {LOW, LOW, LOW, LOW};
unsigned long long arrayLastDebounceTime[] = {-1, 0, 0, 0};
unsigned long long debounceDelay = 50;

byte arrayLedStateLev[] = {HIGH, HIGH, HIGH, HIGH};
int arrayLedStateDoor[] = {-1, maxLed, minLed, minLed};
byte arrayButtonState[] = {LOW, LOW, LOW, LOW};
byte ledStateMove = LOW;

byte moving = LOW;
byte state = LOW; // in queue
int currLevel = 1;
int toLevel = 1;
int sign = 0;
unsigned long long startIntervMove = 0;
const int moveTime = 2000;
const int blinkTime = 250;
unsigned long long startBlinkTime = 0;

int arrayDoorState[] = {-1, 4, 2, 2};

/*
0 - open
1 - transition open to close
2 - closed
3 - transition close to open
4 - can't close
*/
unsigned long long startDoorTime = 0;
const int minDoorTime = 0;
const int doorTransitionTime = musicNorm * 3.5; //// depends on song for transition duration so the song interval is shorter
const int stayOpen = 1000;

//// for level queue
struct nod{
  int level;
  nod *next;
};

nod *currNod = NULL;
nod* lastNod = NULL;

void setup() {
  for(int i = firstLevel; i <= lastLevel; i++){
    pinMode(arrayLedPinLev[i], OUTPUT);
    pinMode(arrayLedPinDoor[i], OUTPUT);
    pinMode(arrayButtonPin[i], INPUT_PULLUP);
  }
  pinMode(ledPinMove, OUTPUT);
  pinMode(ledPinQueue, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  //// for each level:
  for(int i = firstLevel; i <= lastLevel; i++){
    //// read button state and test if there is a change using debounce
    arrayReading[i] = digitalRead(arrayButtonPin[i]);
    if(arrayReading[i] != arrayLastReading[i]){
      arrayLastDebounceTime[i] = millis();
    }
    
    if((millis() - arrayLastDebounceTime[i]) > debounceDelay){
      if(arrayReading[i] != arrayButtonState[i]){
        //// if yes, update and test if pressed
        arrayButtonState[i] = arrayReading[i];
        if(arrayButtonState[i] == LOW){
          //// if yes, add level in queue if it is different from last in queue or current one in empty queue case
          nod *q = new nod;
          q -> level = i;
          q -> next = NULL;
          if(lastNod != NULL){
            if(lastNod -> level == q -> level){
              delete q;
            }
            else{
              lastNod -> next = q;
              lastNod = q;
            }
          }
          else{
            if(currLevel == q -> level){
              delete q;
            }
            else{
              currNod = q;
              state = HIGH;
              lastNod = q;
              startIntervMove = millis();
              startBlinkTime = millis();
            }
          }
        }
      }
    }
    //// update led state
    if(currLevel == i){
      arrayLedStateLev[i] = HIGH;
    }
    else{
      arrayLedStateLev[i] = LOW;
    }
    digitalWrite(arrayLedPinLev[i], arrayLedStateLev[i]);
    arrayLastReading[i] = arrayReading[i];
  }

  //// if there is something in queue
  if(state == HIGH){
    toLevel = currNod -> level;
    sign = currLevel - toLevel; /// the way the elevator moves
    if(sign == 0);
    else if(sign > 0) sign = -1;
    else sign = 1;
    //// blinking of the "moving" led
    if((millis() - startBlinkTime) > blinkTime){
      startBlinkTime = millis();
      ledStateMove = !ledStateMove;
    }

    /// if the door is open on current level, start closing
    if(arrayDoorState[currLevel] == 0){
      arrayDoorState[currLevel] = 1;
      startDoorTime = millis();
      Serial.println("Door closing on level " + String(currLevel));
      //tone(buzzerPin, buzzerTone, 500);
      if(song1 == LOW){
          startNote = millis();
          currNote = 0;
          int noteDuration = musicNorm / noteDurations2[currNote];
          song2 = HIGH;
          tone(buzzerPin, melody2[currNote], noteDuration);
        }
    }

    /// door opening
    if(arrayDoorState[currLevel] == 1){
      startIntervMove = millis();
      arrayLedStateDoor[currLevel] = map(millis() - startDoorTime, minDoorTime, doorTransitionTime, maxLed, minLed);
      if((millis() - startDoorTime) > doorTransitionTime){
        arrayDoorState[currLevel] = 2;
        Serial.println("Door closed. Going from " + String(currLevel) + " to " + String(toLevel));
        arrayLedStateDoor[currLevel] = LOW;
        moving = HIGH;
      }
    }

    /// when door on current level closed, start moving
    if(((millis() - startIntervMove) > moveTime) || sign == 0){
      currLevel = currLevel + sign; /// when we reach a new level
      if(currLevel == toLevel){ /// check if it is the target level
        ///// if yes, stop moving, start opening door, and update queue
        moving = LOW;
        Serial.println("Reached " + String(currLevel));
        Serial.println("Door opening");
        startDoorTime = millis();
        arrayDoorState[currLevel] = 3;
        //tone(buzzerPin, buzzerTone, 500);
        if(song2 == LOW){
          startNote = millis();
          currNote = 0;
          int noteDuration = musicNorm / noteDurations1[currNote];
          song1 = HIGH;
          tone(buzzerPin, melody1[currNote], noteDuration);
        }
        nod* q = currNod -> next;
        delete currNod;
        currNod = q;
        if(currNod == NULL){
          state = LOW;
          ledStateMove = LOW;
          lastNod = NULL;
        }
        else{
          toLevel = currNod -> level;
          sign = currLevel - toLevel;
          if(sign == 0);
          else if(sign > 0) sign = -1;
          else sign = 1;

        }
      }
      startIntervMove = millis();
    }
  }

  //// door on current level is opening
  if(arrayDoorState[currLevel] == 3){
    startIntervMove = millis();
    arrayLedStateDoor[currLevel] = map(millis() - startDoorTime, minDoorTime, doorTransitionTime, minLed, maxLed);
    if((millis() - startDoorTime) > doorTransitionTime){
        arrayDoorState[currLevel] = 4;
        arrayLedStateDoor[currLevel] = maxLed;
        Serial.println("Door opened. Can't close");
        startDoorTime = millis();
      }
  }

  //// door on current level has just opened, and stays open for an amount of time
  if(arrayDoorState[currLevel] == 4){
    startIntervMove = millis();
    if((millis() - startDoorTime) > stayOpen){
        arrayDoorState[currLevel] = 0;
        Serial.println("Door can close");
      }
  }

  /// update state of "moving" and "busy" leds
  digitalWrite(ledPinMove, ledStateMove && moving);
  digitalWrite(ledPinQueue, state);
  /// for door leds
  for(int i = firstLevel; i <= lastLevel; i++){
    analogWrite(arrayLedPinDoor[i], arrayLedStateDoor[i]);
  }

  //// play song for opening door
  if(song1 == HIGH){
    int noteDuration = musicNorm / noteDurations1[currNote];
    if((millis() - startNote) > noteDuration){
      int pauseBetweenNotes = noteDuration * pauseFactor;
      if((millis() - startNote) > (noteDuration + pauseBetweenNotes)){
        currNote += 1;
        startNote = millis();
        noteDuration = musicNorm / noteDurations1[currNote];
        tone(buzzerPin, melody1[currNote], noteDuration);
        if(currNote == 4){
          song1 = LOW;
          noTone(buzzerPin);
        }
      }
    }
  }
  else if(song2 == HIGH){ /// or for closing door
    int noteDuration = musicNorm / noteDurations2[currNote];
    if((millis() - startNote) > noteDuration){
      int pauseBetweenNotes = noteDuration * pauseFactor;
      if((millis() - startNote) > (noteDuration + pauseBetweenNotes)){
        currNote += 1;
        startNote = millis();
        noteDuration = musicNorm / noteDurations2[currNote];
        tone(buzzerPin, melody2[currNote], noteDuration);
        if(currNote == 4){
          song2 = LOW;
          noTone(buzzerPin);
        }
      }
    }
  }

}
