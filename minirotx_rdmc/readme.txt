* MiniRotx RDMC "Rotate Display Motor Control" Ver.1.0beta
* for use stepper motor 28BYJ-48 and driver ULN2003
* By Salvatore Balestrieri (MatixVision) oct2024 
* e-mail:matixvision@gidalo.it
* youtube: @matixvision
* License: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
* Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
* Compile and use only Raspberry Pi zero W2 

sudo apt-get update
sudo apt install -y wiringpi
sudo mkdir -p /etc/rdm_motore/

download and copy file:
cp rdmc_motore_stepper_v4 /home/pi/.
chmod +x /home/pi/rdmc_motore_stepper_v4
