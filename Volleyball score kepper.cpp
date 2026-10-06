#include <LiquidCrystal.h>
#include <Arduino.h>

// Matches your exact hardware mapping: RS=12, E=11, DB4=5, DB5=4, DB6=3, DB7=2
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
void print();
void score(bool state8, bool state9, bool state10, bool prevState8, bool prevState9, bool prevState10);
void setup();
void loop();
int endGame();
void print();
void reset();


int endScore = 25;
bool isIndoor = true;
int homeScore = 0;
int set = 1;
int awayScore = 0;
int homeSet = 0;
int awaySet = 0;
int homeLoc = 2;
int setLoc = 7;
int awayLoc = 11;
bool lastHomeBtnState = LOW;
bool lastAwayBtnState = LOW;
bool lastResetBtnState = LOW;


void setup() {
  lcd.begin(16, 2); // Set up the LCD's number of columns and rows
  lcd.clear();      // Wipe the memory clean
  pinMode(8,INPUT_PULLUP); //add home
  pinMode(9,INPUT_PULLUP); //reset
  pinMode(10,INPUT_PULLUP);//add away
  lcd.clear();
  lcd.setCursor(homeLoc,0);
  lcd.print("Home");
  lcd.setCursor(setLoc,0);
  lcd.print("Set");
  lcd.setCursor(awayLoc,0);
  lcd.print("Away");
  print(); //print scareboard
}
void loop() {
  bool st8 = digitalRead(8);
  bool st9 = digitalRead(9);
  bool st10 = digitalRead(10);
    if(isIndoor){
        endScore = 25;
        score(st8,st9,st10,lastHomeBtnState,lastAwayBtnState,lastResetBtnState);
        if (endGame() == 1) {
            return;
        }
    }else{
        endScore = 21;
        score(st8,st9,st10,lastHomeBtnState,lastAwayBtnState,lastResetBtnState);
        if (endGame() == 1) {
            return;
        }
    }
    lastHomeBtnState = st8;
    lastAwayBtnState = st10;
    lastResetBtnState = st9;
    delay(100); // Delay to avoid bouncing issues
}
int endGame(){//checks if a team has won the game and resets the scoreboard if so
  if (((homeSet == 2 || awaySet == 2) && !isIndoor) || ((homeSet == 3 || awaySet == 3) && isIndoor)){//check if game is over 2 sets for beach and 3 sets for indoor
      reset();//reset scoreboard
      return 1;//end function
    }
  if (((set == 3) && !isIndoor) || ((set == 5) && isIndoor)){//check if it is the last set3 for beach and 5 for indoor
    endScore = 15;
  }
  if ((homeScore >= endScore || awayScore >= endScore) && (abs(homeScore - awayScore) >= 2)){//check if a team has won by more than 2
    set++;
    if (homeScore > awayScore){//check which team has won and add the set to their score
      homeSet++;
    }else{
      awaySet++;
    }
    homeScore = 0;
    awayScore = 0;
  }
  return 0;
}
void print(){//formats the score to 2 digits and prints it to the lcd
  String homeScoreStr = String(homeScore);
  String awayScoreStr = String(awayScore);
  lcd.setCursor(homeLoc,1);
  lcd.print(homeScoreStr + ":" + homeSet);
  lcd.setCursor(setLoc,1);
  lcd.print(set);
  lcd.setCursor(awayLoc,1);
  lcd.print(awayScoreStr + ":" + awaySet);
}
void reset(){//resets the scoreboard to  all 0 and prints it to the lcd
  homeScore = 0;
  awayScore = 0;
  set = 1;
  homeSet = 0;
  awaySet = 0;
  print();
}
void score(bool state8,bool state9,bool state10, bool prevState8, bool prevState9, bool prevState10){
  if (state8 == LOW && prevState8 == HIGH) {//check if button is pressed
    homeScore = homeScore + 1;//add score to respective team
    print();//print scoreboard
  }
  if (state10 == LOW && prevState10 == HIGH) {//check if button is pressed
    awayScore = awayScore + 1;//add score to respective team
    print();//print scoreboard
  }
  if (state9 == LOW && prevState9 == HIGH) {//check if button is pressed
    reset();//reset scoreboard
  }
}
