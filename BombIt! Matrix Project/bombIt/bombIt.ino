#include <LiquidCrystal.h>
#include <LedControl.h>
#include <string.h>
#include <EEPROM.h>
////////////////////////////////////////////////////////    VARIABLES   /////////////////////////////////////////////////////////////////////////////////
enum State {
  INTRO,
  MAIN_MENU,
  IN_GAME,
  IN_SETTINGS,
  IN_ABOUT,
  IN_LCD_BRIGHTNESS,
  IN_MATRIX_BRIGHTNESS,
  IN_HOW_TO_PLAY,
  IN_ENTER_NAME,
  IN_SAVE_NAME,
  IN_SOUND,
  IN_HIGHSCORE,
  IN_GAME_END,
  IN_RESET,
  IN_GAME_RESULTS,
  IN_TRY_AGAIN
};

State currentState = INTRO;

// Pins
const int xPin = A0;
const int yPin = A1;
const int buzzerPin = 3;
const int swPin = 2;
const int rs = 9;
const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 13;
const int en = 8;
const int d4 = 7;
const int d5 = 6;
const int d6 = 5;
const int d7 = 4;
const int lcdAPin = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int lcdBrightnessAddress = 0;
const int matrixBrightnessAddress = 1;
const int soundModeAddress = 2;
int lastLCDBrightnessVal = -1;
int lastMatrixBrightnessVal = -1;
int matrixBrightnessVal;
int lcdBrightnessVal;

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
const int matrixSize = 8;
int matrixStart = 0;
int matrixEnd = 7;
const int bigMatrixSize = 16;
const int bigMatrixEnd = 15;
int rowStart = 0;
int rowEnd = 7;
int colStart = 0;
int colEnd = 7;

byte matrix[2 * matrixSize][2 * matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0},
  {0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0},
  {0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0},
  {0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0},
  {0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0},
  {0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0},
  {0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0},
  {0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0},
  {0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

 byte sadFace[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

 byte happyFace[matrixSize][matrixSize]  = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {1, 0, 1, 0, 0, 1, 0, 1},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};


const byte hiMessage[matrixSize][matrixSize] PROGMEM = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 1, 0, 0, 1, 0, 1},
  {1, 0, 0, 1, 0, 1, 0, 1},
  {1, 0, 0, 1, 0, 1, 0, 0},
  {1, 0, 0, 1, 0, 1, 0, 1},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

const byte settingsAnimation[matrixSize][matrixSize] PROGMEM = {
  {0, 0, 0, 0, 1, 1, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 1},
  {0, 0, 0, 1, 1, 1, 1, 1},
  {0, 0, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 0, 0, 0, 0},
  {1, 1, 1, 0, 0, 0, 0, 0},
  {1, 1, 0, 0, 0, 0, 0, 0}
};

const byte questionMark[matrixSize][matrixSize] PROGMEM = {
  {0, 0, 1, 1, 1, 0, 0, 0},
  {0, 1, 0, 0, 0, 1, 0, 0},
  {0, 1, 0, 0, 0, 1, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 1, 0, 0, 0, 0},
  {0, 0, 0, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 0, 0, 0, 0}
};

const byte menuAnimation[matrixSize][matrixSize] PROGMEM = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 1, 1, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 1, 1, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 1, 1, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

const byte bombAnimation[matrixSize][matrixSize] PROGMEM = {
  {0, 0, 0, 0, 0, 1, 0, 0},
  {0, 0, 0, 0, 1, 1, 1, 0},
  {0, 0, 0, 1, 0, 1, 0, 0},
  {0, 0, 1, 1, 1, 0, 0, 0},
  {0, 1, 0, 0, 0, 1, 0, 0},
  {0, 1, 0, 0, 0, 1, 0, 0},
  {0, 1, 0, 0, 0, 1, 0, 0},
  {0, 0, 1, 1, 1, 0, 0, 0}
};

const byte cup[matrixSize][matrixSize] PROGMEM = {
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0}
};

const int maxBombs = 3;  
const int bombsStart = 0;

struct Bomb {
  int row;
  int col;
  unsigned long startTime;
  bool active;
};

Bomb bombs[maxBombs];
unsigned long bombBlinkDuration = 3000;  
unsigned long bombExplosionDuration = 1000;  

int rowCurrentPos;
int colCurrentPos;
int rowLastPos;
int colLastPos;

bool gameLost = false;
bool collision = false;
bool inGameplay = false;
bool gameWon = false;

int xValue = 0;
int yValue = 0;
bool joyMoved = false;
int minThreshold = 350;
int maxThreshold = 650;
unsigned long lastDebounceTime = 0;
const int debounceDelay = 50;
byte swState = HIGH;
byte lastSwState = HIGH;
unsigned long lastMoveTime = 0;
unsigned long moveDelay = 200;

unsigned long blinkStartTime = 0;
unsigned long blinkInterval = 500;  

const int UP = 0;
const int DOWN = 1;
const int LEFT = 2;
const int RIGHT = 3;

const int ledOn = 1;
const int ledOff = 0;
const int stopVal = 0;
const int runningVal = 1;
const int addr = 0;
const int baud = 9600;
const int initialVal = 0;
const int moveOnePos = 1;

byte clover[] = {
  B00000,
  B01010,
  B11011,
  B00100,
  B11011,
  B01010,
  B00000,
  B00000
};

byte arrowRight[] = {
  B00000,
  B10000,
  B01000,
  B00100,
  B00010,
  B00100,
  B01000,
  B10000
};

byte arrowLeft[] = {
  B00000,
  B00010,
  B00100,
  B01000,
  B10000,
  B01000,
  B00100,
  B00010
};

byte arrowUp[] = {
  B00000,
  B00000,
  B00100,
  B01010,
  B10001,
  B00000,
  B00000,
  B00000
};

