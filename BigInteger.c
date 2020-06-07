/*
 * BigInteger.c
 *
 *  Created on: 18 ene. 2019
 *      Author: DoHITB
 *
 *  CHANGELOG
 *    v1.1
 *      - Revisar las funciones y variables, hacer "static" convenientemente.
 *      - Cambiar las referencias de punteros a "void" para evitar consumo de memoria
 *      - Añadidas validaciones en las creaciones de BigInteger
 *      - Nueva función BImemcpy para recuperar valores útiles de BigInteger
 *      - Nueva función validateBI para validar la estructura de un BigInteger pasado como puntero
 *    v1.2
 *      - Añadida mejora de rendimiento para función pow
 *    v1.3
 *      - Parche para evitar acumulación de memoria
 *      - Eliminada librería time.h (inecesaria)
 *    v1.4
 *      - Cambio de función BISQRT por nqrt, que realiza n-ésimas potencias.
 *      - Eliminar "static" de BIT (innecesario)
 *      - Creada función pAdd, wrapper de add sin validación de punteros para uso interno
 *      - Creada función pSub, wrapper de sub sin validación de punteros para uso interno
 *      - Creada función sMul, wrapper de mul sin validación de punteros para uso interno
 *      - Modificados nombres de parámetros en la cabecera para nomenclatura coherente
 *      - Eliminado error 19 (referencia innecesaria a BISQRT)
 *      - Modificada función signum para trabajar con int en lugar de BigInteger.
 *        ==> Reducido un 90,566% el consumo de memoria de nqrt a través del uso de wrappers
 *        ==> Aumentado un 53,656% el rendimiento de nqrt a través del uso de wrappers
 *          ==> Se usa nqrt como referencia debido a que nqrt -> pow -> mul -> pmul -> sum
 *            Es el camino que más funciones concatena.
 *    v1.5
 *      - Cambio de función toString para que mueva los datos a un char* en lugar de printarlos
 *    v1.51
 *      - Eliminar librería conio.h (innecesaria)
 *    v1.6
 *      - Bugfix en validateBI para números negativos
 *      - Bugfix en toString para números negativos
 *    v2.0
 *      - Test Integrado (324 operaciones distintas) finalizado. Cambios realizados
 *        * Quitar validateBI de pSub
 *        * Bugfix en suma si b < 0 y |a| < |b|
 *        * Bugfix en suma si |a| < |b| y a, b < 0
 *        * Buffix en división si a / b, b < 0
 *      - Limpieza de código
 *    v2.1
 *      - Bugfix en funcion "division"
 */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "BigInteger.h"

int MAX_LENGTH = 4096;
float BI_VERSION = 2.1f;

/*
 * Función initialize
 *
 * Da valores a ciertos datos útiles.
 */
static void _BI_initialize() {
  newBI(&_ZERO, "0", 0);
  newBI(&_ONE, "1", 0);
  newBI(&_TEN, "10", 0);
  newBI(&_HUND, "100", 0);
  newBI(&_MIN, "1", -1);
}

/*
 * Función BImemcpy
 *
 * Copia en el puntero destino la variable útil deseada
 */
void BImemcpy(void* dst, int value) {
  if (ini == 0) {
    ini = 1;
    _BI_initialize();
  }
  
  if (value == 0)
    memcpy(dst, &_ZERO, sizeof(struct BigInteger));
  else if(value == 1)
    memcpy(dst, &_ONE, sizeof(struct BigInteger));
  else if(value == 10)
    memcpy(dst, &_TEN, sizeof(struct BigInteger));
  else if(value == 100)
    memcpy(dst, &_HUND, sizeof(struct BigInteger));
  else if(value == -1)
    memcpy(dst, &_MIN, sizeof(struct BigInteger));
}

/*
 * Función newBI.
 *
 * Genera un nuevo dato BI a partir del string que recibe como entrada.
 * Se cargan en orden inverso para permitir el crecimiento de manera sencilla.
 * Si "sig" es -1, se crea el número en negativo
 *
 */
void newBI(void *dst, char* s, int sig){
  struct BigInteger* ret = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  int i = strlen(s) - 1;
  int f = i;
  int j = 0;
  int c;

  //validamos puntero
  if (ret == NULL) 
    showError(2);

  //limpiamos el array
  clean(ret);

  if (i > MAX_LENGTH)
    showError(1);

  //recorremos el string y lo guardamos en integers
  for (; i >= 0; i--) {
    c = (int)(s[i] - 48);

    if (c >= 0 && c <= 9)
      ret->n[j++] = c;
    else
      showError(3);
  }

  ret->count = f;

  if(sig == -1)
    ret->n[ret->count] *= -1;

  memcpy(dst, ret, sizeof(struct BigInteger));


  free(ret);
}

/*
 * Función pAdd. Usar para sumar dos números.
 *
 * Realiza la operación de suma, teniendo en cuenta los signos de los números.
 *
 * Si los signos son iguales, hace una suma, sino, una resta.
 */
