#include "WMPort.h"

WMPort::WMPort(void)
{
}


void WMPort::setOutput(uint8_t port)
{
	pinMode(port, OUTPUT);
}

void WMPort::setInput(uint8_t port)
{
	pinMode(port, INPUT);
}

void WMPort::setPuOn(uint8_t port)
{
	pinMode(port, INPUT_PULLUP);
}

bool WMPort::readDPort(uint8_t port)
{
	bool val;
  	setInput(port);
  	val = digitalRead(port);
  	return(val);
}

bool WMPort::readDPuPort(uint8_t port)
{
	bool val;
  	setPuOn(port);
  	val = digitalRead(port);
  	return(val);
}

int16_t WMPort::readAport(uint8_t port)
{
	int16_t val;
  	setInput(port);
  	val = analogRead(port);
  	return(val);
}

void WMPort::writeDport(uint8_t port,bool value)
{
	setOutput(port);
	digitalWrite(port, value);
}

void WMPort::writeAport(uint8_t port,int16_t value)
{
	analogWrite(port, value);
}
