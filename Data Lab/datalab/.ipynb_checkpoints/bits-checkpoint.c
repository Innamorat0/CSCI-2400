/* 
 * CS:APP Data Lab 
 * 
 * <Sam Feig Safe9522>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 10.0.0.  Version 10.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2017, fifth edition, plus
   the following additions from Amendment 1 to the fifth edition:
   - 56 emoji characters
   - 285 hentaigana
   - 3 additional Zanabazar Square characters */
/* We do not support C11 <threads.h>.  */
/* 
 * bitOr - x|y using only ~ and & 
 *   Example: bitOr(6, 5) = 7
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitOr(int x, int y) {
//      Demorgan's Law: not (A or B) = not A and not B (add another not around it to cancel out)
    return ~(~x & ~y);
}
/* 
 * evenBits - return word with all even-numbered bits set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int evenBits(void) {
//     Binary of 0x55 is 01010101 which has a 1 in every second bit
//     Shift left 8 and add 0x55 to make it 16 bits long
//     Shift left 16 and add itself to make it 32 bits long
    int mask = (0x55 << 8) + 0x55;
    return (mask << 16) + mask;
}
/* 
 * minusOne - return a value of -1 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int minusOne(void) {
//     Two's Compliment (flipping 00000000 gives 11111111 which is -1 in two's compliment)
    return ~(0);
}
/* 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allEvenBits(int x) {
//     0x55 is 01010101 in binary
//     Shift left 8 and add 0x55 to make it 16 bits long
//     Shift left 16 and add itself to make it 32 bits long
//     & mask with x to check if any odd bits are set to 1
//     XOR it with the mask to get 1 if there are odd bits set to 1, flip that to return 1 if only even bits
    int mask = (0x55 << 8) + 0x55;
    mask += (mask << 16);
    
    return !((x & mask) ^ mask);
}
/* 
 * anyOddBit - return 1 if any odd-numbered bit in word set to 1
 *   Examples anyOddBit(0x5) = 0, anyOddBit(0x7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int anyOddBit(int x) {
//     0xAA is 10101010 in binary
//     Shift left 8 and add 0xAA to make it 16 bits long
//     Shift left 16 and add itself to make it 32 bits long
//     & mask with x to check if any even bits are set to 1
//     flip that to return 0 if only odd bits, flip that again to get it to return 1
    int mask = (0xAA << 8) + 0xAA;
    mask += (mask << 16);
    
    return !!(x & mask);
}
/* 
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {
//     Shift n and m by 3 to get the entire byte
//     Shift 0xFF by a and OR it with 0xFF shifted by b
//     
//     Shift back to the right by a and AND with 0xFF to get first byte to shift
//     Shift back to the right by b and AND with 0xFF to get second byte to shift
//     OR everything back together with the input ANDed with flip to combine bytes again
    int a = n << 3;
    int b = m << 3;
    
    int flip = ~((0xFF << a) | (0xFF << b)); //gets non shifted part of the input
	int firstbyte = (x >> a) & 0xFF;
	int secondbyte = (x >> b) & 0xFF;
	
    return ((x & flip) | ((secondbyte << a) | (firstbyte << b)));
}
/* 
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
    int sum = x + y;
    
//     Check if x and y have same sign but sum is different (Overflow occured)
//     Shifting right 31 gets only the most significant digit
//     Not it all to flip output
    return !(((sum ^ x) & (sum ^ y)) >> 31);
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
//     x != 0, subtract 1 from 0 => 0xffffffff
//     x = 0, subtract 1 from 1 => 0x00000000
    int mask = (!x + ~0x00);
    
//     x != 0, mask with z, 
//     x == 0, mask with y
    return ((~mask) & z) | ((mask) & y);
}
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
    int neg = 1 << 31;
	int less = ~(neg | 0x39); // >0x39 is added, result goes negative
	int greater = ~(0x30); // <0x30 is added, result is negative
	
//    Add x and check the sign bit for each
	less = neg & (less + x) >> 31;
	greater = neg & (greater + 1 + x) >> 31;
	
//     If result is negative, it is not ASCII
	return !(less | greater);
}
/* 
 * replaceByte(x,n,c) - Replace byte n in x with c
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: replaceByte(0x12345678,1,0xab) = 0x1234ab78
 *   You can assume 0 <= n <= 3 and 0 <= c <= 255
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 3
 */
