
#ifndef WMPort_H
#define WMPort_H

#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct
{
	uint8_t pin1;
	uint8_t pin2;
}WMPin;


extern WMPin wmPort[9];

class WMPort
{
public:
	WMPort(void);

	void setOutput(uint8_t port);
	void setInput(uint8_t port);
	void setPuOn(uint8_t port);
	bool readDPort(uint8_t port);
	bool readDPuPort(uint8_t port);
	int16_t readAport(uint8_t port);
	void writeDport(uint8_t port,bool value);
	void writeAport(uint8_t port,int16_t value);
	
};
#endif
