#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>

#define CLOCK_PIN 23
#define DATA_PIN 24

void setup() {
    if (gpioInitialise() < 0) {
        printf("Failed to initialize GPIOs\n");
        exit(1);
    }
    gpioSetMode(CLOCK_PIN, PI_OUTPUT);
    gpioSetMode(DATA_PIN, PI_INPUT);
}

int read_HX711() {
    int data = 0;
    while (gpioRead(DATA_PIN)); // Wait for DATA_PIN to go low
    for (int i = 0; i < 24; ++i) {
        gpioWrite(CLOCK_PIN, 1); // Send pulse
        time_sleep(1); // Wait a short time
        gpioWrite(CLOCK_PIN, 0); // End pulse
        data |= gpioRead(DATA_PIN) << (23 - i); // Read bit and add to data
    }
    return data;
}

int main() {
    setup();
    while (1) {
        int weight = read_HX711();
        printf("Weight: %d\n", weight);
        time_sleep(0.3); // Wait a short time before next reading
    }
}
