#include <rocs.hpp>

const char* ROCS_NAME = "led_example";

const int LED_PIN = 13;

uint8_t selector = 0;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // Generally, three things need to happen at startup.
    // 1. Rocs must be initialized.
    // 2. Optionally, a read handler could be set (if reading from the
    //    device is desired).
    // 3. Optionally, a write handler could be set (if writing to the
    //    device is desired).

    // Initialize rocs. This is required.
    // First argument is the I2C address of this Rocs Responder.
    // Second argument is the name of this Rocs Responder.
    // Third argument is the length of the name string.
    rocs::init(0x11, ROCS_NAME, strlen(ROCS_NAME));

    // Set the read handler. This is a function with the following signature:
    //     uint8_t my_read_handler_name(uint8_t register);
    // This function gets called every time the Rocs Controller reads from a register
    // on this Responder.
    rocs::set_read_handler(read_handler);

    // Set the write handler. This is a function with the following signature:
    //     void my_write_handler_name(uint8_t register, uint8_t value);
    // This function gets called every time the Rocs Controller writes to a register
    // on this Responder.
    rocs::set_write_handler(write_handler);
}

void write_handler(uint8_t reg, uint8_t val) {
    switch (reg) {
        case 1:
            selector = val;
            break;
    }
}

uint8_t read_handler(uint8_t reg) {
    switch (reg) {
        case 1:
            return selector;
    }

    return 0;
}

void loop() {
    if (selector == 1) {
        digitalWrite(LED_PIN, HIGH);
    } else {
        digitalWrite(LED_PIN, LOW);
    }

    delay(20);
}
