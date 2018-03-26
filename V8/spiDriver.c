// SPI serial communaction library for rgb led lights strip using WS2801 chip
#pragma systemFile // get rid of unused function warnings


void sendByte(int byte) //Send 8 bits, most significant to least significant.
{
  for(int i = 128; i >= 1; i = i / 2)
  {
    SensorValue[data] = byte & i;
    SensorValue[clock] = 1;
    //This is where you may want to add a delay if it is being unreliable
    SensorValue[clock] = 0;
  }
}

void startTransmission() //Send Start Frame
{
	for(int i = 0; i < 4; i++)
	{
		sendByte(0);
	}
}

void endTransmission() //Send End Frame
{
	for(int i = 0; i < 4; i++)
	{
		sendByte(255);
	}
}

//Send a single LED frame, these get pushed down the strip as more and more are added
//Brightness: From 0 to 31, an LED-wide modifier to the brightness
//R,G,B: The RGB color value of the LED
void sendLEDFrame(int brightness, int r, int g, int b)
{
	sendByte(224 | brightness); //The brightness byte is 111xxxxx, there are only 5 bits that you can use
	sendByte(b);
	sendByte(g);
	sendByte(r);
}

//Sets the whole strip's color and brightness
//Length: How many LEDs are in the strip
//Brightness, R,G,B: Same as above
void setStripColor(int length, int brightness, int r, int g, int b) //Set the whole strip to a uniform color.
{
	startTransmission();
	for(int i = 0; i < length; i++)
	{
		sendLEDFrame(brightness, r, g, b);
	}
	endTransmission();
}
