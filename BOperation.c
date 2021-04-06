/*
 * BOperation.c
 *
 *  Created on: 15 nov. 2020
 *    Author: DoHITB under MIT License
 *
 *  CHANGELOG
 *    v0.1
 *      - Creación a partir de BigInteger 4.71 ~ 5.00
 */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"
#include "BOperation.h"
#include "BigInteger.h"
#include "BigDouble.h"

static int validate = 
#ifdef CVALIDATE
  CVALIDATE;
#else
  1;
#endif

/*
 * Función dummyOp1. Función vacía para usar en cal2op
 */
static void dummyOp1(void* da, void* db, void* m) {
  return;
}

/*
 * Función dummyOp2. Función vacía para usar en cal2op
 */
static void dummyOp2(void* da, void* db, int* di) {
  return;
}

/*
 * Función adjustData. Usar para ajustar un dato.
 *
 * Realiza operaciones tipo va {*= | /=} 10^offset
 */
static void adjustData(void* va, int offset, int up) {
  int i;

  if (up == 1) {
    //añadir cifras
    //desplazamos "offset" cifras
    for (i = ((BigInteger*)va)->count; i >= 0; i--)
      ((BigInteger*)va)->n[i + offset] = ((BigInteger*)va)->n[i];

    //colocamos "offset" cifras
    for (i = 0; i < offset; i++)
      ((BigInteger*)va)->n[i] = 0;

    //reajustamos count
    ((BigInteger*)va)->count += offset;
  } else {
    //quitar cifras
    //desplazamos "offset" cifras
    for(i = offset; i <= ((BigInteger*)va)->count; i++)
      ((BigInteger*)va)->n[i - offset] = ((BigInteger*)va)->n[i];

    //borramos "offset" cifras. Empezamos por 1 para evitar desplazamiento
    for (i = 1; i <= offset; i++)
      ((BigInteger*)va)->n[((BigInteger*)va)->count - offset + i] = 0;

    //reajustamos count
    ((BigInteger*)va)->count -= offset;
  }
}

/*
 * Función decimalize. Usar para ajustar los datos decimales
 * 
 * Mueve los datos de manera coherente en caso que "pos" sea negativo (es decir, 
 * el dato es tipo 0,n)
 */
static void decimalize(void* va) {
  int dpos = ((BigDouble*)va)->cpos * -1;
  int i = 0;

  //validamos para evitar problemas
  if (dpos < 0)
    return;

  if (((BigDouble*)va)->count + dpos > MAX_LENGTH) {
    //desplazamos dpos posiciones, borrando así dpos posiciones sobrantes.
    for (i = 0; i <= ((BigDouble*)va)->count - dpos; i++)
      ((BigDouble*)va)->n[i] = ((BigDouble*)va)->n[i + dpos];

    for (; i <= ((BigDouble*)va)->count; i++)
      ((BigDouble*)va)->n[i] = 0;

    //reajustamos count
    ((BigDouble*)va)->count -= dpos;
  }

  //añadimos dpos 0's a partir de count. Como la estructura está normalizada, basta con aumentar count
  ((BigDouble*)va)->count += dpos;

  //ahora, la posición decimal será count
  ((BigDouble*)va)->cpos = ((BigDouble*)va)->count;
}

/*
 * Función cal2op. Usar para llamar a las operaciones
 * 
 * Funciona como wrapper para la llamada centralizada a las operaciones.
 * Realiza funciones de ajuste de datos, correcciones, etc.
 */