static void pAdd(void *va, void *vb){
  struct BigInteger* a = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* b = (struct BigInteger*)malloc(sizeof(struct BigInteger));

  if (a == NULL || b == NULL)
    showError(4);

  memcpy(a, va, sizeof(struct BigInteger));
  memcpy(b, vb, sizeof(struct BigInteger));

  //int sig = signum(a, b);
  int sig = signum(a->n[a->count], b->n[b->count]);

  //normalizamos los operandos
  if(sig == 10)
    //a negativo, b positivo. Cambiamos el signo de "a" y hacemos suma
    a->n[a->count] *= -1;
  else if (sig == 1)
    //b negativo, a positivo. Cambiamos el signo de "b" y hacemos suma
    b->n[b->count] *= -1;
  else if (sig == 11){
    //a negativo, b negativo. Cambiamos signos y hacemos suma
    a->n[a->count] *= -1;
    b->n[b->count] *= -1;
  }

  //si ambos signos son iguales, se suma, sino, se resta
  if (sig == 0 || sig == 11)
    addition(a, b);
  else 
    pSub(a, b);

  if(sig == 10 || sig == 11)
    //en estos casos, siempre se le va la vuelta al signo
    a->n[a->count] *= -1;

  //ajustamos el resultado
  memcpy(va, a, sizeof(struct BigInteger));

  //liberamos memoria
  free(a);
  free(b);
}

/*
 * Función add. Usar para sumar dos números.
 *
 * Realiza la operación de suma, teniendo en cuenta los signos de los números.
 *
 * Si los signos son iguales, hace una suma, sino, una resta.
 */
void add(void *va, void *vb){
  //validamos los datos antes de tratarlos
  validateBI(va);
  validateBI(vb);

  //delegamos en la función estática
  pAdd(va, vb);
}

/*
 * Función addition.
 *
 * Simula la operación a = a + b
 */
static void addition(void* va, void* vb) {
  struct BigInteger* a = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* b = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  int limit;
  int min;
  int move;
  int i;

  if (a == NULL || b == NULL)
    showError(5);

  memcpy(a, va, sizeof(struct BigInteger));
  memcpy(b, vb, sizeof(struct BigInteger));

  //asumimos que a tiene la mayor longitud
  limit = a->count;

  //asumimos que b tiene la menor longitud
  min = b->count;

  //indicador de necesidad de arrastre
  move = 0;
  i = 0;

  //si no es así, rectificamos
  if(b->count > limit){
    limit = b->count;
    min = a->count;
    move = 1;
  }

  //sumamos los dígitos que coinciden
  for(;i <= min;i++)
    a->n[i] += b->n[i];

  //los dígitos que no coinciden los traspasamos
  if(move == 1){
    for(;i <= limit;i++)
      a->n[i] = b->n[i];

    a->count = limit;
  }

  //gestionamos el acarreo
  carryAdd(a);

  //ajustamos resultado
  memcpy(va, a, sizeof(struct BigInteger));

  //liberamos memoria
  free(a);
  free(b);
}

/*
 * Función carryAdd.
 *
 * Gestiona el acarreo de la suma. Si hay movimiento de datos, se mueve el valor 1 a ret.
 * De esta manera, podemos llamar hasta que no haya cambios en el acarreo.
 */
static void carryAdd(void* va) {
  int i;
  int m = 0;
  int acc;
  int ret;
  struct BigInteger* a = (struct BigInteger*)malloc(sizeof(struct BigInteger));

  if (a == NULL)
    showError(6);

  memcpy(a, va, sizeof(struct BigInteger));

  do{
    ret = 0;
    acc = 0;

    //recorremos a y vamos sumando el acarreo
    for(i = m;i <= a->count; i++){
      //sumamos acarreo
      a->n[i] += acc;

      //como acc es int, podemos dividir entre 10 y sacar el acarreo
      acc = a->n[i] / 10;

      if (acc > 0) {
        //normalizamos el número
        a->n[i] = a->n[i] % 10;
        
        //si es el primer dígito del que hacemos acarreo, lo guardamos
        if (ret == 0)
          m = i + 1;

        ret = 1;
      }
    }

    //si ha quedado acarreo, lo guardamos al final;
    if (acc > 0) {
      if (a->count == MAX_LENGTH)
        showError(1);
      else {
        a->n[++a->count] = acc;

        //si es el primer dígito del que hacemos acarreo, lo guardamos
        if (ret == 0)
          m = i + 1;

        ret = 1;
      }
    }
  }while(ret > 0);

  //ajustamos resultado
  memcpy(va, a, sizeof(struct BigInteger));

  //liberamos memoria
  free(a);
}

/*
 * Función pSub. Usar para restar dos números.
 *
 * Simula la operación a = a - b.
 *
 * Si len(a) < len(b), se intercambian los valores
 */
