/*
 * BOperation.h
 *
 *  Created on: 15 nov. 2020
 *      Author: DoHITB under MIT License
 */

#ifndef BOPERATION_H_
#define BOPERATION_H_

#include "BigInteger.h"
#include "BigDouble.h"

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

  //cal2op
  void* a;
  void* b;
} memory;

typedef struct operation {
  void* va;
  void* vb;
  void* m;
  int* ret;
  char op;
} operation;

//Generales
void showError(int k);
void toString(void *vb, char* dst);
void clean(void *va);
void iniStr(char** dst);
static char getKind(void* a);
static void adjustData(void* va, int offset, int up);
void equals(void* va, void* vb, void* m, int* ret);
void init(void** m);
size_t getMemorySize();
static void rePos(void* va);
static void decimalize(void* va);
  
//Suma
void add(void* va, void* vb, void* m);
  
//Resta
void sub(void* va, void* vb, void* m);
  
//Multiplicación
void mul(void *va, void *vb, void* m);
  
//División
void dvs(void *va, void *vb, void* m);
  
//Raíz Cuadrada
void nqrt(void* va, int n, void* m);
  
//Potencia
void bipow(void *va, int p, void* m);

//Operación por lotes
//void operate(int count, ...);

//Utilidades
void biSig(void* va);
void BI2BD(void* dst, void* src);

//Cálculo
static void cal2op(void* va, void* vb, void* m, char k, int* ret);
static void dummyOp1(void* da, void* db, void* m);
static void dummyOp2(void* da, void* db, int* di);

#endif /* BOPERATION_H_ */
