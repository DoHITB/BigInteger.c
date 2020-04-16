/*
 * BigInteger.c
 *
 *  Created on: 18 ene. 2019
 *      Author: DoHITB
 */
#include "stdio.h"
#include "conio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "BigInteger.h"

int MAX_LENGTH = 512;
float BI_VERSION = 1.0;

/*
 * Función initialize
 *
 * Da valores a ciertos datos útiles.
 */
void _BI_initialize() {
	_ZERO = newBI("0");
	_ONE  = newBI("1");
	_TEN  = newBI("10");
	_HUND = newBI("100");
	_MIN  = newBI("0");

	sub(&_MIN, _ONE);
}


/*
 * Función newBI.
 *
 * Genera un nuevo dato BI a partir del string que recibe como entrada.
 * Se cargan en orden inverso para permitir el crecimiento de manera sencilla.
 */
struct BigInteger newBI(char* s){
	struct BigInteger ret;
	int i = strlen(s) - 1;
	int f = i;
	int j = 0;

	//limpiamos el array
	clean(&ret);

	if (i > MAX_LENGTH) {
		showError(1);
	}

	//recorremos el string y lo guardamos en integers
	for(;i >= 0;i--)
		ret.n[j++] = (int)(s[i] - 48);

	ret.count = f;

	return ret;
}

/*
 * Función add. Usar para sumar dos números.
 *
 * Realiza la operación de suma, teniendo en cuenta los signos de los números.
 *
 * Si los signos son iguales, hace una suma, sino, una resta.
 */
void add(struct BigInteger *a, struct BigInteger b){
	int sig = signum(a, b);

	//normalizamos los operandos
	if(sig == 10)
		//a negativo, b positivo. Cambiamos el signo de "a" y hacemos suma
		a->n[a->count] *= -1;
	else if(sig == 1)
		//b negativo, a positivo. Cambiamos el signo de "b" y hacemos suma
		b.n[b.count] *= -1;
	else if(sig == 11){
		//a negativo, b negativo. Cambiamos signos y hacemos suma
		a->n[a->count] *= -1;
		b.n[b.count] *= -1;
	}

	//si ambos signos son iguales, se suma, sino, se resta
	if(sig == 0 || sig == 11)
		addition(a, b);
	else
		sub(a, b);

	if(sig == 10 || sig == 11)
		//en estos casos, siempre se le va la vuelta al signo
		a->n[a->count] *= -1;
}

/*
 * Función addition.
 *
 * Simula la operación a = a + b
 */
void addition(struct BigInteger *a, struct BigInteger b){
	//asumimos que a tiene la mayor longitud
	int limit = a->count;

	//asumimos que b tiene la menor longitud
	int min = b.count;

	//indicador de necesidad de arrastre
	int move = 0;

	int i = 0;

	//si no es así, rectificamos
	if(b.count > limit){
		limit = b.count;
		min = a->count;
		move = 1;
	}

	//sumamos los dígitos que coinciden
	for(;i <= min;i++)
		a->n[i] += b.n[i];

	//los dígitos que no coinciden los traspasamos
	if(move == 1){
		for(;i <= limit;i++)
			a->n[i] = b.n[i];

		a->count = limit;
	}

	//gestionamos el acarreo
	carryAdd(a);
}

/*
 * Función carryAdd.
 *
 * Gestiona el acarreo de la suma. Si hay movimiento de datos, se mueve el valor 1 a ret.
 * De esta manera, podemos llamar hasta que no haya cambios en el acarreo.
 */
void carryAdd(struct BigInteger *a){
	int i;
	int m = 0;
	int acc;
	int ret;

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
}

/*
 * Función sub. Usar para restar dos números.
 *
 * Simula la operación a = a - b.
 *
 * Si len(a) < len(b), se intercambian los valores
 */
void sub(struct BigInteger *a, struct BigInteger b){
	int sig;
	int comp;
	struct BigInteger aux = *a;

	hardEquals(aux, b, &comp);

	//if(a->count < b.count){
	if(comp == 2){
		sub(&b, aux);

		//cambiamos el signo
		b.n[b.count] *= -1;

		*a = b;
	}else if(comp == 0){
		a->n[0] = 0;
		a->count = 0;
	}else{
		sig = signum(a, b);

		//normalizamos los signos
		if(sig == 1)
			b.n[b.count] *= -1;
		else if(sig == 10){
			a->n[a->count] *= -1;
		}else if(sig == 11){
			a->n[a->count] *= -1;
			b.n[b.count] *= -1;
		}

		//si tienen el mismo signo, se resta, sino se suma
		if(sig == 0 || sig == 11)
			subtract(a, &b, a);
		else
			add(a, b);

		if(sig == 10 || sig == 11)
			//en estos casos, siempre se le va la vuelta al signo
			a->n[a->count] *= -1;
	}
}

