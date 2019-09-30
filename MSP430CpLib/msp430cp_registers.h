#include <msp430.h>

// Global definations
#define REG_8b volatile unsigned char*

// Register operations
/// <summary>Write data to register</summary>
/// <param name="REG">Register address pointer</param>
/// <param name="DATA">Data to write</param>
#define REG_W(REG, DATA) (*(REG) = (DATA))
/// <summary>Read data from register</summary>
/// <param name="REG">Register address pointer</param>
#define REG_R(REG) (*(REG))
/// <summary>Write data to register with data mask</summary>
/// <param name="REG">Register address pointer</param>
/// <param name="DATA">Data to write</param>
/// <param name="MASK">Data mask</param>
#define REG_WM(REG, DATA, MASK) (*(REG) = ((*REG) & (~(MASK))) | ((DATA) & (MASK)))
/// <summary>Read data from register with data mask</summary>
/// <param name="REG">Register address pointer</param>
/// <param name="MASK">Data mask</param>
#define REG_RM(REG, MASK) (*(REG) & (MASK))

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

