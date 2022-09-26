/*
 * BigInteger.h
 *
 *  Created on: 25 ene. 2019
 *      Author: DoHITB under MIT License
 */
#ifndef BIGINTEGER_H_
#define BIGINTEGER_H_

 //MAX_LENGTH: Global varialbe to set BigInteger length.
static int MAX_LENGTH =
#ifdef C_MAX_LENGTH
C_MAX_LENGTH;
#else
4096;
#endif

/*****************************************************************************
 *                                 Structures                                *
 *****************************************************************************/
 //Main struct
typedef struct BigInteger {
  char k;
  int count;
  signed char n[
#ifdef C_MAX_LENGTH
    C_MAX_LENGTH
#else
    4096
#endif
  ];
} BigInteger;

//BIT, multiplication and division buffer
typedef struct BIT {
  BigInteger BI[10];
  int status[10];
} BIT;

//Working variables
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

/*****************************************************************************
 *                              Useful variables                             *
 *****************************************************************************/
//Quick values ({0..10, 100, -1})
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

//Decimal point when using BOperation
static int BI_point;

//Status code
static int BIReturnCode;

#if CUDA_ENABLED == 1
__device__ static int cBIReturnCode;
#endif

/*****************************************************************************
 *                            Function definition                            *
 *****************************************************************************/
 //CUDA specific
#if CUDA_ENABLED == 1
 //h2d
void h2d(void** device, void* host, int quantity, size_t size);

//d2h
void d2h(void* host, void* device, int quantity, size_t size);

//CUnewBI
__device__ void CUnewBI(void* dst, char* s, int sig);

//CUclean
__device__ static void CUclean(void* va);

//CUstrlen
__device__ static int CUstrlen(char* s);

//CUsMul
__device__ void CUsMul(void* va, void* vb, void* vc, void* vd);

//CUpAdd
__device__ void CUpAdd(void* va, void* vb);

//CUpSub
__device__ void CUpSub(void* va, void* vb);

//CUsubtract
__device__ static void CUsubtract(void* va, void* vb);

//CucarrySub
__device__ static void CUcarrySub(void* va, int carryType);

//CUrecount
__device__ static void CUrecount(void* va);

//CUaddition
__device__ static void CUaddition(void* va, void* vb);

//CUpMul
__device__ static void CUpMul(int pos, void* vpart);

//CUcarryAdd
__device__ void CUcarryAdd(void* va, int move, int min);

//CUhardEquals
__device__ void CUhardEquals(void* va, void* vb, int* ret);

//CUsignum
__device__ static int CUsignum(int a, int b);

//CUsub
__device__ void CUsub(void* va, void* vb, void* vc);

//CUsDvs
__device__ void CUsDvs(void* va, void* vb, void* xa, void* xb);

//CUdivide
__device__ static void CUdivide(void* va, void* vb, void* xa, void* xb);

//CUpAppend
__device__ void CUpAppend(void* va, int b);

//CUsqrt
__device__ void CUsqrt(void* va, void* vb, void* vc, void* vd);
#endif

//newBI
void newBI(void* dst, const char* s, int sig);

//validateBI
void validateBI(void* a);

//getPoint
int getPoint();

//getReturnCode
int getReturnCode();

//setReturnCode
void setReturnCode(int k);

//_BI_initialize
#if BI_STANDALONE == 1 
static
#endif
 void _BI_initialize();

//BImemcpy
void BImemcpy(void* dst, int orig);

//pAppend
static void pAppend(void* va, int b);

//signum
static int signum(int a, int b);

//hardEquals
#if BI_STANDALONE == 1 
static
#endif
 void hardEquals(void* va, void* vb, int* ret);

//pAdd
#if BI_STANDALONE == 1
static
#endif
 void pAdd(void* va, void* vb, void* m);

//addition
static void addition(void* va, void* vb);

//carryAdd
#if BI_STANDALONE == 1
static
#endif
 void carryAdd(void* va, int move, int min);

//pSub
#if BI_STANDALONE == 1
static
#endif
 void pSub(void* va, void* vb, void* m);

//subtract
static void subtract(void* va, void* vb);

//carrySub
static void carrySub(void* va, int carryType);

//recount
static void recount(void* va);

//sMul
#if BI_STANDALONE == 1
static
#endif
 void sMul(void* va, void* vb, void* m);

//pMul
static void pMul(int pos, void* vpart);

//sDvs
#if BI_STANDALONE == 1
static
#endif
 void sDvs(void* va, void* vb, void* m);

//divide
static void divide(void* va, void* vb, void* m);

//sNqrt
#if BI_STANDALONE == 1
static
#endif
 void sNqrt(void* va, int n, void* m);

//sBipow
#if BI_STANDALONE == 1
static
#endif
 void sBipow(void* va, int p, void* m);

//Standalone definitions
#if BI_STANDALONE == 1
//add
void add(void* va, void* vb, void* m);

//sub
void sub(void* va, void* vb, void* m);

//mul
void mul(void* va, void* vb, void* m);

//dvs
void dvs(void* va, void* vb, void* m);

//nqrt
void nqrt(void* va, int n, void* m);

//bipow
void bipow(void* va, int p, void* m);

//mod
void mod(void* va, void* vb, void* m);

//toString
void toString(void* vb, char* dst);

//equals
void equals(void* va, void* vb, int* ret);

//iniStr
void iniStr(char** dst);

//init
void init(void** m);

//getMemorySize 
size_t getMemorySize();

//clean
void clean(void* va);

//showError
#if CUDA_ENABLED == 1
__host__ __device__
#endif
 static void showError(int k);
#endif /* BI_STANDALONE */
#endif /* BIGINTEGER_H_ */
