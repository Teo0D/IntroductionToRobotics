#include "LedControl.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>
/*

################## Matrix Project ##################

Task List:
  - Matrix Control
  - Lcd Control
  - Menu Logic
  - Game Logic

Checkpoint #1 : LCD Base Functionalities
  - Intro Message + animation
  - Menu :
    - Start game
    - Settings - mandatory :
      - LCD brightness control in EEPROM
      - Matrix brightness control in EEPROM
    - About
  - End Message
  - During gameplay logic

Checkpoint #2 : Game Logic - TBC

Extra to add:
  - Photocell for brightness

TO DO:
  - Game logic function - The homework for #1
  - Buzzer Logic

DOING:
  - During Gameplay LDC Display function - common messages for #1


DONE:
  - Matrix logic - God help us
  - Joystick logic
  - Player attributes
  - Player movement in fog of war
  - LCD logic
  - Menu Messages + Scroll with joystick
  - Menu Logic : Start game
  - Menu Logic : Settings functions
  - Menu Logic : about with automatic scrolling
  - Intro : Message, animation on matrix, song
  - End Game Message + animation + song


*/

/// Matrix control variables
const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;
const byte matrixSize = 8;
const byte virtualMatrixSize = 16;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
byte matrixBrightness = 2;
byte matrixChanged = HIGH;
byte currRow = 0;  /// Upper left corner to display
byte currCol = 0;
const byte numOfBrightLevelsMatrix = 16;

const byte moveIndent = 2;

unsigned short matrix[virtualMatrixSize] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/// Joystick variables
const int xPin = A0;  // col
const int yPin = A1;
const int minThreshold = 200;
const int maxThreshold = 600;

/// Button variables
const byte buttonPin = A5;
byte buttonReading = LOW;
byte buttonLastReading = LOW;
byte buttonState = LOW;
unsigned long long lastDebounceTime = 0;
const byte debounceDelay = 50;

/// Player variables - position, direction of movement, blink, movement
byte playerPosX = 5;
byte playerPosY = 5;
byte playerLastPosX = 5;
byte playerLastPosY = 5;
short playerDirectionX = 1;
short playerDirectionY = 0;
short playerLastDirectionX = 1;
short playerLastDirectionY = 0;
const byte moveCheckIntervalPlayer = 200;
unsigned long long lastMovedJoystick = 0;
const short blinkIntevalPlayer = 1000;
unsigned long long lastBlinkPlayer = 0;
const short moveCheckInterval = 200;
short lives = 3;

/// LCD
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
const byte lcdBrightnessPin = 3;
// const byte minBrightnessLcd = 0;
// const byte maxBrightnessLcd = 255;
const byte numOfBrightLevelsLcd = 6;
const byte lcdBrightnessValues[numOfBrightLevelsLcd] = {
  10, 50, 100, 150, 200, 255
};
byte lcdBrightnessId = 3;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

/// States
byte state = 1;
/*
  - 0 : Intro
  - 1 : Menu
  - 2 : in Game
  - 3 : After Game
  - 4 : In Settings
  - 5 : set lcd brightness
  - 6 : set matrix brightness
  - 7 : about

*/
byte menuState = 0;
const byte numOfMenuStates = 3;
const byte numOfSettings = 3;
const short aboutDelay = 600;
unsigned long long lastAboutDisplay = 0;
short aboutCounter = 0;
String message = "Hi! I am Teo and I made a game! ";

const short animation1Delay = 200;
unsigned long long lastAnimation = 0;
short animationCounter = -1;

/// aux button
const byte auxButtonPin = A4;
byte auxButtonReading = LOW;
byte auxButtonLastReading = LOW;
byte auxButtonState = LOW;
unsigned long long auxLastDebounceTime = 0;

byte scrollBoth[8] = {
  0b00100,
  0b01110,
  0b11111,
  0b00000,
  0b00000,
  0b11111,
  0b01110,
  0b00100
};

