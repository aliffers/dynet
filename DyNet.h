#ifndef DYNET_H
#define DYNET_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class DyNet {

  public:
		DyNet ();
		~DyNet ();
		int _redePin;
		void begin (int roPin, int diPin, int redePin);
		void receiveOpCode (void (*)(byte*));
		void sendOpCode (byte*);
		
  private:
		SoftwareSerial* _rs485;
		byte checksum (byte*);
		void setchecksum (byte*);
		bool checkchecksum (byte*);
		void rxMode ();
		void txMode ();

};

#endif
