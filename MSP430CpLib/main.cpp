#include <msp430.h>

void Delay()
{
	long counter = 0;
	while (counter++ < 5000)
		asm("nop");
}

void MainLoop()
{
	WDTCTL = WDTPW | WDTHOLD;
	
	P1DIR |= (1 << 0);
	
	for (;;)
	{
		P1OUT |= (1 << 0);
		Delay();
		P1OUT &= ~(1 << 0);
		Delay();
	}
}

int main()
{
	MainLoop();
}