static void pSub(void* va, void *vb){
  int sig;
  int comp;
  struct BigInteger* a = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* b = (struct BigInteger*)malloc(sizeof(struct BigInteger));

  if (a == NULL || b == NULL)
    showError(7);

  memcpy(a, va, sizeof(struct BigInteger));
  memcpy(b, vb, sizeof(struct BigInteger));

  hardEquals(a, b, &comp);
  sig = signum(a->n[a->count], b->n[b->count]);

  //si ambos son negativos, comp = 1 significa a < b
  if((comp == 2 && sig < 11) || (comp == 1 && sig == 11)){
    pSub(b, a);

    //cambiamos el signo
    b->n[b->count] *= -1;

    //reasignamos
    //a = b;
    memcpy(a, b, sizeof(struct BigInteger));
  }else if(comp == 0){
    a->n[0] = 0;
    a->count = 0;
  }else{
    //normalizamos los signos
    if(sig == 1)
      b->n[b->count] *= -1;
    else if(sig == 10){
      a->n[a->count] *= -1;
    }else if(sig == 11){
      a->n[a->count] *= -1;
      b->n[b->count] *= -1;
    }

    //si tienen el mismo signo, se resta, sino se suma
    if(sig == 0 || sig == 11)
      subtract(a, b, a);
    else 
      pAdd(a, b);

    if(sig == 10 || sig == 11)
      //en estos casos, siempre se le va la vuelta al signo
      a->n[a->count] *= -1;
  }

  //ajustamos el resultado
  memcpy(va, a, sizeof(struct BigInteger));

  //liberamos memoria
  free(a);
  free(b);
}

/*
 * Función sub. Usar para restar dos números.
 *
 * Simula la operación a = a - b.
 *
 * Si len(a) < len(b), se intercambian los valores
 */
void sub(void *va, void *vb) {
  //validamos los punteros antes de tratarlos
  validateBI(va);
  validateBI(vb);

  //delegamos en la función estática
  pSub(va, vb);
}

/*
 * Función subrtact.
 *
 * Realiza la resta.
 */
static void subtract(void *va, void *vb, void *vret){
  int i = 0;
  int accType = 0;
  struct BigInteger* a = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* b = (struct BigInteger*)malloc(sizeof(struct BigInteger));

  if (a == NULL || b == NULL)
    showError(8);

  memcpy(a, va, sizeof(struct BigInteger));
  memcpy(b, vb, sizeof(struct BigInteger));

  //restamos los dígitos comunes
  for(;i <= b->count;i++)
    a->n[i] -= b->n[i];

  //si el último dígito es negativo
  if(a->n[a->count] < 0)
    accType = 1;

  carrySub(a, accType);

  //movemos el resultado
  memcpy(vret, a, sizeof(struct BigInteger));

  //liberamos memoria
  free(a);
  free(b);
}

/*
 * Función carrySub.
 *
 * Gestiona el acarreo de la resta. Si carryType = 0, el acarreo
 * se gestiona como a = 10 + a; sino, se invierte el signo (excepto al último dígito)
 */
static void carrySub(void *va, int carryType){
  int i;
  int m = 0;
  int acc;
  int ret;
  struct BigInteger* a = (struct BigInteger*)malloc(sizeof(struct BigInteger));

  if (a == NULL)
    showError(9);

  memcpy(a, va, sizeof(struct BigInteger));

  do{
    ret = 0;
    acc = 0;

    if(carryType == 0){
      for(i = m;i <= a->count; i++){
        //restamos el acarreo al número
        a->n[i] -= acc;

        if(a->n[i] < 0){
          //normalizamos el número
          a->n[i] += 10;
          acc = 1;

          //si es el primer dígito del que hacemos acarreo, lo guardamos
          if (ret == 0)
            m = i + 1;

          ret = 1;
        }else{
          acc = 0;
        }
      }
    }else{
      //en esta opción, no es necesario pasar una segunda vez por acarreos.
      for(i = 0;i < a->count;i++)
        if(a->n[i] < 0)
          //normalizamos el número
          a->n[i] = a->n[i] * -1;
    }

    //contamos de nuevo los dígitos
    recount(a);
  }while(ret > 0);

  //ajustamos el resultado
  memcpy(va, a, sizeof(struct BigInteger));

  //liberamos memoria
  free(a);
}

/*
 * Función recount.
 *
 * Recuenta las cifras, para ver si hay que disminuir el conteo.
 */
static void recount(void *va){
  struct BigInteger* a = (struct BigInteger*)malloc(sizeof(struct BigInteger));

  if (a == NULL)
    showError(10);

  memcpy(a, va, sizeof(struct BigInteger));

  while(a->n[a->count--] == 0);

  ++a->count;

  if(a->count < 0)
    a->count = 0;

  //ajustamos el resultado
  memcpy(va, a, sizeof(struct BigInteger));

  //liberamos memoria
  free(a);
}

/*
 * Función equals.
 *
 * Función pública de hardEquals
 * Compara dos números. Devuelve 0 si "a" = "b"; 1 si "a" > "b"; 2 si "a" < "b".
 */
void equals(void* va, void* vb, int* ret) {
  //validamos punteros
  validateBI(va);
  validateBI(vb);

  hardEquals(va, vb, ret);
}

/*
 * Función hardEquals.
 *
 * Compara dos números. Devuelve 0 si "a" = "b"; 1 si "a" > "b"; 2 si "a" < "b".
 */
