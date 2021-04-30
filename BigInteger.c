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
 *      - Nueva función validateBI para validar la eura de un BigInteger pasado como puntero
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
 *        * Revisión y reeuración de los códigos de error de showError
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
 *    v4.2
 *      - Bugfix cuando a < b
 *    v4.3
 *      - Bugfix en división con una cifra
 *      - Bigfix en bipow cuando va < 0
 *    v4.4
 *      - Creación de valores estáticos del 1 al 10
 *      - Mejora del rendimiento de _BI_initialize
 *      - Bugfix:
 *        * Cuando se opera (a + a)
 *        * Cuando se opera (a * a)
 *        * Cuando se opera (a - a)
 *        * Cuando se opera (a / a)
 *    v4.5
 *      - Cambio de carryAdd a función pública
 *    v4.6
 *      - Añadimos parámetro k a BigInteger como preparación a la versión 5
 *    v4.7
 *      - Cambio en la función de validación para mejor rendimiento
 *      - Eliminar cast en malloc para mejora de rendimiento
 *      - Bugfix en sNqrt cuando longitud de datos < 0
 *      - Mejora de rendimiento en clean
 *    v4.71
 *      - Añadida función iniBIT para crear estructuras BIT y mejorar rendimiento.
 *      - La multiplicación y la división ahora funcionan con un BIT externo
 *    v5.0
 *      - Cambio del tipo de "n" de int a char, reduciendo en un 75% el uso de memoria
 *      - Uso de variables externas para aumentar rendimiento (incrementando coste de memoria)
 *      - Eliminada la función _free
 *      - Añadido parámetro de precompilador para longitud de BI
 *      - Añadido parámetro de precompilador para validación de datos opcional
 *      - Añadido parámetro de precompilador para standalone de BigInteger
 *      - Mejora en newBI para permitir números negativos sin especificar sig = -1
 *      - Se elimina la función iniBIT, que queda integrada en nueva funcion init
 *      - Revisión de comentarios e identación del código
 *      - Creados typedef para poder eliminar las referencias a "struct"
 *      - Bugfix en la suma para aumentar rendimiento
 *      - Mejora de rendimiento en la resta al cambiar llamada a pAdd por addition
 *      - Mejora de rendimiento en la resta al eliminar carryType
 *      - Mejora de rendimiento en la multiplicación al cambiar llamada a pAdd por addition
 *      - Mejora de rendimiento en la división al cambiar el funcionamiento de pAppend
 *        * pAppend pasa a ser una función interna para uso exclusivo de la división
 *        * Se elimina dtmp como variable
 *      - Mejora de rendimiento en la raíz al cambiar pAdd por addition
 *      - Mejora de rendimiento en hardEquals cuando va y vb apuntan a la misma dirección de memoria
 *      - Función clean pasa a ser pública
 *      - Creada la interfaz BOperation
 *        * Se abstraen funciones comunes.
 *        * _BI_Initialize ya no es static
 *    v5.01
 *      - Bugfix en HardEquals 
 *    v5.1
 *      - Añadido "signed" a n
 *      - Cambios para añadir el modo servicio
 *        * Nueva variable estática BIReturnCode
 *        * Nueva función getReturnCode
 *        * Nueva directiva BI_SERVICE para compilar en modo Servicio
 *        * Añadimos "return" después de cada llamada a "showError" para forzar el fin de función
 */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"
#include "BigInteger.h"
#if BI_STANDALONE != 1
#include "BOperation.h"
#endif

static float BI_VERSION = 5.01f;

#if BI_STANDALONE == 1
static int validate =
#ifdef CVALIDATE
CVALIDATE;
#else
1;
#endif
#endif

#if BI_STANDALONE == 1
/*
 * Función getMemorySize
 *
 * Retorna la t_size relativo a memory.
 */
size_t getMemorySize() {
  return (sizeof(BigInteger) * 19) + sizeof(BIT);
}
#endif

#if BI_STANDALONE == 1
/*
 * Función init
 *
 * Arranca el motor de BigInteger.
 */
void init(void** m) {
  //suma
  ((memory*)m)->vt = malloc(sizeof(BigInteger));

  //resta
  ((memory*)m)->stmp = malloc(sizeof(BigInteger));

  //multiplicación
  ((memory*)m)->mpart = malloc(sizeof(BigInteger));
  ((memory*)m)->mret = malloc(sizeof(BigInteger));
  ((memory*)m)->mzero = malloc(sizeof(BigInteger));
  ((memory*)m)->mone = malloc(sizeof(BigInteger));
  ((memory*)m)->mtmp = malloc(sizeof(BigInteger));

  //division
  ((memory*)m)->done = malloc(sizeof(BigInteger));
  ((memory*)m)->dtmp = malloc(sizeof(BigInteger));
  ((memory*)m)->dret = malloc(sizeof(BigInteger));
  ((memory*)m)->dTemp = malloc(sizeof(BigInteger));
  ((memory*)m)->biTemp = malloc(sizeof(BigInteger));

  //raiz
  ((memory*)m)->sret = malloc(sizeof(BigInteger));
  ((memory*)m)->sraw = malloc(sizeof(BigInteger));
  ((memory*)m)->sbase = malloc(sizeof(BigInteger));
  ((memory*)m)->szero = malloc(sizeof(BigInteger));

  //potencia
  ((memory*)m)->bres = malloc(sizeof(BigInteger));
  ((memory*)m)->btmp = malloc(sizeof(BigInteger));

  //append
  ((memory*)m)->aaux = malloc(sizeof(BigInteger));

  //BIT
  ((memory*)m)->biBIT = malloc(sizeof( BIT));

  //valores comunes
  _BI_initialize();

  BImemcpy(((memory*)m)->vt, 2);
}
#endif

