/*
 * BigDouble.c
 *
 *  Created on: 12 oct. 2020
 *      Author: DoHITB under MIT Liscense
 *
 *  CHANGELOG
 *    v0.1
 *      - Funci�n de creaci�n
 *      - Funci�n de display
 *      - F�nci�n de suma
 *    v0.2
 *      - Remodelaci�n completa de la estructura
 *      - Funci�n de creaci�n
 *      - Funci�n de conversi�n BI > BD
 *    v1.0
 *      - Modelo funcional basado en BigOperation.
 */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "BigDouble.h"

float version = 1.0f;

/*
 * Funci�n initialize
 *
 * Da valores a ciertos datos �tiles.
 */
void _BD_initialize(int value) {
  int i = 0;

  if (value == 0) {
    if (dini[0] == 0) {
      //lo creamos manualmente ya que clean copia de _ZERO
      for (; i < D_MAX_LENGTH; i++)
        _DZERO.n[i] = 0;

      _DZERO.count = 0;
      _DZERO.cpos = 0;
      _DZERO.k = 'd';
      _DZERO.sig = 0;
    }

    dini[0] = 1;
  } else if (value == 1) {
    if (dini[1] == 0)
      newBD(&_DONE, "1", 0);

    dini[1] = 1;
  } else if (value == 2) {
    if (dini[2] == 0)
      newBD(&_DTWO, "2", 0);

    dini[2] = 1;
  } else if (value == 3) {
    if (dini[3] == 0)
      newBD(&_DTHREE, "3", 0);

    dini[3] = 1;
  } else if (value == 4) {
    if (dini[4] == 0)
      newBD(&_DFOUR, "4", 0);

    dini[4] = 1;
  } else if (value == 5) {
    if (dini[5] == 0)
      newBD(&_DFIVE, "5", 0);

    dini[5] = 1;
  } else if (value == 6) {
    if (dini[6] == 0)
      newBD(&_DSIX, "6", 0);

    dini[6] = 1;
  } else if (value == 7) {
    if (dini[7] == 0)
      newBD(&_DSEVEN, "7", 0);

    dini[7] = 1;
  } else if (value == 8) {
    if (dini[8] == 0)
      newBD(&_DEIGHT, "8", 0);

    dini[8] = 1;
  } else if (value == 9) {
    if (dini[9] == 0)
      newBD(&_DNINE, "9", 0);

    dini[9] = 1;
  } else if (value == 10) {
    if (dini[10] == 0)
      newBD(&_DTEN, "10", 0);

    dini[10] = 1;
  } else if (value == 100) {
    if (dini[11] == 0)
      newBD(&_DHUND, "100", 0);

    dini[11] = 1;
  } else if (value == -1) {
    if (dini[12] == 0)
      newBD(&_DMIN, "1", -1);

    dini[12] = 1;
  }
}

/*
 * Funci�n BDmemcpy
 *
 * Copia en el puntero destino la variable �til deseada
 */
void BDmemcpy(void* dst, int value) {
  _BD_initialize(value);

  if (value == 0)
    memcpy(dst, &_DZERO, sizeof(BigDouble));
  else if (value == 1)
    memcpy(dst, &_DONE, sizeof(BigDouble));
  else if (value == 2)
    memcpy(dst, &_DTWO, sizeof(BigDouble));
  else if (value == 3)
    memcpy(dst, &_DTHREE, sizeof(BigDouble));
  else if (value == 4)
    memcpy(dst, &_DFOUR, sizeof(BigDouble));
  else if (value == 5)
    memcpy(dst, &_DFIVE, sizeof(BigDouble));
  else if (value == 6)
    memcpy(dst, &_DSIX, sizeof(BigDouble));
  else if (value == 7)
    memcpy(dst, &_DSEVEN, sizeof(BigDouble));
  else if (value == 8)
    memcpy(dst, &_DEIGHT, sizeof(BigDouble));
  else if (value == 9)
    memcpy(dst, &_DNINE, sizeof(BigDouble));
  else if (value == 10)
    memcpy(dst, &_DTEN, sizeof(BigDouble));
  else if (value == 100)
    memcpy(dst, &_DHUND, sizeof(BigDouble));
  else if (value == -1)
    memcpy(dst, &_DMIN, sizeof(BigDouble));
  else
    BDmemcpy(dst, 0);
}

/*
 * newBD
 * 
 * Crea un elemento BigDouble
 */
void newBD(void* dst, char* s, int sig) {
  int i = (int)strlen(s) - 1;
  int f = i;
  int j = 0;
  int c;
  int ssig = sig;
  int cma = 0;

  //ajustamos el tipo
  ((BigDouble*)dst)->k = 'd';

  //limpiamos el array
  clean(dst);

  if (i > D_MAX_LENGTH + 1)
    showError(1);

  //por defecto, no hay decimales
  ((BigDouble*)dst)->cpos = 0;

  //recorremos el string y lo guardamos en integers. Si encontramos la coma la almacenamos
  for (; i >= 0; i--) {
    c = (int)(s[i] - 48);

    if (c >= 0 && c <= 9)
      ((BigDouble*)dst)->n[j++] = c;
    else {
      if (s[i] == coma) {
        ((BigDouble*)dst)->cpos = j;
        cma = 1;
      } else if (s[i] == '-')
        ssig = -1;
      else
        showError(2);
    }
  }

  //si nos env�an un negativo, restamos una posici�n
  if (s[0] == '-') 
    --f;
  
  //si no hay decimales y hay signo negativo, cpos = -1
  if (((BigDouble*)dst)->cpos < 0)
    ((BigDouble*)dst)->cpos = 0;

  //si hay decimales, restamos una posici�n
  if (cma == 1)
    --f;

  ((BigDouble*)dst)->count = f;

  if (ssig == -1)
    ((BigDouble*)dst)->n[((BigDouble*)dst)->count] *= -1;

  //ajustamos el indicador de signo
  ((BigDouble*)dst)->sig = ssig;
}

/*
 * Funci�n validateBD
 *
 * Valida que todos los datos del BD sean coherentes
 */
void validateBD(void* a) {
  int i = 0;

  //validamos el tipo
  if (((BigDouble*)a)->k != 'd')
    showError(99);

  //validamos la longitud
  if (((BigDouble*)a)->count < 0 || ((BigDouble*)a)->count > D_MAX_LENGTH)
    showError(99);

  //validamos la posici�n decimal
  if(((BigDouble*)a)->cpos < 0 || ((BigDouble*)a)->cpos > D_MAX_LENGTH)
    showError(99);

  //validamos el resto de d�gitos, que pueden ser positivos o negativos
  for (; i < D_MAX_LENGTH; i++) {
    if (((BigDouble*)a)->n[i] < -9 || ((BigDouble*)a)->n[i] > 9)
      showError(99);
  }
}