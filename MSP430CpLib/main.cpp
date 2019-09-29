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
	
	MSP430_GPIO led = MSP430_GPIO(MSP430_GPIO_Port::P4, 7);
	led.Initialize();
	led.DisableInterrupt();
	
	for (;;)
	{
		led.SetHigh();
		Delay(5000);
		led.SetLow();
		Delay(5000);
	}
}

int main()
{
	MainLoop();
}

