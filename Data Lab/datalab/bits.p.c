#line 177 "bits.c"
int bitOr(int x, int y) {

  return ~(~x & ~y);
#line 31 "<command-line>"
#include "/usr/include/stdc-predef.h"
#line 180 "bits.c"
}
#line 187
int evenBits(void) {
#line 191
    int mask=(  0x55 << 8) + 0x55;
    return (mask << 16) + mask;
}
#line 200
int minusOne(void) {

  return ~(0);
}
#line 211
int allEvenBits(int x) {
#line 217
    int mask=(  0x55 << 8) + 0x55;
    mask +=( mask << 16);

    return !((x & mask) ^ mask);
}
#line 229
int anyOddBit(int x) {
#line 235
    int mask=(  0xAA << 8) + 0xAA;
    mask +=( mask << 16);

    return !!(x & mask);
}
#line 249
int byteSwap(int x, int n, int m) {
#line 256
    int a=  n << 3;
    int b=  m << 3;

    int flip=  ~((0xFF << a) |( 0xFF << b));
 int firstbyte=(  x >> a) & 0xFF;
 int secondbyte=(  x >> b) & 0xFF;

    return ((x & flip) |(( secondbyte << a) |( firstbyte << b)));
}
#line 273
int addOK(int x, int y) {
    int sum=  x + y;
#line 278
    return !(((sum ^ x) &( sum ^ y)) >> 31);
}
#line 287
int conditional(int x, int y, int z) {
  int mask=(  !x + ~0x00);
  return ((~mask) & z) |(( mask) & y);
}
#line 300
int isAsciiDigit(int x) {
    int neg=  1 << 31;
 int less=  ~(neg | 0x39);
 int greater=  ~(0x30);

 less = neg &( less+x) >> 31;
 greater = neg &( greater+1+x) >>31;

 return !(less|greater);
}
#line 319
int replaceByte(int x, int n, int c) {
    int a=  n << 3;
 int mask=  0xFF << a;
 int shift=  c << a;
 return (x & ~mask) | shift;
}
#line 337
int reverseBits(int x) {

    int bit1=(  0x55 << 8) + 0x55;
    int bit2=(  0x33 << 8) + 0x33;
    int bit3=(  0x0F << 8) + 0x0F;
    int bit4=(  0x00 << 8) + 0xFF;
    int mask=(  0xFF << 8) + 0xFF;
    bit1 +=( bit1 << 16);
    bit2 +=( bit2 << 16);
    bit3 +=( bit3 << 16);
    bit4 +=( bit4 << 16);


    x =(( x >> 1) & bit1) |(( x & bit1) << 1);
    x =(( x >> 2) & bit2) |(( x & bit2) << 2);
    x =(( x >> 4) & bit3) |(( x & bit3) << 4);
    x =(( x >> 8) & bit4) |(( x & bit4) << 8);
    x =(( x >> 16) & mask) |(( x & mask) << 16);

    return x;
}
#line 368
int satAdd(int x, int y) {
#line 373
    int sum=  x + y;
 int isOverflow=((  sum^x) &( sum^y)) >> 31;
 return (sum >>( isOverflow & 31)) +( isOverflow << 31);
}
#line 391
unsigned float_abs(unsigned uf) {
  unsigned mask=  0x7FFFFFFF;
  unsigned minNaN=  0x7F800001;
  unsigned res=  uf & mask;
  if (res >= minNaN) 
    return uf;
  else 
    return res;
}
#line 412
int float_f2i(unsigned uf) {
    unsigned sign=  uf >> 31;
    unsigned exp=(  uf >> 23) & 0xFF;
    unsigned frac=( uf & 0x7FFFFF);
 unsigned bias=  0x7F;
 unsigned res=  frac;


    if (exp== 0xFF) 
        return 0x80000000u;


    if (exp< bias) 
        return 0x0;


    exp -= bias;


    if (exp>= 31) 
        return 0x80000000u;


    if (exp> 22) 
        res = frac <<( exp - 23);
    else 
        res = frac >>( 23 - exp);


    res += 1 << exp;


    if (sign) 
        res = -res;

    return res;
}
#line 460
unsigned float_half(unsigned uf) {
  int exp=  uf & 0x7F800000;
  int sign=  uf & 0x80000000;
  int frac=  uf & 0x007FFFFF;

  if (exp== 0x7F800000) 
      return uf;

  if ((!exp)||( exp == 0x00800000)) {
     frac = frac | exp;
     frac =( uf & 0x00FFFFFF) >> 1;
     frac +=(( uf & 3) >> 1) &( uf & 1);

     return sign | frac;
  }
  return (sign |(( exp - 1) & 0x7F800000) | frac);
}
