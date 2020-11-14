/*
 * BigInteger.h
 *
 *  Created on: 25 ene. 2019
 *      Author: DoHITB under MIT License
 */

#ifndef BIGINTEGER_H_
#define BIGINTEGER_H_

//struct
struct BigInteger{
  char k;
  int count;
  int n[4096];
};

//BIT, Buffer para multiplicación y división
struct BIT{
  struct BigInteger BI[10];
  int status[10];
};

/*
 * Variables útiles
 */
static struct BigInteger _ZERO;
static struct BigInteger _ONE;
static struct BigInteger _TWO;
static struct BigInteger _THREE;
static struct BigInteger _FOUR;
static struct BigInteger _FIVE;
static struct BigInteger _SIX;
static struct BigInteger _SEVEN;
static struct BigInteger _EIGHT;
static struct BigInteger _NINE;
static struct BigInteger _TEN;
static struct BigInteger _HUND;
static struct BigInteger _MIN;

static struct BIT* biBIT;
static int ibit = 0;

//status BImemcpy
static int ini[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//Generales
static void _BI_initialize(int value);
void BImemcpy(void* dst, int orig);
void newBI(void *dst, char* s, int sig);
static void showError(int k);
void toString(void *vb, char* dst);
void append(void *va, void *vb);
static void pAppend(void *va, void *vb);
static void clean(void *va);
static int signum(int a, int b);
void validateBI(void* a);
void iniStr(char** dst);
void iniBIT();
void _free(int n, ...);

//Suma
void add(void* va, void* vb);
static void pAdd(void* va, void* vb);
static void addition(void* va, void* vb);
void carryAdd(void* va, int move, int min);

//Resta
void sub(void* va, void* vb);
static void pSub(void* va, void* vb);
static void subtract(void *va, void *vb);
static void carrySub(void* va, int carryType);
static void recount(void *va);
static void hardEquals(void *va, void *vb, int *ret);
void equals(void* va, void* vb, int* ret);

//Multiplicación
void mul(void *va, void *vb);
static void sMul(void* va, void* vb, void* table);
static void pMul(int pos, void *vpart);

//División
void dvs(void *va, void *vb);
static void sDvs(void* va, void* vb);
static void divide(void *va, void *vb, void* table);

//Raíz Cuadrada
void nqrt(void* va, int n);
static void sNqrt(void* va, int n);

//Potencia
void bipow(void *va, int p);
static void sBipow(void* va, int p);

#endif /* BIGINTEGER_H_ */
