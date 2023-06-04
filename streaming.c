#include <libwebsockets.h>
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>

#define CLOCK_PIN 27
#define DATA_PIN 22
#define SERVER_PORT 8080

char weight_string[20] = {0};
int new_data = 0;

struct lws_context *context = NULL;

static int callback_server(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);

static struct lws_protocols protocols[] = {
    {
        "weight-data",   // name
        callback_server, // callback
        0, // per_session_data_size
        0, // rx_buffer_size
    },
    { NULL, NULL, 0, 0 } // terminator
};

void setup() {
    if (gpioInitialise() < 0) {
        printf("Failed to initialize GPIOs\n");
        exit(1);
    }
    gpioSetMode(CLOCK_PIN, PI_OUTPUT);
    gpioSetMode(DATA_PIN, PI_INPUT);
    gpioWrite(CLOCK_PIN, 0);
}

int read_HX711() {
    int data = 0;
    int count = 0;
    while (gpioRead(DATA_PIN)); // Wait for DATA_PIN to go low
    for (int i = 0; i < 26; i++) {
        gpioWrite(CLOCK_PIN, 1); // Send pulse
        gpioWrite(CLOCK_PIN, 0); // End pulse
        data |= gpioRead(DATA_PIN) << (23 - i); // Read bit and add to data
        count = i;
    }
    return data;
}

static int callback_server(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED:
            printf("Client has connected to server\n");
            break;

        case LWS_CALLBACK_SERVER_WRITEABLE:
            if (new_data) {
                unsigned char buf[LWS_PRE + sizeof(weight_string)];
                unsigned char *p = &buf[LWS_PRE];
                size_t n = sprintf((char *)p, "%s", weight_string);
                lws_write(wsi, p, n, LWS_WRITE_TEXT);
                new_data = 0;
            }
            break;

        default:
            break;
    }
    return 0;
}

void setup_websockets() {
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof info);
    info.port = SERVER_PORT; // The port our server will listen on
    info.protocols = protocols;

    context = lws_create_context(&info);
    if (!context) {
        fprintf(stderr, "Creating libwebsocket context failed\n");
        exit(1);
    }
}

int main() {
    setup();
    setup_websockets();

    while (!gpioRead(DATA_PIN)); // Wait for DATA_PIN to go high
    while (1) {
        int weight = read_HX711();
        printf("Weight: %d\n", weight);
        snprintf(weight_string, sizeof(weight_string), "%d", weight);
        new_data = 1;

        lws_callback_on_writable_all_protocol(context, &protocols[0]);

        time_sleep(0.1); // Wait a short time before next reading
        lws_service(context, 0);
    }
}