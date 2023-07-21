# KidCal: Smart Scale Web Application

## Overview

KidCal is a smart scale program designed to display real-time weight data on a local web application. Hosted on a Raspberry Pi, the application is divided into two main components:

- **kidcal-display**: This component is built using React and provides a user-friendly interface to visualize the weight data. It communicates with `kidcal-stream` to fetch the latest weight readings and updates the display in real-time.

- **kidcal-stream**: Written in C, this component reads raw weight data from the scale and streams it to the front end. It ensures that the data is transmitted efficiently and accurately to `kidcal-display`.

## Hardware

- **Raspberry Pi**: Both the `kidcal-display` and `kidcal-stream` applications are hosted on a Raspberry Pi with wireless network access.

- **HX711 ADC Breakout Board**: The HX711 is a 24 bit Analog to Digital Converter for weigh scales. The kidcal-streaming application interfaces directly with the HX711 to read raw weight data.

- **5KG Load Cells - Qty. 4**: One load cell sits at each corner of the scale, connected in parallel to the single HX711 ADC.

## Setup and Configuration

### Prerequisites

Ensure you have a Raspberry Pi with a compatible OS installed. The Pi must connect to the local network by default on boot.

### Installation Steps

1. **Clone the Repository**:

   ```bash
   git clone https://github.com/tyler-wadekamper/kidcal.git
   cd kidcal
   ```

2. **Install kidcal-display Dependencies**:
   Navigate to the `kidcal-display` directory and install the necessary dependencies.
   Assuming npm is installed:

   ```bash
   cd kidcal-display
   npm install
   ```

3. **Install kidcal-stream Dependencies**:
   Navigate to the `kidcal-stream` directory and install the necessary dependencies.
   Update the package list and upgrade the existing packages.

   ```bash
   sudo apt update
   sudo apt upgrade -y
   ```

   Install `libwebsockets` to allow initiation of a socket connection to the front-end.

   ```bash
   git clone https://github.com/warmcat/libwebsockets.git
   cd libwebsockets
   mkdir build
   cd build
   cmake ..
   make && sudo make install
   pkg-config --modversion libwebsockets
   ```

   Install `pigpio` to allow control of the Raspberry Pi general-purpose input outputs (GPIO).

   ```bash
   sudo apt install -y pigpio
   ```

   To ensure the `pigpio` daemon starts on boot:

   ```bash
   sudo systemctl enable pigpiod
   sudo systemctl start pigpiod
   ```

4. **Compile streaming.c**:

   ```bash
   gcc -o streaming streaming.c -lwebsockets -lpigpio
   ```

5. **Enable `kidcal-stream` to Run on Boot**:
   Add the following line to your `rc.local` file (before the `exit 0` line):

   ```bash
   /path/to/kidcal/kidcal-stream/streaming &
   ```

6. **Install PM2**:
   PM2 is a process manager for Node.js applications. It will be used to ensure the React app starts on boot.

   ```bash
   npm install pm2 -g
   ```

7. **Configure PM2 to Start `kidcal-display` on Boot**:

   ```bash
   pm2 start /path/to/kidcal/kidcal-display/npm -- start
   pm2 save
   pm2 startup
   ```

8. **Port forward your Pi**:
   Your router must allow traffic to the Raspberry Pi on port 3000. Log into the router and forward this port on the Pi.

9. **Set a static IP address for the Pi**:
   The streaming application is set up to connect to 192.168.1.21. This process assigns that IP address statically for wireless connections.
   Open your `dhcpcd.conf` configuration file:

   ```bash
   sudo nano /etc/dhcpcd.conf
   ```

   At the bottom of this file, add the following lines to set a static IP for your wireless connection `wlan0`:

   ```bash
   interface wlan0
   static ip_address=192.168.1.21/24
   static routers=192.168.1.1
   static domain_name_servers=192.168.1.1 8.8.8.8
   ```

   Log into your router and edit the DHCP IP range to exclude this IP to avoid conflicts.

10. **Verify that the setup is correct**:
    Power cycle the raspberry pi and verify that your Pi is connected to the network at 192.168.1.21, the streaming application is running, and the react application is running. You should noe be able to connect to the web application and see the real-time weight data!
