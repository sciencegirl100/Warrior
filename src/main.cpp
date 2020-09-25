#include <Arduino.h>
#include <Keyboard.h>
#include <Keypad.h>
#define FLASH_DELAY 150

const uint16_t cmdBtn = A3;
const byte R = 4;
const byte C = 3;
char keys[R][C] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte RP[R] = {2, 3, 4, 5};
byte CP[C] = {6, 7, 8};
Keypad key = Keypad(makeKeymap(keys), RP, CP, R, C);
unsigned long lCnt;
unsigned long sTme;
uint8_t RUN_MODE = 0;

void setup(){
  Serial.begin(9600);
  lCnt = 0;
  sTme = millis();
  pinMode(cmdBtn, INPUT_PULLUP);
  pinMode(A0, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(10);
  digitalWrite(A0, LOW);
  delay(100);
  if(digitalRead(cmdBtn)){
    Keyboard.begin();
    RUN_MODE = 0;
    // Serial.write("NORMAL");
    for(uint8_t i = 0; i < 5; i++){
      digitalWrite(LED_BUILTIN, HIGH);
      delay(FLASH_DELAY);
      digitalWrite(LED_BUILTIN, LOW);
      delay(FLASH_DELAY);
    }
  }else{
    // Serial.write("PROGRAM");
    RUN_MODE = 1;
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void loop() {
  if (!RUN_MODE){
    lCnt++;
    if ( (millis()-sTme)>5000 ) {
      // Serial.print("Average loops per second = ");
      // Serial.println(lCnt/5);
      sTme = millis();
      lCnt = 0;
    }
    // Fills kpd.key[ ] array with up-to 10 active keys.
    // Returns true if there are ANY active keys.
    if (key.getKeys()) {
      for (int i=0; i<LIST_MAX; i++) {  // Scan the whole key list.
        if ( key.key[i].stateChanged ) { // Only find keys that have changed state.
          switch (key.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
          case PRESSED:
          // buff = " PRESSED.";
          if (key.key[i].kchar == '*'){
            Keyboard.press(KEY_BACKSPACE);
          }else if (key.key[i].kchar == '#'){
            Keyboard.press(KEY_RETURN);
          }else{
            Keyboard.press(key.key[i].kchar);
          }
          break;
          case HOLD:
          // buff = "
          break;
          case RELEASED:
          // buff = " RELEASED.";
          if (key.key[i].kchar == '*'){
            Keyboard.release(KEY_BACKSPACE);
          }else if (key.key[i].kchar == '#'){
            Keyboard.release(KEY_RETURN);
          }else{
            Keyboard.release(key.key[i].kchar);
          }
          break;
          case IDLE:
          // buff = " I
          Keyboard.releaseAll();
        }
        // Serial.print("Key ");
        // Serial.print(key.key[i].kchar);
        // Serial.println(buff);
        }
      }
    }
  }
}