int replaceByte(int x, int n, int c) {
//     n * 2^3 == n * 8
    int a = n << 3;
	int mask = 0xFF << a; // shift mask
//     Shift bits of c into nth byte
	int shift = c << a; 
//     Flip mask to get byte wide of 0's. mask x, zeroing x's nth byte, replace it with byte from c
	return (x & ~mask) | shift;
}
/* reverseBits - reverse the bits in a 32-bit integer,
              i.e. b0 swaps with b31, b1 with b30, etc
 *  Examples: reverseBits(0x11111111) = 0x88888888
 *            reverseBits(0xdeadbeef) = 0xf77db57b
 *            reverseBits(0x88888888) = 0x11111111
 *            reverseBits(0)  = 0
 *            reverseBits(-1) = -1
 *            reverseBits(0x9) = 0x90000000
 *  Legal ops: ! ~ & ^ | + << >> and unsigned int type
 *  Max ops: 90
 *  Rating: 4
 */
int reverseBits(int x) {
//     Create masks for each of the bytes we want to flip
    int byte1 = (0x55 << 8) + 0x55; //bit1 = 0x55555555 odd and even bits
    int byte2 = (0x33 << 8) + 0x33; //bit2 = 0x33333333 consecutive pairs
    int byte3 = (0x0F << 8) + 0x0F; //bit3 = 0x0F0F0F0F nibbles
    int byte4 = (0x00 << 8) + 0xFF; //bit4 = 0x00FF00FF bytes
    int mask = (0xFF << 8) + 0xFF; //mask = 0xFFFF 2-byte long pairs
    byte1 += (byte1 << 16);
    byte2 += (byte2 << 16);
    byte3 += (byte3 << 16);
    byte4 += (byte4 << 16);
    
//     Swap each respective byte by shifting right by (1,2,4,8,16) and ANDing with the byte mask
//     OR that with x ANDed with the byte mask and shift back (1,2,4,8,16) to get the flipped bytes
    x = ((x >> 1) & byte1) | ((x & byte1) << 1);
    x = ((x >> 2) & byte2) | ((x & byte2) << 2);
    x = ((x >> 4) & byte3) | ((x & byte3) << 4);
    x = ((x >> 8) & byte4) | ((x & byte4) << 8);
    x = ((x >> 16) & mask) | ((x & mask) << 16);
    
    return x;
}
/*
 * satAdd - adds two numbers but when positive overflow occurs, returns
 *          maximum possible value, and when negative overflow occurs,
 *          it returns minimum positive value.
 *   Examples: satAdd(0x40000000,0x40000000) = 0x7fffffff
 *             satAdd(0x80000000,0xffffffff) = 0x80000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 30
 *   Rating: 4
 */
int satAdd(int x, int y) {
//     Sum the two values
//     Check for overflow by XORing sum with x & comparing with XOR'd sum of y then getting the sign of integer
//     if negative, sum shifts right by 31 and gives smallest possible value 
//     if postive, shifts to left by 31 and gives largest possible values
    int sum = x + y;
	int isOverflow  = ((sum ^ x) & (sum ^ y)) >> 31;
	return (sum >> (isOverflow & 31)) + (isOverflow << 31);
}
/*
 * Extra credit
 */
/* 
 * float_abs - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_abs(unsigned uf) {
    unsigned mask = 0x7FFFFFFF;		
    unsigned minNaN = 0x7F800001;
    
//     Set sign bit to 0
    unsigned res = uf & mask; 
    
//     Return arg if NaN, all NaN >= minimum NaN
    if (res >= minNaN)
        return uf;
    else
        return res;
}
/* 
 * float_f2i - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int float_f2i(unsigned uf) {
    unsigned sign = uf >> 31;
    unsigned exp = (uf >> 23) & 0xFF;
    unsigned frac =(uf & 0x7FFFFF);
	unsigned bias = 0x7F;
	unsigned res = frac;
  
//     special cases: NaN and Inf
    if(exp == 0xFF) 
        return 0x80000000;
  
//     denormalized case and normalized exp less than Bias
    if(exp < bias)
        return 0x0;
  
//     normalized case
    exp -= bias;
  
//     overflow
    if(exp >= 31)
        return 0x80000000;
  
//     get integer result after shift corresponding bits
    if(exp > 22)
        res = frac << (exp - 23);
    else 
        res = frac >> (23 - exp);

//     add 1 << exp for normalized case
    res += 1 << exp;
  
//     if sign = 1, change its sign
    if(sign)
        res = -res;
    
    return res;
}
/* 
 * float_half - Return bit-level equivalent of expression 0.5*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_half(unsigned uf) {
  int exp = uf & 0x7F800000;
  int sign = uf & 0x80000000;
  int frac = uf & 0x007FFFFF; 
    
//     NaN or Inifity
    if(exp == 0x7F800000) 
        return uf; 
    
//     Shift from normalized to denormalized
    if((!exp) || (exp == 0x00800000)) {
        frac = frac | exp;
        frac = (uf & 0x00FFFFFF) >> 1;
        frac += ((uf & 3) >> 1) & (uf & 1);
      
        return sign | frac;
  }
//     Decrease exp by 1
    return (sign | ((exp - 1) & 0x7F800000) | frac);
}
