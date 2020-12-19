
//   Learning Auto Blipper

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>



const int tpsread = A1;
const int keypad = A2;
const int blipper = 3;
const int clutch = 51;

int clutchval;
int tpsbottom;
bool decelstate;
bool blipstate;
int currentblip = 50;
float yaxis = 0;
float currenttime;
float starttime;
int tpsvalue;
bool button1;
bool button2;
bool button3;
bool button4;
bool button5;
bool button6;
bool button7;
bool button8;
bool button9;
bool button10;
bool button11;
bool button12;
int pagenum = 0;
bool doneflag;
float lastclear;
bool exitflag;
bool setupstate;
bool prevstate;
int tpspercent;
int setuppage;
int address = 0;

bool page0;
bool page1;
bool page2;
bool page3;
bool page4;

//Magnetometer Registers
#define AK8963_ADDRESS   0x0C
#define WHO_AM_I_AK8963  0x00 // should return 0x48
#define INFO             0x01
#define AK8963_ST1       0x02  // data ready status bit 0
#define AK8963_XOUT_L   0x03  // data
#define AK8963_XOUT_H  0x04
#define AK8963_YOUT_L  0x05
#define AK8963_YOUT_H  0x06
#define AK8963_ZOUT_L  0x07
#define AK8963_ZOUT_H  0x08
#define AK8963_ST2       0x09  // Data overflow bit 3 and data read error status bit 2
#define AK8963_CNTL      0x0A  // Power down (0000), single-measurement (0001), self-test (1000) and Fuse ROM (1111) modes on bits 3:0
#define AK8963_ASTC      0x0C  // Self test control
#define AK8963_I2CDIS    0x0F  // I2C disable
#define AK8963_ASAX      0x10  // Fuse ROM x-axis sensitivity adjustment value
#define AK8963_ASAY      0x11  // Fuse ROM y-axis sensitivity adjustment value
#define AK8963_ASAZ      0x12  // Fuse ROM z-axis sensitivity adjustment value

#define SELF_TEST_X_GYRO 0x00
#define SELF_TEST_Y_GYRO 0x01
#define SELF_TEST_Z_GYRO 0x02

#define SELF_TEST_X_ACCEL 0x0D
#define SELF_TEST_Y_ACCEL 0x0E
#define SELF_TEST_Z_ACCEL 0x0F


LiquidCrystal_I2C lcd(0x27, 20, 4);



void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("Starting");
  delay(1000);
  Serial.begin(115200);
  pinMode(clutch, INPUT_PULLUP);



}

void loop() {
  keypadcheck();
  screenpage();



  clutchval = digitalRead(clutch);



}

void state() {
  float threshold = 100; //ms recording time
  if (yaxis < -0.1) { //when ever the accelerometer gets a decel g force over 0.1g
    currenttime = millis();
    if (threshold <= millis() - currenttime) {
      decelstate = true;
    } else {
      decelstate = false;
    }
  } else {
    decelstate = false;
  }
}

