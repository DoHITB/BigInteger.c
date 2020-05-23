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
//void toString(struct BigInteger b);
void toString(void *vb);
//void append(struct BigInteger *a, struct BigInteger b);
void append(void *va, void *vb);
static void pAppend(void *va, void *vb);
//void clean(struct BigInteger *a);
static void clean(void *va);
//static int signum(struct BigInteger *a, struct BigInteger b);
//static int signum(void *va, void *vb);
static int signum(int a, int b);
void validateBI(void* a);

//Suma
//void add(struct BigInteger *a, struct BigInteger b);
void add(void* va, void* vb);
static void pAdd(void* va, void* vb);
//static void addition(struct BigInteger *a, struct BigInteger b);
static void addition(void* va, void* vb);
//static void carryAdd(struct BigInteger *a);
static void carryAdd(void* va);

//Resta
//void sub(struct BigInteger *a, struct BigInteger b);
void sub(void* va, void* vb);
static void pSub(void* va, void* vb);
//static void subtract(struct BigInteger *a, struct BigInteger *b, struct BigInteger *ret);
static void subtract(void *va, void *vb, void *vret);
//static void carrySub(struct BigInteger *a, int carryType);
static void carrySub(void* va, int carryType);
//static void recount(struct BigInteger *a);
static void recount(void *va);
//void hardEquals(struct BigInteger a, struct BigInteger b, int *ret);
static void hardEquals(void *va, void *vb, int *ret);
void equals(void* va, void* vb, int* ret);

//Multiplicación
//void mul(struct BigInteger *a, struct BigInteger b);
void mul(void *va, void *vb);
static void sMul(void* va, void* vb);
//static void pMul(int pos, struct BigInteger *part);
static void pMul(int pos, void *vpart);

//División
//void dvs(struct BigInteger *a, struct BigInteger b);
void dvs(void *va, void *vb);
//static void divide(struct BigInteger *a, struct BigInteger b);
static void divide(void *va, void *vb);
//static void makeBase(struct BigInteger *base, struct BigInteger b, int length, int* start);
static void makeBase(void *vbase, void *vb, int length, int* start);
//static void shift(struct BigInteger* base, int start);
static void shift(void* vbase, int start);
//static void rev(struct BigInteger *a);
static void rev(void *va);

//Raíz Cuadrada
//struct BigInteger BISQRT(struct BigInteger a);
//void BISQRT(void *va);
void nqrt(void* va, int n);

//Potencia
//void pow(struct BigInteger *a, int p);
void pow(void *va, int p);

#endif /* BIGINTEGER_H_ */