static void hardEquals(void* va, void* vb, int* ret){
  int i;
  int sig;
  struct BigInteger* a = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* b = (struct BigInteger*)malloc(sizeof(struct BigInteger));

  if (a == NULL || b == NULL)
    showError(11);

  memcpy(a, va, sizeof(struct BigInteger));
  memcpy(b, vb, sizeof(struct BigInteger));

  //calculamos el signo
  /*
   *00: a >= 0, b >= 0
   *01: a >= 0, b <  0
   *10: a <  0, b >= 0
   *11: a <  0, b <  0
   */
  sig = signum(a->n[a->count], b->n[b->count]);

  if (sig == 1) {
    //a >= 0, b < 0, por tanto a > b
    *ret = 1;
  }else if (sig == 10) {
    //a < 0, b >= 0, por tanto a < b
    *ret = 2;
  }else {
    //comparten signo. Hacemos comparación manual
    *ret = 0;

    if (a->count < b->count)
      //si "a" tiene menos dígitos que "b"
      * ret = 2;
    else if (a->count > b->count)
      //si "a" tiene más dítigos que "b"
      * ret = 1;
    else {
      //tienen los mismos dígitos. Comparamos uno a uno.
      for (i = a->count; i >= 0; i--) {
        if (a->n[i] < b->n[i])
          * ret = 2;
        else if ((a->n[i] > b->n[i]))
          * ret = 1;

        if (*ret > 0)
          break;
      }
    }
    
    if (sig == 11) {
      //ambos tienen signo negativo. Cambiamos el retorno
      //ya que se p. ej. -1 > -10
      if (*ret == 2)
        * ret = 1;
      else if (*ret == 1)
        * ret = 2;
    }
  }

  //liberamos memoria
  free(a);
  free(b);
}

/*
 * Función sMul. Función para multiplicar dos números.
 *
 * Simula la operación a = a * b
 */
static void sMul(void *va, void *vb){
  int sig;
  int i = 0;
  struct BigInteger* part;
  struct BIT *table = (struct BIT*)malloc(sizeof(struct BIT));
  struct BigInteger* a = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* b = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* zero = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* one = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* min = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  int x;
  int comp;
  int calc = 0;

  if (table == NULL || a == NULL || b == NULL || zero == NULL || one == NULL || min == NULL)
    showError(12);

  memcpy(a, va, sizeof(struct BigInteger));
  memcpy(b, vb, sizeof(struct BigInteger));
  BImemcpy(zero, 0);
  BImemcpy(one, 1);
  BImemcpy(min, -1);
  part = (struct BigInteger*)malloc(sizeof(struct BigInteger) * (b->count + 1));

  if (part == NULL)
    showError(12);

  hardEquals(a, zero, &comp);

  //si a = 0, no hace falta cálculo.
  if (comp == 0) 
    calc = 1;

  hardEquals(b, zero, &comp);

  //si b = 0, no hace falta cálculo.
  if (comp == 0) {
    memcpy(a, zero, sizeof(struct BigInteger));
    calc = 1;
  }

  sig = signum(a->n[a->count], b->n[b->count]);

  //normalizamos los operandos
  if(sig == 1)
    b->n[b->count] *= -1;
  else if(sig == 10)
    a->n[a->count] *= -1;
  else if(sig == 11){
    a->n[a->count] *= -1;
    b->n[b->count] *= -1;
  }

  hardEquals(a, one, &comp);

  //si a = 1, entonces a * b = b.
  if (comp == 0) {
    memcpy(a, b, sizeof(struct BigInteger));
    calc = 1;
  }

  hardEquals(a, min, &comp);
  //si a = -1, entonces a * b = -b (el signo se ajusta a posteriori)
  if (comp == 0) {
    memcpy(a, b, sizeof(struct BigInteger));
    calc = 1;
  }

  hardEquals(b, one, &comp);

  //si b = 1, entonces a * b = a.
  if (comp == 0) 
    calc = 1;

  hardEquals(b, min, &comp);

  //si b = -1, entonces a * b = -a (el signo se ajusta a posteriori).
  if (comp == 0)
    calc = 1;

  //inicializamos BIT
  memcpy(&table->BI[0], zero, sizeof(struct BigInteger));
  memcpy(&table->BI[1], a, sizeof(struct BigInteger));

  table->status[0] = 1;
  table->status[1] = 1;
  table->status[2] = 0;
  table->status[3] = 0;
  table->status[4] = 0;
  table->status[5] = 0;
  table->status[6] = 0;
  table->status[7] = 0;
  table->status[8] = 0;
  table->status[9] = 0;

  //si el número no está ya calculado, lo calculamos
  if (calc == 0) {
    //realizamos los productos parciales
    for (i = 0; i <= b->count; i++) {
      //validamos si BIT[n] existe
      if (table->status[b->n[i]] == 0) {
        clean(&part[i]);
        //no tenemos el resultado. Calculamos el valor
        for (x = 0; x <= a->count; x++)
          part[i].n[x] = a->n[x] * b->n[i];

        part[i].count = x - 1;
        carryAdd(&part[i]);

        //movemos el valor a BIT
        table->BI[b->n[i]] = part[i];
        table->status[b->n[i]] = 1;
      }else {
        //tenemos el resultado
        part[i] = table->BI[b->n[i]];
      }

      pMul(i, &part[i]);
    }

    //realizamos el sumatorio
    for (i = 1; i <= b->count; i++)
      pAdd(&part[0], &part[i]);

    memcpy(a, &part[0], sizeof(struct BigInteger));
  }

  //si los signos son diferentes, invertimos el signo
  if(sig == 1 || sig == 10)
    a->n[a->count] *= -1;

  //ajustamos el resultado
  memcpy(va, a, sizeof(struct BigInteger));

  //liberamos memoria
  free(part);
  free(table);
  free(a);
  free(b);
  free(zero);
  free(one);
  free(min);
}

