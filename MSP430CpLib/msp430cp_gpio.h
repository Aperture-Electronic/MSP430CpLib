#pragma once

// GPIO location enumerations and definations
/// <summary>
/// GPIO Port IO (Each port has up to 8 GPIO pins. Only P1/P2 port have interrupt capability)
/// <para>NOTE: The port is usable or not, is depending on the device, see also the device's datasheet to get more information.</para>
/// </summary>
enum class MSP430_GPIO_Port
{
	P1,
	P2,
	P3,
	P4,
	P5,
	P6,
	P7,
	P8
};

// GPIO functions/modes configurations enumerations
/// <summary>
/// GPIO Direction (Results in PxDIR register)
/// <para>Selects the direction of the corresponding GPIO pin.</para>
/// <para>NOTE: GPIO pins that are selected for other functions must be set as required by the other function.</para>
/// </summary>
enum class MSP430_GPIO_Direction
{
	Input,
	Output
};

/// <summary>
/// GPIO Function (Results in PxSEL/PxSEL2 register)
/// <para>Selects the function of the corresponding GPIO pin. 
/// (Reserved(01) means no funtion/specific function, see also corresponding MSP430 device's datasheet to get more information)</para>
/// <para>NOTE: Setting the IO function does NOT automatically set the pin direction.</para>
/// <para>NOTE: On some I/O ports on the MSP430F261x and MSP430F2416/7/8/9, 
/// setting pullup/pulldown enabled when primary/secondary peripheral selected does not disable the logic output driver.
/// This combination is not recommended and may result in unwanted current flow through the internal resistor.</para>
/// <para>NOTE : On P1/P2, primary/secondary peripheral function enabling does disable the corresponding pin's interrupt function. 
/// Therefore, signals on these pins will not generate P1/P2 interrupts, regardless of the state of the corresponding P1IE/P2IE bit.</para>
/// </summary>
enum class MSP430_GPIO_Function
{
	Stardand,
	Primary,
	Reserved,
	Secondary
};

/// <summary>
/// GPIO Pullup/Pulldown Resistor (Results in PxREN register)
/// <para>Enables or disables the pullup/pulldown resistor of the corresponding GPIO pin.</para>
/// </summary>
enum class MSP430_GPIO_PullResistor
{
	On,
	Off
};

// GPIO interrupt configuration enumerations
/// <summary>
/// GPIO Interrupt Enable/Disable Switch (Results in PxIE register)
/// <para>Enable/disable the associated PxIFG interrupt flag</para>
/// </summary>
enum class MSP430_GPIO_InterruptSwitch
{
	On,
	Off
};

/// <summary>
/// GPIO Interrupt Edge Select (Results in PxIES register)
/// <para>Selects the interrupt edge for the corresponding GPIO pin.</para>
/// <para>NOTE: Writing to PxIESx when interrupt enabled can result in setting the corresponding interrupt flags.</para>
/// <para>Interrupt flag will set when: Posedge->Negedge and pin input 0, or Negedge->Posedge and pin input 1.</para>
/// </summary>
enum class MSP430_GPIO_InterruptTrig
{
	/// <summary>Interrupt when input 0 -> 1 (Verilog: always@(posedge pin))</summary>
	Posedge,
	/// <summary>Interrupt when input 1 -> 0 (Verilog: always@(negedge pin))</summary>
	Negedge
};

/// <summary>
/// MSP430 GPIO(General purpose I/O) pin class
/// </summary>
class MSP430_GPIO_Pin
{
private:
	// Corresponding GPIO location (Port and ID)


	// Corresponding GPIO function/mode configuration
	// Pin function
	MSP430_GPIO_Function function;
	// Pin direction
	MSP430_GPIO_Direction direction;
	// Pullup/pulldown resistor
	MSP430_GPIO_PullResistor pullResistor;

	// Corrsponding GPIO interrupt configuration
	// Interrupt switch
	MSP430_GPIO_InterruptSwitch interruptSw;
	// Interrupt trig mode
	MSP430_GPIO_InterruptTrig interruptTrig;

public:
	MSP430_GPIO_Pin();

};
