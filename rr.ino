#include <LCD_I2C.h>
 
LCD_I2C lcd(0x27, 16, 2); //0x27 lub 0x3F
 
 //Player and Enemies
int enemiesX[] = {0, 1, 3, 1, 0};
int enemiesY[] = {15, 12, 10, 8, 6};
//int enemiesNumber = 5; // !! length of arrays above
int playerX = 2; //start position for player
int speed = 150; //the lower value the slower it goes

bool alive = true; //is player alive? -> should game continue?

uint8_t playerRight[8] =
{
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b10100,
    0b11110,
    0b11110,
    0b10100,
};
uint8_t playerLeft[8] =
{
    0b10100,
    0b11110,
    0b11110,
    0b10100,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
};
uint8_t obstacleRight[8] =
{
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b01010,
    0b11110,
    0b11110,
    0b01010,
};
uint8_t obstacleLeft[8] =
{
    0b01010,
    0b11110,
    0b11110,
    0b01010,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
};
uint8_t obstacleBoth[8] =
{
    0b01010,
    0b11110,
    0b11110,
    0b01010,
    0b01010,
    0b11110,
    0b11110,
    0b01010,
};
uint8_t playerObstacleLeft[8] =
{
    0b01010,
    0b11110,
    0b11110,
    0b01010,
    0b10100,
    0b11110,
    0b11110,
    0b10100,
};
uint8_t playerObstacleRight[8] =
{
    0b10100,
    0b11110,
    0b11110,
    0b10100,
    0b01010,
    0b11110,
    0b11110,
    0b01010,
};
 
//Pins
int buttonLeft = 8;
int buttonRight = 5;

int leftButtonState;
int rightButtonState;
int leftPreviousState = HIGH;
int rightPreviousState = HIGH;

int lc=0;
int rc=0;

void checkButton() {
  
  leftButtonState = digitalRead(buttonLeft);
  rightButtonState = digitalRead(buttonRight);  

  if(leftButtonState == LOW && leftPreviousState != LOW) {
    if(playerX > 0) {playerX--;};
    //Serial.print("Left Button pressed\n");//xx
  } 
  if(rightButtonState == LOW && rightPreviousState != LOW) {
    if(playerX < 3) {playerX++;};
    //Serial.print("Right Button pressed\n");
  } 
    
  leftPreviousState = leftButtonState;  
  rightPreviousState = rightButtonState;
  

  //delay(100);
  
  


}
 
void moveAll() {
  for(int i = 0; i < sizeof(enemiesX) / sizeof(int); i++) {
    enemiesY[i]--;
    if(enemiesY[i] < 0) {
        createEnemy(i); //replace enemy at i
    }

    updatePlayer();

  }
}

void playerDisplay() {
  switch(playerX) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.write(1);
      break;        

    case 1:
      lcd.setCursor(0, 0);
      lcd.write(0);
      break;
              
    case 2:
      lcd.setCursor(0, 1);
      lcd.write(1);
      break;
              
    case 3:
      lcd.setCursor(0, 1);
      lcd.write(0);
      break;
  }
}

void enemiesDisplay() {
  for(int i = 0; i < sizeof(enemiesX) / sizeof(int); i++) {

    if (enemiesY[i] > 0 || enemiesX[i] != playerX) {
      switch(enemiesX[i]) {
            case 0:
              lcd.setCursor(enemiesY[i], 0);
              lcd.write(2);
              break;
            case 1:
              lcd.setCursor(enemiesY[i], 0);
              lcd.write(3);
              break;
            case 2:
              lcd.setCursor(enemiesY[i], 1);
              lcd.write(2);
              break;
            case 3:
              lcd.setCursor(enemiesY[i], 1);
              lcd.write(3);
              break;
          }
    } else { //zderzenie
      alive = false;
    }




    updatePlayer();


  }
}

void createEnemy(int i) {
  enemiesX[i] = random(3);
  enemiesY[i] = 15;
}

void updatePlayer() {
  checkButton();
  playerDisplay();
}
 
void setup()
{
    pinMode(buttonLeft, INPUT_PULLUP);
    pinMode(buttonRight, INPUT_PULLUP);
    pinMode(buttonLeft, INPUT);
    pinMode(buttonRight, INPUT);
    
    
    lcd.begin();
    lcd.backlight();
 
    lcd.createChar(0, playerRight);
    lcd.createChar(1, playerLeft);
    lcd.createChar(2, obstacleLeft);
    lcd.createChar(3, obstacleRight);
    lcd.createChar(4, obstacleBoth);
    lcd.createChar(5, playerObstacleLeft);
    lcd.createChar(6, playerObstacleRight);

    Serial.begin(9600);//xx
}
 
void loop()
{
  if(alive) {
    lcd.clear();

    enemiesDisplay();
    
    for (int a=0;a<speed;a++) { 
      updatePlayer();
      delay(1);
    }

    moveAll();

  } else {
    lcd.setCursor(0, playerX/2);
    lcd.print("X"); //mark place of death
    lcd.setCursor(8, 0);
    lcd.print("YOU LOSE");
  }





}
