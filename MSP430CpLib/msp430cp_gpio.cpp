#include <msp430.h>
#include "msp430cp_device.h"
#include "msp430cp_gpio.h"

// GPIO registers
extern REG_8b PxIN[GPIO_PORT_COUNT];
extern REG_8b PxOUT[GPIO_PORT_COUNT];
extern REG_8b PxDIR[GPIO_PORT_COUNT];
extern REG_8b PxREN[GPIO_PORT_COUNT];
extern REG_8b PxSEL[GPIO_PORT_COUNT];
#ifdef GPIO_PORT_HAS_FUNSEL2
extern REG_8b PxSEL2[GPIO_PORT_COUNT];
#endif
#ifdef GPIO_PORT_HAS_DS
extern REG_8b PxDS[GPIO_PORT_COUNT];
#endif
extern REG_8b PxIE[GPIO_PORT_SUPPORT_INT_COUNT];
extern REG_8b PxIFG[GPIO_PORT_SUPPORT_INT_COUNT];
extern REG_8b PxIES[GPIO_PORT_SUPPORT_INT_COUNT];

/// <summary>Create a new GPIO object, set the location only and let other parameters to default</summary>
/// <param name="port">GPIO port</param>
/// <param name="port">GPIO pin</param>
MSP430_GPIO::MSP430_GPIO(MSP430_GPIO_Port port, MSP430_GPIO_Pin pin)
{
	this->port = port;
	this->pin = pin;
}

/// <summary>Create a new GPIO object, set the location, function and direction</summary>
/// <param name="port">GPIO port</param>
/// <param name="port">GPIO pin</param>
/// <param name="function">GPIO function</param>
/// <param name="direction">GPIO direction</param>
MSP430_GPIO::MSP430_GPIO(MSP430_GPIO_Port port, MSP430_GPIO_Pin pin, MSP430_GPIO_Function function, MSP430_GPIO_Direction direction) : MSP430_GPIO::MSP430_GPIO(port, pin)
{
	this->function = function;
	this->direction = direction;
}

/// <summary>Create a new GPIO object, set the location, function, direction and pullup/pulldown resistor</summary>
/// <param name="port">GPIO port</param>
/// <param name="port">GPIO pin</param>
/// <param name="function">GPIO function</param>
/// <param name="direction">GPIO direction</param>
/// <param name="pullResistor">GPIO pullup/pulldown resistor enable/disable</param>
MSP430_GPIO::MSP430_GPIO(MSP430_GPIO_Port port, MSP430_GPIO_Pin pin, MSP430_GPIO_Function function, MSP430_GPIO_Direction direction, MSP430_GPIO_PullResistor pullResistor) : MSP430_GPIO::MSP430_GPIO(port, pin, function, direction)
{
	this->pullResistor = pullResistor;
}

/// <summary>Enable the corresponding pin's interrupt and set its trig edge</summary>
/// <param name="interruptTrig">Trig edge for interrupt</param>
void MSP430_GPIO::EnableInterrupt(MSP430_GPIO_InterruptTrig interruptTrig)
{
	this->interruptSw = MSP430_GPIO_InterruptSwitch::On;
	this->interruptTrig = interruptTrig;

	// Set interrupt
	REG_SBIT(reg_PxIE, pin, static_cast<int> (this->interruptSw));
	REG_SBIT(reg_PxIES, pin, static_cast<int> (this->interruptTrig));
}

/// <summary>Disable the corresponding pin's interrupt</summary>
void MSP430_GPIO::DisableInterrupt(void)
{
	this->interruptSw = MSP430_GPIO_InterruptSwitch::Off;

	// Set interrupt
	REG_SBIT(reg_PxIE, pin, static_cast<int> (this->interruptSw));
}

/// <summary>Check if the interrupt flag on corresponding pin was setted</summary>
bool MSP430_GPIO::CheckInterruptFlag(void)
{
	return REG_GBIT(reg_PxIFG, pin);
}

void MSP430_GPIO::ClearInterruptFlag(void)
{
	REG_SBIT0(reg_PxIFG, pin);
}

/// <summary>Set the corresponding GPIO pin output to HIGH(1) (Only effect when using standard function)</summary>
void MSP430_GPIO::SetHigh()
{
	REG_SBIT1(reg_PxOUT, pin);
}

/// <summary>Set the corresponding GPIO pin output to LOW(0) (Only effect when using standard function)</summary>
void MSP430_GPIO::SetLow()
{
	REG_SBIT0(reg_PxOUT, pin);
}

/// <summary>Set the corresponding GPIO pin output by a setting value (Only effect when using standard function)</summary>
/// <param name="value">GPIO pin output value</param>
void MSP430_GPIO::SetValue(MSP430_GPIO_Value value)
{
	REG_SBIT(reg_PxOUT, pin, value);
}

/// <summary>Get the corresponding GPIO pin input</summary>
/// <return>GPIO pin input value</return>
MSP430_GPIO_Value MSP430_GPIO::GetValue()
{
	return REG_GBIT(reg_PxIN, pin);
}

/// <summary>Check if the corresponding GPIO pin input is HIGH(1)</summary>
bool MSP430_GPIO::CheckHigh()
{
	return (GetValue() == 1);
}

/// <summary>Check if the corresponding GPIO pin input is LOW(0)</summary>
bool MSP430_GPIO::CheckLow()
{
	return (GetValue() == 0);
}

/// <summary>Initialize a hardware GPIO by this GPIO instance</summary>
void MSP430_GPIO::Initialize()
{
	// Get the port register pointer
	MSP430_GPIO_Port port = this->port;
	this->reg_PxIN = PxIN[static_cast<int> (port)];
	this->reg_PxOUT = PxOUT[static_cast<int> (port)];
	this->reg_PxDIR = PxDIR[static_cast<int> (port)];
	this->reg_PxREN = PxREN[static_cast<int> (port)];
	this->reg_PxSEL = PxSEL[static_cast<int> (port)];
#ifdef GPIO_PORT_HAS_FUNSEL2
	this->reg_PxSEL2 = PxSEL2[static_cast<int> (port)];
#endif
#ifdef GPIO_PORT_HAS_DS
	this->reg_PxDS = PxDS[static_cast<int> (port)];
#endif
	this->reg_PxIE = PxIE[static_cast<int> (port)];
	this->reg_PxIFG = PxIFG[static_cast<int> (port)];
	this->reg_PxIES = PxIES[static_cast<int> (port)];

	MSP430_GPIO_Pin pin = this->pin;

	// Set pin direction
	REG_SBIT(this->reg_PxDIR, pin, static_cast<int> (this->direction));
	
	// Set pullup/pulldown resistor
	REG_SBIT(this->reg_PxREN, pin, static_cast<int> (this->pullResistor));

	// Set function
	unsigned char func = static_cast<unsigned char> (this->function);
	REG_SBIT(this->reg_PxSEL, pin, func & 0x01);
#ifdef GPIO_PORT_HAS_FUNSEL2
	REG_SBIT(gpio.reg_PxSEL2, pin, (func & 0x02) >> 1);
#endif
}
