#include <Arduino.h>
#include "DyNet.h"

DyNet::DyNet() {
	_rs485 = NULL;
}

DyNet::~DyNet () {
	if (_rs485 != NULL) delete _rs485;
}

void DyNet::begin (int roPin, int diPin, int redePin) {
	_redePin = redePin;
	pinMode (roPin, INPUT);
	pinMode (diPin, OUTPUT);
	pinMode (redePin, OUTPUT);
	_rs485 = new SoftwareSerial (roPin, diPin);
	_rs485->begin (9600);
	rxMode ();
}

void DyNet::rxMode () {
	digitalWrite (_redePin, LOW);
	_rs485->listen();
}

void DyNet::txMode () {
	digitalWrite (_redePin, HIGH);
}

void DyNet::receiveOpCode (void (*fn)(byte*)) {
	delay (10);
	byte opCode[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < 8; i++) {
		if (_rs485->available ())
			opCode[i] = _rs485->read ();
		else
			return;
		delay (1);
	}
	if (checksum (opCode)) fn (opCode);
}

void DyNet::sendOpCode (byte *opCode) {
	delay (10);
	setchecksum (opCode);
	txMode ();
	_rs485->write (opCode, 8);
	rxMode ();
}

byte DyNet::checksum (byte *opCode) { return (~(opCode[0] + opCode[1] + opCode[2] + opCode[3] + opCode[4] + opCode[5] + opCode[6]) + 1) & 0xFF; }
void DyNet::setchecksum (byte *opCode) { opCode[7] = checksum (opCode); }
bool DyNet::checkchecksum (byte *opCode) { return opCode[7] == checksum (opCode); }