/*
 * Función mul. Función para multiplicar dos números.
 *
 * Simula la operación a = a * b
 */
void mul(void *va, void *vb){
  //validamos los datos antes de usarlos
  validateBI(va);
  validateBI(vb);

  //delegamos en la función estática
  sMul(va, vb);
}

/*
 * Función pMul.
 *
 * Efectúa una multiplicación parcial de a y b
 */
static void pMul(int pos, void *vpart){
  struct BigInteger* part = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  int i;

  if (part == NULL)
    showError(13);

  memcpy(part, vpart, sizeof(struct BigInteger));
  i = part->count + pos;

  //generamos offset
  for (; i >= pos; i--) 
    part->n[i] = part->n[i - pos];

  //noramlizamos las posiciones de offset
  for (i = 0; i < pos; i++) 
    part->n[i] = 0;

  part->count += pos;

  //ajustamos resultado
  memcpy(vpart, part, sizeof(struct BigInteger));

  //liberamos memoria
  free(part);
}

/*
 * Función dvs. Función para dividir dos números.
 *
 * Simula la operación a = a / b
 */
void dvs(void *va, void *vb){
  struct BigInteger* a = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* b = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* temp = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* one = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  int sig;
  int comp;

  if (a == NULL || b == NULL || temp == NULL || one == NULL)
    showError(14);

  //validamos punteros
  validateBI(va);
  validateBI(vb);

  memcpy(a, va, sizeof(struct BigInteger));
  memcpy(b, vb, sizeof(struct BigInteger));

  BImemcpy(temp, 0);
  BImemcpy(one, 1);

  sig = signum(a->n[a->count], b->n[b->count]);

  //normalizamos los operandos
  if(sig == 1)
    b->n[b->count] *= -1;
  else if(sig == 10)
    a->n[a->count] *= -1;
  else if(sig == 11){
    a->n[a->count] *= -1;
    b->n[b->count] *= -1;
  }

  hardEquals(a, b, &comp);

  if (comp == 0)
    //si a = b, a/b = 1
    memcpy(a, one, sizeof(struct BigInteger));
  else if (comp == 2)
    //si a < b, a / b = 0
    memcpy(a, temp, sizeof(struct BigInteger));
  else if(comp == 1){
    //si a > b, buscamos un número que solucione ax = n
    hardEquals(b, one, &comp);

    if(comp != 0)
      divide(a, b);
  }

  //si los signos son diferentes, invertimos el signo
  if (sig == 1 || sig == 10)
    a->n[a->count] *= -1;

  //ajustamos resultado
  memcpy(va, a, sizeof(struct BigInteger));

  //liberamos memoria
  free(a);
  free(b);
  free(temp);
  free(one);
}

/*
 * Función divide.
 *
 * Realiza la división utilizando el teorema de Bolzano
 */