byte scrollUp[8] = {
  0b00100,
  0b01110,
  0b11111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte scrollDown[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b01110,
  0b00100
};

byte brightnessBar[8] = {
	0b01110,
	0b01110,
	0b01110,
	0b01110,
	0b01110,
	0b01110,
	0b01110,
	0b01110
};

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  lc.shutdown(0, false);
  matrixBrightness = EEPROM.read(1);
  lc.setIntensity(0, matrixBrightness);
  lc.clearDisplay(0);

  pinMode(buttonPin, INPUT_PULLUP);

  lcd.begin(16, 2);
  pinMode(lcdBrightnessPin, OUTPUT);
  lcdBrightnessId = EEPROM.read(0);
  analogWrite(lcdBrightnessPin, lcdBrightnessValues[lcdBrightnessId]);

  lcd.createChar(0, scrollDown);
  lcd.createChar(1, scrollBoth);
  lcd.createChar(2, scrollUp);
  lcd.createChar(3, brightnessBar);
  lcd.createChar(4, heart);
  InitMessage();
}

void PrintVirtualMatrix() {  //// for debugging purposes
  for (int i = 0; i < virtualMatrixSize; i++) Serial.println(matrix[i]);
  Serial.println("--------");
  Serial.print(currRow);
  Serial.print(" ");
  Serial.print(currCol);
  Serial.print(" ");
  Serial.print(playerPosX);
  Serial.print(" ");
  Serial.println(playerPosY);
  Serial.println("--------");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (matrixChanged == HIGH) {  /// only refresh matrix if sommething has changed
    UpdateMatrix();
    matrixChanged = LOW;
  }
  switch (state) {
    case 0:
      Animation1(1);
      break;
    case 1:
      MenuUpdate(numOfMenuStates);
      break;
    case 4 :
      MenuUpdate(numOfSettings);
      break;
    case 2:
      CheckPlayer();
      break;
    case 3:
      Animation1(3);
      break;
    case 5:
      MenuUpdate(numOfBrightLevelsLcd);
      break;
    case 6: 
      MenuUpdate(numOfBrightLevelsMatrix);
      break;
    case 7:
      DisplayAbout();
      break;
  }
  CheckButton();
}

void Animation1(short afterState) {
  if(millis() - lastAnimation > animation1Delay) {
    if(animationCounter == -1);
    else matrix[animationCounter] = 0;
    matrix[++animationCounter] = (1 << virtualMatrixSize) - 1;
    if(animationCounter == matrixSize) {
      animationCounter = -1;
      state = afterState;
      if(state == 1)
        InitMenu();
    }
    matrixChanged = HIGH;
    lastAnimation = millis();
  }
}

void InitMessage() {
  animationCounter = -1;
  state = 0;
  lcd.setCursor(0, 0);
  lcd.print("Hello there and ");
  lcd.setCursor(0, 1);
  lcd.print("welcome!        ");
}

unsigned short CalcNumAnd() {
  int a = 1;
  a = a << (virtualMatrixSize - currCol);
  a -= 1;
  int b = 1;
  b = b << (virtualMatrixSize - currCol - matrixSize);
  b -= 1;
  a -= b;
  return a;
}

void UpdateMatrix() {  /// function called when there is a change in the matrix to refresh
  unsigned short mask = CalcNumAnd();
  for (int row = 0; row < matrixSize; row++) {
    lc.setRow(0, row, (matrix[row + currRow] & mask) >> (matrixSize - currCol));
  }
  PrintVirtualMatrix();
}

void SetBitValue(byte row, byte col, byte value) {
  byte currBit = (matrix[row + currRow] >> (virtualMatrixSize - col - 1 - currCol)) & 1;
  if (currBit == value)
    ;
  else if (currBit == 0) {
    matrix[row + currRow] = matrix[row + currRow] + (1 << (virtualMatrixSize - 1 - col - currCol));
  } else {
    matrix[row + currRow] = matrix[row + currRow] - (1 << (virtualMatrixSize - 1 - col - currCol));
  }
}