static void cal2op(void* va, void* vb, void* m, char k, int* ret) {
  void (*cal)(void*, void*, void*);
  void (*cal2)(void*, void*, int*);
  char ka = getKind(va);
  char kb = getKind(vb);
  int adj = 0;
  int adi = 0;
  int dvi = 0;
  int dvj = 0;

  //asignaciones iniciales
  cal = dummyOp1;
  cal2 = dummyOp2;

  //asignamos dinámicamente la función de cálculo
  if (k == 'a')
    cal = pAdd;
  else if (k == 's')
    cal = pSub;
  else if (k == 'm')
    cal = sMul;
  else if (k == 'd')
    cal = sDvs;
  else if (k == 'e')
    cal2 = hardEquals;
  else
    return;

  if (((memory*)m)->a == NULL || ((memory*)m)->b == NULL) 
    showError(90);

  //validamos los datos antes de tratarlos
  if (ka == 'i') {
    if (validate == 1)
      validateBI(va);
  } else if (ka == 'd') {
    if(validate == 1)
      validateBD(va);
  }

  if (kb == 'i') {
    if (validate == 1)
      validateBI(vb);
  } else if (kb == 'd') {
    if(validate == 1)
      validateBD(vb);
  }

  if (ka == 'i' && kb == 'i') {
    //operando int : int
    if (k == 'e')
      cal2(va, vb, ret);
    else
      cal(va, vb, m);
  } else {
    //copiamos los datos ajustarlos
    if (ka == 'i') {
      //a es int
      memcpy(((memory*)m)->a, va, sizeof(BigInteger));

      if (kb == 'd') {
        //operando int : dou. Retiramos de b las cifras decimales
        memcpy(((memory*)m)->b, vb, sizeof(BigInteger));
        adjustData(((memory*)m)->b, ((BigDouble*)vb)->cpos, 0);
      }
    } else if (ka == 'd') {
      //a es double
      memcpy(((memory*)m)->a, va, sizeof(BigInteger));

      if (kb == 'd') {
        //b es double
        memcpy(((memory*)m)->b, vb, sizeof(BigInteger));

        if (((BigDouble*)va)->cpos > ((BigDouble*)vb)->cpos) {
          //a tiene más cifras decimales. Añadimos la diferencia de decimales a b
          adjustData(((memory*)m)->b, ((BigDouble*)va)->cpos - ((BigDouble*)vb)->cpos, 1);

          //en la multiplicación necesitamos guardar el ajuste de decimales
          if (k == 'm')
            adj = ((struct BigDouble*)va)->cpos;

        } else if (((BigDouble*)va)->cpos < ((BigDouble*)vb)->cpos) {
          //b tiene más cifras decimales. Añadimos la diferencia de decimales a a
          adjustData(((memory*)m)->a, ((BigDouble*)vb)->cpos - ((BigDouble*)va)->cpos, 1);

          //indicamos el ajuste de decimales
          adj = ((struct BigDouble*)vb)->cpos;
        }
      } else if (kb == 'i') {
        //operando dou : int. Añadimos cifras al int
        memcpy(((memory*)m)->b, vb, sizeof(BigInteger));
        adjustData(((memory*)m)->b, ((BigDouble*)va)->cpos, 1);

        //en la multiplicación necesitamos guardar el ajuste de decimales
        if(k == 'm')
          adj = ((struct BigDouble*)va)->cpos;
      }

      if (k == 'd') {
        adj = 0;

        //estamos con un double. Si estamos dividiendo tenemos que validar que es factible a < b
        if (((BigInteger*)((memory*)m)->a)->count < ((BigInteger*)((memory*)m)->b)->count) {
          //a tiene menos cifras que b
          adj = ((BigInteger*)((memory*)m)->b)->count - ((BigInteger*)((memory*)m)->a)->count;
          adjustData(((memory*)m)->a, adj, 1);
        }

        hardEquals(((memory*)m)->a, ((memory*)m)->b, &dvi);

        //si aún a < b, multiplicamos por 10
        if (dvi == 2) {
          adjustData(((memory*)m)->a, 1, 1);
          ++adj;
        }
        
      }
    }

    //delegamos en la función estática de BigInteger
    if (k == 'e')
      cal2(((memory*)m)->a, ((memory*)m)->b, ret);
    else
      cal(((memory*)m)->a, ((memory*)m)->b, m);

    //algunas operaciones no necesitan ajustar datos
    if (k != 'e') {
      //ajustamos decimales si es necesario

      //copiamos a a va
      if (ka == 'i')
        memcpy(va, ((memory*)m)->a, sizeof(BigInteger));
      else if (ka == 'd') {
        //primero restauramos la posición decimal
        adi = ((BigDouble*)va)->cpos;

        memcpy(va, ((memory*)m)->a, sizeof(BigDouble));

        //si estamos multiplicando, el contador decimal será el doble debido al ajuste previo
        if (k == 'm')
          adj *= 2;

        if (k == 'd') {
          //el ajuste decimal de la división es distinto.
          ((BigDouble*)va)->cpos = (((BigDouble*)va)->count - getPoint()) + adj;

          //si cpos >= count, el resultado es 0,n. Lo pasamos a modo negativo para que decimalize lo trate
          if (((BigDouble*)va)->cpos > ((BigDouble*)va)->count)
            ((BigDouble*)va)->cpos = ((BigDouble*)va)->count - ((BigDouble*)va)->cpos - 1;

          //si cpos == count y hemos incrementado para igualar, dividimos entre 10
          if (((BigDouble*)va)->cpos == ((BigDouble*)va)->count && dvi == 2)
            ((BigDouble*)va)->cpos = -1;

          //si cpos es negativo o es mayor que count, es que es 0,n
          if (((BigDouble*)va)->cpos < 0) 
            decimalize(va);
          else {
            //si cpos = longitud, es que no hay decimales. Lo aislamos de decimalize porque en ese caso count = cpos
            if (((BigDouble*)va)->cpos == ((BigDouble*)va)->count)
              ((BigDouble*)va)->cpos = 0;
          }
        } else {
          //reajustamos el valor decimal
          if (adj > 0)
            ((BigDouble*)va)->cpos = adj;
          else
            ((BigDouble*)va)->cpos = adi;

          //para la resta, reajustamos los decimales (si la parte entera de a = parte entera de b)
          if (k == 's') {
            if (((BigDouble*)va)->cpos > ((BigDouble*)va)->count)
              ((BigDouble*)va)->cpos = ((BigDouble*)va)->count - ((BigDouble*)va)->cpos;

            //si cpos es negativo o es mayor que count, es que es 0,n
            if (((BigDouble*)va)->cpos < 0) 
              decimalize(va);
          }
        }
      }
    }

    //reajustamos el tipo de dato (algunas operaciones - como la resta cuando a < b - intercambian datos)
    ((BigInteger*)va)->k = ka;
    ((BigInteger*)vb)->k = kb;
    
    //recalculamos los 0 poco significativos (dobule o integer en una división)
    if(ka == 'd')
      rePos(va);
  }
}