void screenpage() {   //screen control

  if (button11 == true) { //next page
    if (prevstate == false) {
      pagenum++;
      prevstate = true;
    }
  } else if (button10 == true) {  //prev page
    if (prevstate == false) {
      pagenum--;
      prevstate = true;
    }
  } else {
    prevstate = false;
  }
  if (button12 == true) { //when exit button is pressed
    exitflag = true;
  } else {
    exitflag = false;
  }
  pagenum = constrain(pagenum, 0 , 6);
  if (exitflag == true) {
    lcd.clear();
    pagenum = 0; //returns to homepage
    setupstate = false;
    exitflag = false;
  } else {
    if (setupstate == false) {
      if (pagenum == 0) {
        if (page0 == false) { //initial clear message
          lcd.clear();
          lastclear = millis();
        }
        if (lastclear <= millis() - 100) { //refresh rate
          lcd.clear();
          lastclear = millis();
          Serial.println(lastclear);
        }
        lcd.setCursor(0, 0);
        lcd.print("-----Auto BlipR-----");
        lcd.setCursor(0, 1);
        lcd.print("Clutch:");
        lcd.setCursor(7, 1);
        lcd.print(clutchval);
        lcd.setCursor(0, 2);
        lcd.print("Mode:");
        lcd.setCursor(5, 2);
        lcd.print(yaxis);
        lcd.setCursor(0, 3);
        lcd.print("TPS:");
        lcd.setCursor(19, 3);
        lcd.print(pagenum);
        page0 = true;
        page1 = false;
        page2 = false;
      } else if (pagenum == 1) {
        if (page1 == false) { //initial clear message
          lcd.clear();
          lastclear = millis();
        }
        if (lastclear <= millis() - 100) { //refresh rate
          lcd.clear();
          lastclear = millis();
          Serial.println(lastclear);
        }
        lcd.setCursor(4, 0);
        lcd.print("Page 2");
        if (button1 == true) {
          lcd.setCursor(1, 1);
          lcd.print("button1");
        }
        lcd.setCursor(19, 3);
        lcd.print(pagenum);
        page0 = false;
        page1 = true;
        page2 = false;
      } else if (pagenum == 2) {
        if (page2 == false) { //initial clear message
          lcd.clear();
          lastclear = millis();
        }
        if (lastclear <= millis() - 100) { //refresh rate
          lcd.clear();
          lastclear = millis();
          Serial.println(lastclear);
        }
        lcd.setCursor(4, 0);
        lcd.print("SETUP PAGE");
        lcd.setCursor(0, 3);
        lcd.print("a");
        lcd.setCursor(19, 3);
        lcd.print(pagenum);
        if (button9 == true) {

        }
        page1 = false;
        page2 = true;
        page3 = false;
      } else if (pagenum == 3) {
        if (page3 == false) { //initial clear message
          lcd.clear();
          lastclear = millis();
        }
        if (lastclear <= millis() - 100) { //refresh rate
          lcd.clear();
          lastclear = millis();
          Serial.println(lastclear);
        }
        page2 = false;
        page3 = true;
        page4 = false;
      }
    }
  }
}
/*
  void settup() {       OBSELETE!!!!!
  bool markflag;
  bool setup1;
  bool setup2;
  bool setup3;
  bool setup4;
  int tpsbottomtemp;
  int tpstoptemp;
  int tpspercenttemp;
  if (button12 == true) { //when exit button is pressed
    exitflag = true;
  } else {
    exitflag = false;
  }
  if (setupstate == true) {
    if (setuppage == 0) {
      if (setup1 == false) { //initial clear message
        lcd.clear();
        lastclear = millis();
        markflag = false;
      }
      if (lastclear <= millis() - 100) { //refresh rate
        lcd.clear();
        lastclear = millis();
        Serial.println(lastclear);
      }

      starttime = millis();
      tpsvalue = analogRead(tpsread);
      lcd.setCursor(0, 0);
      lcd.print("Is Throttle Idle?");
      lcd.setCursor(0, 3);
      lcd.print("Set Idle Level?");
      if (button9 == true) {
        if (markflag == false) {
          tpsbottomtemp = tpsvalue;
          setuppage++;
          markflag = true;
        }
      }
      if (starttime <= millis() - 300000) { //boots you out if youve sat for to long
        exitflag = true;
      }
      setup1 = true;
      setup2 = false;
      setup3 = false;
      setup4 = false;
    } else if (setuppage == 1) {
      if (setup2 == false) { //initial clear message
        lcd.clear();
        lastclear = millis();
        markflag = false;
      }
      if (lastclear <= millis() - 100) { //refresh rate
        lcd.clear();
        lastclear = millis();
        Serial.println(lastclear);
      }
      lcd.setCursor(0, 0);
      lcd.print("APPLY FULL THROTTLE");
      lcd.setCursor(0, 1);
      lcd.print("TPS POT:");
      lcd.setCursor(9, 1);
      lcd.setCursor(0, 3);
      lcd.print("Set Full Throttle?");

      if (button9 == true) {
        tpstoptemp = tpsvalue;
        setuppage++;
      }
      setup1 = false;
      setup2 = true;
      setup3 = false;
      setup4 = false;
    } else if (setuppage == 2) {

    }
  }
  }
*/
void keypadcheck() {
  int keypadval = analogRead(keypad);
  if (button9 == true) {
    Serial.println("Button 9");
  }
  if (keypadval >= 1010 && keypadval <= 1040) { //button 1
    button1 = true;
    button2 = false;
    button3 = false;
    button4 = false;
    button5 = false;
    button6 = false;
    button7 = false;
    button8 = false;
    button9 = false;
    button10 = false;
    button11 = false;
    button12 = false;
  } else if (keypadval >= 900 && keypadval <= 960) {//button 2
    button1 = false;
    button2 = true;
    button3 = false;
    button4 = false;
    button5 = false;
    button6 = false;
    button7 = false;
    button8 = false;
    button9 = false;
    button10 = false;
    button11 = false;
    button12 = false;
  } else if (keypadval >= 830 && keypadval <= 880) { //button 3
    button1 = false;
    button2 = false;
    button3 = true;
    button4 = false;
    button5 = false;
    button6 = false;
    button7 = false;
    button8 = false;
    button9 = false;
    button10 = false;
    button11 = false;
    button12 = false;
  } else if (keypadval >= 770 && keypadval <= 810) { //button 4
    button1 = false;
    button2 = false;
    button3 = false;
    button4 = true;
    button5 = false;
    button6 = false;
    button7 = false;
    button8 = false;
    button9 = false;
    button10 = false;
    button11 = false;
    button12 = false;
  } else if (keypadval >= 707 && keypadval <= 760) { //button 5
    button1 = false;
    button2 = false;
    button3 = false;
    button4 = false;
    button5 = true;
    button6 = false;
    button7 = false;
    button8 = false;
    button9 = false;
    button10 = false;
    button11 = false;
    button12 = false;
  } else if (keypadval >= 667 && keypadval <= 700) { //button 6
    button1 = false;
    button2 = false;
    button3 = false;
    button4 = false;
    button5 = false;
    button6 = false;
    button7 = false;
    button8 = false;
    button9 = false;
    button10 = false;
    button11 = false;
    button12 = false;
  } else if (keypadval >= 624 && keypadval <= 664) { //button 7
    button1 = false;
    button2 = false;
    button3 = false;
    button4 = false;
    button5 = false;
    button6 = false;
    button7 = true;
    button8 = false;
    button9 = false;
    button10 = false;
    button11 = false;
    button12 = false;
  } else if (keypadval >= 597 && keypadval <= 627) { //button 8
    button1 = false;
    button2 = false;
    button3 = false;
    button4 = false;
    button5 = false;
    button6 = false;
    button7 = false;
    button8 = true;
    button9 = false;
    button10 = false;
    button11 = false;
    button12 = false;
  } else if (keypadval >= 565 && keypadval <= 587) { //button 9
    button1 = false;
    button2 = false;
    button3 = false;
    button4 = false;
    button5 = false;
    button6 = false;
    button7 = false;
    button8 = false;
    button9 = true;
    button10 = false;
    button11 = false;
    button12 = false;
  } else if (keypadval >= 535 && keypadval <= 555) { //button 10
    button1 = false;
    button2 = false;
    button3 = false;
    button4 = false;
    button5 = false;
    button6 = false;
    button7 = false;
    button8 = false;
    button9 = false;
    button10 = true;
    button11 = false;
    button12 = false;
  } else if (keypadval >= 509 && keypadval <= 529) { //button 11
    button1 = false;
    button2 = false;
    button3 = false;
    button4 = false;
    button5 = false;
    button6 = false;
    button7 = false;
    button8 = false;
    button9 = false;
    button10 = false;
    button11 = true;
    button12 = false;
  } else if (keypadval >= 487 && keypadval <= 507) { //button 12
    button1 = false;
    button2 = false;
    button3 = false;
    button4 = false;
    button5 = false;
    button6 = false;
    button7 = false;
    button8 = false;
    button9 = false;
    button10 = false;
    button11 = false;
    button12 = true;
  } else { //stops saying a button is pressed when nothing is pressed
    button1 = false;
    button2 = false;
    button3 = false;
    button4 = false;
    button5 = false;
    button6 = false;
    button7 = false;
    button8 = false;
    button9 = false;
    button10 = false;
    button11 = false;
    button12 = false;
  }
}
