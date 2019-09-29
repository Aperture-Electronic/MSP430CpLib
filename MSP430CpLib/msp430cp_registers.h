#include <msp430.h>

// Global definations
#define REG_8b volatile unsigned char*

// Register bit operations
/// <summary>Set corresponding reigster bit to 1</summary>
/// <param name="REG">Register address pointer</param>
/// <param name="BIT">Bit in the register</param>
#define REG_SBIT1(REG, BIT) (*(REG) |= (1 << (BIT)))
/// <summary>Set corresponding reigster bit to 0</summary>
/// <param name="REG">Register address pointer</param>
/// <param name="BIT">Bit in the register</param>
#define REG_SBIT0(REG, BIT) (*(REG) &= (~(1 << (BIT))))
/// <summary>Set corresponding reigster bit to setting value</summary>
/// <param name="REG">Register address pointer</param>
/// <param name="BIT">Bit in the register</param>
/// <param name="VAL">Setting value</param>
#define REG_SBIT(REG, BIT, VAL) ((VAL) ? (REG_SBIT1(REG, BIT)) : (REG_SBIT0(REG, BIT)))
/// <summary>Get corresponding reigster bit</summary>
/// <param name="REG">Register address pointer</param>
/// <param name="BIT">Bit in the register</param>
#define REG_GBIT(REG, BIT) ((*(REG) & (1 << (BIT))) >> (BIT))