/*
 * Función initialize
 *
 * Da valores a ciertos datos útiles.
 */
void _BI_initialize() {
  int i = 0;

  //lo creamos manualmente ya que clean copia de _ZERO
  for (; i < MAX_LENGTH; i++)
    _ZERO.n[i] = 0;

  _ZERO.count = 0;
  _ZERO.k = 'i';

  newBI(&_ONE, "1", 0);
  newBI(&_TWO, "2", 0);
  newBI(&_THREE, "3", 0);
  newBI(&_FOUR, "4", 0);
  newBI(&_FIVE, "5", 0);
  newBI(&_SIX, "6", 0);
  newBI(&_SEVEN, "7", 0);
  newBI(&_EIGHT, "8", 0);
  newBI(&_NINE, "9", 0);
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
  if (value == 0)
    memcpy(dst, &_ZERO, sizeof(BigInteger));
  else if (value == 1) 
    memcpy(dst, &_ONE, sizeof(BigInteger));
  else if (value == 2)
    memcpy(dst, &_TWO, sizeof(BigInteger));
  else if (value == 3)
    memcpy(dst, &_THREE, sizeof(BigInteger));
  else if (value == 4)
    memcpy(dst, &_FOUR, sizeof(BigInteger));
  else if (value == 5)
    memcpy(dst, &_FIVE, sizeof(BigInteger));
  else if (value == 6)
    memcpy(dst, &_SIX, sizeof(BigInteger));
  else if (value == 7)
    memcpy(dst, &_SEVEN, sizeof(BigInteger));
  else if (value == 8)
    memcpy(dst, &_EIGHT, sizeof(BigInteger));
  else if (value == 9)
    memcpy(dst, &_NINE, sizeof(BigInteger));
  else if (value == 10)
    memcpy(dst, &_TEN, sizeof(BigInteger));
  else if (value == 100)
    memcpy(dst, &_HUND, sizeof(BigInteger));
  else if (value == -1) 
    memcpy(dst, &_MIN, sizeof(BigInteger));
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
void newBI(void* dst, char* s, int sig) {
  int i = (int)strlen(s) - 1;
  int f = i;
  int j = 0;
  int c;
  int ssig = sig;

  //ajustamos el tipo
  ((BigInteger*)dst)->k = 'i';

  //limpiamos el array
  clean(dst);

  if (i > MAX_LENGTH + 1) {
    showError(1);
    return;
  }

  //recorremos el string y lo guardamos en integers
  for (; i >= 0; i--) {
    c = (int)(s[i] - 48);

    if (c >= 0 && c <= 9)
      ((BigInteger*)dst)->n[j++] = c;
    else
      if (s[i] == '-') 
        ssig = -1;
      else {
        showError(2);
        return;
      }
  }

  //si nos envían un negativo, restamos una posición
  if (s[0] == '-')
    --f;

  ((BigInteger*)dst)->count = f;

  if (ssig == -1)
    ((BigInteger*)dst)->n[((BigInteger*)dst)->count] *= -1;
}

/*
 * Función pAdd. Usar para sumar dos números.
 *
 * Realiza la operación de suma, teniendo en cuenta los signos de los números.
 *
 * Si los signos son iguales, hace una suma, sino, una resta.
 */
void pAdd(void* va, void* vb, void* m) {
  if (((memory*)m)->vt == NULL) {
    showError(11);
    return;
  }

  if (va == vb) {
    //add(a, a); delegamos en mul para hacer a = a * 2
    sMul(va, ((memory*)m)->vt, m);
  } else {
    //add(a, b); delegamos en la suma
    int sig = signum(((BigInteger*)va)->n[((BigInteger*)va)->count],
                     ((BigInteger*)vb)->n[((BigInteger*)vb)->count]);

    //normalizamos los operandos
    if (sig == 10)
      //a negativo, b positivo. Cambiamos el signo de "a" y hacemos suma
      ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;
    else if (sig == 1)
      //b negativo, a positivo. Cambiamos el signo de "b" y hacemos suma
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
    else if (sig == 11) {
      //a negativo, b negativo. Cambiamos signos y hacemos suma
      ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
    }

    //si ambos signos son iguales, se suma, sino, se resta
    if (sig == 0 || sig == 11)
      addition(va, vb);
    else
      pSub(va, vb, m);

    if (sig == 10 || sig == 11)
      //en estos casos, siempre se le va la vuelta al signo
      ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;

    //desnormalizamos b
    if (sig == 1)
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
    else if (sig == 11)
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
  }
}

#if BI_STANDALONE == 1
/*
 * Función add. Usar para sumar dos números.
 *
 * Realiza la operación de suma, teniendo en cuenta los signos de los números.
 *
 * Si los signos son iguales, hace una suma, sino, una resta.
 */
void add(void* va, void* vb, void* m) {
  //validamos los datos antes de tratarlos
  if (validate == 1) {
    validateBI(va);
    validateBI(vb);
  }

  //delegamos en la función estática
  pAdd(va, vb, m);
}
#endif

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
  limit = ((BigInteger*)va)->count;

  //asumimos que b tiene la menor longitud
  min = ((BigInteger*)vb)->count;

  //indicador de necesidad de arrastre
  move = 0;
  i = 0;

  //si no es así, rectificamos
  if (((BigInteger*)vb)->count > limit) {
    //intercambiamos limit y min
    swap = limit;
    limit = min;
    min = swap;

    move = 1;
  }

  //sumamos los dígitos que coinciden
  for (; i <= min; i++)
    ((BigInteger*)va)->n[i] += ((BigInteger*)vb)->n[i];

  //los dígitos que no coinciden los traspasamos
  if (move == 1) {
    for (; i <= limit; i++)
      ((BigInteger*)va)->n[i] = ((BigInteger*)vb)->n[i];

    ((BigInteger*)va)->count = limit;
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
void carryAdd(void* va, int move, int min) {
  int i = 0;
  int acc;
  int limit;

  acc = 0;

  //move == 1 --> sabemos que hay una parte no común. En min está el límite de la parte común
  if (move == 1)
    limit = min;
  else
    limit = ((BigInteger*)va)->count;

  //recorremos a y vamos sumando el acarreo de la parte común
  for (; i <= limit; i++) {
    //sumamos acarreo
    ((BigInteger*)va)->n[i] += acc;

    //como acc es int, podemos dividir entre 10 y sacar el acarreo
    acc = ((BigInteger*)va)->n[i] / 10;

    if (acc > 0)
      //normalizamos el número
      ((BigInteger*)va)->n[i] = ((BigInteger*)va)->n[i] % 10;
  }

  if (move == 1) {
    //queda parte no común. Acarreamos hatsa que acc sea 0, ya que la parte no común ya está normalizada
    while (acc > 0 && i <= ((BigInteger*)va)->count) {
      //sumamos acarreo
      ((BigInteger*)va)->n[i] += acc;

      //como acc es int, podemos dividir entre 10 y sacar el acarreo
      acc = ((BigInteger*)va)->n[i] / 10;

      if (acc > 0)
        //normalizamos el número
        ((BigInteger*)va)->n[i] = ((BigInteger*)va)->n[i] % 10;

      i++;
    }
  }

  //si ha quedado acarreo, lo guardamos al final;
  if (acc > 0) {
    if (((BigInteger*)va)->count == MAX_LENGTH) {
      showError(1);
      return;
    } else
      ((BigInteger*)va)->n[++(((BigInteger*)va)->count)] = acc;
  }
}

/*
 * Función pSub. Usar para restar dos números.
 *
 * Simula la operación a = a - b.
 *
 * Si len(a) < len(b), se intercambian los valores
 */
void pSub(void* va, void* vb, void* m) {
  int sig;
  int comp;

  if (((memory*)m)->stmp == NULL) {
    showError(10);
    return;
  }

  if (va == vb) {
    //sub(a, a);
    BImemcpy(va, 0);
  } else {
    //sub(a, b);
    hardEquals(va, vb, &comp);

    sig = signum(((BigInteger*)va)->n[((BigInteger*)va)->count],
                 ((BigInteger*)vb)->n[((BigInteger*)vb)->count]);

    //si ambos son negativos, comp = 1 significa a < b
    if ((comp == 2 && sig < 11) || (comp == 1 && sig == 11)) {
      memcpy(((memory*)m)->stmp, vb, sizeof(BigInteger)); //tmp = b;

      pSub(vb, va, m);

      //cambiamos el signo
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;

      //reasignamos
      memcpy(va, vb, sizeof(BigInteger));

      //rescatamos el valor original de vb
      memcpy(vb, ((memory*)m)->stmp, sizeof(BigInteger));
    } else if (comp == 0)
      BImemcpy(va, 0);
    else {
      //normalizamos los signos
      if (sig == 1)
        ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
      else if (sig == 10)
        ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;
      else if (sig == 11) {
        ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;
        ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
      }

      //si tienen el mismo signo, se resta, sino se suma
      if (sig == 0 || sig == 11)
        subtract(va, vb);
      else
        addition(va, vb);

      if (sig == 10 || sig == 11)
        //en estos casos, siempre se le va la vuelta al signo
        ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;

      //desnormalizamos b
      if (sig == 1)
        ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
      else if (sig == 11)
        ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
    }
  }
}

#if BI_STANDALONE == 1
/*
 * Función sub. Usar para restar dos números.
 *
 * Simula la operación a = a - b.
 *
 * Si len(a) < len(b), se intercambian los valores
 */
void sub(void* va, void* vb, void* m) {
  //validamos los punteros antes de tratarlos
  if (validate == 1) {
    validateBI(va);
    validateBI(vb);
  }

  //delegamos en la función estática
  pSub(va, vb, m);
}
#endif

/*
 * Función subrtact.
 *
 * Realiza la resta.
 */
static void subtract(void* va, void* vb) {
  int i = 0;

  //restamos los dígitos comunes
  for (; i <= ((BigInteger*)vb)->count; i++)
    ((BigInteger*)va)->n[i] -= ((BigInteger*)vb)->n[i];

  //si el último dígito es negativo
  if (((BigInteger*)va)->n[((BigInteger*)va)->count] < 0)
    carrySub(va, 1);
  else
    carrySub(va, 0);

  
}

/*
 * Función carrySub.
 *
 * Gestiona el acarreo de la resta. Si carryType = 0, el acarreo
 * se gestiona como a = 10 + a; sino, se invierte el signo (excepto al último dígito)
 */
static void carrySub(void* va, int carryType) {
  int i = 0;
  int acc = 0;

  if (carryType == 0) {
    for (; i <= ((BigInteger*)va)->count; i++) {
      //restamos el acarreo al número
      ((BigInteger*)va)->n[i] -= acc;

      if (((BigInteger*)va)->n[i] < 0) {
        //normalizamos el número
        ((BigInteger*)va)->n[i] += 10;
        acc = 1;
      } else
        acc = 0;
    }
  } else {
    //en esta opción, no es necesario pasar una segunda vez por acarreos.
    for (i = 0; i < ((BigInteger*)va)->count; i++)
      if (((BigInteger*)va)->n[i] < 0)
        //normalizamos el número
        ((BigInteger*)va)->n[i] = ((BigInteger*)va)->n[i] * -1;
  }

  //contamos de nuevo los dígitos
  recount(va);
}

/*
 * Función recount.
 *
 * Recuenta las cifras, para ver si hay que disminuir el conteo.
 */
static void recount(void* va) {
  while (((BigInteger*)va)->n[((BigInteger*)va)->count--] == 0);

  ++((BigInteger*)va)->count;

  if (((BigInteger*)va)->count < 0)
    ((BigInteger*)va)->count = 0;
}

#if BI_STANDALONE == 1
/*
 * Función equals.
 *
 * Función pública de hardEquals
 * Compara dos números. Devuelve 0 si "a" = "b"; 1 si "a" > "b"; 2 si "a" < "b".
 */
void equals(void* va, void* vb, void* m, int* ret) {
  //validamos punteros
  if (validate == 1) {
    validateBI(va);
    validateBI(vb);
  }

  hardEquals(va, vb, ret);
}
#endif

/*
 * Función hardEquals.
 *
 * Compara dos números. Devuelve 0 si "a" = "b"; 1 si "a" > "b"; 2 si "a" < "b".
 */
void hardEquals(void* va, void* vb, int* ret) {
  int i;
  int sig;

  //si el puntero es el mismo, el valor es el mismo
  if (va == vb) {
    *ret = 0;
    return;
  }

  /*calculamos el signo
   *
   *00: a >= 0, b >= 0
   *01: a >= 0, b <  0
   *10: a <  0, b >= 0
   *11: a <  0, b <  0
   */
  sig = signum(((BigInteger*)va)->n[((BigInteger*)va)->count],
               ((BigInteger*)vb)->n[((BigInteger*)vb)->count]);

  if (sig == 1)
    //a >= 0, b < 0, por tanto a > b
    *ret = 1;
  else if (sig == 10)
    //a < 0, b >= 0, por tanto a < b
    *ret = 2;
  else {
    //comparten signo. Hacemos comparación manual
    *ret = 0;

    if (((BigInteger*)va)->count < ((BigInteger*)vb)->count)
      //si "a" tiene menos dígitos que "b"
      *ret = 2;
    else if (((BigInteger*)va)->count > ((BigInteger*)vb)->count)
      //si "a" tiene más dítigos que "b"
      *ret = 1;
    else {
      //tienen los mismos dígitos. Comparamos uno a uno.
      for (i = ((BigInteger*)va)->count; i >= 0; i--) {
        if (((BigInteger*)va)->n[i] < ((BigInteger*)vb)->n[i])
          *ret = 2;
        else if ((((BigInteger*)va)->n[i] > ((BigInteger*)vb)->n[i]))
          *ret = 1;

        if (*ret > 0)
          break;
      }
    }

    if (sig == 11) {
      //ambos tienen signo negativo. Cambiamos el retorno
      //ya que se p. ej. -1 > -10
      if (*ret == 2)
        *ret = 1;
      else if (*ret == 1)
        *ret = 2;
    }
  }
}

/*
 * Función sMul. Función para multiplicar dos números.
 *
 * Simula la operación a = a * b
 */
void sMul(void* va, void* vb, void* m) {
  int sig;
  int i = 0;
  int x;
  int comp;
  int calc = 0;

  if (((memory*)m)->biBIT == NULL || ((memory*)m)->mzero == NULL || ((memory*)m)->mone == NULL ||
      ((memory*)m)->mpart == NULL || ((memory*)m)->mret == NULL || ((memory*)m)->mtmp == NULL) {
    showError(3);
    return;
  }

  if (va == vb) {
    //mul(a, a)
    //copiamos a a tmp para poder trabajar sin colapsar datos
    memcpy(((memory*)m)->mtmp, va, sizeof(BigInteger));

    sMul(va, ((memory*)m)->mtmp, m);
  } else {
    //mul(a, b)
    BImemcpy(((memory*)m)->mzero, 0);
    BImemcpy(((memory*)m)->mret, 0);
    BImemcpy(((memory*)m)->mone, 1);

    //si a = 0, no hace falta cálculo.
    hardEquals(va, ((memory*)m)->mzero, &comp);

    if (comp == 0)
      calc = 1;

    //si b = 0, no hace falta cálculo.
    hardEquals(vb, ((memory*)m)->mzero, &comp);

    if (comp == 0) {
      memcpy(va, ((memory*)m)->mzero, sizeof(BigInteger));
      calc = 1;
    }

    //normalizamos los operandos
    sig = signum(((BigInteger*)va)->n[((BigInteger*)va)->count],
                 ((BigInteger*)vb)->n[((BigInteger*)vb)->count]);

    if (sig == 1)
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
    else if (sig == 10)
      ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;
    else if (sig == 11) {
      ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
    }

    //si |a| = 1, entonces a * b = b (el signo se normaliza después).
    hardEquals(va, ((memory*)m)->mone, &comp);

    if (comp == 0) {
      memcpy(va, vb, sizeof(BigInteger));
      calc = 1;
    }

    //si |b| = 1, entonces a * b = a (el signo se normaliza después).
    hardEquals(vb, ((memory*)m)->mone, &comp);

    if (comp == 0)
      calc = 1;

    //inicializamos BIT
    memcpy(&(( BIT*)((memory*)m)->biBIT)->BI[0], ((memory*)m)->mzero, sizeof(BigInteger));
    memcpy(&(( BIT*)((memory*)m)->biBIT)->BI[1], va, sizeof(BigInteger));

    ((BIT*)((memory*)m)->biBIT)->status[0] = 1;
    ((BIT*)((memory*)m)->biBIT)->status[1] = 1;
    ((BIT*)((memory*)m)->biBIT)->status[2] = 0;
    ((BIT*)((memory*)m)->biBIT)->status[3] = 0;
    ((BIT*)((memory*)m)->biBIT)->status[4] = 0;
    ((BIT*)((memory*)m)->biBIT)->status[5] = 0;
    ((BIT*)((memory*)m)->biBIT)->status[6] = 0;
    ((BIT*)((memory*)m)->biBIT)->status[7] = 0;
    ((BIT*)((memory*)m)->biBIT)->status[8] = 0;
    ((BIT*)((memory*)m)->biBIT)->status[9] = 0;

    //si el número no está ya calculado, lo calculamos
    if (calc == 0) {
      //realizamos los productos parciales
      for (i = 0; i <= ((BigInteger*)vb)->count; i++) {
        //validamos si BIT[n] existe
        if (((BIT*)((memory*)m)->biBIT)->status[((BigInteger*)vb)->n[i]] == 0) {
          clean(((memory*)m)->mpart);
          //no tenemos el resultado. Calculamos el valor
          for (x = 0; x <= ((BigInteger*)va)->count; x++)
            ((BigInteger*)((memory*)m)->mpart)->n[x] = ((BigInteger*)va)->n[x] * ((BigInteger*)vb)->n[i];
          
          ((BigInteger*)((memory*)m)->mpart)->count = x - 1;
          carryAdd(((memory*)m)->mpart, 0, 0);

          //movemos el valor a BIT
          memcpy(&((BIT*)((memory*)m)->biBIT)->BI[((BigInteger*)vb)->n[i]], ((memory*)m)->mpart, sizeof(BigInteger));
          //table->status[b->n[i]] = 1;
          ((BIT*)((memory*)m)->biBIT)->status[((BigInteger*)vb)->n[i]] = 1;
        } else
          //tenemos el resultado
          memcpy(((memory*)m)->mpart, &((BIT*)((memory*)m)->biBIT)->BI[((BigInteger*)vb)->n[i]], sizeof(BigInteger));

        //multiplicación parcial
        pMul(i, ((memory*)m)->mpart);

        //sumamos
        addition(((memory*)m)->mret, ((memory*)m)->mpart);

        //inicializamos
        BImemcpy(((memory*)m)->mpart, 0);
      }

      memcpy(va, ((memory*)m)->mret, sizeof(BigInteger));
    }

    //si los signos son diferentes, invertimos el signo
    if (sig == 1 || sig == 10)
      ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;

    //desnormalizamos vb
    if (sig == 1)
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
    else if (sig == 11)
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
  }
}

#if BI_STANDALONE == 1
/*
 * Función mul. Función para multiplicar dos números.
 *
 * Simula la operación a = a * b
 */
void mul(void* va, void* vb, void* m) {
  //validamos los datos antes de usarlos
  if (validate == 1) {
    validateBI(va);
    validateBI(vb);
  }

  //delegamos en la función estática
  sMul(va, vb, m);
}
#endif

/*
 * Función pMul.
 *
 * Efectúa una multiplicación parcial de a y b
 */
static void pMul(int pos, void* vpart) {
  int i;

  i = ((BigInteger*)vpart)->count + pos;

  if (i >= MAX_LENGTH) {
    showError(1);
    return;
  }

  //generamos offset
  for (; i >= pos; i--)
    ((BigInteger*)vpart)->n[i] = ((BigInteger*)vpart)->n[i - pos];

  //noramlizamos las posiciones de offset
  for (i = 0; i < pos; i++)
    ((BigInteger*)vpart)->n[i] = 0;

  ((BigInteger*)vpart)->count += pos;
}

#if BI_STANDALONE == 1
/*
 * Función dvs. Función para dividir dos números.
 *
 * Simula la operación a = a / b
 */
void dvs(void* va, void* vb, void* m) {
  //validamos los datos antes de usarlos
  if (validate == 1) {
    validateBI(va);
    validateBI(vb);
  }

  //inicializamos el punto decimal
  BI_point = 0;

  //delegamos en la función estática
  sDvs(va, vb, m);
}
#endif

/*
 * Función sDvs. Función para dividir dos números.
 *
 * Simula la operación a = a / b
 */
void sDvs(void* va, void* vb, void* m) {
  int sig;
  int comp;

  if (((memory*)m)->dtmp == NULL || ((memory*)m)->done == NULL) {
    showError(4);
    return;
  }

  //inicializamos el punto decimal
  BI_point = 0;

  if (va == vb) {
    //dvs(a, a)
    BImemcpy(va, 1);
  } else {
    //dvs(a, b)

    BImemcpy(((memory*)m)->dtmp, 0);
    BImemcpy(((memory*)m)->done, 1);

    sig = signum(((BigInteger*)va)->n[((BigInteger*)va)->count],
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count]);

    //normalizamos los operandos
    if (sig == 1)
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
    else if (sig == 10)
      ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;
    else if (sig == 11) {
      ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
    }

    hardEquals(va, vb, &comp);

    if (comp == 0) {
      //si a = b, a/b = 1
      hardEquals(va, ((memory*)m)->dtmp, &comp);

      if (comp == 0)
        //si a = 0 (por tanto, b = 0), retornamos 0
        memcpy(va, ((memory*)m)->dtmp, sizeof(BigInteger));
      else
        //en caso contrario, retornamos 1
        memcpy(va, ((memory*)m)->done, sizeof(BigInteger));
    } else if (comp == 2) 
        //si a < b, a / b = 0 (estamos con integer)
        memcpy(va, ((memory*)m)->dtmp, sizeof(BigInteger));
    else if (comp == 1) {
      //si a > b, buscamos un número que solucione ax = n
      hardEquals(vb, ((memory*)m)->done, &comp);

      if (comp != 0)
        divide(va, vb, m);
    }

    //si los signos son diferentes, invertimos el signo
    if (sig == 1 || sig == 10)
      ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;

    //desnormalizamos b
    if (sig == 1)
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
    else if (sig == 11)
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
  }
}

/*
 * Función divide.
 *
 * Realiza la división utilizando el teorema de Bolzano
 */
static void divide(void* va, void* vb, void* m) {
  int len;
  int i = 0;
  int x = 0;
  int eq;
  int currentBIT;
  int res = 0;
  int mLen;
  int added;
  int dlen;

  if (((memory*)m)->biBIT == NULL || ((memory*)m)->dTemp == NULL || ((memory*)m)->dret == NULL ||
      ((memory*)m)->biTemp == NULL) {
    showError(5);
    return;
  }

  BImemcpy(((memory*)m)->dret, 0);

  len = ((BigInteger*)va)->count - ((BigInteger*)vb)->count;

  //llenamos BIT
  BImemcpy(&(( BIT*)((memory*)m)->biBIT)->BI[0], 0);
  memcpy(&(( BIT*)((memory*)m)->biBIT)->BI[1], vb, sizeof(BigInteger));

  ((BIT*)((memory*)m)->biBIT)->status[0] = 1;
  ((BIT*)((memory*)m)->biBIT)->status[1] = 1;
  ((BIT*)((memory*)m)->biBIT)->status[2] = 0;
  ((BIT*)((memory*)m)->biBIT)->status[3] = 0;
  ((BIT*)((memory*)m)->biBIT)->status[4] = 0;
  ((BIT*)((memory*)m)->biBIT)->status[5] = 0;
  ((BIT*)((memory*)m)->biBIT)->status[6] = 0;
  ((BIT*)((memory*)m)->biBIT)->status[7] = 0;
  ((BIT*)((memory*)m)->biBIT)->status[8] = 0;
  ((BIT*)((memory*)m)->biBIT)->status[9] = 0;

  currentBIT = 1;

  //iniciamos dTemp y temp
  BImemcpy(((memory*)m)->dTemp, 0);

  /*
   * Nos quedamos con los "b.count" primeros dígitos. Si "b" tiene un único
   * dígito, no movemos nada, porque ya moveremos el siguiente dígito más adelante
   */
  if (((BigInteger*)vb)->count > 0)
    for (; i < ((BigInteger*)vb)->count; i++)
      ((BigInteger*)((memory*)m)->dTemp)->n[((BigInteger*)vb)->count - i - 1] = ((BigInteger*)va)->n[((BigInteger*)va)->count - i];
  else
    ((BigInteger*)((memory*)m)->dTemp)->n[0] = 0;

  ((BigInteger*)((memory*)m)->dTemp)->count = ((BigInteger*)vb)->count - 1;

  if (((BigInteger*)((memory*)m)->dTemp)->count == -1)
    ++((BigInteger*)((memory*)m)->dTemp)->count;

  //si "b" tiene una cifra, b.len será 0 pero tenemos que restar una cifra igualmente
  if (((BigInteger*)vb)->count == 0)
    mLen = len - 1;
  else
    mLen = len;

  //guardamos dlen para los cálculos con double
  dlen = len;

  //inicializamos el punto decimal
  BI_point = 0;

  if (((BigInteger*)va)->k == 'd') {
    //va es double. Cambiamos ligeramente el funcionamiento de la división.
    BI_point = mLen;
    len = MAX_LENGTH - 2;
    mLen = MAX_LENGTH - 1;
  }

  //por cada cifra decimal que hemos generado
  for (i = 0; i <= len; i++) {
    //hacemos BI temporal
    if(i <= dlen)
      //todavía hay cifras
      pAppend(((memory*)m)->dTemp, ((BigInteger*)va)->n[dlen - i]);
    else 
      //ya no hay cifras. Bajamos un 0
      pAppend(((memory*)m)->dTemp, 0);

    //Retorna 0 si son iguales, retorna 1 si a > b, retorna 2 si a < b.
    hardEquals(((memory*)m)->dTemp, &((BIT*)((memory*)m)->biBIT)->BI[currentBIT], &eq);

    x = currentBIT;

    if (eq == 1) {
      //si dTemp > max, partimos de ese valor y hasta el máximo.

      //recuperamos el último BI
      memcpy(((memory*)m)->biTemp, &((BIT*)((memory*)m)->biBIT)->BI[x], sizeof(BigInteger));
      added = 0;

      for (; x < 10; x++) {
        //le sumamos la base
        pAdd(((memory*)m)->biTemp, vb, m);

        //lo añadimos a BIT, si no hemos llenado la tabla (rellenamos siempre por adelantado)
        if (currentBIT < 9) {
          memcpy(&((BIT*)((memory*)m)->biBIT)->BI[++currentBIT], ((memory*)m)->biTemp, sizeof(BigInteger));
          ((BIT*)((memory*)m)->biBIT)->status[currentBIT] = 1;
          added = 1;
        } else
          added = 0;

        //validamos si nos sirve
        hardEquals(((memory*)m)->dTemp, ((memory*)m)->biTemp, &eq);

        if (eq == 0) {
          //si dTemp = temp
          res = currentBIT;
          x = 99;
        } else if (eq == 2) {
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
    } else if (eq == 0)
      //si dTemp = max, ya tenemos el valor
      res = currentBIT;
    else if (eq == 2) {
      //si dTemp < max, partimos de ese valor y hasta 0
      --x;

      for (; x >= 0; x--) {
        //validamos si nos sirve
        hardEquals(((memory*)m)->dTemp, &((BIT*)((memory*)m)->biBIT)->BI[x], &eq);

        if (eq == 0) {
          //si dTemp = temp
          res = x;
          x = -99;
        } else if (eq == 1) {
          //si dTemp > max, según el teorema de Bolzano, hemos pasado por dTemp = max, 
          //por tanto, el valor correcto es el siguiente
          res = x;
          x = -99;
        }
      }
    }

    ((BigInteger*)((memory*)m)->dret)->n[len - i] = res;

    //restamos
    pSub(((memory*)m)->dTemp, &((BIT*)((memory*)m)->biBIT)->BI[res], m);
  }

  ((BigInteger*)((memory*)m)->dret)->count = mLen;

  memcpy(va, ((memory*)m)->dret, sizeof(BigInteger));

  recount(va);

  //evitar error de falso desbordamiento
  if (((BigInteger*)va)->n[((BigInteger*)va)->count + 1] > 0)
    ++((BigInteger*)va)->count;
}

#if BI_STANDALONE == 1
/*
 * Función nqrt.
 *
 * Realiza la raíz enésima de a.
 */
void nqrt(void* va, int n, void* m) {
  //validamos punteros
  if (validate == 1) 
    validateBI(va);

  //delegamos en la función estática
  sNqrt(va, n, m);
}
#endif

/*
 * Función sNqrt.
 *
 * Realiza la raíz enésima de a.
 */
void sNqrt(void* va, int n, void* m) {
  int lmax = 0;
  int isEq = 0;

  if (((memory*)m)->sret == NULL || ((memory*)m)->sraw == NULL ||
      ((memory*)m)->sbase == NULL || ((memory*)m)->szero == NULL) {
    showError(9);
    return;
  }

  BImemcpy(((memory*)m)->sbase, 0);
  BImemcpy(((memory*)m)->szero, 0);

  //si el índice de la raíz es 0, devolvemos 0
  if (n <= 0) {
    memcpy(va, ((memory*)m)->szero, sizeof(BigInteger));

    return;
  }

  lmax = ((BigInteger*)va)->count / 2;

  //creamos una base
  ((BigInteger*)((memory*)m)->sbase)->n[lmax] = 1;
  ((BigInteger*)((memory*)m)->sbase)->count = lmax;

  //usamos Bolzano para obtener una aproximación
  memcpy(((memory*)m)->sret, ((memory*)m)->sbase, sizeof(BigInteger));
  memcpy(((memory*)m)->sraw, ((memory*)m)->sbase, sizeof(BigInteger));

  //calculamos la potencia
  sBipow(((memory*)m)->sret, n, m);
  hardEquals(((memory*)m)->sret, va, &isEq);

  while (isEq != 0) {
    while (isEq == 2) {
      //ret < a. Incrementamos y probamos de nuevo hasta el exceso
      addition(((memory*)m)->sraw, ((memory*)m)->sbase);

      memcpy(((memory*)m)->sret, ((memory*)m)->sraw, sizeof(BigInteger));

      sBipow(((memory*)m)->sret, n, m);
      hardEquals(((memory*)m)->sret, va, &isEq);
    }

    //una vez aquí, siempre se dará ret >= a
    if (isEq == 1) {
      //si a > ret, retrocedemos 1 posición base y vamos a ajustar la posición anterior
      pSub(((memory*)m)->sraw, ((memory*)m)->sbase, m);

      ((BigInteger*)((memory*)m)->sbase)->n[lmax] = 0;
      --lmax;

      if (lmax >= 0) {
        //si lmax es mayor que 0, aún tenemos dígitos para pivotar
        ((BigInteger*)((memory*)m)->sbase)->n[lmax] = 1;
        --((BigInteger*)((memory*)m)->sbase)->count;

        //calculamos la potencia y comparamos (siempre será a < ret, ya que si fuera a = ret no hubiera entrado).
        hardEquals(((memory*)m)->sbase, ((memory*)m)->szero, &isEq);

        //si base > 0, aún hay que iterar
        if (isEq == 1)
          isEq = 2;
        else
          //en caso contrario, hemos llegado a la máxima aproximación posible
          isEq = 0;
      } else
        //si lmax es inferior a 0, hemos llegado a la máxima aproximación posible
        isEq = 0;
    }
  }

  memcpy(va, ((memory*)m)->sraw, sizeof(BigInteger));
}

#if BI_STANDALONE == 1
/*
 * Función bipow.
 *
 * Simula la operación a = a^p
 */
void bipow(void* va, int p, void* m) {
  //validamos puntero
  if (validate == 1) 
    validateBI(va);

  if (p == 0)
    //n^0 = 1
    BImemcpy(va, 1);
  else if (p == 1)
    //n^1 = n
    return;
  else
    sBipow(va, p, m);
}
#endif

/*
 * Función sBipow.
 *
 * Simula la operación a = a^p
 */
void sBipow(void* va, int p, void* m) {
  int d2b[10];
  int d2bi = 0;
  int x = 0;
  int t = p;
  int i = 0;
  int sig = 0;

  if (((memory*)m)->bres == NULL || ((memory*)m)->btmp == NULL) {
    showError(6);
    return;
  }

  if (p < 0) {
    BImemcpy(va, 0);

    return;
  }

  BImemcpy(((memory*)m)->bres, 1);

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
  if (t > 0 && d2bi >= 10) {
    showError(8);
    return;
  }

  //normalizamos va
  if (((BigInteger*)va)->n[((BigInteger*)va)->count] < 0) {
    sig = 1;
    ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;
  }

  //iteramos sobre el número binario
  for (; i < d2bi; i++) {
    //calculamos (a^(2^i))
    if (i == 0)
      memcpy(((memory*)m)->btmp, va, sizeof(BigInteger));
    else
      sMul(((memory*)m)->btmp, ((memory*)m)->btmp, m);

    if (d2b[i] == 1)
      sMul(((memory*)m)->bres, ((memory*)m)->btmp, m);
  }

  //revertimos va si el exponente es impar
  if (sig == 1 && p % 2 == 1) 
    ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;

  memcpy(va, ((memory*)m)->bres, sizeof(BigInteger));
}

#if BI_STANDALONE == 1
/*
 * Función showError.
 *
 * Muestra un error en base al índice que se le pasa
 */
static void showError(int k) {
#if BI_SERVICE == 1
  BIReturnCode = k;
#else
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
  else if (k == 11)
    printf("Error. Puntero erróneo en add");
  else if (k == 98)
    printf("Error. Puntero erróneo en validateBI");
  else if (k == 99)
    printf("Error. Error de validación de datos");
  else
    printf("Error. Error desconocido");

  printf("\n");

  exit(k * -1);
#endif
}
#endif

#if BI_STANDALONE == 1
/*
 * Función iniStr.
 *
 * Reserva memoria para un char, para usarlo en toString.
 */
void iniStr(char** dst) {
  *dst = malloc(sizeof(char) * MAX_LENGTH);
}
#endif

#if BI_STANDALONE == 1
/*
 * Función toString.
 *
 * Escribe en pantalla el BigInteger
 */
void toString(void* vb, char* dst) {
  int i = 0;
  int m = ((BigInteger*)vb)->count;
  int sig = 0;

  //validamos puntero
  if (validate == 1) 
    validateBI(vb);

  //si el primer dígito es negativo, lo volvemos positivo y marcamos el caracter
  if (((BigInteger*)vb)->n[m] < 0) {
    ((BigInteger*)vb)->n[m] *= -1;
    dst[i++] = '-';
    sig = 1;
  }

  for (; m >= 0; m--)
    dst[i++] = (char)(((BigInteger*)vb)->n[m] + 48);

  dst[i] = '\0';

  //estamos trabajando sobre el puntero, revisamos y reestablecemos valores
  if (sig == 1)
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
}
#endif

/*
 * Función pAppend.
 *
 * Añade un dígito al final del BigInteger
 */
static void pAppend(void* va, int b) {
  int i;

  for (i = ((BigInteger*)va)->count; i >= 0; i--)
    ((BigInteger*)va)->n[i + 1] = ((BigInteger*)va)->n[i];

  ((BigInteger*)va)->n[0] = b;
  ++((BigInteger*)va)->count;
}

#if BI_STANDALONE == 1
/*
 * Función clean.
 *
 * Limpia la eura
 */
void clean(void* va) {
  BImemcpy(va, 0);
}
#endif

/*
 * Función signum.
 *
 * Devuelve la cantidad de datos negativos que hay en la operación.
 */
static int signum(int a, int b) {
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
  int i = 0;

  //validamos el tipo
  if (((BigInteger*)a)->k != 'i') {
    showError(99);
    return;
  }

  //validamos la longitud
  if (((BigInteger*)a)->count < 0 || ((BigInteger*)a)->count > MAX_LENGTH) {
    showError(99);
    return;
  }

  //validamos el resto de dígitos, que pueden ser positivos o negativos
  for (; i < MAX_LENGTH; i++) {
    if (((BigInteger*)a)->n[i] < -9 || ((BigInteger*)a)->n[i] > 9) {
      showError(99);
      return;
    }
  }
}

/*
 * Función getPoint
 *
 * Retorna la posición de corte decimal para la división
 */
int getPoint() {
  return BI_point;
}

/*
 * Función getReturnCode
 *
 * Retorna el código de estado
 */
int getReturnCode() {
  return BIReturnCode;
}

/*
 * Función setReturnCode
 *
 * Modifica el código de estado
 */
void setReturnCode(int k) {
  BIReturnCode = k;
}
