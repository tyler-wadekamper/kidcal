# kidcal streaming
This application is a C streaming application that streams raw load cell data to the console. It's primary purpose is to test hardware setup and basic driver functionality.

## Getting Started
### Hardware
Connect a load cell to an HX711 ADC circuit. Provide 3.3V or 5V to the HX711 and connect the clock and data pins to your Pi according to the pin numbers in streaming.c.

### Clone
Clone this repository on a Raspberry Pi 3 model B or a pinout equivalent Raspberry Pi.

### Install pigpio
sudo apt update && sudo apt upgrade
sudo apt-get install pigpio

### Compile streaming application
gcc -o streaming streaming.c -lpigpio -lrt -lpthread

### Run the application
sudo ./streaming