/*
 * Función subrtact.
 *
 * Realiza la resta.
 */
void subtract(struct BigInteger *a, struct BigInteger *b, struct BigInteger *ret){
	int i = 0;
	int accType = 0;

	//restamos los dígitos comunes
	for(;i <= b->count;i++)
		a->n[i] -= b->n[i];

	//si el último dígito es negativo
	if(a->n[a->count] < 0)
		accType = 1;

	carrySub(a, accType);

	//movemos el resultado
	*ret = *a;
}

/*
 * Función carrySub.
 *
 * Gestiona el acarreo de la resta. Si carryType = 0, el acarreo
 * se gestiona como a = 10 + a; sino, se invierte el signo (excepto al último dígito)
 */
void carrySub(struct BigInteger *a, int carryType){
	int i;
	int m = 0;
	int acc;
	int ret;

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
}

/*
 * Función recount.
 *
 * Recuenta las cifras, para ver si hay que disminuir el conteo.
 */
void recount(struct BigInteger *a){
	while(a->n[a->count--] == 0);

	++a->count;

	if(a->count < 0)
		a->count = 0;
}

/*
 * Función hardEquals.
 *
 * Compara dos números. Devuelve 0 si "a" = "b"; 1 si "a" > "b"; 2 si "a" < "b".
 */