inline byte GetBitValue(byte row, byte col) {
  return (matrix[row + currRow] >> (virtualMatrixSize - 1 - col - currCol)) & 1;
}

void CheckAuxiliaryButton() {
  auxButtonReading = digitalRead(auxButtonPin);
  if (auxButtonReading != auxButtonLastReading) auxLastDebounceTime = millis();
  if (millis() - auxLastDebounceTime > debounceDelay && auxButtonReading != auxButtonState) {
    auxButtonState = auxButtonReading;
    if (auxButtonReading == LOW) {

    }
  }
  auxButtonLastReading = auxButtonReading;
}

void CheckButton() {  /// Check if button was pressed - shoot bullet
  buttonReading = digitalRead(buttonPin);
  if (buttonReading != buttonLastReading) lastDebounceTime = millis();
  if (millis() - lastDebounceTime > debounceDelay && buttonReading != buttonState) {
    buttonState = buttonReading;
    if (buttonReading == LOW  //&& bulletShot == LOW
    ) {
      //Shoot();
      ////////////////////////// Insert Button Logic ////////////////////////
      if(state == 0) {
        state = 1;
        InitMenu();
        ClearMatrix();
      }
      else if(state == 2) /// game logic
      {
        if(lives == 1) { ExitGame(); lives = 3;}
        else {
          lives--;
          DisplayLives();
        }
      }
      else if(state == 3) {
        state = 1;
        InitMenu();
        ClearMatrix();
      }
      else if(state == 1) {
        if(menuState == 0) StartGame();
        else if(menuState == 1) EnterSettings();
        else if(menuState == 2) {
          lcd.setCursor(0, 0);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.clear();
          lcd.setCursor(16, 1);
          lcd.autoscroll();
          aboutCounter = 0;
          DisplayAbout();
        }
      }
      else if(state == 4) {
        if(menuState == 0) LcdBrightnessDisplay();
        else if(menuState == 1) MatrixBrightnessDisplay();
        else if(menuState == 2) InitMenu();
      }
      else if(state == 5) {
        EEPROM.update(0, lcdBrightnessId);
        EnterSettings();
      }
      else if(state == 6) {
        EEPROM.update(1, matrixBrightness);
        ClearMatrix();
        EnterSettings();
      }
      else if(state == 7) {
        lcd.noAutoscroll();
        lcd.clear();
        InitMenu();
      }
    }
  }
  buttonLastReading = buttonReading;
}

byte CheckMove() {
  int valueX = analogRead(xPin);
  int valueY = analogRead(yPin);

  if (valueX < minThreshold) {
    return 1;
  }
  if (valueX > maxThreshold) return 2;
  if (valueY > maxThreshold) return 3;
  if (valueY < minThreshold) return 4;

  return 0;
}

void CheckPlayer() {                                      /// every "interval" check or change
  if (millis() - lastMovedJoystick > moveCheckIntervalPlayer) {  /// movement of player
    CheckMovePlayer(CheckMove());
    lastMovedJoystick = millis();
  }

  if (millis() - lastBlinkPlayer > blinkIntevalPlayer) {  /// blink of player
    byte value = GetBitValue(playerPosX, playerPosY);
    if (value == 0) value = 1;
    else value = 0;
    SetBitValue(playerPosX, playerPosY, value);
    lastBlinkPlayer = millis();
    matrixChanged = HIGH;
  }
}

