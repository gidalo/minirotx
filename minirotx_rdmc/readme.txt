Compile and use:

* sudo apt-get update
* sudo apt install -y wiringpi
* gpio -v
* gcc -o rdmc  minirotx_rdmc.c -lwiringPi
* sudo ./rdmc  forward
* sudo ./rdmc backward