/*
 * Función rePos. Reposiciona los datos
 * 
 * Esta función elimina los 0 poco significativos de la parte decimal
 */
static void rePos(void* va) {
  int i = -1;
  
  //si solo tiene 1 cifra, no lo tratamos
  if (((BigDouble*)va)->count == 0)
    return;

  //si no hay posiciones decimales, no lo tratamos
  if (((BigDouble*)va)->cpos == 0)
    return;

  while (((BigDouble*)va)->n[++i] == 0);
  
  //si i > cpos significa que el nº termina en 0 y no tiene decimales.
  if (i > ((BigDouble*)va)->cpos)
    i = ((BigDouble*)va)->cpos;

  if (i > 0) {
    //hay 0's poco significativos. Los quitamos
    adjustData(va, i, 0);

    //reajustamos cpos (adjustData modifica cpos y no queremos eso en este caso
    ((BigDouble*)va)->cpos -= i;
  }
}

/*
 * Función add. Usar para sumar dos números.
 *
 * Realiza la operación de suma, teniendo en cuenta los signos de los números.
 *
 * Si los signos son iguales, hace una suma, sino, una resta.
 */
void add(void* va, void* vb, void* m) {
  cal2op(va, vb, m, 'a', NULL);
}

/*
 * Función sub. Usar para restar dos números.
 *
 * Simula la operación a = a - b.
 *
 * Si len(a) < len(b), se intercambian los valores
 */
void sub(void* va, void* vb, void* m) {
  cal2op(va, vb, m, 's', NULL);
}

/*
 * Función equals.
 *
 * Función pública de hardEquals
 * Compara dos números. Devuelve 0 si "a" = "b"; 1 si "a" > "b"; 2 si "a" < "b".
 */
void equals(void* va, void* vb, void* m, int* ret) {
  cal2op(va, vb, m, 'e', ret);
}

/*
 * Función mul. Función para multiplicar dos números.
 *
 * Simula la operación a = a * b
 */
void mul(void* va, void* vb, void* m) {
  cal2op(va, vb, m, 'm', NULL);
}

/*
 * Función dvs. Función para dividir dos números.
 *
 * Simula la operación a = a / b
 */
void dvs(void* va, void* vb, void* m) {
  cal2op(va, vb, m, 'd', NULL);
}

/*
 * Función nqrt.
 *
 * Realiza la raíz enésima de a.
 */
void nqrt(void* va, int n, void* m) {
  //de momento, solo funciona para integers
  if (getKind(va) == 'i') {
    //validamos punteros
    if(validate == 1)
      validateBI(va);

    //delegamos en la función estática
    sNqrt(va, n, m);
  }
}

/*
 * Función bipow.
 *
 * Simula la operación a = a^p
 */