static void divide(void *va, void *vb){
  struct BigInteger* a = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* b = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BIT *table = (struct BIT*)malloc(sizeof(struct BIT));
  struct BigInteger* dTemp = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* ret = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* temp = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* biTemp = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  int len;
  int i = 0;
  int x = 0;
  int max;
  int eq;
  int currentBIT;
  int res = 0;
  int mLen;
  int added;

  if (a == NULL || b == NULL || table == NULL || dTemp == NULL || ret == NULL || temp == NULL || biTemp == NULL)
    showError(15);

  memcpy(a, va, sizeof(struct BigInteger));
  memcpy(b, vb, sizeof(struct BigInteger));
  BImemcpy(ret, 0);

  len = a->count - b->count;
  max = b->count;
  ret->count = 0;
  dTemp->count = 0;
  
  //llenamos BIT
  BImemcpy(&table->BI[0], 0);
  memcpy(&table->BI[1], b, sizeof(struct BigInteger));

  table->status[0] = 1;
  table->status[1] = 1;
  table->status[2] = 0;
  table->status[3] = 0;
  table->status[4] = 0;
  table->status[5] = 0;
  table->status[6] = 0;
  table->status[7] = 0;
  table->status[8] = 0;
  table->status[9] = 0;

  currentBIT = 1;

  //iniciamos dTemp y temp
  BImemcpy(dTemp, 0);
  BImemcpy(temp, 0);
  
  /*
   * Nos quedamos con los "b.count" primeros dígitos. Si "b" tiene un único
   * dígito, no movemos nada, porque ya moveremos el siguiente dígito más adelante
   */
  if (b->count > 0) {
    for(;i < b->count;i++)
      dTemp->n[b->count - i - 1] = a->n[a->count - i];
  }else {
    dTemp->n[0] = 0;
  }

  dTemp->count = b->count - 1;

  if (dTemp->count == 0)
    --dTemp->count;

  //si "b" tiene una cifra, b.len será 0 pero tenemos que restar una cifra igualmente
  if (b->count == 0)
    mLen = len - 1;
  else
    mLen = len;

  //por cada cifra decimal que hemos generado
  for (i = 0; i <= len; i++) {
    //hacemos BI temporal
    temp->count = 0;
    temp->n[0] = a->n[len - i];

    pAppend(temp, dTemp);
    memcpy(dTemp, temp, sizeof(struct BigInteger));

    //Retorna 0 si son iguales, retorna 1 si a > b, retorna 2 si a < b.
    hardEquals(dTemp, &table->BI[currentBIT], &eq);

    x = currentBIT;

    if (eq == 1) {
      //si dTemp > max, partimos de ese valor y hasta el máximo.
      
      //recuperamos el último BI
      memcpy(biTemp, &table->BI[x], sizeof(struct BigInteger));
      added = 0;

      for (; x < 10; x++) {
        //le sumamos la base
        pAdd(biTemp, b);

        //lo añadimos a BIT, si no hemos llenado la tabla (rellenamos siempre por adelantado)
        if (currentBIT < 9) {
          memcpy(&table->BI[++currentBIT], biTemp, sizeof(struct BigInteger));
          table->status[currentBIT] = 1;
          added = 1;
        }else
          added = 0;

        //validamos si nos sirve
        hardEquals(dTemp, biTemp, &eq);

        if (eq == 0) {
          //si dTemp = temp
          res = currentBIT;
          x = 99;
        }else if (eq == 2) {
          //si dTemp < max, según el teorema de Bolzano, hemos pasado por dTemp = max, 
          //por tanto, el valor correcto es el anterior
          //si no hemos llegado al final de la tabla, será el índice anterior
          //si hemos llegado al final de la tabla, será el índice actual
          if (added == 1)
            res = (currentBIT - 1);
          else
            res = currentBIT;

          x = 99;
        }
      }
    }else if (eq == 0) {
      //si dTemp = max, ya tenemos el valor
      res = currentBIT;
    }else if (eq == 2) {
      //si dTemp < max, partimos de ese valor y hasta 0
      --x;

      for (; x >= 0; x--) {
        //validamos si nos sirve
        hardEquals(dTemp, &table->BI[x], &eq);

        if (eq == 0) {
          //si dTemp = temp
          res = x;
          x = -99;
        }
        else if (eq == 1) {
          //si dTemp > max, según el teorema de Bolzano, hemos pasado por dTemp = max, 
          //por tanto, el valor correcto es el siguiente
          res = x;
          x = -99;
        }
      }
    }

    ret->n[len - i] = res;

    //restamos
    pSub(dTemp, &table->BI[res]);
  }

  ret->count = mLen;

  //*a = ret;
  memcpy(a, ret, sizeof(struct BigInteger));

  recount(a);
  
  //evitar error de falso desbordamiento
  if (a->n[a->count + 1] > 0)
    ++a->count;
  
  //ajustamos valores
  memcpy(va, a, sizeof(struct BigInteger));

  //liberamos memoria
  free(table);
  free(a);
  free(b);
  free(dTemp);
  free(ret);
  free(temp);
  free(biTemp);
}

/*
 * Función makeBase
 *
 * simula la operación base = b * 10^length
 */
static void makeBase(void* vbase, void *vb, int length, int* start){
  struct BigInteger* base = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* b = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  int i = 0;

  if (base == NULL || b == NULL)
    showError(16);

  memcpy(base, vbase, sizeof(struct BigInteger));
  memcpy(b, vb, sizeof(struct BigInteger));

  //estos dos bucles se pueden paralelizar
  for(;i < length;i++)
    base->n[i] = 0;

  base->count = i;
  *start = i;

  for(i = 0;i <= b->count;i++)
    base->n[base->count++] = b->n[i];

  --base->count;

  //ajustamos resultado
  memcpy(vbase, base, sizeof(struct BigInteger));

  //liberamos memoria
  free(base);
  free(b);
}

/*
 * Función shift.
 *
 * Desplaza un BI en "start" posiciones
 */
static void shift(void* vbase, int start){
  struct BigInteger *base = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  int i = start;

  if (base == NULL)
    showError(17);

  memcpy(base, vbase, sizeof(struct BigInteger));

  for (; i <= base->count; i++) 
    base->n[i] = base->n[i + 1];

  base->n[base->count] = 0;

  --base->count;

  //ajustamos resultado
  memcpy(vbase, base, sizeof(struct BigInteger));

  //libreamos memoria
  free(base);
}

/*
 * Función rev.
 *
 * Da la vuelta a un BigInteger
 */
static void rev(void* va) {
  struct BigInteger* a = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  int i = 0;
  int t = 0;
  int max;

  if (a == NULL)
    showError(18);

  memcpy(a, va, sizeof(struct BigInteger));

  max = a->count / 2;

  for (; i <= max; i++) {
    t = a->n[i];
    a->n[i] = a->n[a->count - i];
    a->n[a->count - i] = t;
  }

  //ajustamos resultados
  memcpy(va, a, sizeof(struct BigInteger));

  //liberamos memoria
  free(a);
}

/*
 * Función nqrt.
 *
 * Realiza la raíz enésima de a.
 */
