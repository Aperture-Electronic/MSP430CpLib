#include <msp430.h>
#include "msp430cp_gpio.h"

void Delay(int k)
{
	long counter = 0;
	while (counter++ < k)
		asm("nop");
}


void MainLoop()
{
	WDTCTL = WDTPW | WDTHOLD;
	
	MSP430_GPIO_Bank leds = MSP430_GPIO_Bank(MSP430_GPIO_Port::P1, MSP430_GPIO_Direction::Output);
	leds.SetAccessMask(1, 3);
	leds.Initialize();
	
	for (;;)
	{
		leds.ReverseValue();
		Delay(5000);
		leds.ReverseValue();
		Delay(5000);
	}
}

int main()
{
	MainLoop();
}

