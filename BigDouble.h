/*
 * BigDouble.h
 *
 *  Created on: 12 oct. 2020
 *      Author: DoHITB under MIT Liscense
 */

#ifndef BIGDOUBLE_H_
#define BIGDOUBLE_H_

 //struct
typedef struct BigDouble {
  char k;
  int count;
  signed char n[4096];
  int cpos;
  char sig;
} BigDouble;

/*
 * Variables útiles
 */
static struct BigDouble _DZERO;
static struct BigDouble _DONE;
static struct BigDouble _DTWO;
static struct BigDouble _DTHREE;
static struct BigDouble _DFOUR;
static struct BigDouble _DFIVE;
static struct BigDouble _DSIX;
static struct BigDouble _DSEVEN;
static struct BigDouble _DEIGHT;
static struct BigDouble _DNINE;
static struct BigDouble _DTEN;
static struct BigDouble _DHUND;
static struct BigDouble _DMIN;

static int dini[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

//delimitador decimal
static char coma = ',';
static char stComa[2] = {',', '\0'};

//longitud máxima
static int D_MAX_LENGTH =
#ifdef CD_MAX_LENGTH
  CD_MAX_LENGTH;
#else
  4095;
#endif

//creacion
void newBD(void* dst, char* s, int sig);

//otros
void validateBD(void* a);
void BDmemcpy(void* dst, int value);
void _BD_initialize(int value);
#endif /* BIGDOUBLE_H_ */