void nqrt(void* va, int n) {
  struct BigInteger* a = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* ret = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* i = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* one = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  int stop = 0;

  if (a == NULL || ret == NULL || i == NULL || one == NULL)
    showError(26);

  //validamos punteros
  validateBI(va);

  memcpy(a, va, sizeof(struct BigInteger));
  BImemcpy(ret, 0);
  BImemcpy(i, 0);
  BImemcpy(one, 1);

  hardEquals(a, ret, &stop);

  if (stop == 0) {
    free(a);
    free(i);
    free(one);
    free(ret);

    return;
  }

  do {
    pAdd(ret, one);
    memcpy(i, ret, sizeof(struct BigInteger));
    pow(i, n);
    hardEquals(i, a, &stop);
  } while (stop == 2);

  if (stop > 0)
    pSub(ret, one);

  //ajustamos resultado
  memcpy(va, ret, sizeof(struct BigInteger));

  //liberamos memoria
  free(a);
  free(ret);
  free(i);
  free(one);
}

/*
 * Función pow.
 *
 * Simula la operación a = a^p
 */
void pow(void *va, int p) {
  struct BigInteger *a = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  int z = 1;
  struct BigInteger* res = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  int d2b[10];
  int d2bi = 0;
  int x = 0;
  int t = p;
  int i = 0;
  struct BigInteger* tmp = (struct BigInteger*)malloc(sizeof(struct BigInteger));

  if (a == NULL || res == NULL)
    showError(20);

  //validamos puntero
  validateBI(va);

  memcpy(a, va, sizeof(struct BigInteger));

  if (p == 0)
    //n^0 = 1
    BImemcpy(a, 1);
  else if (p == 1) {
    //n^1 = n
    //limpiamos memoria
    free(a);
    free(res);
    free(tmp);

    return;
  }else {
    BImemcpy(res, 1);

    /*
     * 1.2: Factorizamos la potencia en base 2 para poder
     *      mejorar el rendimiento de la operación
     *
     *      Como funciona
     *       - Convertimos el exponente en base 2, y almacenamos
     *         los datos en un array.
     *
     *       - Por cada iteración, vamos calculando a^2^i, es decir
     *         a^0, a^2, a^4, a^8, a^16, ...
     *
     *       - Si la posición de d2b es 1, multiplicamos, y de esta manera
     *         obtenemos el resultado.
     *
     *         Por ejemplo 8^12
     *           12 = 1100 (aunque se guarda como 0011) ==> 4 + 8
     *           8^12 = 8^4 * 8^8 = 8^(4+8)
     *
     *         De esta manera, hacemos únicamente 8 multiplicaciones en lugar de 12
     *         (para este ejemplo). En números más grandes la diferencia sería aún mayor
     */
    //conversión de decimal a binario
    while (t > 0 && d2bi < 10) {
      //dividimos el número por dos
      x = t / 2;

      //guardamos el resto en d2b
      d2b[d2bi++] = (t - x * 2);

      //reasignamos el valor
      t = x;
    }

    //si el número es muy grande, damos error
    if (t > 0 && d2bi >= 10)
      showError(25);

    //iteramos sobre el número binario
    for (; i < d2bi; i++) {
      //calculamos (a^(2^i))
      if (i == 0)
        memcpy(tmp, a, sizeof(struct BigInteger));
      else
        sMul(tmp, tmp);

      if (d2b[i] == 1)
        sMul(res, tmp);
      
    }

    memcpy(a, res, sizeof(struct BigInteger));
  }

  //ajustamos resultado
  memcpy(va, a, sizeof(struct BigInteger));

  //limpiamos memoria
  free(a);
  free(res);
  free(tmp);
}

/*
 * Función showError.
 *
 * Muestra un error en base al índice que se le pasa
 */
static void showError(int k){
  if (k == 1)
    printf("Error. Limite alcanzado");
  else if (k == 2)
    printf("Error. Puntero erróneo en newBI");
  else if (k == 3)
    printf("Error. Datos erróneos en newBI");
  else if (k == 4)
    printf("Error. Puntero erróneo en add");
  else if (k == 5)
    printf("Error. Puntero erróneo en addition");
  else if (k == 6)
    printf("Error. Puntero erróneo en carryAdd");
  else if (k == 7)
    printf("Error. Puntero erróneo en sub");
  else if (k == 8)
    printf("Error. Puntero erróneo en subraction");
  else if (k == 9)
    printf("Error. Puntero erróneo en carrySub");
  else if (k == 10)
    printf("Error. Puntero erróneo en recount");
  else if (k == 11)
    printf("Error. Puntero erróneo en hardEquals");
  else if (k == 12)
    printf("Error. Puntero erróneo en mul");
  else if (k == 13)
    printf("Error. Puntero erróneo en pMul");
  else if (k == 14)
    printf("Error. Puntero erróneo en dvs");
  else if (k == 15)
    printf("Error. Puntero erróneo en divide");
  else if (k == 16)
    printf("Error. Puntero erróneo en makeBase");
  else if (k == 17)
    printf("Error. Puntero erróneo en shift");
  else if (k == 18)
    printf("Error. Puntero erróneo en rev");
  /*else if (k == 19)
    printf("Error. Puntero erróneo en BISQRT");*/
  else if (k == 20)
    printf("Error. Puntero erróneo en pow");
  else if (k == 21)
    printf("Error. Puntero erróneo en toString");
  else if (k == 22)
    printf("Error. Puntero erróneo en pAppend");
  else if (k == 23)
    printf("Error. Puntero erróneo en clean");
  else if (k == 24)
    printf("Error. Puntero erróneo en signum");
  else if (k == 25)
    printf("Error. Exponente demasiado grande");
  else if (k == 26)
    printf("Error. Puntero erróneo en nqrt");
  else if (k == 98)
    printf("Error. Puntero erróneo en validateBI");
  else if (k == 99)
    printf("Error. Error de validación de datos");
  else
    printf("Error. Error desconocido");

    printf("\n");

  exit(k * -1);
}