void CheckMovePlayer(byte move) {  /// Movement of player based on joystick input

  playerLastPosX = playerPosX;
  playerLastPosY = playerPosY;
  playerLastDirectionX = playerDirectionX;
  playerLastDirectionY = playerDirectionY;

  if (move == 1) {
    Serial.println("dreapta");
    if (playerPosX < matrixSize - 1 - moveIndent) playerPosX++;
    else if (currRow == virtualMatrixSize - matrixSize) {
      if (playerPosX != matrixSize - 1) playerPosX++;
    } else {
      SetBitValue(playerLastPosX, playerLastPosY, 0);
      currRow++;
      SetBitValue(playerPosX, playerPosY, 1);
      matrixChanged = true;
    }

    playerDirectionX = 1;
    playerDirectionY = 0;
  } else if (move == 2) {
    Serial.println("stanga");
    if (playerPosX > moveIndent) playerPosX--;
    else if (currRow == 0) {
      if (playerPosX != 0) playerPosX--;
    } else {
      SetBitValue(playerLastPosX, playerLastPosY, 0);
      currRow--;
      SetBitValue(playerPosX, playerPosY, 1);
      matrixChanged = true;
    }
    playerDirectionX = -1;
    playerDirectionY = 0;
  }

  else if (move == 3) {
    Serial.println("jos");
    if (playerPosY < matrixSize - 1 - moveIndent) playerPosY++;
    else if (currCol == virtualMatrixSize - matrixSize) {
      if (playerPosY != matrixSize - 1) playerPosY++;
    } else {
      SetBitValue(playerLastPosX, playerLastPosY, 0);
      currCol++;
      SetBitValue(playerPosX, playerPosY, 1);
      matrixChanged = true;
    }
    playerDirectionX = 0;
    playerDirectionY = 1;
  } else if (move == 4) {
    Serial.println("sus");
    if (playerPosY > moveIndent) playerPosY--;
    else if (currCol == 0) {
      if (playerPosY != 0) playerPosY--;
    } else {
      SetBitValue(playerLastPosX, playerLastPosY, 0);
      currCol--;
      SetBitValue(playerPosX, playerPosY, 1);
      matrixChanged = true;
    }
    Serial.println(playerPosY);
    playerDirectionX = 0;
    playerDirectionY = -1;
  }

  if (((playerPosX != playerLastPosX || playerPosY != playerLastPosY) && GetBitValue(playerPosY, playerPosX) != 1)
      // && (bulletShot == LOW || (bulletPosX != playerPosX || bulletPosY != playerPosY))
  ) {  /// can't move on top of wall, bullet
    matrixChanged = true;
    SetBitValue(playerLastPosX, playerLastPosY, 0);
    SetBitValue(playerPosX, playerPosY, 1);
    lastMovedJoystick = millis();
  } else {  /// if the movement is illegal, take previous position
    playerPosX = playerLastPosX;
    playerPosY = playerLastPosY;
  }
  //if(playerLastDirectionX != playerDirectionX || playerLastDirectionY != playerDirectionY) DirectionLedLight(); /// only update direction leds if the direction has changed
}

void InitMenu() {
  state = 1;
  lcd.setCursor(0, 0);
  lcd.print("Welcome!        ");
  menuState = 0;
  MenuEntriesPrint();
  lastMovedJoystick = millis();
}

void MenuEntriesPrint() {
  lcd.setCursor(0, 1);
  if (menuState == 0) {
    lcd.print("Start game!    ");
  } else if (menuState == 1) {
    lcd.print("Settings       ");
  } else if (menuState == 2) {
    lcd.print("About          ");
  }
  lcd.setCursor(15, 1);
  if (menuState == 0) {
    lcd.write(byte(0));
  } else if (menuState == numOfMenuStates - 1) {
    lcd.write(2);
  } else {
    lcd.write(byte(1));
  }
}