void hardEquals(struct BigInteger a, struct BigInteger b, int *ret){
	int i;
	int sig;

	//calculamos el signo
	/*
	 *00: a >= 0, b >= 0
	 *01: a >= 0, b <  0
	 *10: a <  0, b >= 0
	 *11: a <  0, b <  0
	 */
	sig = signum(&a, b);

	if (sig == 1) {
		//a >= 0, b < 0, por tanto a > b
		*ret = 1;
	}else if (sig == 10) {
		//a < 0, b >= 0, por tanto a < b
		*ret = 2;
	}else {
		//comparten signo. Hacemos comparación manual
		*ret = 0;

		if (a.count < b.count)
			//si "a" tiene menos dígitos que "b"
			* ret = 2;
		else if (a.count > b.count)
			//si "a" tiene más dítigos que "b"
			* ret = 1;
		else {
			//tienen los mismos dígitos. Comparamos uno a uno.
			for (i = a.count; i >= 0; i--) {
				if (a.n[i] < b.n[i])
					* ret = 2;
				else if ((a.n[i] > b.n[i]))
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
 * Función mul. Función para multiplicar dos números.
 *
 * Simula la operación a = a * b
 */
void mul(struct BigInteger *a, struct BigInteger b){
	int sig;
	int i = 0;
	struct BigInteger* part = malloc(sizeof(struct BigInteger) * (b.count + 1));
	struct BIT *table = malloc(sizeof(struct BIT));
	int currentBIT;
	int x;
	int comp;
	int calc = 0;
	struct BigInteger zero = newBI("0");
	struct BigInteger one = newBI("1");

	hardEquals(*a, zero, &comp);

	//si a = 0, no hace falta cálculo.
	if (comp == 0) {
		calc = 1;
	}

	hardEquals(b, zero, &comp);

	//si b = 0, no hace falta cálculo.
	if (comp == 0) {
		*a = zero;
		calc = 1;
	}

	sig = signum(a, b);
	//normalizamos los operandos
	if(sig == 1)
		b.n[b.count] *= -1;
	else if(sig == 10)
		a->n[a->count] *= -1;
	else if(sig == 11){
		a->n[a->count] *= -1;
		b.n[b.count] *= -1;
	}

	hardEquals(*a, one, &comp);

	//si a = 1, entonces a * b = b.
	if (comp == 0) {
		*a = b;
		calc = 1;
	}

	hardEquals(b, one, &comp);

	//si b = 1, entonces a * b = a.
	if (comp == 0) {
		calc = 1;
	}

	//inicializamos BIT
	table->BI[0] = zero;
	table->BI[1] = *a;

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
		for (i = 0; i <= b.count; i++) {
			//validamos si BIT[n] existe
			if (table->status[b.n[i]] == 0) {
				clean(&part[i]);
				//no tenemos el resultado. Calculamos el valor
				for (x = 0; x <= a->count; x++)
					part[i].n[x] = a->n[x] * b.n[i];

				part[i].count = x - 1;
				carryAdd(&part[i]);

				//movemos el valor a BIT
				table->BI[b.n[i]] = part[i];
				table->status[b.n[i]] = 1;
			}else {
				//tenemos el resultado
				part[i] = table->BI[b.n[i]];
			}

			pMul(i, &part[i]);
		}


		//realizamos el sumatorio
		for (i = 1; i <= b.count; i++) {
			add(&part[0], part[i]);
		}

		*a = part[0];
	}

	//si los signos son diferentes, invertimos el signo
	if(sig == 1 || sig == 10)
		a->n[a->count] *= -1;

	//liberamos memoria
	free(part);
	free(table);
}

/*
 * Función pMul.
 *
 * Efectúa una multiplicación parcial de a y b
 */
void pMul(int pos, struct BigInteger *part){
	int i = part->count + pos;

	//generamos offset
	for (; i >= pos; i--) {
		part->n[i] = part->n[i - pos];
	}

	//noramlizamos las posiciones de offset
	for (i = 0; i < pos; i++) {
		part->n[i] = 0;
	}

	part->count += pos;
}

/*
 * Función dvs. Función para dividir dos números.
 *
 * Simula la operación a = a / b
 */
void dvs(struct BigInteger *a, struct BigInteger b){
	struct BigInteger temp;
	struct BigInteger aux;
	struct BigInteger one;
	int sig;
	int comp;

	temp = newBI("0");
	one  = newBI("1");

	sig = signum(a, b);

	//normalizamos los operandos
	if(sig == 1)
		b.n[b.count] *= -1;
	else if(sig == 10)
		a->n[a->count] *= -1;
	else if(sig == 11){
		a->n[a->count] *= -1;
		b.n[b.count] *= -1;
	}

	aux = *a;

	hardEquals(aux, b, &comp);

	if(comp == 0)
		//si a = b, a/b = 1
		*a = one;
	else if(comp == 2)
		//si a < b, a / b = 0
		*a = temp;
	else if(comp == 1){
		//si a > b, buscamos un número que solucione ax = n
		hardEquals(b, one, &comp);

		if(comp == 0){
			//si b = 1, no hay nada que calcular
			return;
		}else{
			divide(a, b);
		}
	}

	//si los signos son diferentes, invertimos el signo
	if (sig == 1 || sig == 10)
		a->n[a->count] *= -1;
}

/*
 * Función divide.
 *
 * Realiza la división utilizando el teorema de Bolzano
 */
void divide(struct BigInteger *a, struct BigInteger b){
	struct BIT *table = malloc(sizeof(struct BIT));
	struct BigInteger dTemp;
	struct BigInteger ret;
	struct BigInteger temp;
	struct BigInteger biTemp;
	struct BigInteger zero = newBI("0");
	int len = a->count - b.count;
	int i = 0;
	int x = 0;
	int max = b.count;
	int eq;
	int currentBIT;
	int res;
	int mLen;
	int added;

	ret.count = 0;
	dTemp.count = 0;
	
	//llenamos BIT
	table->BI[0] = newBI("0");
	table->BI[1] = b;

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

	/*
	 * Nos quedamos con los "b.count" primeros dígitos. Si "b" tiene un único
	 * dígito, no movemos nada, porque ya moveremos el siguiente dígito más adelante
	 */
	if (b.count > 0) {
		for(;i < b.count;i++)
			dTemp.n[b.count - i - 1] = a->n[a->count - i];
	}else {
		dTemp.n[0] = 0;
	}

	dTemp.count = b.count - 1;

	if (dTemp.count == 0)
		--dTemp.count;

	//si "b" tiene una cifra, b.len será 0 pero tenemos que restar una cifra igualmente
	if (b.count == 0)
		mLen = len - 1;
	else
		mLen = len;


	//por cada cifra decimal que hemos generado
	for (i = 0; i <= len; i++) {
		//hacemos BI temporal
		temp.count = 0;
		temp.n[0] = a->n[len - i];

		pAppend(&temp, &dTemp);
		dTemp = temp;

		//Retorna 0 si son iguales, retorna 1 si a > b, retorna 2 si a < b.
		hardEquals(dTemp, table->BI[currentBIT], &eq);

		x = currentBIT;

		if (eq == 1) {
			//si dTemp > max, partimos de ese valor y hasta el máximo.
			
			//recuperamos el último BI
			biTemp = table->BI[x];
			added = 0;

			for (; x < 10; x++) {
				//le sumamos la base
				add(&biTemp, b);

				//lo añadimos a BIT, si no hemos llenado la tabla (rellenamos siempre por adelantado)
				if (currentBIT < 9) {
					table->BI[++currentBIT] = biTemp;
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
				hardEquals(dTemp, table->BI[x], &eq);

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

		ret.n[len - i] = res;

		//restamos
		sub(&dTemp, table->BI[res]);
	}

	ret.count = mLen;

	*a = ret;

	recount(a);
	free(table);
}

/*
 * Función BISQRT.
 *
 * Realiza la raíz cuadrada de a y la retorna como BI.
 */
struct BigInteger BISQRT(struct BigInteger a) {
	struct BigInteger ret = newBI("0");
	struct BigInteger i = newBI("0");
	struct BigInteger one = newBI("1");
	int stop = 0;

	hardEquals(a, ret, &stop);

	if (stop == 0)
		return ret;

	do {
		add(&ret, one);
		memcpy(&i, &ret, sizeof(struct BigInteger));
		mul(&i, i);
		hardEquals(i, a, &stop);
	} while (stop == 2);

	if(stop > 0)
		sub(&ret, one);

	return ret;
}

/*
 * Función pow.
 *
 * Simula la operación a = a^p
 */
void pow(struct BigInteger *a, int p) {
	int z = 1;
	struct BigInteger res;

	if (p == 0) 
		//n^0 = 1
		*a = newBI("1");
	else if (p == 1) 
		//n^1 = n
		return;
	else {
		res = *a;

		for (; z < p; z++)
			mul(a, res);
	}
}

/*
 * Función rev.
 *
 * Da la vuelta a un BigInteger
 */
void rev(struct BigInteger *a){
	int i = 0;
	int t = 0;
	int max = a->count / 2;

	for(;i <= max;i++){
		t = a->n[i];
		a->n[i] = a->n[a->count - i];
		a->n[a->count - i] = t;
	}
}

/*
 * Función makeBase
 *
 * simula la operación base = b * 10^length
 */
void makeBase(struct BigInteger* base, struct BigInteger b, int length, int* start){
	int i = 0;

	//estos dos bucles se pueden paralelizar
	for(;i < length;i++)
		base->n[i] = 0;

	base->count = i;
	*start = i;

	for(i = 0;i <= b.count;i++)
		base->n[base->count++] = b.n[i];

	--base->count;
}

/*
 * Función shift.
 *
 * Desplaza un BI en "start" posiciones
 */
void shift(struct BigInteger* base, int start){
	int i = start;

	for (; i <= base->count; i++) 
		base->n[i] = base->n[i + 1];

	base->n[base->count] = 0;

	--base->count;
}

/*
 * Función showError.
 *
 * Muestra un error en base al índice que se le pasa
 */
void showError(int k){
	if(k == 1)
		printf("Error. Limite alcanzado\n");
}

/*
 * Función toString.
 *
 * Escribe en pantalla el BigInteger
 */
void toString(struct BigInteger b){
	for(;b.count >= 0;b.count--)
		printf("%i", b.n[b.count]);
}

/*
 * Función append
 *
 * Sinónimo de la función pAppend para poder trabajar con b como
 * puntero, o con b como valor.
 */
void append(struct BigInteger *a, struct BigInteger b){
	pAppend(a, &b);
}

/*
 * Función pAppend.
 *
 * Adjunta un BigInteger a otro. OJO, esta función se debe usar en el orden
 * contrario al habitual. Si queremos unir "1" y "2" para tener "12",
 * a = 2 y b = 1 (ya que los array de BigInteger van al revés.
 */
void pAppend(struct BigInteger *a, struct BigInteger *b){
	//si b = 0, no hacemos nada
	//no usamos equals para no dañar el rendimiento
	if (b->count == 0 && b->n[0] == 0) {
		return;
	}
	
	int i = 0;
	int x = 0;
	struct BigInteger aux;

	aux.count = 0;

	if (a->count == 0) {
		for (; i <= b->count; i++)
			a->n[++a->count] = b->n[i];
	}else {
		//ponemos tantos 0's como posiciones tenga "a"
		for (; i <= a->count; i++)
			aux.n[aux.count++] = 0;

		//traspasamos "b" a aux
		for (i = 0; i <= b->count; i++)
			aux.n[aux.count++] = b->n[i];

		--aux.count;

		add(a, aux);
	}
}

/*
 * Función clean.
 *
 * Limpia la estructura
 */
void clean(struct BigInteger *a){
	int i = MAX_LENGTH - 1;

	for(;i > a->count;i--)
		a->n[i] = 0;
}

/*
 * Función signum.
 *
 * Devuelve la cantidad de datos negativos que hay en la operación.
 */
int signum(struct BigInteger* a, struct BigInteger b){
	int ret = 0;

	if(a->n[a->count] < 0)
		ret = 10;

	if(b.n[b.count] < 0)
		++ret;

	return ret;
}