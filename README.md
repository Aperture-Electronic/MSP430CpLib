# MSP430CpLib
## Introduction
A C++ Library for MSP430 development. This libaray can be use in VisualGDB to develop MSP430 series 16-bits microcontrollors.
This library using C++ for programming (standard = C++17), and declare internal functions and peripherals on MSP430 microcontroller
by using object-oriented types and classes. 
This library has complete annotations and prompts (support for XML document export and Visual Studio IntelliSense). 
Developers can use it to speed up development and understand the working principle of MSP430 microcontroller better. 
Each reuse function has enumerated types to choose from, and developers will enjoy the development process.

## Device Support
This library is based on official library from TI, and now support all of the devices in the official library list, include:
* MSP430AFExxx
* MSP430BTxxxx
* MSP430Cxxx
* MSP430CGxxx
* MSP430Exxx
* MSP430Fxxx
* MSP430Fxxxx
* MSP430FExxx
* MSP430FGxxx
* MSP430FRxxxx
* MSP430FWxxx
* MSP430Gxxx
* MSP430Lxxx
* MSP430Pxxx
* MSP430SLxxx
* MSP430TCHxxx
* MSP430Xxxx
* MSP430XExxx
* MSP430XGxxx
* MSP430XWxxx
* CC430Fxxxx

## Function/Peripheral Support
More and more functions and periperals support will add into this library. Please stay tuned.
Now, this library have these function/peripheral support:
  * GPIO Pin (single pin)
  * GPIO initialize (direction, function, pull resistors, etc.)
  * GPIO standard operate (write, read)
  * GPIO dynamic operate (reverse direction, reverse output, etc.)

* GPIO Bank (multi pin on single port)
  * GPIO bank data mask (can use a part of the port to group to a bank)
  * GPIO bank initialize (direction, pull resistors, etc. (NOTE: GPIO function must be set to Standard))
  * GPIO bank standard operate with data mask (bank write, bank read)
  * GPIO bank dynamic operate with data mask (reverse direction, reverse output, etc.)

## Copyright
This library is open source and comply with CC-BY-SA protocol.


