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

/// <summary>Hardware link from program to registers</summary>
void MSP430_GPIO::HardLink(void)
{
	// Get the port register pointer, then link them
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
}

/// <summary>Create a new GPIO object, set the location only and let other parameters to default</summary>
/// <param name="port">GPIO port</param>
/// <param name="pin">GPIO pin</param>
MSP430_GPIO::MSP430_GPIO(MSP430_GPIO_Port port, MSP430_GPIO_Pin pin)
{
	this->port = port;
	this->pin = pin;

	// Link the hardware
	HardLink();
}

/// <summary>Create a new GPIO object, set the location, function and direction</summary>
/// <param name="port">GPIO port</param>
/// <param name="pin">GPIO pin</param>
/// <param name="function">GPIO function</param>
/// <param name="direction">GPIO direction</param>
MSP430_GPIO::MSP430_GPIO(MSP430_GPIO_Port port, MSP430_GPIO_Pin pin, MSP430_GPIO_Function function, MSP430_GPIO_Direction direction) : MSP430_GPIO::MSP430_GPIO(port, pin)
{
	this->function = function;
	this->direction = direction;
}

/// <summary>Create a new GPIO object, set the location, function, direction and pullup/pulldown resistor</summary>
/// <param name="port">GPIO port</param>
/// <param name="pin">GPIO pin</param>
/// <param name="function">GPIO function</param>
/// <param name="direction">GPIO direction</param>
/// <param name="pullResistor">GPIO pullup/pulldown resistor enable/disable</param>
MSP430_GPIO::MSP430_GPIO(MSP430_GPIO_Port port, MSP430_GPIO_Pin pin, MSP430_GPIO_Function function, MSP430_GPIO_Direction direction, MSP430_GPIO_PullResistor pullResistor) : MSP430_GPIO::MSP430_GPIO(port, pin, function, direction)
{
	this->pullResistor = pullResistor;
}

