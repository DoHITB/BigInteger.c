/*
 * BigInteger.h
 *
 *  Created on: 25 ene. 2019
 *      Author: DoHITB
 */

#ifndef BIGINTEGER_H_
#define BIGINTEGER_H_

//struct
struct BigInteger{
  int count;
  int n[512];
};

//BIT, Buffer para multiplicación y división
struct BIT{
  struct BigInteger BI[10];
  int status[10];
};

/*
 * Variables útiles
 */
struct BigInteger _ZERO;
struct BigInteger _ONE;
struct BigInteger _TEN;
struct BigInteger _HUND;
struct BigInteger _MIN;


//Generales
void _BI_initialize();
struct BigInteger newBI(char* s);
void showError(int k);
void toString(struct BigInteger b);
void append(struct BigInteger *a, struct BigInteger b);
void pAppend(struct BigInteger *a, struct BigInteger *b);
void clean(struct BigInteger *a);
int signum(struct BigInteger *a, struct BigInteger b);

//Suma
void add(struct BigInteger *a, struct BigInteger b);
void addition(struct BigInteger *a, struct BigInteger b);
void carryAdd(struct BigInteger *a);

//Resta
void sub(struct BigInteger *a, struct BigInteger b);
void subtract(struct BigInteger *a, struct BigInteger *b, struct BigInteger *ret);
void carrySub(struct BigInteger *a, int carryType);
void recount(struct BigInteger *a);
void hardEquals(struct BigInteger a, struct BigInteger b, int *ret);

//Multiplicación
void mul(struct BigInteger *a, struct BigInteger b);
void pMul(/*struct BigInteger *a, int x, */int pos, struct BigInteger *part);

//División
void dvs(struct BigInteger *a, struct BigInteger b);
void divide(struct BigInteger *a, struct BigInteger b);
void makeBase(struct BigInteger *base, struct BigInteger b, int length, int* start);
void shift(struct BigInteger* base, int start);
void rev(struct BigInteger *a);

//Raíz Cuadrada
struct BigInteger BISQRT(struct BigInteger a);

//Potencia
void pow(struct BigInteger *a, int p);

#endif /* BIGINTEGER_H_ */
