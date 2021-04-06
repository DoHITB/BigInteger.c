/*
 * BigInteger.h
 *
 *  Created on: 25 ene. 2019
 *      Author: DoHITB under MIT License
 */
#ifndef BIGINTEGER_H_
#define BIGINTEGER_H_

static int MAX_LENGTH = 
#ifdef C_MAX_LENGTH
  C_MAX_LENGTH;
#else
  4096;
#endif

//struct
typedef struct BigInteger {
  char k;
  int count;
  char n[
#ifdef C_MAX_LENGTH
    C_MAX_LENGTH
#else
    4096
#endif
  ];
} BigInteger;

//BIT, Buffer para multiplicación y división
typedef struct BIT{
  struct BigInteger BI[10];
  int status[10];
} BIT;

//variables de trabajo
#if BI_STANDALONE == 1
typedef struct memory {
  //add
  void* vt;

  //sub
  void* stmp;

  //mul
  void* mpart;
  void* mret;
  void* mzero;
  void* mone;
  void* mtmp;

  //dvs
  void* done;
  void* dtmp;
  void* dret;
  void* dTemp;
  void* biTemp;

  //nqrt
  void* sret;
  void* sraw;
  void* sbase;
  void* szero;

  //bipow
  void* bres;
  void* btmp;

  //append
  void* aaux;

  //BIT
  void* biBIT;
} memory;
#endif

/*
 * Variables útiles
 */
static BigInteger _ZERO;
static BigInteger _ONE;
static BigInteger _TWO;
static BigInteger _THREE;
static BigInteger _FOUR;
static BigInteger _FIVE;
static BigInteger _SIX;
static BigInteger _SEVEN;
static BigInteger _EIGHT;
static BigInteger _NINE;
static BigInteger _TEN;
static BigInteger _HUND;
static BigInteger _MIN;

//Baliza para división con double
static int BI_point;

//Generales
void _BI_initialize();
void BImemcpy(void* dst, int orig);
void newBI(void *dst, char* s, int sig);
static void pAppend(void* va, int b);
static int signum(int a, int b);
void validateBI(void* a);
void hardEquals(void* va, void* vb, int* ret);
int getPoint();

//Suma
void pAdd(void* va, void* vb, void* m);
static void addition(void* va, void* vb);
void carryAdd(void* va, int move, int min);

//Resta
void pSub(void* va, void* vb, void* m);
static void subtract(void *va, void *vb);
static void carrySub(void* va, int carryType);
static void recount(void *va);

//Multiplicación
void sMul(void* va, void* vb, void* m);
static void pMul(int pos, void *vpart);

//División
void sDvs(void* va, void* vb, void* m);
static void divide(void *va, void *vb, void* m);

//Raíz Cuadrada
void sNqrt(void* va, int n, void* m);

//Potencia
void sBipow(void* va, int p, void* m);

//Standalone
#if BI_STANDALONE == 1 
void add(void* va, void* vb, void* m);
void sub(void* va, void* vb, void* m);
void mul(void *va, void *vb, void* m);
void dvs(void *va, void *vb, void* m);
void nqrt(void* va, int n, void* m);
void bipow(void *va, int p, void* m);
static void showError(int k);
void toString(void *vb, char* dst);
void clean(void *va);
void equals(void* va, void* vb, void* m, int* ret);
void iniStr(char** dst);
void init(void** m);
size_t getMemorySize();
#endif
#endif /* BIGINTEGER_H_ */
