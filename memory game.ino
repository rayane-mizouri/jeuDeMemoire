#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int ledPins[] = {9, 8, 7, 6};
const int buttonPins[] = {A0, A1, A2, A3};
int sequenceLength = 1;
int sequence[10];
int playerSequence[10];
int score = 0;
bool gameOver = false;

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  lcd.begin(16, 2);
  lcd.print("Memory Game");
  lcd.setCursor(0, 1);
  lcd.print("Score: 0  Level: 1");
  delay(2000);
  
  randomSeed(analogRead(0));
  for (int i = 0; i < sequenceLength; i++) {
    sequence[i] = random(0, 4);
  }
  displaySequence();
}

void loop() {
  readPlayerSequence();
  
  for (int i = 0; i < sequenceLength; i++) {
    if (playerSequence[i] != sequence[i]) {
      gameOver = true;
      break;
    }
  }
  
  if (!gameOver) {
    score++;
    if (score % 5 == 0) {
      sequenceLength++;
      lcd.setCursor(0, 1);
      lcd.print("            ");
      lcd.setCursor(0, 1);
      lcd.print("Level: ");
      lcd.print(sequenceLength);
    }
  } else {
    playErrorTone();
    lcd.clear();
    lcd.print("Game Over!");
    lcd.setCursor(0, 1);
    lcd.print("Score: ");
    lcd.print(score);
    delay(2000);
    lcd.clear();
    lcd.print("Memory Game");
    lcd.setCursor(0, 1);
    lcd.print("Score: 0  Level: 1");
    score = 0;
    sequenceLength = 1;
    gameOver = false;
    randomSeed(analogRead(0));
    for (int i = 0; i < sequenceLength; i++) {
      sequence[i] = random(0, 4);
    }
    displaySequence();
  }
  
  lcd.setCursor(7, 1);
  lcd.print(score);
  delay(1000);
}

void displaySequence() {
  for (int i = 0; i < sequenceLength; i++) {
    digitalWrite(ledPins[sequence[i]], HIGH);
    delay(500);
    digitalWrite(ledPins[sequence[i]], LOW);
    delay(100);
  }
}

void readPlayerSequence() {
  for (int i = 0; i < sequenceLength; i++) {
    int buttonPressed = -1;
    while (buttonPressed == -1) {
      for (int j = 0; j < 4; j++) {
        if (digitalRead(buttonPins[j]) == LOW) {
          buttonPressed = j;
          digitalWrite(ledPins[j], HIGH);
          delay(250);
          digitalWrite(ledPins[j], LOW);
          break;
        }
      }
    }
    playerSequence[i] = buttonPressed;
    delay(500);
  }
}

void playErrorTone() {
  for (int i = 0; i < 3; i++) {
    tone(10, 500);
    delay(200);
    noTone(10);
    delay(100);
  }
}