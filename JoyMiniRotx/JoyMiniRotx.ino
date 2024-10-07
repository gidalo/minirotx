/*
 * 
 * JoyMiniRotx.ino for mini emulator
 * Compile : USB MCU (e.g. Arduino Leonardo, Arduino Micro, etc.).
 * Arduino ATMEGA32U4 Mini Arduino Leonardo USB ATMEGA32U4 Pro Micro
 * 
 * MOD by MatixVision  (F) 2024 v.1.0.4 - MiniRotx OCTOBER 2024
 * Salvatore Balestrieri (MatixVision) oct2024 
 * e-mail:matixvision@gidalo.it
 * youtube: @matixvision
 * for use Mini-Rotx retrogames
 * License: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
 * 
*/

#include <Joystick.h>

// set the pin where the buttons are connected
int button1=2;
int button2=3;
int button3=4;
int button4=5;
int button5=6;

// defines button totals
#define PINS 5


Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   PINS, 0,               // Button Count, Hat Switch Count
                   true, true, false,     // X and Y, but no Z Axis
                   false, false, false,   // No Rx, Ry, or Rz
                   false, false,          // No rudder or throttle
                   false, false, false);  // No accelerator, brake, or steering

void setup() {
	
//I declare input and set internal pull-up resistance
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);
  pinMode(button5, INPUT_PULLUP);


  Joystick.begin();
  Joystick.setXAxisRange(-1, 1);
  Joystick.setYAxisRange(-1, 1);
}


void loop() {

 //sensitivity threshold amount if the joystick is analogue rather than contact
 //the activation threshold is: 0-300 700-1024 while 300-700 represents the central position
 
    if (analogRead(A0) < 300) { Joystick.setXAxis(1); } if (analogRead(A0) > 700) {
      Joystick.setXAxis(-1);
    }
    if ((analogRead(A0) > 300) && (analogRead(A0) < 700)) {
      Joystick.setXAxis(0);
    }

    if (analogRead(A1) < 300) { Joystick.setYAxis(1); } if (analogRead(A1) > 700) {
      Joystick.setYAxis(-1);
    }
    if ((analogRead(A1) > 300) && (analogRead(A1) < 700)) {
      Joystick.setYAxis(0);
    }


//if pressed, the buttons send the inputs to LOW which return a HIGH logical state on the gamepad command

if(digitalRead(button1) == LOW){Joystick.setButton(0, HIGH);}else{Joystick.setButton(0, LOW);}
if(digitalRead(button2) == LOW){Joystick.setButton(1, HIGH);}else{Joystick.setButton(1, LOW);}
if(digitalRead(button3) == LOW){Joystick.setButton(2, HIGH);}else{Joystick.setButton(2, LOW);}
if(digitalRead(button4) == LOW){Joystick.setButton(3, HIGH);}else{Joystick.setButton(3, LOW);}
if(digitalRead(button5) == LOW){Joystick.setButton(4, HIGH);}else{Joystick.setButton(4, LOW);}


delay(8);
}
