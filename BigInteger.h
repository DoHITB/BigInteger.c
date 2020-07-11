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
static struct BigInteger _TEN;
static struct BigInteger _HUND;
static struct BigInteger _MIN;

static int ini = 0;


//Generales
static void _BI_initialize();
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

//Suma
void add(void* va, void* vb);
static void pAdd(void* va, void* vb);
static void addition(void* va, void* vb);
static void carryAdd(void* va);

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
static void sMul(void* va, void* vb);
static void pMul(int pos, void *vpart);

//División
void dvs(void *va, void *vb);
static void divide(void *va, void *vb);

//Raíz Cuadrada
void nqrt(void* va, int n);

//Potencia
void bipow(void *va, int p);

#endif /* BIGINTEGER_H_ */
