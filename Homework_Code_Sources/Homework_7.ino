#include "LedControl.h"
/*

  Small game on 8x8 matrix - Shoot walls

Game rules and logic :
  - Setup : Random player starting position and wall placement
  - Player : Blinks slowly, moves depending on joystick input, can't go through walls or bullet
  - Bullet : one at a time on the map, blinks fast, destroys walls on contact, destroyed if it lands on player's position
  - Four Leds : to indicate the direction of the player - the direction that the bullet follows when shot

TO DO:

DOING:

DONE:
  - Setup hardware
  - Matrix logic
  - Joystick logic
  - Implement payer movement + blink
  - Implement bullet being shot + blink + interact with walls and player
  - Fix "Bullet goes through wall if on next position from player" bug - add "&& buttonReading != buttonState" in CheckButton()
  - Fix "Player not moving on same row and column of bullet" bug - (bulletPosX != playerPosX || bulletPosY != playerPosY)
  - Add leds for indicating direction of player
  - Take out player movement out of bounds resulting in going on the other side

*/

/// Matrix control variables
const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;
const byte matrixSize = 8;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
byte matrixBrightness = 2;
byte matrixChanged = HIGH;

/// Joystick variables 
const int xPin = A0;
const int yPin = A1;
const int minThreshold = 200;
const int maxThreshold = 600;

/// Player variables - position, direction of movement, blink, movement
int playerPosX = 0;
int playerPosY = 0;
int playerLastPosX = 0;
int playerLastPosY = 0;
int playerDirectionX = 1;
int playerDirectionY = 0;
int playerLastDirectionX = 1;
int playerLastDirectionY = 0;
const byte moveIntervalPlayer = 200;
unsigned long long lastMovedPlayer = 0;
const short blinkIntevalPlayer = 1000;
unsigned long long lastBlinkPlayer = 0;

const int initNumWalls = 40;
int numWalls = 0;

/// Bullet variables - movement, direction, blinking
int bulletPosX = -1;
int bulletPosY = -1;
int bulletDirectionX = 0;
int bulletDirectionY = 0;
byte bulletShot = LOW;
const int moveIntervalBullet = 1000;
unsigned long long lastMoveBullet = 0;
const int blinkIntervalBullet = 200;
unsigned long long lastBlinkBullet = 0;

/// Button variables - shooting
const byte buttonPin = 3;
byte buttonReading = LOW;
byte buttonLastReading = LOW;
byte buttonState = LOW;
unsigned long long lastDebounceTime = 0;
const byte debounceDelay = 50;

/// Matrix in memory
byte matrix[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0}
};

// const byte ledPinUp = 7;
// const byte ledPinDown = 8;
// const byte ledPinLeft = 9;
// const byte ledPinRight = 6;

/// Pins for direction leds
byte arrayLedPin[] = {7, 8, 9, 6};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  randomSeed(analogRead(A3)); /// increase randomness if nothing connected

  pinMode(buttonPin, INPUT_PULLUP);
  for(int i = 0; i < 4; i++) pinMode(arrayLedPin[i], OUTPUT);

  /// Setup for the game : turn led on for direction, generate walls, setup matrix, generate random position for player
  DirectionLedLight();
  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightness);
  lc.clearDisplay(0);
  playerPosX = random(0, matrixSize);
  playerLastPosX = playerPosX;
  playerPosY = random(0, matrixSize);
  playerLastPosY = playerPosY;
  matrix[playerPosX][playerPosY] = 1;
  while(numWalls != initNumWalls) {
    numWalls += GenerateWalls();
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  if(matrixChanged == HIGH) { /// only refresh matrix if sommething has changed
    UpdateMatrix();
    matrixChanged = LOW;
  }
  CheckPlayerAndBullet(); 
  CheckButton();
}

void DirectionLedLight() { //// Check direction and turn on right led, "magic numbers" for memory 
  int currLed = -1;
  if(playerDirectionX == 1) currLed = 2;
  else if(playerDirectionX == -1) currLed = 3;
  else if(playerDirectionY == 1) currLed = 1;
  else currLed = 0;
  for(int i = 0; i < 4; i++) {
    if(currLed == i) digitalWrite(arrayLedPin[i], 255);
    else digitalWrite(arrayLedPin[i], LOW);
  }
}

int GenerateWalls() { /// function for generating walls - return 1 if a wall was placed, 0 otherwise (there was already something on the position)
  int valueX = random(0, matrixSize);
  int valueY = random(0, matrixSize);
  if(matrix[valueX][valueY] == 1) return 0;
  matrix[valueX][valueY] = 1;
  return 1;
}

void CheckButton() { /// Check if button was pressed - shoot bullet
  buttonReading = digitalRead(buttonPin);
  if(buttonReading != buttonLastReading) lastDebounceTime = millis();
  if(millis() - lastDebounceTime > debounceDelay && buttonReading != buttonState) {
    buttonState = buttonReading;
    if(buttonReading == LOW && bulletShot == LOW) {
        Shoot();
    }
  }
  buttonLastReading = buttonReading;
}

