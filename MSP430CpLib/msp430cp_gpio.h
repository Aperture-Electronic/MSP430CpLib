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

// GPIO value definations
/// <summary>
/// GPIO value(High or low) defined by a 8-bits unsigned integer (0 for low, other for high)
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
	/// <summary>Port</summary>
	MSP430_GPIO_Port port;
	/// <summary>Pin Id</summary>
	MSP430_GPIO_Pin pin;
	
	// Corresponding GPIO function/mode configuration
	/// <summary>Pin function</summary>
	MSP430_GPIO_Function function = MSP430_GPIO_Function::Stardand;
	/// <summary>Pin direction</summary>
	MSP430_GPIO_Direction direction = MSP430_GPIO_Direction::Output;
	/// <summary>Pullup/pulldown resistor</summary>
	MSP430_GPIO_PullResistor pullResistor = MSP430_GPIO_PullResistor::Off;

	// Corrsponding GPIO interrupt configuration
	/// <summary>Interrupt switch</summary>
	MSP430_GPIO_InterruptSwitch interruptSw = MSP430_GPIO_InterruptSwitch::Off;
	/// <summary>Interrupt trig mode</summary>
	MSP430_GPIO_InterruptTrig interruptTrig;

	// Private low-level linking functions
	/// <summary>Hardware link from program to registers</summary>
	void HardLink(void);

public:
	// Constructor
	/// <summary>Create a new GPIO object, set the location only and let other parameters to default</summary>
	/// <param name="port">GPIO port</param>
	/// <param name="pin">GPIO pin</param>
	MSP430_GPIO(MSP430_GPIO_Port port, MSP430_GPIO_Pin pin);
	/// <summary>Create a new GPIO object, set the location, function and direction</summary>
	/// <param name="port">GPIO port</param>
	/// <param name="pin">GPIO pin</param>
	/// <param name="function">GPIO function</param>
	/// <param name="direction">GPIO direction</param>
	MSP430_GPIO(MSP430_GPIO_Port port, MSP430_GPIO_Pin pin, MSP430_GPIO_Function function, MSP430_GPIO_Direction direction);
	/// <summary>Create a new GPIO object, set the location, function, direction and pullup/pulldown resistor</summary>
	/// <param name="port">GPIO port</param>
	/// <param name="pin">GPIO pin</param>
	/// <param name="function">GPIO function</param>
	/// <param name="direction">GPIO direction</param>
	/// <param name="pullResistor">GPIO pullup/pulldown resistor enable/disable</param>
	MSP430_GPIO(MSP430_GPIO_Port port, MSP430_GPIO_Pin pin, MSP430_GPIO_Function function, MSP430_GPIO_Direction direction, MSP430_GPIO_PullResistor pullResistor);
	/// <summary>Delete this GPIO instance, reset the hardware registers, and free up all memory space</summary>
	~MSP430_GPIO();

	// Interrupt control (Separate control)
	/// <summary>Enable the corresponding pin's interrupt and set its trig edge</summary>
	/// <param name="interruptTrig">Trig edge for interrupt</param>
	void EnableInterrupt(MSP430_GPIO_InterruptTrig interruptTrig);
	/// <summary>Disable the corresponding pin's interrupt</summary>
	void DisableInterrupt(void);
	/// <summary>Check if the interrupt flag on corresponding pin was setted</summary>
	bool CheckInterruptFlag(void);
	/// <summary>Clear the interrupt flag then interrupt can be re-detected</summary>
	void ClearInterruptFlag(void);

	// GPIO initialize or re-configuration
	/// <summary>Initialize a hardware GPIO by this GPIO instance</summary>
	void Initialize(void);
	/// <summary>
	/// Deinitialize the corresponding hardware GPIO and set all registers to default.
	/// <para>(Default: Input, Standard Function, Resistor Off)</para>
	/// </summary>
	void Deinitialize(void);

	// Stardand GPIO operation
	/// <summary>Set the corresponding GPIO pin output to HIGH(1) (Only effect when using standard function)</summary>
	void SetHigh(void);
	/// <summary>Set the corresponding GPIO pin output to LOW(0) (Only effect when using standard function)</summary>
	void SetLow(void);
	/// <summary>Set the corresponding GPIO pin output by a setting value (Only effect when using standard function)</summary>
	/// <param name="value">GPIO pin output value</param>
	void SetValue(MSP430_GPIO_Value value);
	/// <summary>Get the corresponding GPIO pin input</summary>
	/// <return>GPIO pin input value</return>
	MSP430_GPIO_Value GetValue(void);
	/// <summary>Check if the corresponding GPIO pin input is HIGH(1)</summary>
	bool CheckHigh(void);
	/// <summary>Check if the corresponding GPIO pin input is LOW(0)</summary>
	bool CheckLow(void);

	// Dymanic GPIO operation
	/// <summary>
	/// Dymanically reverse the corresponding pin's value
	/// <para>NOTE: This function will read and effect on register directly.</para>
	/// <para>NOTE: This function is only effect when using standard function.</para>
	/// </summary>
	void ReverseValue(void);
	/// <summary>Reverse the corresponding GPIO's direction
	/// <para>NOTE: This function will read and effect on register directly.</para>
	/// </summary>
	void ReverseDirection(void);
	/// <summary>
	/// Dymanically set the corresponding pin's direction
	/// <para>NOTE: This function will effect on register directly.</para>
	/// </summary>
	/// <param name="direction">Setting direction</param>
	void SetDirection(MSP430_GPIO_Direction direction);
	/// <summary>
	/// Get the corresponding pin's direction
	/// </summary>
	MSP430_GPIO_Direction GetDirection(void);
	/// <summary>
	/// Dymanically set the corresponding pin's function
	/// <para>NOTE: This function will effect on register directly.</para>
	/// </summary>
	/// <param name="function">Setting function</param>
	void SetFunction(MSP430_GPIO_Function function);
	/// <summary>
	/// Dymanically set the corresponding pin's pullup/pulldown resistor
	/// <para>NOTE: This function will effect on register directly.</para>
	/// </summary>
	/// <param name="direction">Switch of the pullup/pulldown resistor</param>
	void SetPullResistor(MSP430_GPIO_PullResistor sw);
};

