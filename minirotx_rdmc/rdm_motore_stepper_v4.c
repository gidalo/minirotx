/* 
* MiniRotx RDMC "Rotate Display Motor Control" Ver.1.0beta
* for use stepper motor 28BYJ-48 and driver ULN2003
* By Salvatore Balestrieri (MatixVision) oct2024 
* e-mail:matixvision@gidalo.it
* youtube: @matixvision
* License: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
* Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
* Compile and use only Raspberry Pi zero W2 
* sudo apt-get update
* sudo apt install -y wiringpi
* gpio -v
* gcc -o rdmc  rdm_motore_stepper_v4.c -lwiringPi
* sudo ./rdmc  orizzontale
* sudo ./rdmc  verticale
*/

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#define IN1 21  // GPIO 5 (WiringPi 21)
#define IN2 22  // GPIO 6 (WiringPi 22)
#define IN3 4   // GPIO 23 (WiringPi 4)
#define IN4 25  // GPIO 26 (WiringPi 25)

int stepSequence[8][4] = {
    {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0},
    {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {1, 0, 0, 1}

#define STEPS_PER_REV 4096 // Numero di passi per una rivoluzione completa del motore
