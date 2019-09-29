#include <msp430.h>
#include "msp430cp_registers.h"

// GPIO location enumerations and definations
/// <summary>
/// GPIO Port (Each port has up to 8 GPIO pins. Only P1/P2 port have interrupt capability)
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

/// <summary>
/// GPIO Pin Id
/// </summary>
typedef unsigned char MSP430_GPIO_Pin;

// GPIO status definations
/// <summary>
/// GPIO status(High or low) defined by a 8-bits unsigned integer (0 for low, other for high)
/// </summary>
typedef unsigned char MSP430_GPIO_Value;

// GPIO functions/modes configurations enumerations
/// <summary>
/// GPIO Direction (Results in PxDIR register)
/// <para>Selects the direction of the corresponding GPIO pin.</para>
/// <para>NOTE: GPIO pins that are selected for other functions must be set as required by the other function.</para>
/// </summary>
enum class MSP430_GPIO_Direction
{
	Input = 0,
	Output = 1
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
	Stardand = 0b00,
	Primary = 0b01,
	Reserved = 0b10,
	Secondary = 0b11
};

/// <summary>
/// GPIO Pullup/Pulldown Resistor (Results in PxREN register)
/// <para>Enables or disables the pullup/pulldown resistor of the corresponding GPIO pin.</para>
/// </summary>
enum class MSP430_GPIO_PullResistor
{
	Off = 0,
	On = 1
};

// GPIO interrupt configuration enumerations
/// <summary>
/// GPIO Interrupt Enable/Disable Switch (Results in PxIE register)
/// <para>Enable/disable the associated PxIFG interrupt flag</para>
/// </summary>
enum class MSP430_GPIO_InterruptSwitch
{
	Off = 0,
	On = 1
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
	Posedge = 0,
	/// <summary>Interrupt when input 1 -> 0 (Verilog: always@(negedge pin))</summary>
	Negedge = 1
};

/// <summary>
/// MSP430 GPIO(General purpose I/O) pin class
/// </summary>
class MSP430_GPIO
{
private:
	// Register for hardware operation
	REG_8b reg_PxIN;
	REG_8b reg_PxOUT;
	REG_8b reg_PxDIR;
	REG_8b reg_PxREN;
	REG_8b reg_PxSEL;
#ifdef GPIO_PORT_HAS_FUNSEL2
	REG_8b reg_PxSEL2;
#endif
#ifdef GPIO_PORT_HAS_DS
	REG_8b reg_PxDS
#endif
	REG_8b reg_PxIE;
	REG_8b reg_PxIFG;
	REG_8b reg_PxIES;

	// Corresponding GPIO location (Port and ID)

	// Port
	MSP430_GPIO_Port port;

	// Pin Id
	MSP430_GPIO_Pin pin;
	

	// Corresponding GPIO function/mode configuration

	// Pin function
	MSP430_GPIO_Function function = MSP430_GPIO_Function::Stardand;
	// Pin direction
	MSP430_GPIO_Direction direction = MSP430_GPIO_Direction::Output;
	// Pullup/pulldown resistor
	MSP430_GPIO_PullResistor pullResistor = MSP430_GPIO_PullResistor::Off;

	// Corrsponding GPIO interrupt configuration

	// Interrupt switch
	MSP430_GPIO_InterruptSwitch interruptSw = MSP430_GPIO_InterruptSwitch::Off;
	// Interrupt trig mode
	MSP430_GPIO_InterruptTrig interruptTrig;

public:
	// Constructor
	MSP430_GPIO(MSP430_GPIO_Port port, MSP430_GPIO_Pin pin);
	MSP430_GPIO(MSP430_GPIO_Port port, MSP430_GPIO_Pin pin, MSP430_GPIO_Function function, MSP430_GPIO_Direction direction);
	MSP430_GPIO(MSP430_GPIO_Port port, MSP430_GPIO_Pin pin, MSP430_GPIO_Function function, MSP430_GPIO_Direction direction, MSP430_GPIO_PullResistor pullResistor);

	// Interrupt control (Separate control)
	void EnableInterrupt(MSP430_GPIO_InterruptTrig interruptTrig);
	void DisableInterrupt(void);
	bool CheckInterruptFlag(void);
	void ClearInterruptFlag(void);

	// GPIO initialize or re-configuration
	void Initialize();

	// Stardand GPIO operation
	void SetHigh();
	void SetLow();
	void SetValue(MSP430_GPIO_Value status);
	MSP430_GPIO_Value GetValue();
	bool CheckHigh();
	bool CheckLow();
};
