#include "DoorController.h"

DoorController::DoorController(uint8_t pin) {
    servo.attach(pin);
    closeDoor();
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
