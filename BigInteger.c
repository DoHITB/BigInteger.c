/*
 * BigInteger.c
 *
 *  Created on: 18 ene. 2019
 *    Author: DoHITB under MIT License
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
 *        ==> Se usa nqrt como referencia debido a que nqrt -> pow -> mul -> pmul -> sum
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
 *    v2.2
 *      - Optimización de memoria: arreglada fuga de memoria en validateBI
 *    v2.3
 *      - Renombrada función "pow" por "bipow" para evitar conflictos internos.
 *    v2.4
 *      - Solucionar error con división cuando len(a) - len(b) = 1
 *    v2.5
 *      - Cambio en nqrt para mejora de rendimiento. Cálculo de raíz por Bolzano
 *    v2.51
 *      - Bugfix en validateBI
 *    v3.0
 *      - Revisión general.
 *        * Eliminadas líneas comentadas innecesarias
 *        * Cambiada la firma de subtraction por subtract(a, b)
 *        * Eliminada la función "rev" (innecesaria)
 *        * Eliminada variable "min" de multiplicación
 *        * Eliminada validación "a = -1" y "b = -1" de multiplicación, ya que nunca se darán
 *        * Añadida validación de longitud en pMul para evitar heap overflow
 *        * Cambio en sMul. En lugar de hacer los productos y sumar, se va realizando el sumatorio
 *          de manera gradual, de esta manera se consume menos memoria y se incrementa el rendimiento
 *          (-26'946% de tiempo empleado en 10.000 multiplicaciones con dos números de 50 cifras)
 *        * Cambio en carryAdd. Se elimina el bucle "Do/While", ya que es innecesario. Con esto
 *          se incrementa el rendimiento (-4'414% de tiempo empleado en 10.000 multiplicaciones con
 *          dos números de 50 cifras)
 *        * Eliminar función makeBase. Innecesaria
 *        * Eliminar función shift. Innecesaria
 *        * Nueva función iniStr. Reserva memoria para usar en toString
 *        * Limpieza de código
 *    v3.01
 *      - Modificado "carrySub" para eliminar bucle "Do/While".
 *    v3.02
 *      - Modificado "carryAdd". El acarreo se hace en dos partes.
 *        * Parte común: se evalúa todo el acarreo
 *        * Parte no común: se evalúa hasta que no hay acarreo.
 *        * Incremento del 3.448% de rendimiento con 10.000 sumas de 100 dígitos
 *    v3.1
 *    - Modificado "nqrt"
 *      * cambio en las llamadas de "add" por "pAdd"
 *      * cambio en las llamadas de "equals" por "hardEquals"
 *      * cambio en las llamadas de "sub" por "pSub"
 *      * cambio en las llamadas de "bipow" por "sBipow"
 *      * incremento del rendimiento de 38'758% basado en 10.000 raices de índice 10
 *    - Nueva función "sBiPow", wrapper de "bipow" sin validación de BigInteger
 *    - Bugfix en "carryAdd"
 *    v3.11
 *      - Eliminar variable "max" en "divide"
 *      - Eliminar vairable "x" en "pAppend"
 *    v4.0
 *      - Revisión para entrada en CUDA
 *        * Cambio en "clean". Limpiamos el puntero directamente, ahorrando un 31'638% de rendimiento
 *        * Cambio en "newBI". Usamos directamente el puntero, ahorrando un 5'479% de rendimiento
 *        * Cambio en función de suma. Usamos directamente el puntero, ahorrando un 21'223% de rendimiento global
 *          Se reduce el consumo de RAM en un 50%.
 *        * Cambio en función de suma. Usamos directamente el puntero, ahorrando un 25'957% de rendimiento global
 *          Se reduce el consumo de RAM en un 50%.
 *        * Cambio en función de multiplicación. Usamos directamente el puntero, ahorrando un 26'901% de rendimiento global
 *        * Cambio en función de división. Usamos directamente el puntero, ahorrando un 10'162% de rendimiento global
 *        * Cambio en función de raíz. Usamos directamente el puntero, ahorrando un 11'837% de rendimiento global
 *        * Cambio en función de potencia. Usamos directamente el puntero, ahorrando un 6'540% de rendimiento global
 *        * Cambio en función de print. Usamos directamente el puntero, ahorrando un 8'878% de rendimiento global
 *        * Cambio en función de append. Usamos directamente el puntero, ahorrando un 7'435% de rendimiento global
 *        * Validaciones de puntero adicionales en nqrt
 *        * Limpieza general de código
 *        * Revisión y reestructuración de los códigos de error de showError
 *        * En total, se eliminan 201 líneas de código con la optimización
 *      - Bugfix en "validateBI"
 *      - Cambiada función "dvs" para delegar en función estática "sDvs"
 *      - Cambiada función "nqrt" para delegar en función estática "sNqrt"
 *      - Nueva función "_free" para liberar memoria masivamente
 *      - Bugfix en toString para números negativos
 *      - Pasamos algunas variables a static
 *      - Eliminada librería "limits.h"
 *      - Resultados de PU
 *        * Añadido valor por defecto en BImemcpy: 0
 *        * Llamada a clean en pAppend
 *        * Bugfix en append de números negativos
 *        * Bugfix en division (0 / 0 = 1)
 *        * Bugfix en raíz de índice 0 y negativo
 *        * Bugfix en potencia de índice negativo
 *    v4.1
 *      - Bugfix en resta cuando a < b
 */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"