void Shoot() { /// Function to setup button when the button was pressed - take direction of player, check if there is a wall on it or out of bounds
  bulletDirectionX = playerDirectionX;
  bulletDirectionY = playerDirectionY;
  bulletPosX = playerPosX + playerDirectionX;
  bulletPosY = playerPosY + playerDirectionY;
  if(bulletPosX == matrixSize || bulletPosX == -1 || bulletPosY == matrixSize || bulletPosY == -1) return;
  if(matrix[bulletPosX][bulletPosY] == 1) {
    matrix[bulletPosX][bulletPosY] = 0;
    bulletShot = LOW;
    return;
  }
  bulletShot = HIGH;
  lastMoveBullet = millis();
  lastBlinkBullet = millis();
  matrix[bulletPosX][bulletPosY] = 1;
  matrixChanged = HIGH;
}

void UpdateMatrix() { /// function called when there is a change in the matrix to refresh
  for(int row = 0; row < matrixSize; row++) {
    for(int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}

void CheckPlayerAndBullet() { /// every "interval" check or change
  if(bulletShot == HIGH && millis() - lastBlinkBullet > blinkIntervalBullet) { /// blink of bullet if on matrix
    matrix[bulletPosX][bulletPosY] = ! matrix[bulletPosX][bulletPosY];
    lastBlinkBullet = millis();
    matrixChanged = HIGH;
  }

  if(bulletShot == HIGH && (millis() - lastMoveBullet > moveIntervalBullet)) { /// movement of bullet if on matrix
    MoveBullet();
    lastMoveBullet = millis();
    matrixChanged = HIGH;
  }

  if(millis() - lastMovedPlayer > moveIntervalPlayer) { /// movement of player
    CheckMovePlayer();
    lastMovedPlayer = millis();
  }

  if(millis() - lastBlinkPlayer > blinkIntevalPlayer) { /// blink of player
    matrix[playerPosX][playerPosY] = ! matrix[playerPosX][playerPosY];
    lastBlinkPlayer = millis();
    matrixChanged = HIGH;
  }
}

void CheckMovePlayer() { /// Movement of player based on joystick input
  int valueX = analogRead(xPin);
  int valueY = analogRead(yPin);
  playerLastPosX = playerPosX;
  playerLastPosY = playerPosY;
  playerLastDirectionX = playerDirectionX;
  playerLastDirectionY = playerDirectionY;

  if(valueX < minThreshold) {
    if(playerPosX < matrixSize - 1) playerPosX++;
    // else playerPosX = 0;
    playerDirectionX = 1;
    playerDirectionY = 0;
  }
  else if(valueX > maxThreshold) {
    if(playerPosX > 0) playerPosX--;
    // else playerPosX = matrixSize - 1;
    playerDirectionX = -1;
    playerDirectionY = 0;
  }

  else if(valueY > maxThreshold) {
    if(playerPosY < matrixSize - 1) playerPosY++;
    // else playerPosY = 0;
    playerDirectionX = 0;
    playerDirectionY = 1;
  }
  else if(valueY < minThreshold) {
    if(playerPosY > 0) playerPosY--;
    // else playerPosY = matrixSize - 1;
    playerDirectionX = 0;
    playerDirectionY = -1;
  }
 
  if((playerPosX != playerLastPosX || playerPosY != playerLastPosY) && 
    matrix[playerPosX][playerPosY] != 1 && (
      bulletShot == LOW || (bulletPosX != playerPosX || bulletPosY != playerPosY)
    )) { /// can't move on top of wall, bullet
      matrixChanged = true;
      matrix[playerLastPosX][playerLastPosY] = 0;
      matrix[playerPosX][playerPosY] = 1;
      lastBlinkPlayer = millis();
  } 
  else { /// if the movement is illegal, take previous position
    playerPosX = playerLastPosX;
    playerPosY = playerLastPosY;
  }
  if(playerLastDirectionX != playerDirectionX || playerLastDirectionY != playerDirectionY) DirectionLedLight(); /// only update direction leds if the direction has changed
}

void MoveBullet() { /// Bullet movement - test for collision and out of bounds
  bulletPosX += bulletDirectionX;
  bulletPosY += bulletDirectionY;
  matrix[bulletPosX - bulletDirectionX][bulletPosY - bulletDirectionY] = 0;
  
  if(bulletPosX == matrixSize || bulletPosX == -1 || bulletPosY == matrixSize || bulletPosY == -1) {
    bulletShot = LOW;
    return;
  }
  if(bulletPosX == playerPosX && bulletPosY == playerPosY) {
    bulletShot = LOW;
    return;
  }
  if(matrix[bulletPosX][bulletPosY] == 1) {
    matrix[bulletPosX][bulletPosY] = 0;
    bulletShot = LOW;
    return;
  }
  matrix[bulletPosX][bulletPosY] = 1;
}