byte arrowDown[] = {
  B00000,
  B00000,
  B10001,
  B01010,
  B00100,
  B00000,
  B00000,
  B00000
};

byte fullChar[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

const String alphabet[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
String playerName = "";  
int currentLetterIndex = 0;
int lastLetterIndex = -1;
int letterPosition = 0;
String firstLetter;
String secondLetter;
String thirdLetter;
int currentPositionName = 0;
int lcdPositionName = -1;
int currentPosition = 0;
int lcdPosition = -1;
int currentPositionSettings = 0;
int lcdPositionSettings = -1;
unsigned long startMillis;
const int zero = 0;
const int one = 1;
const int two = 2;
const int three = 3;
const int four = 4;
const int five = 5;
const int six = 6;
const int matrixMax = 10;
const int alphabetSize = 25;
const int stepLCD = 50;
const int lcdMax = 250;
const int lcdMin = 50;
const int millisToSeconds = 1000;
const int hundred = 100;
const int resetVal = 0;
const int positionZero = 0;
const int firstPos = 1;
const int secondPos = 2;
const int thirdPos = 3;
const int fourthPos = 4;
const int lastPosLCD = 15;
const int twelve = 12;

const int timeBonusWeight = 50;
const int bombBonusWeight = 20;
float playerScore;
const int baseScore = 0;
int bombCounter = 0;
int gameStartTime;
int recordedTime;
int soundVal;
int lastSoundVal = -1;
unsigned long lastMillisAbout = 0;
const int lcdWidth = 16; 
const int lcdHeight = 2;
int index = 0;
const int intervalAbout = 350;
unsigned long lastMillisHowToPlay = 0;
const int intervalHowToPlay = 350;
bool inGame = false;
bool startGame = false;

struct Player {
  float score = baseScore;
  char name[four] = "AAA";
};
const int numPlayers = 5;
Player players[numPlayers];
bool newHighscore = false;
int currentHighscorePosition = 0;
int lastHighscorePosition = -1;
bool playerAdded = false;
int lcdTryAgain = -1;
int tryAgainPosition = 0;

const int interval2s = 2000;
const int freq2000 = 2000;
const int freq1000 = 1000;
const int freq1500 = 1500;
const int interval300 = 300;
const int interval100 = 100;

////////////////////////////////////////////////////////    VARIABLES   /////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////    MAIN   /////////////////////////////////////////////////////////////////////////////////
void setup() {
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(swPin, INPUT_PULLUP);
  pinMode(lcdAPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  lcd.begin(lcdWidth, lcdHeight);
  lcd.createChar(one, clover);
  lcd.createChar(secondPos, arrowRight);
  lcd.createChar(three, arrowUp);
  lcd.createChar(four, arrowDown);
  lcd.createChar(five, arrowLeft);
  lcd.createChar(six, fullChar);
  startMillis = millis();
  Serial.begin(baud);
  matrixBrightnessVal = EEPROM.read(matrixBrightnessAddress);
  lcdBrightnessVal = EEPROM.read(lcdBrightnessAddress);
  soundVal = EEPROM.read(soundModeAddress);
  lc.shutdown(addr, false);
  lc.setIntensity(addr, matrixBrightnessVal);
  analogWrite(lcdAPin, lcdBrightnessVal);
}

void loop() {
  moveJoystick();
  if(!inGame){
    currentMenuState();
  } else {
    gameplay();
    inGameLCD();
  }
}

////////////////////////////////////////////////////////    MAIN   /////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////    GAME FUNCTIONS  /////////////////////////////////////////////////////////////////////////////////

void playExplosionSound() {
  
  tone(buzzerPin, freq2000, interval300);  
}

void playMoveSound() {
  tone(buzzerPin, freq1000, interval100);  
}

bool playerActive = false;
void gameplay(){
  blinkPlayer();
  for (int i = bombsStart; i < maxBombs; i++) {
    if (bombs[i].active) {
      if (millis() - bombs[i].startTime <= bombBlinkDuration) {
        matrix[bombs[i].row][bombs[i].col] = !matrix[bombs[i].row][bombs[i].col];
        updateRoom(colStart, colEnd, rowStart, rowEnd);
      } else if (millis() - bombs[i].startTime <= bombBlinkDuration + bombExplosionDuration) { 
        explodeBomb(i);
      } else {
        if (!gameLost) {
          clearBomb(i);
        } else {
          gameOver();
        }
      }
    }
  }

  if (collision) {
    gameOver();
    gameLost = true;
  }
  checkGameWon();
}

void blinkPlayer(){
 if(playerActive){
  if (millis() - blinkStartTime >= blinkInterval) {
    matrix[rowLastPos][colLastPos] = !matrix[rowLastPos][colLastPos];
    updateRoom(colStart, colEnd, rowStart, rowEnd);
    blinkStartTime = millis();
  }
 }
}
void moveJoystick(){
  if(inGame && playerActive){
    int reading = digitalRead(swPin);
  
  if (reading != lastSwState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != swState) {
      swState = reading;
      if (swState == LOW) {
        Serial.println(F("Button Pressed"));
        bombCounter++;
        placeBomb();
        Serial.println(bombCounter);
      }
    }
  }

  lastSwState = reading;

  xValue = analogRead(xPin);
  yValue = analogRead(yPin);

  if (xValue < minThreshold && joyMoved == false) {
    if (millis() - lastMoveTime >= moveDelay) {
      playerMove(DOWN);
      joyMoved = true;
      lastMoveTime = millis();
      Serial.println(F("Moved down"));
    }
  }

  if (xValue > maxThreshold && joyMoved == false) {
    if (millis() - lastMoveTime >= moveDelay) {
      playerMove(UP);
      joyMoved = true;
      lastMoveTime = millis();
      Serial.println(F("Moved up"));
    }
  }

  if (yValue < minThreshold && joyMoved == false) {
    if (millis() - lastMoveTime >= moveDelay) {
      playerMove(LEFT);
      joyMoved = true;
      lastMoveTime = millis();
      Serial.println(F("Moved left"));
    }
  }

  if (yValue > maxThreshold && joyMoved == false) {
    if (millis() - lastMoveTime >= moveDelay) {
      playerMove(RIGHT);
      joyMoved = true;
      lastMoveTime = millis();
      Serial.println(F("Moved right"));
    }
  }

  if (xValue >= minThreshold && yValue >= minThreshold && yValue <= maxThreshold) {
    joyMoved = false;
  }

  } else {
    moveOnLCD();
  }
}

void updateRoom(int startCol, int endCol, int startRow, int endRow) {
  for (int row = matrixStart; row < matrixSize; row++) {
    for (int col = matrixStart; col < matrixSize; col++) {
      lc.setLed(addr, row, col, matrix[row + startRow][col + startCol]);
    }
  }
}

void clearMatrix() {
  for (int row = matrixStart; row < matrixSize; row++) {
    for (int col = matrixStart; col < matrixSize; col++) {
      matrix[row][col] = ledOff;
      lc.setLed(ledOff, row, col, matrix[row][col]);
    }
  }
}

void playerMove(int direction) {
  matrix[rowLastPos][colLastPos] = ledOff;
  updateRoom(colStart, colEnd, rowStart, rowEnd);
  if(soundVal){
    playMoveSound();
  }
  switch (direction) {
    case UP:
      upMovement();
      break;
    case DOWN:
      downMovement();
      break;
    case LEFT:
      leftMovement();
      break;
    case RIGHT:
      rightMovement();
      break;
  }
  if (matrix[rowCurrentPos][colCurrentPos] == ledOn) { 
    collision = true;
  } else {
    matrix[rowCurrentPos][colCurrentPos] = ledOn;
    updateRoom(colStart, colEnd, rowStart, rowEnd);
    rowLastPos = rowCurrentPos;
    colLastPos = colCurrentPos;
    blinkStartTime = millis();
  }
}

void upMovement() {
  if (rowLastPos == matrixSize && colLastPos < matrixSize) { // room three to room one
    rowEnd = matrixEnd;
    rowStart = matrixStart;
    colStart = matrixStart;
    colEnd = matrixEnd;
    rowCurrentPos = rowEnd;
    colCurrentPos = colLastPos;
    } else if (rowLastPos == matrixSize && colLastPos > matrixEnd){ // room four to room two
      rowEnd = matrixEnd;
      rowStart = matrixStart;
      colStart = matrixSize;
      colEnd = bigMatrixEnd;
      rowCurrentPos = rowEnd;
      colCurrentPos = colLastPos;
    }
      else if(rowLastPos == matrixStart){
        rowCurrentPos = matrixStart;
        colCurrentPos = colLastPos;
      } 
      else {
      rowCurrentPos = rowLastPos - moveOnePos;
      colCurrentPos = colLastPos;
  }
  updateRoom(colStart, colEnd, rowStart, rowEnd);
}

void downMovement() {
  if (rowLastPos == matrixEnd && colLastPos < matrixSize) { // room one to room three
    rowStart = matrixSize;
    rowEnd = bigMatrixEnd;
    colStart = matrixStart;
    colEnd = matrixEnd;
    rowCurrentPos = rowStart;
    colCurrentPos = colLastPos;
  } else if (rowLastPos == matrixEnd && colLastPos > matrixEnd) { // room two to room four
    rowStart = matrixSize;
    rowEnd = bigMatrixEnd;
    colStart = matrixSize;
    colEnd = bigMatrixEnd;
    rowCurrentPos = rowStart;
    colCurrentPos = colLastPos;
  }
  else if(rowLastPos == bigMatrixEnd){
        rowCurrentPos = bigMatrixEnd;
        colCurrentPos = colLastPos;
  } else {
    rowCurrentPos = rowLastPos + moveOnePos;
    colCurrentPos = colLastPos;
  }
  updateRoom(colStart, colEnd, rowStart, rowEnd);
}

void leftMovement() {
  if (colLastPos == matrixSize && rowLastPos < matrixSize) { // room two to room one
    rowStart = matrixStart;
    rowEnd = matrixEnd;
    colStart = matrixStart;
    colEnd = matrixEnd;
    colCurrentPos = colEnd;
    rowCurrentPos = rowLastPos;
  } else if (colLastPos == matrixSize && rowLastPos > matrixEnd) { // room four to room three
    rowStart = matrixSize;
    rowEnd = bigMatrixEnd;
    colStart = matrixStart;
    colEnd = matrixEnd;
    colCurrentPos = colEnd;
    rowCurrentPos = rowLastPos;
  } else 
    if(colLastPos == matrixStart){
      rowCurrentPos = rowLastPos;
      colLastPos = matrixStart;
    }
   else {
    rowCurrentPos = rowLastPos;
    colCurrentPos = colLastPos - moveOnePos;
  }
  updateRoom(colStart, colEnd, rowStart, rowEnd);
}

void rightMovement() {
  if (colLastPos == matrixEnd && rowLastPos < matrixSize) { // room one to room two
    rowStart = matrixStart;
    rowEnd = matrixEnd;
    colStart = matrixSize;
    colEnd = bigMatrixEnd;
    rowCurrentPos = rowLastPos;
    colCurrentPos = colStart;
  } else if (colLastPos == matrixEnd && rowLastPos > matrixEnd){ // room three to room four
    rowStart = matrixSize;
    rowEnd = bigMatrixEnd;
    colStart = matrixSize;
    colEnd = bigMatrixEnd;
    rowCurrentPos = rowLastPos;
    colCurrentPos = colStart;
  } 
  else if(colLastPos == bigMatrixEnd){
    colCurrentPos = bigMatrixEnd;
    rowCurrentPos = rowLastPos;
  } else {
    rowCurrentPos = rowLastPos;
    colCurrentPos = colLastPos + moveOnePos;
  }
}

void placeBomb() {
  for (int i = bombsStart; i < maxBombs; i++) {
    if (!bombs[i].active) {
      bombs[i].row = rowCurrentPos;
      bombs[i].col = colCurrentPos;
      bombs[i].startTime = millis();
      bombs[i].active = true;
      matrix[bombs[i].row][bombs[i].col] = ledOn;
      updateRoom(colStart, colEnd, rowStart, rowEnd);
      break; 
    }
  }
}

void explodeBomb(int bombIndex) {
  if(playerActive){
  for (int i = matrixStart; i < bigMatrixSize; i++) {
    matrix[bombs[bombIndex].row][i] = ledOn;  
    matrix[i][bombs[bombIndex].col] = ledOn;  
    if (rowCurrentPos == bombs[bombIndex].row || colCurrentPos == bombs[bombIndex].col) {
      gameLost = true;
    }
  }
  updateRoom(colStart, colEnd, rowStart, rowEnd);
  if(soundVal){
    playExplosionSound();
  }
  }
}

void clearBomb(int bombIndex) {
  matrix[bombs[bombIndex].row][bombs[bombIndex].col] = ledOff;
  for (int i = matrixStart; i < bigMatrixSize; i++) {
    matrix[bombs[bombIndex].row][i] = ledOff;
    matrix[i][bombs[bombIndex].col] = ledOff;
  }
  updateRoom(colStart, colEnd, rowStart, rowEnd);
  bombs[bombIndex].active = false;
}

void gameOver() {
  playerActive = false;
  int stop = runningVal;
  for (int row = matrixStart; row < matrixSize; row++) {
    for (int col = matrixStart; col < matrixSize; col++) {
      lc.setLed(addr, row, col, sadFace[row][col]);
    }
  }
  currentState = IN_GAME_END;
  inGame = false;
  recordedTime = millis()/millisToSeconds - gameStartTime/millisToSeconds;
  playerScore = baseScore;
}

void checkGameWon() {
  int counter = initialVal;
  for (int row = matrixStart; row < bigMatrixSize; row++) {
    for (int col = matrixStart; col < bigMatrixSize; col++) {
      if (matrix[row][col] == ledOff) {
        counter++;
      }
    }
  }
  if (counter == (bigMatrixSize * bigMatrixSize)) {
    gameWin();
    gameStartTime = resetVal;
  }
}

void gameWin() {
  int stop = runningVal;
  for (int row = matrixStart; row < matrixSize; row++) {
    for (int col = matrixStart; col < matrixSize; col++) {
      lc.setLed(addr, row, col, happyFace[row][col]);
    }
  }
  currentState = IN_GAME_END;
  inGame = false;
  gameWon = true;
  recordedTime = millis()/millisToSeconds - gameStartTime/millisToSeconds;
  playerScore = calculateScore(recordedTime, bombCounter);
  gameStartTime = resetVal;
}

void generateMatrix() {
  randomSeed(analogRead(zero));
  for (int i = matrixStart + moveOnePos; i < bigMatrixSize; i++) {
    for (int j = matrixStart + moveOnePos; j < bigMatrixSize; j++) {
      matrix[i][j] = random(two);  
    }
  }
}

float calculateScore(unsigned long elapsedMillis, int bombs) {
  float timeBonus = timeBonusWeight * (one - static_cast<float>(elapsedMillis) / (elapsedMillis + one));
  float bombBonus = bombBonusWeight * (one - static_cast<float>(bombs) / (bombs + one));
  float totalScore = (baseScore + timeBonus + bombBonus) * hundred;
  gameStartTime = resetVal;
  return totalScore;
}
////////////////////////////////////////////////////////    GAME FUNCTIONS  /////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////    MENU FUNCTIONS  /////////////////////////////////////////////////////////////////////////////////
void playMenuSound() {
  tone(buzzerPin, freq1500, interval100);
}

void currentMenuState(){
  Serial.print("Current State: ");
   Serial.println(currentState);
    switch (currentState) {
    case INTRO: {
      introMessage();
      printAnimation(hiMessage);
      if (millis() - startMillis >= interval2s) {
        currentState = MAIN_MENU;
      }
      break;
    }
    case MAIN_MENU: {
        if (currentPosition != lcdPosition) {
          lcdPosition = currentPosition;
          mainMenu();
          printAnimation(menuAnimation);
        }
      break;
    }
    case IN_GAME: {
      rowCurrentPos = matrixStart;
      colCurrentPos = matrixStart;
      rowLastPos = matrixStart;
      colLastPos = matrixStart;
      rowStart = matrixStart;
      rowEnd = matrixEnd;
      colStart = matrixStart;
      colEnd = matrixEnd;
      inGame = true;
      playerActive = true;
      gameStartTime = resetVal;
      gameStartTime = millis();
      collision = false;
      break;
    }
    case IN_SETTINGS: {
      if (currentPositionSettings != lcdPositionSettings) {
        lcdPositionSettings = currentPositionSettings;
        settingsMenu();
        printAnimation(settingsAnimation);
      }
      break;
    }
    case IN_ABOUT:{
      about();
      printAnimation(bombAnimation);
      break;
    }
    case IN_LCD_BRIGHTNESS: {
      if (lastLCDBrightnessVal != lcdBrightnessVal) {
        lastLCDBrightnessVal = lcdBrightnessVal;
        LCDBrightness();
      }
      break;
    }
    case IN_MATRIX_BRIGHTNESS: {
      if (lastMatrixBrightnessVal != matrixBrightnessVal) {
        lastMatrixBrightnessVal = matrixBrightnessVal;
        matrixBrightness();
      }
      break;
    }
    case IN_HOW_TO_PLAY: {
      howToPlay();
      printAnimation(questionMark);
      break;
    }
    case IN_ENTER_NAME: {
      if (lastLetterIndex != currentLetterIndex) {
        lastLetterIndex = currentLetterIndex;
        enterName();
      }
      break;
    }
    case IN_SAVE_NAME: {
      if (currentPositionName != lcdPositionName) {
        lcdPositionName = currentPositionName;
        saveName();
      }
      break;
    }
    case IN_GAME_END: {
      rowCurrentPos = matrixStart;
      colCurrentPos = matrixStart;
      rowLastPos = matrixStart;
      colLastPos = matrixStart;
      rowStart = matrixStart;
      rowEnd = matrixEnd;
      colStart = matrixStart;
      colEnd = matrixEnd;
      gameStartTime = matrixStart;
      playerActive = true;
      if(gameWon){
        Player newPlayer;
        playerName.toCharArray(newPlayer.name, four);
        newPlayer.score = playerScore;
        currentState = IN_ENTER_NAME;
      }
      if(gameLost){
        currentState = IN_TRY_AGAIN;
      }
      break;
     }
     case IN_TRY_AGAIN: {
      generateMatrix();
        gameStartTime = resetVal;
      if (lcdTryAgain != tryAgainPosition) {
        lcdTryAgain = tryAgainPosition;
        tryAgain();
      }
      break;
    }
    case IN_GAME_RESULTS: {
      rowStart = matrixStart;
      rowEnd = matrixEnd;
      colStart = matrixStart;
      colEnd = matrixEnd;
      rowCurrentPos = matrixStart;
      colCurrentPos = matrixStart;
      rowLastPos = matrixStart;
      colLastPos = matrixStart;
      Player newPlayer;
      gameStartTime = resetVal;
      playerName.toCharArray(newPlayer.name, four);
      newPlayer.score = playerScore;
      if(!playerAdded){
          addPlayer(newPlayer);
       }
      if(!newHighscore){
        printGameResults();
      } else {
        lcd.clear();
        lcd.setCursor(positionZero, positionZero);
        lcd.print("New highscore!");
        lcd.setCursor(positionZero, firstPos);
        lcd.print("Main menu: ");
        lcd.write(three);
        }
      break;
    }
    case IN_HIGHSCORE: {
      Serial.println("starea e IN_HIGHSCORE");
      if (currentHighscorePosition != lastHighscorePosition) {
        lastHighscorePosition = currentHighscorePosition;
        Serial.println("Calling highscore()");
        highscore();
        printAnimation(cup);
      }
      break;
      }
      case IN_RESET: {
        resetHighscore();
        break;
      }
      case IN_SOUND: {
        if (soundVal != lastSoundVal) {
          lastSoundVal = soundVal;
          sound();
      }
      break;
      }
  }

}

void printGameResults(){
  lcd.clear();
  lcd.setCursor(positionZero, positionZero);
  lcd.print(playerName);
  lcd.print("'s score: ");
  lcd.print(playerScore);
  lcd.print(F("!"));
  lcd.setCursor(positionZero, firstPos);
  lcd.print("Main menu: ");
  lcd.write(three);
  gameStartTime = resetVal;
}

void inGameLCD(){
  lcd.clear();
  lcd.setCursor(positionZero, positionZero);
  lcd.print("Bombs used: ");
  lcd.print(bombCounter);
  lcd.setCursor(positionZero, firstPos);
  lcd.print("Time: ");
  lcd.print((millis() - gameStartTime)/millisToSeconds);
}

void introMessage() {
  lcd.setCursor(fourthPos, positionZero);
  lcd.print(F("Welcome!"));
  lcd.setCursor(secondPos, firstPos);
  lcd.print(F("Good luck!"));
  lcd.write(one);
  lcd.write(one);
}

void printAnimation(const byte animation[matrixSize][matrixSize]) {
  for (int row = matrixStart; row < matrixSize; row++) {
    for (int col = matrixStart; col < matrixSize; col++) {
      // Read the byte from PROGMEM 
      byte pixelValue = pgm_read_byte(&(animation[row][col]));
      lc.setLed(addr, row, col, pixelValue);
    }
  }
}

void moveOnLCD() {
  int reading = digitalRead(swPin);
  if (reading != lastSwState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != swState) {
      swState = reading;
      if (swState == LOW) {
        if(soundVal){
          playMenuSound();
        }
        if (currentState == MAIN_MENU) {
          if (currentPosition == thirdPos) {
            currentState = IN_ABOUT;
          } else if (currentPosition == fourthPos) {
            currentState = IN_HOW_TO_PLAY;
          } else if (currentPosition == secondPos) {
            currentState = IN_SETTINGS;
            lcdPositionSettings = -firstPos;
            currentPositionSettings = positionZero;
          } else if (currentPosition == positionZero) {
             currentState = IN_GAME;
           } else if (currentPosition == firstPos) {
             currentState = IN_HIGHSCORE;
           } 
        } else if (currentState == IN_SETTINGS) {
          if (currentPositionSettings == secondPos) {
            currentState = IN_MATRIX_BRIGHTNESS;
          } else if (currentPositionSettings == firstPos) {
            currentState = IN_LCD_BRIGHTNESS;
          } else if (currentPositionSettings == fourthPos) {
            currentState = MAIN_MENU;
            lcdPosition = -firstPos;
            currentPosition = positionZero;
          } else if (currentPositionSettings == positionZero) {
            currentState = IN_RESET;
            letterPosition = positionZero;
          } else if (currentPositionSettings == thirdPos) {
            currentState = IN_SOUND;
            lastSoundVal = -firstPos;
          }
        }   else if(currentState == IN_ENTER_NAME){
            playerName += alphabet[currentLetterIndex];
            currentLetterIndex = positionZero;
            letterPosition++; 
            if (letterPosition == thirdPos) {
              currentState = IN_SAVE_NAME;
              letterPosition = positionZero; 
            }
        } else if (currentState == IN_SAVE_NAME){
          if (lcdPositionName == positionZero){
            currentState = IN_GAME_RESULTS;  
            lcdPositionName = -firstPos;
          } else if (lcdPositionName == firstPos){
            currentState = IN_ENTER_NAME;
            playerName = "";
            lcdPositionName = -firstPos;
            gameStartTime = positionZero;
          }
        }
          else if (currentState == IN_TRY_AGAIN){
            generateMatrix();
            if (tryAgainPosition == positionZero){
              currentState = MAIN_MENU;  
              lcdTryAgain = -firstPos;
              lcdPosition = -firstPos;
            } else if (tryAgainPosition == firstPos){
              currentState = IN_GAME;
              gameStartTime = resetVal;
              collision = false;
              gameStartTime = millis();
              lcdTryAgain = -firstPos;
              inGame = true;
              playerActive = true;
              gameWon = false;
              gameLost = false;
              bombCounter = resetVal;
            }
          }
        }
      }
    }
  

  lastSwState = reading;

  xValue = analogRead(xPin);
  yValue = analogRead(yPin);

  if (xValue < minThreshold && joyMoved == false) {
    if (millis() - lastMoveTime >= moveDelay) {
      if (currentState == MAIN_MENU) {
        if (currentPosition + firstPos == five) {
          currentPosition = fourthPos;
        } else {
          currentPosition++;
        }
      }
      if (currentState == IN_SETTINGS) {
        if (currentPositionSettings + firstPos == five) {
          currentPositionSettings = fourthPos;
        } else {
          currentPositionSettings++;
        }
      } else
      if(currentState == IN_ENTER_NAME){
          if(currentLetterIndex + firstPos == alphabetSize + one){
            currentLetterIndex = resetVal;
          } else {
            currentLetterIndex++;
          }
      }
      if(currentState == IN_SAVE_NAME){
        if(currentPositionName + firstPos > firstPos){
          currentPositionName = firstPos;
        } else {
          currentPositionName++;
        }
      }
      if(currentState == IN_TRY_AGAIN){
        if(tryAgainPosition + firstPos > firstPos){
          tryAgainPosition = firstPos;
        } else {
          tryAgainPosition++;
        }
      }
      joyMoved = true;
      lastMoveTime = millis();
      Serial.println(F("Moved down"));
      if(soundVal){
        playMoveSound();
      }
    }
  }

  if (xValue > maxThreshold && joyMoved == false) {
    if (millis() - lastMoveTime >= moveDelay) {
      if (currentState == MAIN_MENU) {
        if (currentPosition - firstPos < positionZero) {
          currentPosition = positionZero;
        } else {
          currentPosition--;
        }
      }
      if (currentState == IN_SETTINGS) {
        if (currentPositionSettings - firstPos < positionZero) {
          currentPositionSettings = positionZero;
        } else {
          currentPositionSettings--;
        }
      }
      if (currentState == IN_ABOUT) {
        Serial.println(F("Going back"));
        currentState = MAIN_MENU;
        lcdPosition = -firstPos;
        currentPosition = thirdPos;
        index = resetVal;
      }
      if (currentState == IN_RESET) {
        Serial.println(F("Going back"));
        currentState = IN_SETTINGS;
        currentPositionSettings = positionZero;
        lcdPositionSettings = -firstPos;
      }
      if (currentState == IN_HOW_TO_PLAY) {
        Serial.println(F("Going back"));
        currentState = MAIN_MENU;
        lcdPosition = -firstPos;
        currentPosition = thirdPos;
        index = resetVal;
      }
      if (currentState == IN_LCD_BRIGHTNESS) {
        Serial.println(F("Going back"));
        EEPROM.update(lcdBrightnessAddress, lcdBrightnessVal);
        currentState = IN_SETTINGS;
        lastLCDBrightnessVal = -firstPos;
        lcdPositionSettings = -firstPos;
        currentPositionSettings = firstPos;
      }
      if (currentState == IN_MATRIX_BRIGHTNESS) {
        Serial.println(F("Going back"));
        EEPROM.update(matrixBrightnessAddress, matrixBrightnessVal);
        currentState = IN_SETTINGS;
        lastMatrixBrightnessVal = -firstPos;
        lcdPositionSettings = -firstPos;
        currentPositionSettings = secondPos;
      }
      if (currentState == IN_HIGHSCORE) {
        Serial.println(F("Going back"));
        currentState = MAIN_MENU;
        lastHighscorePosition = -firstPos;
        lcdPosition = -firstPos;
        currentPosition = firstPos;
      }
      if (currentState == IN_SOUND) {
        Serial.println(F("Going back"));
        currentState = IN_SETTINGS;
        currentPositionSettings = thirdPos;
        lastSoundVal = -firstPos;
        EEPROM.update(soundModeAddress, soundVal);
      }
      if (currentState == IN_GAME_RESULTS) {
        Serial.println(F("Going back"));
        currentState = MAIN_MENU;
        playerName = "";
        playerScore = baseScore;
        gameWon = false;
        gameLost = false;
        inGame = false;
        bombCounter = resetVal;
        playerAdded = false;
        lcdPosition = -firstPos;
        currentPosition = positionZero;
        gameStartTime = resetVal;
        generateMatrix();
      }
      if(currentState == IN_ENTER_NAME){
        if(currentLetterIndex - firstPos < positionZero){
          currentLetterIndex = alphabetSize;
        } else {
          currentLetterIndex--;
        }
      }
      if(currentState == IN_SAVE_NAME){
        if(currentPositionName - firstPos < positionZero){
          currentPositionName = positionZero;
        } else {
          currentPositionName--;
        }
      }
      if(currentState == IN_TRY_AGAIN){
        if(tryAgainPosition - firstPos < positionZero){
          tryAgainPosition = positionZero;
        } else {
          tryAgainPosition--;
        }
      }
      joyMoved = true;
      lastMoveTime = millis();
      Serial.println(F("Moved up"));
      if(soundVal){
        playMoveSound();
      }
    }
  }

  if (yValue < minThreshold && joyMoved == false) {
    if (millis() - lastMoveTime >= moveDelay) {
      if (currentState == IN_MATRIX_BRIGHTNESS) {
        if (matrixBrightnessVal - two < secondPos) {
          matrixBrightnessVal = secondPos;
        } else {
          matrixBrightnessVal -= secondPos;
        }
      }
      if (currentState == IN_LCD_BRIGHTNESS) {
        if (lcdBrightnessVal - stepLCD < stepLCD) {
          lcdBrightnessVal = stepLCD;
        } else {
          lcdBrightnessVal -= stepLCD;
        }
      }
      if (currentState == IN_SOUND) {
        if (soundVal - firstPos < positionZero) {
          soundVal = positionZero;
        } else {
          soundVal-- ;
        }
      }
      if (currentState == IN_HIGHSCORE){
        if(currentHighscorePosition - firstPos < positionZero){
          currentHighscorePosition = positionZero;
        } else {
          currentHighscorePosition--;
        }
      }
      joyMoved = true;
      lastMoveTime = millis();
      Serial.println(F("Moved left"));
      if(soundVal){
        playMoveSound();
      }
    }
  }


  if (yValue > maxThreshold && joyMoved == false) {
    if (millis() - lastMoveTime >= moveDelay) {
      if (currentState == IN_MATRIX_BRIGHTNESS) {
        if (matrixBrightnessVal + two > matrixMax) {
          matrixBrightnessVal = matrixMax;
        } else {
          matrixBrightnessVal += two;
        }
      }
      if (currentState == IN_LCD_BRIGHTNESS) {
        if (lcdBrightnessVal + stepLCD > lcdMax) {
          lcdBrightnessVal = lcdMax;
        } else {
          lcdBrightnessVal += stepLCD;
        }
      }
      if (currentState == IN_SOUND) {
        if (soundVal + firstPos == secondPos) {
          soundVal = firstPos;
        } else {
          soundVal++ ;
        }
      }
      if (currentState == IN_HIGHSCORE){
        if(currentHighscorePosition + firstPos > numPlayers - firstPos){
          currentHighscorePosition = numPlayers - firstPos;
        } else {
          currentHighscorePosition++;
        }
      }
      joyMoved = true;
      lastMoveTime = millis();
      Serial.println(F("Moved right"));
      if(soundVal){
        playMoveSound();
      }
    }
  }

  if (xValue >= minThreshold && yValue >= minThreshold && yValue <= maxThreshold) {
    joyMoved = false;
  }
}


void mainMenu() {
  Serial.println(F("Intra in main menu."));
  lcd.clear();
  lcd.setCursor(positionZero, positionZero);
  lcd.print(F("MENU: scroll "));
  lcd.write(three);
  lcd.write(four);
  lcd.setCursor(positionZero, firstPos);
  lcd.write(two);
  if(lcdPosition == positionZero){
    lcd.print(F("Start game"));
  }
  if(lcdPosition == firstPos){
    lcd.print(F("Highscore"));
  }
  if(lcdPosition == secondPos){
    lcd.print(F("Settings"));
  }
  if(lcdPosition == thirdPos){
    lcd.print(F("About"));
  }
  if(lcdPosition == fourthPos){
    lcd.print(F("How to play"));
  }

}

void about() {
  String aboutText = " Game: BombIt!, by: vfranci, GitHub: https://github.com/vfranci";
  lcd.setCursor(positionZero, firstPos);
  lcd.print(F("Back to menu: "));
  lcd.write(three);
  lcd.setCursor(positionZero, positionZero);
  lcd.print(aboutText.substring(index, min(index + lcdWidth, aboutText.length())));
  unsigned long currentMillisAbout = millis();
  if (currentMillisAbout - lastMillisAbout >= intervalAbout) {
    lastMillisAbout = currentMillisAbout;
    if (index < aboutText.length() - lcdWidth) {
      index++;
    } else {
      index = resetVal;
    }
  }
}

void settingsMenu() {
  Serial.println(F("in settings"));
  lcd.clear();
  lcd.setCursor(positionZero, positionZero);
  lcd.print(F("SETTINGS: "));
  lcd.write(three);
  lcd.write(four);
  lcd.setCursor(positionZero, firstPos);
  lcd.write(two);
  if(lcdPositionSettings == positionZero){
    lcd.print(F("Reset highscore"));
  }
  if(lcdPositionSettings == firstPos){
    lcd.print(F("LCD brightness"));
  }
  if(lcdPositionSettings == secondPos){
    lcd.print(F("Game brightness"));
  }
  if(lcdPositionSettings == thirdPos){
    lcd.print(F("Sounds on/off"));
  }
  if(lcdPositionSettings == fourthPos){
    lcd.print(F("Main menu"));
  }
}

void matrixBrightness() {
  Serial.println(F("in Matrix Brightness"));
  lcd.clear();
  lcd.setCursor(positionZero, positionZero);
  lcd.print(F("Back: "));
  lcd.write(three);
  lcd.setCursor(positionZero, firstPos);
  lcd.write(five);
  lcd.setCursor(thirdPos, firstPos);
  int counter = matrixBrightnessVal / two;
  for(int i = resetVal; i < counter; i++){
    lcd.write(six);
    lcd.write(six);
  }
  lcd.setCursor(lastPosLCD, firstPos);
  lcd.write(two);
  lc.setIntensity(addr, matrixBrightnessVal);
}

void LCDBrightness() {
  Serial.println(F("in LCD Brightness"));
  lcd.clear();
  lcd.setCursor(positionZero, positionZero);
  lcd.print(F("Back: "));
  lcd.write(three);
  lcd.setCursor(positionZero, firstPos);
  lcd.write(five);
  lcd.setCursor(thirdPos, firstPos);
  int counter = lcdBrightnessVal / stepLCD;
  for(int i = resetVal; i < counter; i++){
    lcd.write(six);
    lcd.write(six);
  }
  lcd.setCursor(lastPosLCD, firstPos);
  lcd.write(two);
  analogWrite(lcdAPin, lcdBrightnessVal);
}

void howToPlay(){
  String howToPlayText = "Move with joystick, place bombs and destroy walls. Bomb blast and walls can kill you.";
  lcd.setCursor(positionZero, firstPos);
  lcd.print(F("Back to menu: "));
  lcd.write(three);
  lcd.setCursor(positionZero, positionZero);
  lcd.print(howToPlayText.substring(index, min(index + lcdWidth, howToPlayText.length())));
  unsigned long currentMillisHowToPlay = millis();
  if (currentMillisHowToPlay - lastMillisHowToPlay >= intervalHowToPlay) {
    lastMillisHowToPlay = currentMillisHowToPlay;
    if (index < howToPlayText.length() - lcdWidth) {
      index++;
    } else {
      index = resetVal; 
    }
  }
}

void enterName(){
  lcd.clear();
  lcd.setCursor(positionZero, positionZero);
  lcd.print(F("Enter Name: "));
  if (letterPosition == positionZero) {
    firstLetter = alphabet[lastLetterIndex];
  } else if (letterPosition == firstPos) {
    secondLetter = alphabet[lastLetterIndex];
  } else if (letterPosition == secondPos) {
    thirdLetter = alphabet[lastLetterIndex];
  }
  lcd.print(firstLetter);
  if (letterPosition >= firstPos) {
    lcd.print(secondLetter);
  }
  if (letterPosition == secondPos) {
    lcd.print(thirdLetter);
  }
  lcd.setCursor(twelve + playerName.length(), positionZero);
  lcd.cursor();
}

void saveName(){
  Serial.println(playerName);
  lcd.clear();
  lcd.noCursor();
  lcd.setCursor(positionZero, positionZero);
  if(lcdPositionName == positionZero){
    lcd.write(two);
    lcd.print(F("Save: "));
    lcd.print(playerName);
  } else {
    lcd.print(F("Save: "));
    lcd.print(playerName);
  }
  lcd.setCursor(positionZero, firstPos);
  if(lcdPositionName == firstPos){
    lcd.write(two);
    lcd.print(F("Try again"));
  } else {
    lcd.print(F("Try again"));
  }
}


void addPlayer(Player newPlayer) {
  int insertIndex = zero;
  newHighscore = (newPlayer.score > players[zero].score);
  while (insertIndex < numPlayers && newPlayer.score < players[insertIndex].score) {
    insertIndex++;
  }
  for (int i = numPlayers - moveOnePos; i > insertIndex; i--) {
    players[i] = players[i - moveOnePos];
  }
  strncpy(players[insertIndex].name, newPlayer.name, sizeof(players[insertIndex].name) - moveOnePos);
  players[insertIndex].name[sizeof(players[insertIndex].name) - moveOnePos] = '\0';
  players[insertIndex].score = newPlayer.score;
  if (insertIndex < numPlayers) {
    playerAdded = true;
  }
}

void highscore(){
  Serial.println("in highscore");
  lcd.clear();
  lcd.setCursor(positionZero, positionZero);
  lcd.print(F("Back: "));
  lcd.write(three);
  lcd.setCursor(positionZero, firstPos);
  lcd.write(five);
  lcd.setCursor(thirdPos, firstPos);
  lcd.print(currentHighscorePosition + firstPos);
  lcd.print(".");
  lcd.print(players[currentHighscorePosition].name);
  lcd.print(": ");
  lcd.print(players[currentHighscorePosition].score);
  lcd.setCursor(lastPosLCD, firstPos);
  lcd.write(two);
}

void resetHighscore(){
  for (int i = zero; i < numPlayers; ++i) {
    players[i].score = baseScore;
    strncpy(players[i].name, "AAA", sizeof(players[i].name));
  }
  lcd.clear();
  lcd.setCursor(positionZero, firstPos);
  lcd.print(F("Back: "));
  lcd.write(three);
  lcd.setCursor(positionZero, positionZero);
  lcd.print("Highscore reset.");
}

void sound(){
  lcd.clear();
  lcd.setCursor(positionZero, firstPos);
  lcd.print(F("Save & back: "));
  lcd.write(three);
  lcd.setCursor(positionZero, positionZero);
  lcd.print(F("Sound is "));
  lcd.write(five);
  if(soundVal == positionZero){
    lcd.print("OFF");
  } else {
    lcd.print("ON");
  }
  lcd.write(two);
}

void tryAgain(){
  lcd.clear();
  lcd.setCursor(positionZero, positionZero);
  if(tryAgainPosition == positionZero){
    lcd.write(two);
    lcd.print(F("Back to menu: "));
    lcd.write(three);
  } else {
    lcd.print(F("Back to menu: "));
    lcd.write(three);
  }
  lcd.setCursor(positionZero, firstPos);
  if(tryAgainPosition == firstPos){
    lcd.write(two);
    lcd.print(F("Try again"));
  } else {
    lcd.print(F("Try again"));
  }
}

////////////////////////////////////////////////////////    MENU FUNCTIONS  /////////////////////////////////////////////////////////////////////////////////