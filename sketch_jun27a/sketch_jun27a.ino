#include <Arduino.h>

void setup()
{
	Serial1.begin(115200);
	Serial.begin(115200);
}

void loop()
{
	Serial1.println("AT+VER=?");

	while(!Serial1.available());

	String response;

	while(Serial1.available())
	{
		response += (char)Serial1.read();
	}

	Serial.print(response);
}
