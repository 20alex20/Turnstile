#include "DoorController.h"

DoorController::DoorController(uint8_t pin) : pin(pin) {
    servo.attach(pin);
}

void DoorController::openForEntry() {
    servo.write(POS_OPENED_IN);
}

void DoorController::openForExit() {
    servo.write(POS_OPENED_OUT);
}

void DoorController::closeDoor() {
    servo.write(POS_CLOSED);
}