#include "BigInteger.h"

static int MAX_LENGTH = 4096;
static float BI_VERSION = 4.1f;

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
  else if (value == 1)
      memcpy(dst, &_ONE, sizeof(struct BigInteger));
  else if (value == 10)
      memcpy(dst, &_TEN, sizeof(struct BigInteger));
  else if (value == 100)
      memcpy(dst, &_HUND, sizeof(struct BigInteger));
  else if (value == -1)
      memcpy(dst, &_MIN, sizeof(struct BigInteger));
  else
      BImemcpy(dst, 0);
}

/*
 * Función newBI.
 *
 * Genera un nuevo dato BI a partir del string que recibe como entrada.
 * Se cargan en orden inverso para permitir el crecimiento de manera sencilla.
 * Si "sig" es -1, se crea el número en negativo
 */
void newBI(void *dst, char* s, int sig){
  int i = (int)strlen(s) - 1;
  int f = i;
  int j = 0;
  int c;

  //limpiamos el array
  clean(dst);

  if (i > MAX_LENGTH)
    showError(1);

  //recorremos el string y lo guardamos en integers
  for (; i >= 0; i--) {
      c = (int)(s[i] - 48);

      if (c >= 0 && c <= 9)
          ((struct BigInteger*)dst)->n[j++] = c;
      else
          showError(2);
  }

  ((struct BigInteger*)dst)->count = f;

  if(sig == -1)
    ((struct BigInteger*)dst)->n[((struct BigInteger*)dst)->count] *= -1;
}

/*
 * Función pAdd. Usar para sumar dos números.
 *
 * Realiza la operación de suma, teniendo en cuenta los signos de los números.
 *
 * Si los signos son iguales, hace una suma, sino, una resta.
 */
static void pAdd(void *va, void *vb){   
    int sig = signum(((struct BigInteger*)va)->n[((struct BigInteger*)va)->count],
                     ((struct BigInteger*)vb)->n[((struct BigInteger*)vb)->count]);

  //normalizamos los operandos
  if(sig == 10)
    //a negativo, b positivo. Cambiamos el signo de "a" y hacemos suma
    ((struct BigInteger*)va)->n[((struct BigInteger*)va)->count] *= -1;
  else if (sig == 1)
    //b negativo, a positivo. Cambiamos el signo de "b" y hacemos suma
    ((struct BigInteger*)vb)->n[((struct BigInteger*)vb)->count] *= -1;
  else if (sig == 11){
    //a negativo, b negativo. Cambiamos signos y hacemos suma
    ((struct BigInteger*)va)->n[((struct BigInteger*)va)->count] *= -1;
    ((struct BigInteger*)vb)->n[((struct BigInteger*)vb)->count] *= -1;
  }

  //si ambos signos son iguales, se suma, sino, se resta
  if (sig == 0 || sig == 11)
    addition(va, vb);
  else 
    pSub(va, vb);

  if(sig == 10 || sig == 11)
    //en estos casos, siempre se le va la vuelta al signo
    ((struct BigInteger*)va)->n[((struct BigInteger*)va)->count] *= -1;
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
  int limit;
  int min;
  int swap;
  int move;
  int i;

  //asumimos que a tiene la mayor longitud
  limit = ((struct BigInteger*)va)->count;

  //asumimos que b tiene la menor longitud
  min = ((struct BigInteger*)vb)->count;

  //indicador de necesidad de arrastre
  move = 0;
  i = 0;

  //si no es así, rectificamos
  if(((struct BigInteger*)vb)->count > limit){
    //intercambiamos limit y min
    swap = limit;
    limit = min;
    min = swap;

    move = 1;
  }

  //sumamos los dígitos que coinciden
  for(;i <= min;i++)
    ((struct BigInteger*)va)->n[i] += ((struct BigInteger*)vb)->n[i];

  //los dígitos que no coinciden los traspasamos
  if(move == 1){
    for(;i <= limit;i++)
      ((struct BigInteger*)va)->n[i] = ((struct BigInteger*)vb)->n[i];

    ((struct BigInteger*)va)->count = limit;
  }

  //gestionamos el acarreo
  /*
   * Si a->count = b->count --> min = a->count (Se tratarán todos los dígitos)
   * Si a->count > b->count --> min = b->count (Se hará la gestión en dos partes)
   * Si a->count < b->count --> min = a->count (Se hará la gestión en dos partes)
   */
  carryAdd(va, 1, min);
}