/// <summary>
/// MSP430 GPIO(General purpose I/O) bank class
/// </summary>
class MSP430_GPIO_Bank
{
private:
	// Register for hardware operation

	REG_8b reg_PxIN;
	REG_8b reg_PxOUT;
	REG_8b reg_PxDIR;
	REG_8b reg_PxREN;

	// Corresponding GPIO location (Port and ID)
	/// <summary>Port</summary>
	MSP430_GPIO_Port port;

	// Corresponding GPIO function/mode configuration
	/// <summary>Pin direction</summary>
	MSP430_GPIO_Direction direction = MSP430_GPIO_Direction::Output;
	/// <summary>Pullup/pulldown resistor</summary>
	MSP430_GPIO_PullResistor pullResistor = MSP430_GPIO_PullResistor::Off;

	// Private low-level linking functions
	/// <summary>Hardware link from program to registers</summary>
	void HardLink(void);

	// Access mask
	/// <summary>The binary mask for access the GPIO port registers</summary>
	unsigned char accessMask = 0xFF;
public:
	// Constructor
	/// <summary>Create a new GPIO bank object, set the port</summary>
	/// <param name="port">GPIO port</param>
	MSP430_GPIO_Bank(MSP430_GPIO_Port port);
	/// <summary>Create a new GPIO bank object, set the port and direction</summary>
	/// <param name="port">GPIO port</param>
	/// <param name="direction">GPIO direction</param>
	MSP430_GPIO_Bank(MSP430_GPIO_Port port, MSP430_GPIO_Direction direction);
	/// <summary>Create a new GPIO bank object, set the port, direction and pullup/pulldown resistor</summary>
	/// <param name="port">GPIO port</param>
	/// <param name="direction">GPIO direction</param>
	/// <param name="pullResistor">GPIO pullup/pulldown resistor enable/disable</param>
	MSP430_GPIO_Bank(MSP430_GPIO_Port port, MSP430_GPIO_Direction direction, MSP430_GPIO_PullResistor pullResistor);
	/// <summary>Delete this GPIO bank instance, reset the hardware registers, and free up all memory space</summary>
	~MSP430_GPIO_Bank();

	// Access Mask
	/// <summary>
	/// Directly set the access mask for GPIO bank.
	/// <para>The data you read from or write to the GPIO bank will be mask with your setting.
	/// The 1 bit means data allowed, 0 bit means data no effect</para>
	/// </summary>
	/// <param name="mask">Access mask</param>
	void SetAccessMask(unsigned char mask);
	/// <summary>
	/// Directly set the access mask for GPIO bank.
	/// <para>The data you read from or write to the GPIO bank will be mask with your setting.
	/// The significant bit is start from START and end to END</para>
	/// </summary>
	/// <param name="start">Least significant bit (LSB)</param>
	/// <param name="end">Most significant bit (MSB)</param>
	void SetAccessMask(unsigned char start, unsigned char end);

	// GPIO initialize or re-configuration
	/// <summary>Initialize a hardware GPIO bank by this GPIO bank instance</summary>
	void Initialize(void);
	/// <summary>
	/// Deinitialize the corresponding hardware GPIO and set all registers to default.
	/// <para>(Default: Input, Resistor Off)</para>
	/// </summary>
	void Deinitialize(void);

	// Stardand GPIO operation
	/// <summary>Set the GPIO bank output by a setting value</summary>
	/// <param name="value">GPIO bank output value</param>
	void SetValue(unsigned char value);
	unsigned char GetValue(void);

	// Dymanic GPIO operation
	/// <summary>
	/// Dymanically reverse the corresponding GPIO bank's value
	/// <para>NOTE: This function will read and effect on register directly.</para>
	/// </summary>
	void ReverseValue(void);
	/// <summary>Reverse the corresponding GPIO  bank's direction
	/// <para>NOTE: This function will read and effect on register directly.</para>
	/// </summary>
	void ReverseDirection(void);
	/// <summary>
	/// Dymanically set the corresponding bank's direction
	/// <para>NOTE: This function will effect on register directly.</para>
	/// </summary>
	/// <param name="direction">Setting direction</param>
	void SetDirection(MSP430_GPIO_Direction direction);
	/// <summary>
	/// Get the corresponding bank's direction
	/// </summary>
	MSP430_GPIO_Direction GetDirection(void);
	/// <summary>
	/// Dymanically set the corresponding bank's pullup/pulldown resistor
	/// <para>NOTE: This function will effect on register directly.</para>
	/// </summary>
	/// <param name="direction">Switch of the pullup/pulldown resistor</param>
	void SetPullResistor(MSP430_GPIO_PullResistor sw);
};