void MenuUpdate(byte maxStates) {
  if (millis() - lastMovedJoystick > moveCheckInterval) { 
      byte move = CheckMove();
      if(move == 0);
      else if(move == 1){
        if(state == 5) {
          if(lcdBrightnessId < maxStates - 1) {
            lcdBrightnessId++;
            LcdBrightnessDisplay();
          }
        }
        else if(state == 6) {
          if(matrixBrightness < maxStates - 2) {
            matrixBrightness += 2;
            MatrixBrightnessDisplay();
          }
        }
      }
      else if(move == 2) {
        if(state == 5) {
          if(lcdBrightnessId > 0) {
            lcdBrightnessId--;
            LcdBrightnessDisplay();
          }
        }
        else if(state == 6) {
          if(matrixBrightness > 1) {
            matrixBrightness -= 2;
            MatrixBrightnessDisplay();
          }
        }
      }
      else {
        if(move == 3 && menuState < maxStates - 1 
        && (state == 1 || state == 4)
        ) {
          menuState += 1;
          if(state == 1) MenuEntriesPrint();
          else if(state == 4) DisplaySettings();
        }
        else if(move == 4 && menuState > 0
        && (state == 1 || state == 4)
        ) {
          menuState -= 1;
          if(state == 1) MenuEntriesPrint();
          else if(state == 4) DisplaySettings();
        }
      }
      lastMovedJoystick = millis();
    }
}

void StartGame() {
  state = 2;
  ClearMatrix();
  SetBitValue(playerPosX, playerPosY, 1);
  UpdateMatrix();
  DisplayStartInfo();
  DisplayLives();
}

void DisplayStartInfo() {
  lcd.setCursor(0, 0);
  lcd.print("Good luck!    ");
}

void DisplayLives() {
  lcd.setCursor(0, 1);
  lcd.write(4);
  lcd.setCursor(1, 1);
  lcd.print(" : ");
  lcd.setCursor(4, 1);
  lcd.print(String(lives) + "            ");
}

void EnterSettings() {
  lcd.setCursor(0, 0);
  lcd.print("Settings        ");
  state = 4;
  menuState = 0;
  DisplaySettings();
  lastMovedJoystick = millis();
}

void DisplaySettings() {
  lcd.setCursor(0, 1);
  if(menuState == 0) lcd.print("LCD brightness   ");
  else if(menuState == 1) lcd.print("Matrix brightns ");
  else if(menuState == 2) lcd.print("Back            ");
  lcd.setCursor(15, 1);
  if (menuState == 0) {
    lcd.write(byte(0));
  } else if (menuState == numOfSettings - 1) {
    lcd.write(2);
  } else {
    lcd.write(byte(1));
  }
}

void DisplayAbout() {
  state = 7;

  if(millis() - lastAboutDisplay > aboutDelay) {
    lcd.print(message[aboutCounter++]);
    if(aboutCounter == 32) aboutCounter = 0;
    lastAboutDisplay = millis();
  }
}

void LcdBrightnessDisplay() {
  state = 5;
  analogWrite(lcdBrightnessPin, lcdBrightnessValues[lcdBrightnessId]);
  lcd.setCursor(0, 0);
  lcd.print("LCD:            ");
  for(int level = 0; level <= 15; level++) {
    lcd.setCursor(level, 1);
    if(level <= lcdBrightnessId) lcd.write(3);
    else lcd.print(" ");
  }
}

void MatrixBrightnessDisplay() {
  state = 6;
  lcd.setCursor(0, 0);
  lcd.print("Matrix:         ");
  lc.setIntensity(0, matrixBrightness);
  for(int level = 0; level <= 15; level++) {
    lcd.setCursor(level, 1);
    if(level*2 <= matrixBrightness) lcd.write(3);
    else lcd.print(" ");
  }
  int maxValue = (1 << virtualMatrixSize) - 1;
  for(int row = 0; row < virtualMatrixSize; row++) matrix[row] = maxValue;
  UpdateMatrix();
}

void ExitGame() {
  lcd.setCursor(0, 0);
  lcd.print("Hope you had fun");
  lcd.setCursor(0, 1);
  lcd.print("Press for menu! ");
  ClearMatrix();
  currRow = 0;
  currCol = 0;
  state = 3;
}



void ClearMatrix() {
  for(short row = 0; row < virtualMatrixSize; row++) matrix[row] = 0;
  matrixChanged = HIGH;
}