/*
 * Función carryAdd.
 *
 * Gestiona el acarreo de la suma. Si hay movimiento de datos, se mueve el valor 1 a ret.
 * De esta manera, podemos llamar hasta que no haya cambios en el acarreo.
 */
/*static*/ void carryAdd(void* va, int move, int min) {
  int i = 0;
  int acc;
  int limit;

  acc = 0;

  //move == 1 --> sabemos que hay una parte no común. En min está el límite de la parte común
  if (move == 1)
    limit = min;
  else
    limit = ((struct BigInteger*)va)->count;

  //recorremos a y vamos sumando el acarreo de la parte común
  for (; i <= limit; i++){
    //sumamos acarreo
    ((struct BigInteger*)va)->n[i] += acc;

    //como acc es int, podemos dividir entre 10 y sacar el acarreo
    acc = ((struct BigInteger*)va)->n[i] / 10;
    
    if (acc > 0)
      //normalizamos el número
      ((struct BigInteger*)va)->n[i] = ((struct BigInteger*)va)->n[i] % 10;
  }

  if (move == 1) {
    //queda parte no común. Acarreamos hatsa que acc sea 0, ya que la parte no común ya está normalizada
    while (acc > 0 && i <= ((struct BigInteger*)va)->count) {
      //sumamos acarreo
      ((struct BigInteger*)va)->n[i] += acc;

      //como acc es int, podemos dividir entre 10 y sacar el acarreo
      acc = ((struct BigInteger*)va)->n[i] / 10;

      if (acc > 0)
        //normalizamos el número
        ((struct BigInteger*)va)->n[i] = ((struct BigInteger*)va)->n[i] % 10;

      i++;
    }
  }

  //si ha quedado acarreo, lo guardamos al final;
  if (acc > 0) {
    if (((struct BigInteger*)va)->count == MAX_LENGTH)
      showError(1);
    else 
      ((struct BigInteger*)va)->n[++(((struct BigInteger*)va)->count)] = acc;
  }
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
  struct BigInteger* tmp = (struct BigInteger*)malloc(sizeof(struct BigInteger));

  if (tmp == NULL)
      showError(10);

  hardEquals(va, vb, &comp);
  sig = signum(((struct BigInteger*)va)->n[((struct BigInteger*)va)->count], 
               ((struct BigInteger*)vb)->n[((struct BigInteger*)vb)->count]);

  //si ambos son negativos, comp = 1 significa a < b
  if((comp == 2 && sig < 11) || (comp == 1 && sig == 11)){
    memcpy(tmp, vb, sizeof(struct BigInteger)); //tmp = b;

    pSub(vb, va);

    //cambiamos el signo
    ((struct BigInteger*)vb)->n[((struct BigInteger*)vb)->count] *= -1;

    //reasignamos
    memcpy(va, vb, sizeof(struct BigInteger));

    //rescatamos el valor original de vb
    memcpy(vb, tmp, sizeof(struct BigInteger)); 
  }else if(comp == 0)
    BImemcpy(va, 0);
  else{
    //normalizamos los signos
    if(sig == 1)
      ((struct BigInteger*)vb)->n[((struct BigInteger*)vb)->count] *= -1;
    else if(sig == 10)
      ((struct BigInteger*)va)->n[((struct BigInteger*)va)->count] *= -1;
    else if(sig == 11){
      ((struct BigInteger*)va)->n[((struct BigInteger*)va)->count] *= -1;
      ((struct BigInteger*)vb)->n[((struct BigInteger*)vb)->count] *= -1;
    }

    //si tienen el mismo signo, se resta, sino se suma
    if(sig == 0 || sig == 11)
      subtract(va, vb);
    else 
      pAdd(va, vb);

    if(sig == 10 || sig == 11)
      //en estos casos, siempre se le va la vuelta al signo
      ((struct BigInteger*)va)->n[((struct BigInteger*)va)->count] *= -1;
  }

  free(tmp);
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
static void subtract(void *va, void *vb){
  int i = 0;
  int accType = 0;

  //restamos los dígitos comunes
  for(;i <= ((struct BigInteger*)vb)->count;i++)
    ((struct BigInteger*)va)->n[i] -= ((struct BigInteger*)vb)->n[i];

  //si el último dígito es negativo
  if(((struct BigInteger*)va)->n[((struct BigInteger*)va)->count] < 0)
    accType = 1;

  carrySub(va, accType);
}

/*
 * Función carrySub.
 *
 * Gestiona el acarreo de la resta. Si carryType = 0, el acarreo
 * se gestiona como a = 10 + a; sino, se invierte el signo (excepto al último dígito)
 */
/*static*/ void carrySub(void *va, int carryType){
  int i = 0;
  int acc = 0;

  if(carryType == 0){
    for(;i <= ((struct BigInteger*)va)->count; i++){
      //restamos el acarreo al número
      ((struct BigInteger*)va)->n[i] -= acc;

      if(((struct BigInteger*)va)->n[i] < 0){
        //normalizamos el número
        ((struct BigInteger*)va)->n[i] += 10;
        acc = 1;
      }else
        acc = 0;
    }
  }else{
    //en esta opción, no es necesario pasar una segunda vez por acarreos.
    for(i = 0;i < ((struct BigInteger*)va)->count;i++)
      if(((struct BigInteger*)va)->n[i] < 0)
        //normalizamos el número
        ((struct BigInteger*)va)->n[i] = ((struct BigInteger*)va)->n[i] * -1;
  }

  //contamos de nuevo los dígitos
  recount(va);
}

/*
 * Función recount.
 *
 * Recuenta las cifras, para ver si hay que disminuir el conteo.
 */
static void recount(void *va){
  while(((struct BigInteger*)va)->n[((struct BigInteger*)va)->count--] == 0);

  ++((struct BigInteger*)va)->count;

  if (((struct BigInteger*)va)->count < 0)
      ((struct BigInteger*)va)->count = 0;
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

  /*calculamos el signo
   *
   *00: a >= 0, b >= 0
   *01: a >= 0, b <  0
   *10: a <  0, b >= 0
   *11: a <  0, b <  0
   */
  sig = signum(((struct BigInteger*)va)->n[((struct BigInteger*)va)->count], 
               ((struct BigInteger*)vb)->n[((struct BigInteger*)vb)->count]);

  if (sig == 1) 
    //a >= 0, b < 0, por tanto a > b
    *ret = 1;
  else if (sig == 10) 
    //a < 0, b >= 0, por tanto a < b
    *ret = 2;
  else {
    //comparten signo. Hacemos comparación manual
    *ret = 0;

    if (((struct BigInteger*)va)->count < ((struct BigInteger*)vb)->count)
      //si "a" tiene menos dígitos que "b"
      * ret = 2;
    else if (((struct BigInteger*)va)->count > ((struct BigInteger*)vb)->count)
      //si "a" tiene más dítigos que "b"
      * ret = 1;
    else {
      //tienen los mismos dígitos. Comparamos uno a uno.
      for (i = ((struct BigInteger*)va)->count; i >= 0; i--) {
        if (((struct BigInteger*)va)->n[i] < ((struct BigInteger*)vb)->n[i])
          * ret = 2;
        else if ((((struct BigInteger*)va)->n[i] > ((struct BigInteger*)vb)->n[i]))
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
}

/*
 * Función sMul. Función para multiplicar dos números.
 *
 * Simula la operación a = a * b
 */
static void sMul(void* va, void* vb) {
  int sig;
  int i = 0;
  struct BigInteger* part = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* ret = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BIT* table = (struct BIT*)malloc(sizeof(struct BIT));
  struct BigInteger* zero = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* one = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  int x;
  int comp;
  int calc = 0;

  if (table == NULL || zero == NULL || one == NULL || part == NULL || ret == NULL)
    showError(3);

  BImemcpy(zero, 0);
  BImemcpy(ret, 0);
  BImemcpy(one, 1);

  //si a = 0, no hace falta cálculo.
  hardEquals(va, zero, &comp);
  
  if (comp == 0)
    calc = 1;

  //si b = 0, no hace falta cálculo.
  hardEquals(vb, zero, &comp);
  
  if (comp == 0) {
    memcpy(va, zero, sizeof(struct BigInteger));
    calc = 1;
  }

  //normalizamos los operandos
  sig = signum(((struct BigInteger*)va)->n[((struct BigInteger*)va)->count], 
               ((struct BigInteger*)vb)->n[((struct BigInteger*)vb)->count]);

  if (sig == 1)
    ((struct BigInteger*)vb)->n[((struct BigInteger*)vb)->count] *= -1;
  else if (sig == 10)
    ((struct BigInteger*)va)->n[((struct BigInteger*)va)->count] *= -1;
  else if (sig == 11) {
    ((struct BigInteger*)va)->n[((struct BigInteger*)va)->count] *= -1;
    ((struct BigInteger*)vb)->n[((struct BigInteger*)vb)->count] *= -1;
  }

  //si |a| = 1, entonces a * b = b (el signo se normaliza después).
  hardEquals(va, one, &comp);

  if (comp == 0) {
    memcpy(va, vb, sizeof(struct BigInteger));
    calc = 1;
  }

  //si |b| = 1, entonces a * b = a (el signo se normaliza después).
  hardEquals(vb, one, &comp);
  
  if (comp == 0)
    calc = 1;

  //inicializamos BIT
  memcpy(&table->BI[0], zero, sizeof(struct BigInteger));
  memcpy(&table->BI[1], va, sizeof(struct BigInteger));

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
    for (i = 0; i <= ((struct BigInteger*)vb)->count; i++) {
      //validamos si BIT[n] existe
      if (table->status[((struct BigInteger*)vb)->n[i]] == 0) {
        clean(part);
        //no tenemos el resultado. Calculamos el valor
        for (x = 0; x <= ((struct BigInteger*)va)->count; x++)
          part->n[x] = ((struct BigInteger*)va)->n[x] * ((struct BigInteger*)vb)->n[i];

        part->count = x - 1;
        carryAdd(part, 0, 0);

        //movemos el valor a BIT
        memcpy(&table->BI[((struct BigInteger*)vb)->n[i]], part, sizeof(struct BigInteger));
        //table->status[b->n[i]] = 1;
        table->status[((struct BigInteger*)vb)->n[i]] = 1;
      }else 
        //tenemos el resultado
        memcpy(part, &table->BI[((struct BigInteger*)vb)->n[i]], sizeof(struct BigInteger));

      //multiplicación parcial
      pMul(i, part);

      //sumamos
      pAdd(ret, part);

      //inicializamos
      BImemcpy(part, 0);
    }

    memcpy(va, ret, sizeof(struct BigInteger));
  }

  //si los signos son diferentes, invertimos el signo
  if (sig == 1 || sig == 10)
    ((struct BigInteger*)va)->n[((struct BigInteger*)va)->count] *= -1;

  //liberamos memoria
  _free(5, part, table, zero, one, ret);
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
  int i;

  i = ((struct BigInteger*)vpart)->count + pos;

  if (i >= MAX_LENGTH)
    showError(1);

  //generamos offset
  for (; i >= pos; i--) 
    ((struct BigInteger*)vpart)->n[i] = ((struct BigInteger*)vpart)->n[i - pos];

  //noramlizamos las posiciones de offset
  for (i = 0; i < pos; i++) 
    ((struct BigInteger*)vpart)->n[i] = 0;

  ((struct BigInteger*)vpart)->count += pos;
}

/*
 * Función dvs. Función para dividir dos números.
 *
 * Simula la operación a = a / b
 */
void dvs(void* va, void* vb) {
  //validamos los datos antes de usarlos
  validateBI(va);
  validateBI(vb);

  //delegamos en la función estática
  sDvs(va, vb);
}

/*
 * Función sDvs. Función para dividir dos números.
 *
 * Simula la operación a = a / b
 */
static void sDvs(void* va, void* vb){
  struct BigInteger* temp = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* one = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  int sig;
  int comp;

  if (temp == NULL || one == NULL)
    showError(4);

  BImemcpy(temp, 0);
  BImemcpy(one, 1);

  sig = signum(((struct BigInteger*)va)->n[((struct BigInteger*)va)->count], 
               ((struct BigInteger*)vb)->n[((struct BigInteger*)vb)->count]);

  //normalizamos los operandos
  if(sig == 1)
    ((struct BigInteger*)vb)->n[((struct BigInteger*)vb)->count] *= -1;
  else if(sig == 10)
    ((struct BigInteger*)va)->n[((struct BigInteger*)va)->count] *= -1;
  else if(sig == 11){
    ((struct BigInteger*)va)->n[((struct BigInteger*)va)->count] *= -1;
    ((struct BigInteger*)vb)->n[((struct BigInteger*)vb)->count] *= -1;
  }

  hardEquals(va, vb, &comp);

  if (comp == 0) {
      //si a = b, a/b = 1
      hardEquals(va, temp, &comp);

      if (comp == 0)
          //si a = 0 (por tanto, b = 0), retornamos 0
          memcpy(va, temp, sizeof(struct BigInteger));
      else
          //en caso contrario, retornamos 1
          memcpy(va, one, sizeof(struct BigInteger));
  }else if (comp == 2)
    //si a < b, a / b = 0
    memcpy(va, temp, sizeof(struct BigInteger));
  else if(comp == 1){
    //si a > b, buscamos un número que solucione ax = n
    hardEquals(vb, one, &comp);

    if(comp != 0)
      divide(va, vb);
  }

  //si los signos son diferentes, invertimos el signo
  if (sig == 1 || sig == 10)
    ((struct BigInteger*)va)->n[((struct BigInteger*)va)->count] *= -1;

  //liberamos memoria
  _free(2, temp, one);
}

/*
 * Función divide.
 *
 * Realiza la división utilizando el teorema de Bolzano
 */
static void divide(void *va, void *vb){
  struct BIT *table = (struct BIT*)malloc(sizeof(struct BIT));
  struct BigInteger* dTemp = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* ret = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* temp = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* biTemp = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  int len;
  int i = 0;
  int x = 0;
  int eq;
  int currentBIT;
  int res = 0;
  int mLen;
  int added;

  if (table == NULL || dTemp == NULL || ret == NULL || temp == NULL || biTemp == NULL)
    showError(5);

  BImemcpy(ret, 0);

  len = ((struct BigInteger*)va)->count - ((struct BigInteger*)vb)->count;
  ret->count = 0;
  dTemp->count = 0;
  
  //llenamos BIT
  BImemcpy(&table->BI[0], 0);
  memcpy(&table->BI[1], vb, sizeof(struct BigInteger));

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
  if (((struct BigInteger*)vb)->count > 0)
    for(;i < ((struct BigInteger*)vb)->count;i++)
      dTemp->n[((struct BigInteger*)vb)->count - i - 1] = ((struct BigInteger*)va)->n[((struct BigInteger*)va)->count - i];
  else 
    dTemp->n[0] = 0;

  dTemp->count = ((struct BigInteger*)vb)->count - 1;

  //si "b" tiene una cifra, b.len será 0 pero tenemos que restar una cifra igualmente
  if (((struct BigInteger*)vb)->count == 0)
    mLen = len - 1;
  else
    mLen = len;

  //por cada cifra decimal que hemos generado
  for (i = 0; i <= len; i++) {
    //hacemos BI temporal
    temp->count = 0;
    temp->n[0] = ((struct BigInteger*)va)->n[len - i];

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
        pAdd(biTemp, vb);

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
    }else if (eq == 0) 
      //si dTemp = max, ya tenemos el valor
      res = currentBIT;
    else if (eq == 2) {
      //si dTemp < max, partimos de ese valor y hasta 0
      --x;

      for (; x >= 0; x--) {
        //validamos si nos sirve
        hardEquals(dTemp, &table->BI[x], &eq);

        if (eq == 0) {
          //si dTemp = temp
          res = x;
          x = -99;
        }else if (eq == 1) {
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

  memcpy(va, ret, sizeof(struct BigInteger));

  recount(va);
  
  //evitar error de falso desbordamiento
  if (((struct BigInteger*)va)->n[((struct BigInteger*)va)->count + 1] > 0)
      ++((struct BigInteger*)va)->count;

  //liberamos memoria
  _free(5, table, dTemp, ret, temp, biTemp);
}

/*
 * Función nqrt.
 *
 * Realiza la raíz enésima de a.
 */
void nqrt(void* va, int n) {
  //validamos punteros
  validateBI(va);

  //delegamos en la función estática
  sNqrt(va, n);
}

/*
 * Función sNqrt.
 *
 * Realiza la raíz enésima de a.
 */
static void sNqrt(void* va, int n){
  struct BigInteger* ret = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* raw = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* base = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  struct BigInteger* zero = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  int lmax = 0;
  int isEq = 0;

  if (ret == NULL || raw == NULL || base == NULL || zero == NULL)
      showError(9);

  BImemcpy(base, 0);
  BImemcpy(zero, 0);

  //si el índice de la raíz es 0, devolvemos 0
  if (n <= 0) {
      memcpy(va, zero, sizeof(struct BigInteger));
      _free(4, ret, raw, base, zero);

      return;
  }

  lmax = ((struct BigInteger*)va)->count / 2;

  //creamos una base
  base->n[lmax] = 1;
  base->count = lmax;

  //usamos Bolzano para obtener una aproximación
  memcpy(ret, base, sizeof(struct BigInteger));
  memcpy(raw, base, sizeof(struct BigInteger));

  //calculamos la potencia
  sBipow(ret, n);
  hardEquals(ret, va, &isEq);

  while (isEq != 0) {
    while (isEq == 2) {
      //ret < a. Incrementamos y probamos de nuevo hasta el exceso
      pAdd(raw, base);

      memcpy(ret, raw, sizeof(struct BigInteger));

      sBipow(ret, n);
      hardEquals(ret, va, &isEq);
    }

    //una vez aquí, siempre se dará ret >= a
    if (isEq == 1) {
      //si a > ret, retrocedemos 1 posición base y vamos a ajustar la posición anterior
      pSub(raw, base);

      base->n[lmax] = 0;
      --lmax;
      base->n[lmax] = 1;
      --base->count;

      //calculamos la potencia y comparamos (siempre será a < ret, ya que si fuera a = ret no hubiera entrado).
      hardEquals(base, zero, &isEq);

      //si base > 0, aún hay que iterar
      if (isEq == 1)
        isEq = 2;
      else
        //en caso contrario, hemos llegado a la máxima aproximación posible
        isEq = 0;
    }
  }

  memcpy(va, raw, sizeof(struct BigInteger));

  //liberamos memoria
  _free(4, ret, raw, base, zero);
}

/*
 * Función bipow.
 *
 * Simula la operación a = a^p
 */
void bipow(void *va, int p) {
  //validamos puntero
  validateBI(va);

  if (p == 0)
    //n^0 = 1
    BImemcpy(va, 1);
  else if (p == 1) 
    //n^1 = n
    return;
  else 
    sBipow(va, p);
}

/*
 * Función sBipow.
 *
 * Simula la operación a = a^p
 */
static void sBipow(void* va, int p) {
    struct BigInteger* res = (struct BigInteger*)malloc(sizeof(struct BigInteger));
    struct BigInteger* tmp = (struct BigInteger*)malloc(sizeof(struct BigInteger));
    int d2b[10];
    int d2bi = 0;
    int x = 0;
    int t = p;
    int i = 0;

    if(res == NULL || tmp == NULL)
        showError(6);

    if (p < 0) {
        BImemcpy(va, 0);
        _free(2, res, tmp);

        return;
    }

    BImemcpy(res, 1);

    /*
     *  Factorizamos la potencia en base 2 para poder
     *  mejorar el rendimiento de la operación
     *  
     *  Como funciona
     *   - Convertimos el exponente en base 2, y almacenamos
     *     los datos en un array.
     *  
     *   - Por cada iteración, vamos calculando a^2^i, es decir
     *     a^0, a^2, a^4, a^8, a^16, ...
     *  
     *   - Si la posición de d2b es 1, multiplicamos, y de esta manera
     *     obtenemos el resultado.
     *  
     *     Por ejemplo 8^12
     *       12 = 1100 (aunque se guarda como 0011) ==> 4 + 8
     *       8^12 = 8^4 * 8^8 = 8^(4+8)
     *  
     *     De esta manera, hacemos únicamente 8 multiplicaciones en lugar de 12
     *     (para este ejemplo). En números más grandes la diferencia sería aún mayor
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
        showError(8);

    //iteramos sobre el número binario
    for (; i < d2bi; i++) {
        //calculamos (a^(2^i))
        if (i == 0)
            memcpy(tmp, va, sizeof(struct BigInteger));
        else
            sMul(tmp, tmp);

        if (d2b[i] == 1)
            sMul(res, tmp);
    }

    memcpy(va, res, sizeof(struct BigInteger));

    _free(2, res, tmp);
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
    printf("Error. Datos erróneos en newBI");
  else if (k == 3)
    printf("Error. Puntero erróneo en mul");
  else if (k == 4)
    printf("Error. Puntero erróneo en dvs");
  else if (k == 5)
    printf("Error. Puntero erróneo en divide");
  else if (k == 6)
    printf("Error. Puntero erróneo en sBipow");
  else if (k == 7)
    printf("Error. Puntero erróneo en pAppend");
  else if (k == 8)
    printf("Error. Exponente demasiado grande");
  else if (k == 9)
    printf("Error. Puntero erróneo en nqrt");
  else if (k == 10)
    printf("Error. Puntero erróneo en sub");
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
 * Función iniStr.
 *
 * Reserva memoria para un char, para usarlo en toString.
 */
void iniStr(char** dst) {
  *dst = (char*)malloc(sizeof(char) * MAX_LENGTH);
}

/*
 * Función toString.
 *
 * Escribe en pantalla el BigInteger
 */
void toString(void *vb, char* dst){
  int i = 0;
  int m = ((struct BigInteger*)vb)->count;
  int sig = 0;

  //validamos puntero
  validateBI(vb);
  
  //si el primer dígito es negativo, lo volvemos positivo y marcamos el caracter
  if (((struct BigInteger*)vb)->n[m] < 0) {
    ((struct BigInteger*)vb)->n[m] *= -1;
    dst[i++] = '-';
    sig = 1;
  }

  for (; m >= 0; m--)
    dst[i++] = (char)(((struct BigInteger*)vb)->n[m] + 48);

  dst[i] = '\0';

  //estamos trabajando sobre el puntero, revisamos y reestablecemos valores
  if (sig == 1)
      ((struct BigInteger*)vb)->n[((struct BigInteger*)vb)->count] *= -1;
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
  struct BigInteger* aux = (struct BigInteger*)malloc(sizeof(struct BigInteger));
  int i = 0;
  int sig;

  if (aux == NULL)
    showError(7);

  //si b = 0, no hacemos nada
  if (((struct BigInteger*)vb)->count == 0 && ((struct BigInteger*)vb)->n[0] == 0) {
    //libreamos memoria
    free(aux);

    return;
  }

  aux->count = 0;

  sig = signum(((struct BigInteger*)va)->n[((struct BigInteger*)va)->count],
                   ((struct BigInteger*)vb)->n[((struct BigInteger*)vb)->count]);

  if (sig == 1) 
      //si b < 0, lo normalizamos
      ((struct BigInteger*)vb)->n[((struct BigInteger*)vb)->count] *= -1;
  else if(sig == 10)
      //si a < 0, lo normalizamos
      ((struct BigInteger*)va)->n[((struct BigInteger*)va)->count] *= -1;
  else if (sig == 11) {
      //a < 0 y b < 0, lo normalizamos
      ((struct BigInteger*)va)->n[((struct BigInteger*)va)->count] *= -1;
      ((struct BigInteger*)vb)->n[((struct BigInteger*)vb)->count] *= -1;
  }


  if (((struct BigInteger*)va)->count == 0) {
    for (; i <= ((struct BigInteger*)vb)->count; i++)
        ((struct BigInteger*)va)->n[++((struct BigInteger*)va)->count] = ((struct BigInteger*)vb)->n[i];
  }else {
    //limpiamos el BI
    clean(aux);

    //ponemos tantos 0's como posiciones tenga "a"
    for (; i <= ((struct BigInteger*)va)->count; i++)
      aux->n[aux->count++] = 0;

    //traspasamos "b" a aux
    for (i = 0; i <= ((struct BigInteger*)vb)->count; i++)
      aux->n[aux->count++] = ((struct BigInteger*)vb)->n[i];

    --aux->count;

    pAdd(va, aux);
  }

  //una vez movidos los datos recalculamos el signo
  if(sig == 1 || sig == 11)
      ((struct BigInteger*)va)->n[((struct BigInteger*)va)->count] *= -1;

  //libreamos memoria
  free(aux);
}

/*
 * Función clean.
 *
 * Limpia la estructura
 */
static void clean(void *va){
    int i = 0;

    for (; i < MAX_LENGTH; i++) 
        ((struct BigInteger*)va)->n[i] = 0;

    ((struct BigInteger*)va)->count = 0;
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
  int* t = (int*)(malloc(sizeof(int)));
  int i = 0;

  if (t == NULL || a == NULL)
    showError(98);

  memcpy(t, (int*)a, sizeof(int));

  //validamos la variable de longitud
  if (*t < 0 || *t > MAX_LENGTH)
    showError(99);

  memcpy(t, (int*)a + 1, sizeof(int));

  //validamos el resto de dígitos, que pueden ser positivos o negativos
  for (; i < MAX_LENGTH; i++) {
    if (*t < -9 || *t > 9)
      showError(99);

    if(i + 1 < MAX_LENGTH)
        memcpy(t, (int*)a + (i + 1), sizeof(int));
  }

  free(t);
}

/*
 * Función _free
 *
 * Libera masivamente "n" items de memoria
 */
void _free(int n, ...) {
    va_list pa;
    va_start(pa, n);

    for (; n > 0; n--)
        free((void*)va_arg(pa, size_t));
}