/*
 * Función toString.
 *
 * Escribe en pantalla el BigInteger
 */
void toString(void *vb, char* dst){
  struct BigInteger* b = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  int i = 0;

  if (b == NULL)
    showError(21);

  //validamos puntero
  validateBI(vb);

  memcpy(b, vb, sizeof(struct BigInteger));
  
  //si el primer dígito es negativo, lo volvemos positivo y marcamos el caracter
  if (b->n[b->count] < 0) {
    b->n[b->count] *= -1;
    dst[i++] = '-';
  }

  for (; b->count >= 0; b->count--)
    dst[i++] = (char)(b->n[b->count] + 48);

  dst[i] = '\0';

  //libreamos memoria
  free(b);
}

/*
 * Función append
 *
 * Función pública para adjuntar BigInteger.
 *
 * Adjunta un BigInteger a otro. OJO, esta función se debe usar en el orden
 * contrario al habitual. Si queremos unir "1" y "2" para tener "12",
 * a = 2 y b = 1 (ya que los array de BigInteger van al revés.
 */
void append(void *va, void *vb){
  //validamos punteros
  validateBI(va);
  validateBI(vb);

  pAppend(va, vb);
}

/*
 * Función pAppend.
 *
 * Adjunta un BigInteger a otro. OJO, esta función se debe usar en el orden
 * contrario al habitual. Si queremos unir "1" y "2" para tener "12",
 * a = 2 y b = 1 (ya que los array de BigInteger van al revés.
 */
static void pAppend(void *va, void *vb){
  struct BigInteger* a = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* b = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* aux = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  int i = 0;
  int x = 0;

  if (a == NULL || b == NULL || aux == NULL)
    showError(22);

  memcpy(a, va, sizeof(struct BigInteger));
  memcpy(b, vb, sizeof(struct BigInteger));

  //si b = 0, no hacemos nada
  //no usamos equals para no dañar el rendimiento
  if (b->count == 0 && b->n[0] == 0) {
    //libreamos memoria
    free(a);
    free(b);
    free(aux);

    return;
  }

  aux->count = 0;

  if (a->count == 0) {
    for (; i <= b->count; i++)
      a->n[++a->count] = b->n[i];
  }else {
    //ponemos tantos 0's como posiciones tenga "a"
    for (; i <= a->count; i++)
      aux->n[aux->count++] = 0;

    //traspasamos "b" a aux
    for (i = 0; i <= b->count; i++)
      aux->n[aux->count++] = b->n[i];

    --aux->count;

    pAdd(a, aux);
  }

  //ajustamos el resultado
  memcpy(va, a, sizeof(struct BigInteger));

  //libreamos memoria
  free(a);
  free(b);
  free(aux);
}

/*
 * Función clean.
 *
 * Limpia la estructura
 */
static void clean(void *va){
  struct BigInteger* a = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  int i = MAX_LENGTH - 1;

  if (a == NULL)
    showError(23);

  memcpy(a, va, sizeof(struct BigInteger));

  //Si a->count tiene un valor válido
  if (a->count >= 0 && a->count < MAX_LENGTH)
    for (; i > a->count; i--)
      a->n[i] = 0;
  else
    for (i = 0; i < MAX_LENGTH; i++)
      a->n[i] = 0;

  //ajustamos resultado
  memcpy(va, a, sizeof(struct BigInteger));

  //limpiamos memoria
  free(a);
}

/*
 * Función signum.
 *
 * Devuelve la cantidad de datos negativos que hay en la operación.
 */
static int signum(int a, int b){
  int ret = 0;
  
  if (a < 0)
    ret = 10;

  if (b < 0)
    ++ret;

  return ret;
}

/*
 * Función validateBI
 *
 * Valida que todos los datos del BI sean coherentes
 */
void validateBI(void* a) {
  struct BigInteger* temp = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  int* t = (int*)(malloc(sizeof(int)));
  int i = 0;

  memcpy(temp, a, sizeof(struct BigInteger));

  if (t == NULL || a == NULL || temp == NULL)
    showError(98);

  t = temp;

  //validamos la variable de longitud
  if (*t < 0 || *t > MAX_LENGTH)
    showError(99);

  t++;

  //validamos el resto de dígitos, que pueden ser positivos o negativos
  for (; i < MAX_LENGTH; i++) {
    if (*t < -9 || *t > 9)
      showError(99);

    t++;
  }

  t -= MAX_LENGTH;
  t -= 1;

  free(t);
}