void bipow(void* va, int p, void* m) {
  //de momento,  solo funciona para integers
  if (getKind(va) == 'i') {
    //validamos puntero
    if(validate == 1)
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
}

/*
void operate(int count, ...) {
  va_list pa;
  va_start(pa, count);
  
  int i = 0;
  void* cal = malloc(sizeof(operation));

  if (cal == NULL)
    showError(12);

  for (i = 0; i < count; i++) {
    //copiamos el operation que nos envían
    memcpy(cal, (void*)va_arg(pa, size_t), sizeof(operation));

    //llamamos al motor de operaciones
    cal2op(((operation*)cal)->va, ((operation*)cal)->vb, ((operation*)cal)->m, ((operation*)cal)->op, ((operation*)cal)->ret);
  }

  //cerramos
  va_end(pa);

  //liberamos memoria
  free(cal);
}
*/

/*
 * Función biSig.
 *
 * Simula la operación a *= -1
 */
void biSig(void* va) {
  char ka = getKind(va);

  //validamos los datos antes de tratarlos
  if (ka == 'i') {
    if (validate == 1)
      validateBI(va);
  } else if (ka == 'd')
    if(validate == 1)
      validateBD(va);

  //multiplicamos por -1 el primer dígito
  ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;

  //cambiamos el signo si es doule
  if (ka == 'd') {
    if (((BigDouble*)va)->sig == 0)
      ((BigDouble*)va)->sig = -1;
    else
      ((BigDouble*)va)->sig = 0;
  }
}

/*
 * BI2DB
 * Convierte un BigInteger en BigDouble
 */
void BI2BD(void* dst, void* src) {
  if (validate == 1)
    validateBI(src);

  //copiamos BI a BD, ya que comparten estructura
  memcpy(dst, src, sizeof(BigInteger*));

  //ajustamos los datos de BigDouble
  ((BigDouble*)dst)->k = 'd';
  ((BigDouble*)dst)->cpos = 0;

  if (((BigDouble*)dst)->n[((BigDouble*)dst)->count] < 0)
    ((BigDouble*)dst)->sig = -1;
  else
    ((BigDouble*)dst)->sig = 0;

}

/*
 * Función showError.
 *
 * Muestra un error en base al índice que se le pasa
 */
void showError(int k) {
  if (k == 1)
    printf("Error. Limite alcanzado");
  else if (k == 2)
    printf("Error. Datos erróneos en creación");
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
  else if (k == 12)
    printf("Error. Puntero erróneo en operate");
  else if (k == 90)
    printf("Error. Puntero erróneo en calc");
  else if (k == 97)
    printf("Error. Tipo de dato inválido");
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
  *dst = malloc(sizeof(char) * MAX_LENGTH + 3);
}

/*
 * Función toString.
 *
 * Escribe en pantalla el BigInteger
 */
void toString(void* vb, char* dst) {
  int i = 0;
  int m = ((BigInteger*)vb)->count;
  int sig = 0;
  char kb = getKind(vb);
  int len = 0; 
  char dsig = 0;

  //validamos puntero
  if (kb == 'i') {
    if (validate == 1)
      validateBI(vb);
  } else if (kb == 'd') {
    if (validate == 1)
      validateBD(vb);

    len = ((BigDouble*)vb)->count;
    dsig = ((BigDouble*)vb)->sig;
  }

  //si el primer dígito es negativo, lo volvemos positivo y marcamos el caracter
  //tenemos también en cuenta el valor de sig, ya que en los valores decimales con 0 se pierde el signo natural
  if (((BigInteger*)vb)->n[m] < 0 || dsig == -1) {
    ((BigInteger*)vb)->n[m] *= -1;
    dst[i++] = '-';
    sig = 1;
  }

  for (; m >= 0; m--) {
    if(kb == 'd')
      if ((m + 1) == ((BigDouble*)vb)->cpos && ((BigDouble*)vb)->cpos > 0)
        //si es un double, y tiene coma decimal, la ponemos
        dst[i++] = coma;
    
    dst[i++] = (char)(((BigInteger*)vb)->n[m] + 48);
  }

  dst[i] = '\0';

  //estamos trabajando sobre el puntero, revisamos y reestablecemos valores
  if (sig == 1)
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
}


/*
 * Función clean.
 *
 * Limpia la estructura
 */
void clean(void* va) {
  if (getKind(va) == 'i')
    BImemcpy(va, 0);
  else
    BDmemcpy(va, 0);
}

/*
 * Función getKind
 *
 * retorna el tipo de dato que está en uso
 */
static char getKind(void* a) {
  char* c = malloc(sizeof(char));
  char ret = ' ';

  if (c == NULL) 
    showError(97);

  memcpy(c, a, sizeof(char));

  if (*c == 'i' || *c == 'd')
    memcpy(&ret, c, sizeof(char));
  else
    ret = 'i';

  free(c);

  return ret;
}

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
  ((memory*)m)->biBIT = malloc(sizeof(BIT));

  //cal2op
  ((memory*)m)->a = malloc(sizeof(BigInteger));
  ((memory*)m)->b = malloc(sizeof(BigInteger));
  
  //valores comunes
  _BI_initialize();

  BImemcpy(((memory*)m)->vt, 2);
}

size_t getMemorySize() {
  return (sizeof(BigInteger) * 21) + sizeof(BIT);
}