/// <summary>Delete this GPIO instance, reset the hardware registers, and free up all memory space</summary>
MSP430_GPIO::~MSP430_GPIO()
{
	Deinitialize();
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

/// <summary>Clear the interrupt flag then interrupt can be re-detected</summary>
void MSP430_GPIO::ClearInterruptFlag(void)
{
	REG_SBIT0(reg_PxIFG, pin);
}

/// <summary>Set the corresponding GPIO pin output to HIGH(1) (Only effect when using standard function)</summary>
void MSP430_GPIO::SetHigh(void)
{
	REG_SBIT1(reg_PxOUT, pin);
}

/// <summary>Set the corresponding GPIO pin output to LOW(0) (Only effect when using standard function)</summary>
void MSP430_GPIO::SetLow(void)
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
MSP430_GPIO_Value MSP430_GPIO::GetValue(void)
{
	return REG_GBIT(reg_PxIN, pin);
}

/// <summary>Check if the corresponding GPIO pin input is HIGH(1)</summary>
bool MSP430_GPIO::CheckHigh(void)
{
	return (GetValue() == 1);
}

/// <summary>Check if the corresponding GPIO pin input is LOW(0)</summary>
bool MSP430_GPIO::CheckLow(void)
{
	return (GetValue() == 0);
}

/// <summary>
/// Dymanically reverse the corresponding pin's value
/// <para>NOTE: This function will read and effect on register directly.</para>
/// <para>NOTE: This function is only effect when using standard function.</para>
/// </summary>
void MSP430_GPIO::ReverseValue(void)
{
	// Read and write
	SetValue(~GetValue());
}

/// <summary>Reverse the corresponding GPIO's direction
/// <para>NOTE: This function will read and effect on register directly.</para>
/// </summary>
void MSP430_GPIO::ReverseDirection(void)
{
	// Reverse the direction
	if (this->direction == MSP430_GPIO_Direction::Input)
	{
		this->direction = MSP430_GPIO_Direction::Output;
	}
	else
	{
		this->direction = MSP430_GPIO_Direction::Input;
	}

	// Set pin direction
	REG_SBIT(this->reg_PxDIR, this->pin, static_cast<int> (this->direction));
}

/// <summary>
/// Dymanically set the corresponding pin's direction
/// <para>NOTE: This function will effect on register directly.</para>
/// </summary>
/// <param name="direction">Setting direction</param>
void MSP430_GPIO::SetDirection(MSP430_GPIO_Direction direction)
{
	this->direction = direction;
	REG_SBIT(this->reg_PxDIR, this->pin, static_cast<int> (this->direction));
}

/// <summary>
/// Get the corresponding pin's direction
/// </summary>
MSP430_GPIO_Direction MSP430_GPIO::GetDirection(void)
{
	return this->direction;
}

/// <summary>
/// Dymanically set the corresponding pin's function
/// <para>NOTE: This function will effect on register directly.</para>
/// </summary>
/// <param name="function">Setting function</param>
void MSP430_GPIO::SetFunction(MSP430_GPIO_Function function)
{
	this->function = function;

	unsigned char func = static_cast<unsigned char> (this->function);
	REG_SBIT(this->reg_PxSEL, this->pin, func & 0x01);
#ifdef GPIO_PORT_HAS_FUNSEL2
	REG_SBIT(gpio.reg_PxSEL2, this->pin, (func & 0x02) >> 1);
#endif
}

/// <summary>
/// Dymanically set the corresponding pin's pullup/pulldown resistor
/// <para>NOTE: This function will effect on register directly.</para>
/// </summary>
/// <param name="direction">Switch of the pullup/pulldown resistor</param>
void MSP430_GPIO::SetPullResistor(MSP430_GPIO_PullResistor sw)
{
	this->pullResistor = sw;
	REG_SBIT(this->reg_PxREN, this->pin, static_cast<int> (this->pullResistor));
}

/// <summary>Initialize a hardware GPIO by this GPIO instance</summary>
void MSP430_GPIO::Initialize(void)
{
	// Set pin direction
	REG_SBIT(this->reg_PxDIR, this->pin, static_cast<int> (this->direction));
	
	// Set pullup/pulldown resistor
	REG_SBIT(this->reg_PxREN, this->pin, static_cast<int> (this->pullResistor));

	// Set function
	unsigned char func = static_cast<unsigned char> (this->function);
	REG_SBIT(this->reg_PxSEL, this->pin, func & 0x01);
#ifdef GPIO_PORT_HAS_FUNSEL2
	REG_SBIT(gpio.reg_PxSEL2, this->pin, (func & 0x02) >> 1);
#endif
}

/// <summary>
/// Deinitialize the corresponding hardware GPIO and set all registers to default.
/// <para>(Default: Input, Standard Function, Resistor Off)</para>
/// </summary>
void MSP430_GPIO::Deinitialize(void)
{
	MSP430_GPIO_Pin pin = this->pin;

	// Set pin direction
	REG_SBIT(this->reg_PxDIR, pin, static_cast<int> (MSP430_GPIO_Direction::Input));

	// Set pullup/pulldown resistor
	REG_SBIT(this->reg_PxREN, pin, static_cast<int> (MSP430_GPIO_PullResistor::Off));

	// Set function
	unsigned char func = static_cast<unsigned char> (this->function);
	REG_SBIT(this->reg_PxSEL, pin, 0);
#ifdef GPIO_PORT_HAS_FUNSEL2
	REG_SBIT(gpio.reg_PxSEL2, pin, 0);
#endif
}

/// <summary>Hardware link from program to registers</summary>
void MSP430_GPIO_Bank::HardLink(void)
{
	// Get the port register pointer, then link them
	MSP430_GPIO_Port port = this->port;
	this->reg_PxIN = PxIN[static_cast<int> (port)];
	this->reg_PxOUT = PxOUT[static_cast<int> (port)];
	this->reg_PxDIR = PxDIR[static_cast<int> (port)];
	this->reg_PxREN = PxREN[static_cast<int> (port)]; 
}

/// <summary>Create a new GPIO bank object, set the port</summary>
/// <param name="port">GPIO port</param>
MSP430_GPIO_Bank::MSP430_GPIO_Bank(MSP430_GPIO_Port port)
{
	this->port = port;
	
	// Link the hardware
	HardLink();
}

/// <summary>Create a new GPIO bank object, set the port and direction</summary>
/// <param name="port">GPIO port</param>
/// <param name="direction">GPIO direction</param>
MSP430_GPIO_Bank::MSP430_GPIO_Bank(MSP430_GPIO_Port port, MSP430_GPIO_Direction direction) : MSP430_GPIO_Bank(port)
{
	this->direction = direction;
}

/// <summary>Create a new GPIO bank object, set the port, direction and pullup/pulldown resistor</summary>
/// <param name="port">GPIO port</param>
/// <param name="direction">GPIO direction</param>
/// <param name="pullResistor">GPIO pullup/pulldown resistor enable/disable</param>
MSP430_GPIO_Bank::MSP430_GPIO_Bank(MSP430_GPIO_Port port, MSP430_GPIO_Direction direction, MSP430_GPIO_PullResistor pullResistor) : MSP430_GPIO_Bank(port, direction)
{
	this->pullResistor = pullResistor;
}

/// <summary>Delete this GPIO bank instance, reset the hardware registers, and free up all memory space</summary>
MSP430_GPIO_Bank::~MSP430_GPIO_Bank()
{
	Deinitialize();
}

/// <summary>
/// Directly set the access mask for GPIO bank.
/// <para>The data you read from or write to the GPIO bank will be mask with your setting.
/// The 1 bit means data allowed, 0 bit means data no effect/para>
/// </summary>
/// <param name="mask">Access mask</param>
void MSP430_GPIO_Bank::SetAccessMask(unsigned char mask)
{
	this->accessMask = mask;
}

/// <summary>
/// Directly set the access mask for GPIO bank.
/// <para>The data you read from or write to the GPIO bank will be mask with your setting.
/// The significant bit is start from START and end to END</para>
/// </summary>
/// <param name="start">Least significant bit (LSB)</param>
/// <param name="end">Most significant bit (MSB)</param>
void MSP430_GPIO_Bank::SetAccessMask(unsigned char start, unsigned char end)
{
	unsigned char mask = (1 << (end + 1)) - 1;
	mask &= ~((1 << start) - 1);
	this->accessMask = mask;
}

/// <summary>Initialize a hardware GPIO bank by this GPIO bank instance</summary>
void MSP430_GPIO_Bank::Initialize(void)
{
	// Select the stardand I/O function
	REG_WM(PxSEL[static_cast<int> (port)], 0x00, accessMask);
#ifdef GPIO_PORT_HAS_FUNSEL2
	REG_WM(PxSEL2[static_cast<int> (port)], 0x00, accessMask);
#endif
	
	// Close the interrupt
	REG_WM(PxIE[static_cast<int> (port)], 0x00, accessMask);

	// Set pin direction
	REG_WM(this->reg_PxDIR, static_cast<int> (this->direction) ? 0xFF : 0x00, accessMask);

	// Set pullup/pulldown resistor
	REG_WM(this->reg_PxREN, static_cast<int> (this->pullResistor) ? 0xFF : 0x00, accessMask);
}

/// <summary>
/// Deinitialize the corresponding hardware GPIO and set all registers to default.
/// <para>(Default: Input, Resistor Off)</para>
/// </summary>
void MSP430_GPIO_Bank::Deinitialize(void)
{
	// Set direction
	REG_WM(this->reg_PxDIR, 0x00, accessMask);

	// Set pullup/pulldown resistor
	REG_WM(this->reg_PxREN, 0x00, accessMask);
}

/// <summary>
/// Set the GPIO bank output by a setting value
/// <para>NOTE:The value data is masked by access mask.</para>
/// </summary>
/// <param name="value">GPIO bank output value</param>
void MSP430_GPIO_Bank::SetValue(unsigned char value)
{
	REG_WM(this->reg_PxOUT, value, accessMask);
}

/// <summary>
/// Get the GPIO bank input
/// <para>NOTE:The return data is masked by access mask.</para>
/// </summary>
unsigned char MSP430_GPIO_Bank::GetValue(void)
{
	return REG_RM(this->reg_PxIN, accessMask);
}

/// <summary>
/// Dymanically reverse the corresponding GPIO bank's value
/// <para>NOTE: This function will read and effect on register directly.</para>
/// </summary>
void MSP430_GPIO_Bank::ReverseValue(void)
{
	SetValue(~GetValue());
}

/// <summary>Reverse the corresponding GPIO  bank's direction
/// <para>NOTE: This function will read and effect on register directly.</para>
/// </summary>
void MSP430_GPIO_Bank::ReverseDirection(void)
{
	// Reverse the direction
	if (this->direction == MSP430_GPIO_Direction::Input)
	{
		this->direction = MSP430_GPIO_Direction::Output;
	}
	else
	{
		this->direction = MSP430_GPIO_Direction::Input;
	}

	// Set pin direction
	REG_WM(this->reg_PxDIR, static_cast<int> (this->direction) ? 0xFF : 0x00, accessMask);
}

/// <summary>
/// Dymanically set the corresponding bank's direction
/// <para>NOTE: This function will effect on register directly.</para>
/// </summary>
/// <param name="direction">Setting direction</param>
void MSP430_GPIO_Bank::SetDirection(MSP430_GPIO_Direction direction)
{
	this->direction = direction;

	// Set pin direction
	REG_WM(this->reg_PxDIR, static_cast<int> (this->direction) ? 0xFF : 0x00, accessMask);
}

/// <summary>
/// Get the corresponding bank's direction
/// </summary>
MSP430_GPIO_Direction MSP430_GPIO_Bank::GetDirection(void)
{
	return this->direction;
}

/// <summary>
/// Dymanically set the corresponding bank's pullup/pulldown resistor
/// <para>NOTE: This function will effect on register directly.</para>
/// </summary>
void MSP430_GPIO_Bank::SetPullResistor(MSP430_GPIO_PullResistor sw)
{
	this->pullResistor = sw;

	// Set pullup/pulldown resistor
	REG_WM(this->reg_PxREN, static_cast<int> (this->pullResistor) ? 0xFF : 0x00, accessMask);
}








