/*
 * BigInteger.c
 *
 *  Created on: 18 ene. 2019
 *    Author: DoHITB under MIT License
 *
 * +--------------------------------------------------------------------------------+
 * | MIT License                                                                    |
 * |                                                                                |
 * | Copyright (c) 2022 David Oscar Solé González (aka DoHITB)                      |
 * |                                                                                |
 * | Permission is hereby granted, free of charge, to any person obtaining a copy   |
 * | of this software and associated documentation files (the "Software"), to deal  |
 * | in the Software without restriction, including without limitation the rights   |
 * | to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      |
 * | copies of the Software, and to permit persons to whom the Software is          |
 * | furnished to do so, subject to the following conditions:                       |
 * |                                                                                |
 * | The above copyright notice and this permission notice shall be included in all |
 * | copies or substantial portions of the Software.                                |
 * |                                                                                |
 * | THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     |
 * | IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       |
 * | FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    |
 * | AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         |
 * | LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  |
 * | OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  |
 * | SOFTWARE.                                                                      |
 * +--------------------------------------------------------------------------------+
 *
 *  CHANGELOG
 *  v1.1 (beta)
 *    - Review variables and functions, making it "static" when convenient.
 *    - Changing all pointer references to "void" to avoid memory leakage.
 *    - Added new validations on BI creation.
 *    - New function "BImemcpy" to retreive useful values of BigInteger.
 *    - New function "validateBI" to validate the internal structure of a BI void pointer.
 *  v1.11 (beta)
 *    - Added memory cleansing before "return" statements.
 *  v1.2 (beta)
 *    - Added new behaviour for better performance on "pow" function.
 *  v1.3 (beta)
 *    - Patch to avoid memory leackage.
 *    - Deleted "time.h" library (unnecesary).
 *  v1.4 (beta)
 *    - Changed "BIQSRT" function for "nqrt", that makes "n-root"
 *    - Delete "static" from BIT (unnecesary).
 *    - Created wrapper function for "add" ,"sub", and "mul" for internal use.
 *    - Arranged header variable names to naming convention.
 *    - Function "signum" modified to increase performance. It now works with two int.
 *      - 90,566% memory usage reduction on nqrt via wrappers.
 *      - 53,656% performance increase on nqrt via wrappers.
 *      - (nqrt is used to measurement as nqrt -> pow -> mul -> pmul -> add is the longest function chain available).
 *  v1.5 (beta)
 *    - Changes on toString function to move data to char  instead of printing.
 *  v1.6 (beta)
 *    - Bugfix on validateBI on negative numbers.
 *    - Bugfix on toString on negative numbers.
 *  v2.0 (Release)
 *    - SIT (324 diferent operations) done. Fixes done.
 *      - Remove validateBI from pSub.
 *      - Bugfix on add if b < 0 and |a| < |b|.
 *      - Bugfix on add if |a| < |b| and a, b < 0.
 *      - Bugfix on division if a / b, b < 0.
 *    - Code cleansing
 *  v2.1 (Release)
 *    - Bugfix on "division" function.
 *  v2.2 (Release)
 *    - Bugfix on "validateBI" function. Avoid "t" data leackage.
 *  v2.3 (Release)
 *    - Naming change on "pow" function. Now it's called "bipow" to avoid internal errors.
 *  v2.4 (Release)
 *    - Bugfix on division when len(a) - len(b) = 1.
 *  v2.5 (Release)
 *    - Change on "nqrt" function for performance improvement. Root calculation via Bolzano.
 *  v2.51 (Release)
 *    - Bugfix on "validateBI".
 *  v3.0 (Major Release)
 *    - Deleted unnecesary commented lines.
 *    - Changed the signature of "subtraction" for "subtract(a, b)".
 *    - Deleted function "rev" (unnecesary).
 *    - Deleted "min" variable from multiplication.
 *    - Deleted "a = -1" and "b = -1" validation on multiplication (unnecesary).
 *    - Added length on "pMul" function to avoid heap overflow.
 *    - Change on "sMul" function. Instead of doing all the products and then all the add, now it make product-addition loop. This way, there are less memory consumption and better performance.
 *    - Change on "carryAdd" function. Deleted "Do/While" loop (unncesary).
 *    - Deleted function "makeBase" (unnecesary).
 *    - Deleted function "shift" (unnecesary).
 *    - New function iniStr. It allocate memory for "toString" function.
 *    - General code cleansing.
 *  v3.01 (Release)
 *    - Deleted "Do/While" loop on "carrySub" function.
 *  v3.02 (Release)
 *    - Optimization on "carryAdd" function. Carry is made in two times.
 *      - Common part: it's all carried.
 *      - Non-common part: it's carried unti acc = 0.
 *      - 3.448% performance increase tested with 10.000 additions of 100 digit numbers.
 *  v3.1 (Release)
 *    - Function "nqrt" modified.
 *      - Change on "add" calls by "pAdd".
 *      - Change on "equals" calls by "hardEquals".
 *      - Change on "sub" calls by "pSub".
 *      - Change on "bipow" calls by "sBipow".
 *      - Performance increase of 38'758% based on 10.000 nqrt calls.
 *    - New function "sBipow", static wrapper for "bipow".
 *    - Bugfix on "carryAdd".
 *  v3.11 (Release)
 *    - Deleted unused variables.
 *  v4.0 (Major Release)
 *    - Review for CUDA codification.
 *      - Changes on all functionalities. We operate directly upon the pointer, saving a bunch of performance (e.g: up to 25'957% on addition operation). Also reduce RAM consumption to 50%.
 *      - Additional validations on "nqrt" function.
 *      - General code cleansing.
 *      - Review and removal of "showError" codes.
 *      - Overall, 201 code lines were removed on the optimisation.
 *    - Bugfix on "validateBI".
 *    - Changes on "dvs" to delegate on static function "sDvs".
 *    - Changes on "nqrt" to delegate on static funcion "sNqrt".
 *    - New function "free" to massive memory free.
 *    - Bugfix on "toString" with negative numbers.
 *    - Some variables are changed to static.
 *    - Deleted "limits.h" library.
 *    - Component Test performed. Changes derived of it:
 *      - Added default value on "BImemcpy": 0.
 *      - Added a "clean" call on pAppend.
 *      - Bugfix on "append" with negative numbers.
 *      - Bugfix on division (0 / 0 = 1).
 *      - Bugifx on 0 or below index root.
 *      - Bugfix on below 0 index power.
 *  v4.1 (Release)
 *    - Bugfix on subtraction when a < b.
 *    - Added cast to int on strlen.
 *  v4.2 (Release)
 *    - Bugfix on "b" when a < b.
 *  v4.3 (Release)
 *    - Bugfix on one-digit division.
 *    - Bugfix on bipow when va < 0.
 *  v4.4 (Release)
 *    - New static BI data.
 *    - Improvement on BI_initialize.
 *    - Bugfix on same-operator functions.
 *      - add(a, a).
 *      - sub(a, a).
 *      - mul(a, a).
 *      - dvs(a, a).
 *  v4.5 (Release)
 *    - carryAdd is no longer static.
 *  v4.6 (Release)
 *    - add new field "k" on BigInteger, as preparation for Major Release BigInteger v5.0.
 *  v4.7 (Release)
 *    - "validateBI" changes to improve performance.
 *    - Deleted cast on malloc to imporve performance.
 *    - Bugfix on sNqrt when data lenght < 0.
 *    - Performance improvement on clean function.
 *  v4.71 (Release)
 *    - Added function iniBIT to allocate BIT memory.
 *    - Now multiplication and division works with external BIT to increase performance.
 *  v5.0 (Major Release)
 *    - Changed "n" type from int to char, reducing up to 75% of memory usage.
 *    - Use of external variables to increase performance (gaining memory usage).
 *    - "free" function has been removed.
 *    - New precompiler parameter added for setting BI length (C_MAX_LENGTH). If not present, 4096 will be set as length.
 *    - New precompiler parameter added for making BI validations optional (CVALIDATE). If it have a value of 1, validations will be made.
 *    - New precompiler parameter added or Standalone BI (BI_STANDALONE).
 *    - "iniBIT" function has been removed, as it has been integrated on "init" function.
 *    - Comment and identation revision.
 *    - Typedef created in order to remove "struct" references (code clarify).
 *    - Improvement made on "add" in order to improve performance.
 *    - Improvement made on "subtract". Now we call "addition" instead of "pAdd". Performance improvement.
 *    - Improvement made on "subtract". "CarryType" has been removed.
 *    - Improvement made on "mul". Now we call "addition" instead of "pAdd". Performance improvement.
 *    - Performance improvement made on "division" as "append" functionality has been rewritten.
 *       - "dtmp" variable removed.
 *       - "pAppend" is now an internal (static) function for "division" exclusive use.
 *    - Improvement made on "nqrt". Now we call "addition" instead of "pAdd". Performance improvement.
 *    - Performance improvement on "hardEquals" when "va" and "vb" are the same memory pointer.
 *    - "Clean" function is now public.
 *    - "BOperation" interface now created. Shall be used when BI_STANDALONE has not been declared.
 *      - Common functions have been refactorized.
 *      - _BI_Initialize not longer static.
 *    - New interface BigDouble created. Now you can use Integer and Double operations
 *      - "nqrt" and "bipow" are still integer-only operations.
 *  v5.01 (Release)
 *    - Bugfix on hardEquals function
 *  v5.02 (Release)
 *    - Improvement on "n" variable. Now it's signed char
 *  v5.1 (Release)
 *    - Included service mode by using precompiler directive "BI_SERVICE". If "BI_SERVICE == 1", program won't stop if error found.
 *    - Improved dynamically BigDouble creation by using precompiler directives
 *    - Bugfixes
 *  v5.2 (Release)
 *    - Bugfix on division while "b" have a signle digit
 *  v5.22 (Bugfix)
 *    - General bugfix on division
 *  v5.3 (Release)
 *    - Added new "mod" function that performs a %= b
 *  v5.31 (Bugfix)
 *    - Bugfix on "getMemorySize" function
 *  v5.32 (Bugfix)
 *    - Bugfix (again) on division function
 *  v6.0
 *    - CUDA Integration
 *      - Added precompiler directive "CUDA_ENABLED". While it's set to one, CUDA can be used.
 *      - "CUDA_ENABLED" directive shall be used aside with "BI_STANDALONE".
 *    - CUDA Functionalities
 *    - Comment translation begins
 *    - Function visibility (static) had been reviewed and improved by using "BI_STANDALONE" directive
 *    - Changes on mod signature to match the rest of the function signatures
 *    - Added validation on mod function
 *    - Removed "m" parameter for "equals" function
 *    - newBI function now has a "const" on chat to avoid compiling warnings
 *    - Changes on pSub to avoid stack loop issue during CUDA linking
 */

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "BigInteger.h"
#if BI_STANDALONE != 1
#include "BOperation.h"
#endif

#if CUDA_ENABLED == 1
#include "stdlib.h"
#include "conio.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#endif

static float BI_VERSION = 6.0f;

#if BI_STANDALONE == 1
static int validate =
#ifdef CVALIDATE
CVALIDATE;
#else
1;
#endif
#endif

#if CUDA_ENABLED == 1
/*
 * h2d
 *
 * Copies host memory to device memory.
 * It assumes that both device and host are an array of quantity items.
 */
void h2d(void** device, void* host, int quantity, size_t size) {
  if (quantity <= 0) {
    showError(52);

    return;
  }

  cudaMalloc(device, size * quantity);
  cudaMemcpy(*device, host, size * quantity, cudaMemcpyHostToDevice);
}

/*
 * d2h
 *
 * Copies device memory to host memory.
 * It assumes that both device and host are an array of quantity items.
 */
void d2h(void* host, void* device, int quantity, size_t size) {
  if (quantity <= 0) {
    showError(53);

    return;
  }

  cudaMemcpy(host, device, size * quantity, cudaMemcpyDeviceToHost);
}

/*
 * CUnewBI.
 *
 * Generates a new BI from the input string (dst).
 * The load is made on reverse order to allow a simple growing mechanism.
 * If "sig" is -1, the number will be negative.
 *
 * CUDA version
 */
__device__ void CUnewBI(void* dst, char* s, int sig) {
  int i = CUstrlen(s) - 1;
  int f = i;
  int j = 0;
  int c;
  int ssig = sig;

  //sign adjustment
  ((BigInteger*)dst)->k = 'i';

  //clean the array
  CUclean(dst);

  //iterate over the string and save data as integers
  for (; i >= 0; i--) {
    c = (int)(s[i] - 48);

    if (c >= 0 && c <= 9)
      ((BigInteger*)dst)->n[j++] = c;
    else
      if (s[i] == '-')
        ssig = -1;
  }

  //if a negative is sent, remove a position
  if (s[0] == '-')
    --f;

  ((BigInteger*)dst)->count = f;

  if (ssig == -1)
    ((BigInteger*)dst)->n[((BigInteger*)dst)->count] *= -1;
}

/*
 * CUclean. Cleans a BigInteger, moving 0 as value
 *
 * CUDA version
 */
__device__ static void CUclean(void* va) {
  int i = 0;

  for (i = 0; i < 1024; i++)
    ((BigInteger*)va)->n[i] = 0;

  ((BigInteger*)va)->count = 0;
}

/*
 * CUsMul.
 *
 * Simulates a *= b
 *
 * CUDA version
 */
__device__ void CUsMul(void* va, void* vb, void* vc, void* vd) {
  int sig;
  int i;
  int x;

  //clean aux vars (vc, vd)
  CUclean(vc);
  CUclean(vd);

  //get signum
  sig = CUsignum(((BigInteger*)va)->n[((BigInteger*)va)->count],
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count]);

  //normalize
  if (sig == 1)
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
  else if (sig == 10)
    ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;
  else if (sig == 11) {
    ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
  }

  //for each digit on vb
  for (i = 0; i <= ((BigInteger*)vb)->count; i++) {
    //perform partial product
    for (x = 0; x <= ((BigInteger*)va)->count; x++)
      ((BigInteger*)vc)->n[x] = ((BigInteger*)va)->n[x] * ((BigInteger*)vb)->n[i];

    //adjust length and carry
    ((BigInteger*)vc)->count = x - 1;
    CUcarryAdd(vc, 0, 0);

    //get offset
    CUpMul(i, vc);

    //add
    CUaddition(vd, vc);
  }

  //move result
  memcpy(va, vd, sizeof(BigInteger));

  //if signs are even, switch sign
  if (sig == 1 || sig == 10)
    ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;

  //denormalize
  if (sig == 1)
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
  else if (sig == 11)
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;

  //final recount
  CUrecount(va);
}

/*
 * CUpAdd
 *
 * Performs addition operation, having in count the number signs.
 * If signs are not the same, it performs subtract.
 *
 * CUDA version
 */
__device__ void CUpAdd(void* va, void* vb) {
  //get signum
  int sig = CUsignum(((BigInteger*)va)->n[((BigInteger*)va)->count],
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count]);

  //normalize
  if (sig == 10)
    ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;
  else if (sig == 1)
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
  else if (sig == 11) {
    ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
  }

  //if both signs are the same, add, else, subtract
  if (sig == 0 || sig == 11)
    CUaddition(va, vb);
  else
    CUsubtract(va, vb);

  if (sig == 10 || sig == 11)
    //switch back the sign
    ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;

  //denormalize
  if (sig == 1)
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
  else if (sig == 11)
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
}

/*
 * CUpSub.
 *
 * Performs subtraction operation, having in count the number signs.
 * If signs are not the same, it performs addition.
 *
 * CUDA version
 */
__device__ void CUpSub(void* va, void* vb) {
  int sig;

  //get signum
  sig = CUsignum(((BigInteger*)va)->n[((BigInteger*)va)->count],
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count]);

  //normalize
  if (sig == 1)
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
  else if (sig == 10)
    ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;
  else if (sig == 11) {
    ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
  }

  //if both signs are the same, subtract, else, add
  if (sig == 0 || sig == 11)
    CUsubtract(va, vb);
  else
    CUaddition(va, vb);

  if (sig == 10 || sig == 11)
    //switch back the sign
    ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;

  //denormalize
  if (sig == 1)
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
  else if (sig == 11)
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;

}

/*
 * CUsubrtact.
 *
 * Performs a -= b
 *
 * CUDA version
 */
__device__ static void CUsubtract(void* va, void* vb) {
  int i = 0;

  //subtract common part
  for (; i <= ((BigInteger*)vb)->count; i++)
    ((BigInteger*)va)->n[i] -= ((BigInteger*)vb)->n[i];

  //if last digit is negartive
  if (((BigInteger*)va)->n[((BigInteger*)va)->count] < 0)
    CUcarrySub(va, 1);
  else
    CUcarrySub(va, 0);
}

/*
 * CUcarrySub.
 *
 * Manages subtraction carry. If carryType = 0, the carry
 * manages as a += 10; else, it invert the sign (except last digit)
 *
 * CUDA version
 */
__device__ static void CUcarrySub(void* va, int carryType) {
  int i = 0;
  int acc = 0;

  if (carryType == 0) {
    for (; i <= ((BigInteger*)va)->count; i++) {
      //subtract carry to number
      ((BigInteger*)va)->n[i] -= acc;

      if (((BigInteger*)va)->n[i] < 0) {
        //normalize
        ((BigInteger*)va)->n[i] += 10;
        acc = 1;
      }
      else
        acc = 0;
    }
  } else {
    for (i = 0; i < ((BigInteger*)va)->count; i++)
      if (((BigInteger*)va)->n[i] < 0)
        //normalize
        ((BigInteger*)va)->n[i] = ((BigInteger*)va)->n[i] * -1;
  }

  //recount digits
  CUrecount(va);
}

/*
 * CUrecount.
 *
 * Count the digits, to check if count has to be diminished.
 *
 * CUDA version
 */
__device__ static void CUrecount(void* va) {
  while (((BigInteger*)va)->n[((BigInteger*)va)->count--] == 0);

  ++((BigInteger*)va)->count;

  if (((BigInteger*)va)->count < 0)
    ((BigInteger*)va)->count = 0;
}

/*
 * CUaddition.
 *
 * performs a += b
 *
 * CUDA version
 */
__device__ static void CUaddition(void* va, void* vb) {
  int limit;
  int min;
  int swap;
  int move;
  int i;

  //assume "a" has the bigger length
  limit = ((BigInteger*)va)->count;

  //assume "b" has the shortest length
  min = ((BigInteger*)vb)->count;

  //carry indicator
  move = 0;
  i = 0;

  //if assumption is wrong, rectify
  if (((BigInteger*)vb)->count > limit) {
    //swap limit and min
    swap = limit;
    limit = min;
    min = swap;

    move = 1;
  }

  //add all common digits
  for (; i <= min; i++)
    ((BigInteger*)va)->n[i] += ((BigInteger*)vb)->n[i];

  //move the even digits
  if (move == 1) {
    for (; i <= limit; i++)
      ((BigInteger*)va)->n[i] = ((BigInteger*)vb)->n[i];

    ((BigInteger*)va)->count = limit;
  }

  //carry management
  CUcarryAdd(va, 1, min);
}

/*
 * CUpMul.
 *
 * Ponderates vpart a pos number of positions, leaving pos 0's on the right side
 * That's a fancy way to say it performs vpart = vpart * 10^pos
 *
 * CUDA version
 */
__device__ static void CUpMul(int pos, void* vpart) {
  int i;

  i = ((BigInteger*)vpart)->count + pos;

  //generate offset
  for (; i >= pos; i--)
    ((BigInteger*)vpart)->n[i] = ((BigInteger*)vpart)->n[i - pos];

  //normalize offset positions
  for (i = 0; i < pos; i++)
    ((BigInteger*)vpart)->n[i] = 0;

  ((BigInteger*)vpart)->count += pos;
}

/*
 * CUcarryAdd.
 *
 * Manages addition carry.
 *
 * CUDA version
 */
__device__ void CUcarryAdd(void* va, int move, int min) {
  int i = 0;
  int acc;
  int limit;

  acc = 0;

  //move == 1 --> we know there's a non-common part. Min will be the common part threshold
  if (move == 1)
    limit = min;
  else
    limit = ((BigInteger*)va)->count;

  //iterate and manage carry on the common part
  for (; i <= limit; i++) {
    //add carry
    ((BigInteger*)va)->n[i] += acc;

    //as acc is int, we can divide by 10 and get the carry
    acc = ((BigInteger*)va)->n[i] / 10;

    if (acc > 0)
      //normalize
      ((BigInteger*)va)->n[i] = ((BigInteger*)va)->n[i] % 10;
  }

  if (move == 1) {
    //there's non-common part. Carry unitl acc = 0, as the non-common part is always normalized.
    while (acc > 0 && i <= ((BigInteger*)va)->count) {
      //add carry
      ((BigInteger*)va)->n[i] += acc;

      //as acc is int, we can divide by 10 and get the carry
      acc = ((BigInteger*)va)->n[i] / 10;

      if (acc > 0)
        //normalize
        ((BigInteger*)va)->n[i] = ((BigInteger*)va)->n[i] % 10;

      i++;
    }
  }

  //if there's a carry left, we move it to the end
  if (acc > 0)
    ((BigInteger*)va)->n[++(((BigInteger*)va)->count)] = acc;
}

/*
 * CUhardEquals.
 *
 * Compares two numbers.
 *   Return 0 if a = b
 *   Return 1 if a > b
 *   Return 2 if a < b
 *
 * CUDA version
 */
__device__ void CUhardEquals(void* va, void* vb, int* ret) {
  int i;
  int sig;

  //if pointer value is the same, they share value
  if (va == vb) {
    *ret = 0;
    return;
  }

  //get signum
  sig = CUsignum(((BigInteger*)va)->n[((BigInteger*)va)->count],
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count]);

  if (sig == 1)
    //a >= 0, b < 0, so a > b
    *ret = 1;
  else if (sig == 10)
    //a < 0, b >= 0, so a < b
    *ret = 2;
  else {
    //they share signum. Manual comparation
    *ret = 0;

    if (((BigInteger*)va)->count < ((BigInteger*)vb)->count)
      //count(a) < count(b)
      *ret = 2;
    else if (((BigInteger*)va)->count > ((BigInteger*)vb)->count)
      //count(a) > count(b)
      *ret = 1;
    else {
      //count(a) = count(b). Manual compare
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
      //both have negative sign. Switch return
      if (*ret == 2)
        *ret = 1;
      else if (*ret == 1)
        *ret = 2;
    }
  }
}

/*
 * CUsignum.
 *
 * Returns the quantity of negative data that are on an operation
 *   0 : None
 *   1 : b is negative
 *   10: a is negative
 *   11: a and b are negative
 *
 * CUDA version
 */
__device__ static int CUsignum(int a, int b) {
  int ret = 0;

  if (a < 0)
    ret = 10;

  if (b < 0)
    ++ret;

  return ret;
}

/*
 * CUsDvs.
 *
 * Performs operation a /= b
 *
 * CUDA version
 */
__device__ void CUsDvs(void* va, void* vb, void* xa, void* xb) {
  int sig;
  int comp;

  //initalize temp vars (xa, xb)
  CUnewBI(xa, "0", 0);
  CUnewBI(xb, "1", 0);


  //signum
  sig = CUsignum(((BigInteger*)va)->n[((BigInteger*)va)->count],
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count]);

  //normalize
  if (sig == 1)
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
  else if (sig == 10)
    ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;
  else if (sig == 11) {
    ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
  }

  CUhardEquals(va, vb, &comp);

  if (comp == 0) {
    //if a = b, a / b = 1
    CUhardEquals(va, xa, &comp);

    if (comp == 0)
      //if a = 0, then b = 0 (as a = b), then a / b = 0
      memcpy(va, xa, sizeof(BigInteger));
    else
      //otherwise, as a = b, a / b = 1
      memcpy(va, xb, sizeof(BigInteger));
  } else if (comp == 2) {
    //if a < b, then a / b = 0 (as we're on integer)
    memcpy(va, xa, sizeof(BigInteger));
  } else if (comp == 1) {
    //if a > b, then a / b = n
    CUhardEquals(vb, xb, &comp);

    if (comp != 0)
      //only search n if b != 1
      CUdivide(va, vb, xa, xb);
  }

  //if sign are even, we switch the sign
  if (sig == 1 || sig == 10)
    ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;

  //denormalize
  if (sig == 1)
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
  else if (sig == 11)
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
}

/*
 * CUdivide.
 *
 * Perform division via Bolzano
 *
 * CUDA version
 */
__device__ static void CUdivide(void* va, void* vb, void* xa, void* xb) {
  int len;
  int i = 0;
  int x = 0;
  int res = 0;
  int eq;

  len = ((BigInteger*)va)->count - ((BigInteger*)vb)->count;

  //initalize temp data (xa, xb)
  CUclean(xa);
  CUclean(xb);

  /*
   * Keep "b.count" first digits. If "b" has a single digit we don't move
   * anything, because we later will move a digit.
   */
  if (((BigInteger*)vb)->count > 0)
    for (; i < ((BigInteger*)vb)->count; i++)
      ((BigInteger*)xb)->n[((BigInteger*)vb)->count - i - 1] = ((BigInteger*)va)->n[((BigInteger*)va)->count - i];
  else
    ((BigInteger*)xb)->n[0] = 0;

  ((BigInteger*)xb)->count = ((BigInteger*)vb)->count - 1;

  if (((BigInteger*)xb)->count == -1)
    ++((BigInteger*)xb)->count;

  //for each digit we generated
  for (i = 0; i <= len; i++) {
    //make a temporal BI
    CUpAppend(xb, ((BigInteger*)va)->n[len - i]);

    /*
     * if b.len == 0, there's a bug on which the result of append has {n0} form with len = 1
     * when it should have {n} form with len = 0 (it only happens during first iteration)
     */
    if (((BigInteger*)xb)->count == 1 && ((BigInteger*)xb)->n[1] == 0)
      --((BigInteger*)xb)->count;

    for (x = 0; x < 10; x++) {
      if (((BigInteger*)xb)->n[((BigInteger*)xb)->count] == 0 && ((BigInteger*)xb)->count == 0) {
        //the result is 0
        res = x;
        x = 99;
      } else if (((BigInteger*)xb)->n[((BigInteger*)xb)->count] < 0) {
        //value is negative, we got the threshold
        CUaddition(xb, vb);
        res = (x - 1);
        x = 99;
      } else {
        //any other scenario. Check it
        CUhardEquals(xb, vb, &eq);

        if (eq == 2) {
          //xb < vb --> found
          res = x;
          x = 99;
        } else {
          //xb >= vb
          CUsubtract(xb, vb);
        }
      }
    }

    //move value
    ((BigInteger*)xa)->n[len - i] = res;
  }

  //move length
  ((BigInteger*)xa)->count = len;

  //copy result
  memcpy(va, (BigInteger*)xa, sizeof(BigInteger));

  //recount
  CUrecount(va);

  //avoid fake overflow error
  if (((BigInteger*)va)->n[((BigInteger*)va)->count + 1] > 0)
    ++((BigInteger*)va)->count;
}

/*
 * CUpAppend.
 *
 * Adds a digit at the end of the BigInteger
 *
 * CUDA version
 */
__device__ void CUpAppend(void* va, int b) {
  int i;

  for (i = ((BigInteger*)va)->count; i >= 0; i--)
    ((BigInteger*)va)->n[i + 1] = ((BigInteger*)va)->n[i];

  ((BigInteger*)va)->n[0] = b;
  ++((BigInteger*)va)->count;
}

/*
 * CUsub. Use it to subtract two numbers.
 *
 * CUDA version
 */
__device__ void CUsub(void* va, void* vb, void* vc) {
  int comp;
  int sig;

  CUhardEquals(va, vb, &comp);
  sig = CUsignum(((BigInteger*)va)->n[((BigInteger*)va)->count],
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count]);

  //if |va| < |vb|
  if ((comp == 2 && sig < 11) || (comp == 1 && sig == 11)) {
    //vc = vb
    memcpy(vc, vb, sizeof(BigInteger));

    //vc -= va
    CUpSub(vc, va);

    //switch sign
    ((BigInteger*)vc)->n[((BigInteger*)vc)->count] *= -1;

    //move result
    memcpy(va, vc, sizeof(BigInteger));
  } else {
    //delegate on standard function
    CUpSub(va, vb);
  }
}

/*
 * CUsqrt. Use it to get the square root of a number.
 *
 * CUDA version
 */
__device__ void CUsqrt(void* va, void* vb, void* vc, void* vd) {
  char even = ((BigInteger*)va)->count % 2;
  int digits = ((BigInteger*)va)->count + 1;
  char fstep;
  int x = 0;
  int y = 0;
  int d = 0;
  int eq;
  char z;

  CUclean(vb);
  CUclean(vc);
  CUclean(vd);

  //create the first remainder
  if (even == 1) {
    //if even = 1 means that count is even, so there's a odd number of digits
    fstep = ((BigInteger*)va)->n[((BigInteger*)va)->count] * 10 + ((BigInteger*)va)->n[((BigInteger*)va)->count - 1];
    d = ((BigInteger*)va)->count - 2;
  } else {
    //else, there's an even number of digits
    fstep = ((BigInteger*)va)->n[((BigInteger*)va)->count];
    d = ((BigInteger*)va)->count - 1;
  }

  //get the root of fstep
  while (x * x++ <= fstep);
  if (--x * x > fstep)
    --x;

  //"x" will be the first digit of the root
  ((BigInteger*)vb)->n[((BigInteger*)vb)->count] = x;

  //save remainder on vc. It will always happens that fstep >= x
  ((BigInteger*)vc)->n[((BigInteger*)vc)->count] = fstep - (x * x);

  //recount
  CUrecount(vc);

  //use "digits" as counter
  if (digits == 1 || digits == 2) {
    //special case, for 1 and 2 digits, the result will have 1 digit only
    digits = 0;
  } else {
    digits = (digits / 2 + digits % 2) - 1;
  }

  //using "x" as temp counter, adding 1 as we already have the first digit
  x = 1;

  //while there's digits to move
  while (x <= digits) {
    //get the next two digits
    //first, we make an offset on vc
    y = ((BigInteger*)vc)->count + 2;

    for (; y >= 2; y--)
      ((BigInteger*)vc)->n[y] = ((BigInteger*)vc)->n[y - 2];

    //then we move correlative positions from va to vc
    ((BigInteger*)vc)->n[1] = ((BigInteger*)va)->n[d--];
    ((BigInteger*)vc)->n[0] = ((BigInteger*)va)->n[d--];

    //set vc length up to two
    ((BigInteger*)vc)->count += 2;

    //manage carry
    CUcarryAdd(vc, 0, 0);

    //copy current result (vb) to vd
    memcpy(vd, vb, sizeof(BigInteger));

    //calculate vb += vb. Doesn't matter the order here
    for (y = 0; y <= ((BigInteger*)vd)->count; y++)
      ((BigInteger*)vd)->n[y] += ((BigInteger*)vd)->n[y];

    //carry
    CUcarryAdd(vd, 0, 0);

    //find a number n = [1, 9] that satisfies 10vd + n^2 = vc
    //first, we create the value for n = 1
    y = ((BigInteger*)vd)->count + 1;

    for (; y >= 1; y--)
      ((BigInteger*)vd)->n[y] = ((BigInteger*)vd)->n[y - 1];

    ((BigInteger*)vd)->n[0] = 1;
    ++((BigInteger*)vd)->count;

    //init z, that will hold values for n
    z = 1;

    //assume we don't get to vb
    eq = 2;

    //searching z
    while (eq == 2) {
      //compare vd vs vb
      CUhardEquals(vd, vc, &eq);

      if (eq == 1) {
        //vd > vb. We need the previous digit
        //restore previous state
        if (z == 1) {
          //if z = 1, we're on the first iteration, so previous value will be 0
          CUnewBI(vd, "0", 0);
          --z;
        } else {
          //get z back
          --z;
        }
      } else if (eq == 2) {
        //vd < vb. The value we're searching is still not this
        //increase vd and z. Increase is 10vd + 2n + 1
        //get first part for vd

        //(vd * 20)
        for (y = 0; y <= ((BigInteger*)vb)->count; y++)
          ((BigInteger*)vd)->n[y + 1] += (((BigInteger*)vb)->n[y] * 2);

        //carry
        CUcarryAdd(vd, 0, 0);

        //+2n + 1
        ((BigInteger*)vd)->n[0] += (2 * z) + 1;

        //carry again
        CUcarryAdd(vd, 0, 0);

        ++z;
      }

      //if we iterated 10 times, value will be 9
      if (z == 10) {
        //back to 9
        --z;

        //force quit
        eq = 1;
      }
    }

    //restore previous value
    if (eq == 1) {
      //if we went too far, we get back to previous value
      //copy current result (vb) to vd
      memcpy(vd, vb, sizeof(BigInteger));

      //get vb += vb
      for (y = 0; y <= ((BigInteger*)vd)->count; y++)
        ((BigInteger*)vd)->n[y] += ((BigInteger*)vd)->n[y];

      //carry
      CUcarryAdd(vd, 0, 0);

      //create the rest of the value
      y = ((BigInteger*)vd)->count + 1;

      //offset
      for (; y >= 1; y--)
        ((BigInteger*)vd)->n[y] = ((BigInteger*)vd)->n[y - 1];

      //move z value
      ((BigInteger*)vd)->n[0] = z;
      ++((BigInteger*)vd)->count;

      //multiply by z
      for (y = 0; y <= ((BigInteger*)vd)->count; y++)
        ((BigInteger*)vd)->n[y] *= z;

      //carry
      CUcarryAdd(vd, 0, 0);
    }

    //move the digit
    /*
     * After the loop, we will have the digit to move on "z"; the partial result on vd; and the current remainder
     * on vc. We subtract vc -= vd (always will be vc >= vd) to update the remainder.
     */
    CUsubtract(vc, vd);

    //offseting vb
    y = ((BigInteger*)vb)->count + 1;

    for (; y >= 1; y--)
      ((BigInteger*)vb)->n[y] = ((BigInteger*)vb)->n[y - 1];

    //move z
    ((BigInteger*)vb)->n[0] = z;
    ++((BigInteger*)vb)->count;

    //next pair of digits
    x++;
  }

  //move resukt
  memcpy(va, vb, sizeof(BigInteger));
}

/*
 * CUstrlen. Mock-up for strlen as CUDA does not implement the function
 *
 * CUDA version
 */
__device__ static int CUstrlen(char* s) {
  int ret = 0;

  while (*s != '\0') {
    ++s;
    ++ret;
  }

  return ret;
}
#endif

/*
 * newBI.
 *
 * Generates a new BI from the input string (dst).
 * The load is made on reverse order to allow a simple growing mechanism.
 * If "sig" is -1, the number will be negative.
 */
void newBI(void* dst, const char* s, int sig) {
  int i = (int)strlen(s) - 1;
  int f = i;
  int j = 0;
  int c;
  int ssig = sig;

  //sign adjustment
  ((BigInteger*)dst)->k = 'i';

  //clean the array
  clean(dst);

  if (i > MAX_LENGTH + 1) {
    showError(1);
    return;
  }

  //iterate over the string and save data as integers
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

  //if a negative is sent, remove a position
  if (s[0] == '-')
    --f;

  ((BigInteger*)dst)->count = f;

  if (ssig == -1)
    ((BigInteger*)dst)->n[((BigInteger*)dst)->count] *= -1;
}

/*
 * validateBI
 *
 * It validates that all BI data are cohesive
 */
void validateBI(void* a) {
  int i = 0;

  //type validation
  if (((BigInteger*)a)->k != 'i') {
    showError(99);
    return;
  }

  //length validation
  if (((BigInteger*)a)->count < 0 || ((BigInteger*)a)->count > MAX_LENGTH) {
    showError(99);
    return;
  }

  //validate the rest of the digits, that can be positive or negative
  for (; i < MAX_LENGTH; i++) {
    if (((BigInteger*)a)->n[i] < -9 || ((BigInteger*)a)->n[i] > 9) {
      showError(99);
      return;
    }
  }
}

/*
 * getPoint
 *
 * Returns the decimal point cut for the division
 */
int getPoint() {
  return BI_point;
}

/*
 * getReturnCode
 *
 * Returns status code
 */
int getReturnCode() {
  return BIReturnCode;
}

/*
 * setReturnCode
 *
 * Sets the status code
 */
void setReturnCode(int k) {
  BIReturnCode = k;
}

/*
 * _BI_initialize
 *
 * Give value to util data
 */
#if BI_STANDALONE == 1 
static
#endif
void _BI_initialize() {
  int i = 0;

  //we create it from scratch as "clean" copies from _ZERO
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
 * BImemcpy
 *
 * Copies on dst pointer the useful data
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
    memcpy(dst, &_ZERO, sizeof(BigInteger));
}

/*
 * pAppend.
 *
 * Adds a digit at the end of the BigInteger
 */
static void pAppend(void* va, int b) {
  int i;

  for (i = ((BigInteger*)va)->count; i >= 0; i--)
    ((BigInteger*)va)->n[i + 1] = ((BigInteger*)va)->n[i];

  ((BigInteger*)va)->n[0] = b;
  ++((BigInteger*)va)->count;
}

/*
 * signum.
 *
 * Returns the quantity of negative data that are on an operation
 *   0 : None
 *   1 : b is negative
 *   10: a is negative
 *   11: a and b are negative
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
 * hardEquals.
 *
 * Compares two numbers.
 *   Return 0 if a = b
 *   Return 1 if a > b
 *   Return 2 if a < b
 */
#if BI_STANDALONE == 1 
static
#endif
void hardEquals(void* va, void* vb, int* ret) {
  int i;
  int sig;

  //if pointer value is the same, they share value
  if (va == vb) {
    *ret = 0;
    return;
  }

  //get signum
  sig = signum(((BigInteger*)va)->n[((BigInteger*)va)->count],
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count]);

  if (sig == 1)
    //a >= 0, b < 0, so a > b
    *ret = 1;
  else if (sig == 10)
    //a < 0, b >= 0, so a < b
    *ret = 2;
  else {
    //they share signum. Manual comparation
    *ret = 0;

    if (((BigInteger*)va)->count < ((BigInteger*)vb)->count)
      //count(a) < count(b)
      *ret = 2;
    else if (((BigInteger*)va)->count > ((BigInteger*)vb)->count)
      //count(a) > count(b)
      *ret = 1;
    else {
      //count(a) = count(b). Manual compare
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
      //both have negative sign. Switch return
      if (*ret == 2)
        *ret = 1;
      else if (*ret == 1)
        *ret = 2;
    }
  }
}

/*
 * pAdd
 *
 * Performs addition operation, having in count the number signs.
 * If signs are not the same, it performs subtract.
 */
#if BI_STANDALONE == 1
static
#endif
 void pAdd(void* va, void* vb, void* m) {
  if (((memory*)m)->vt == NULL) {
    showError(11);
    return;
  }

  if (va == vb) {
    //add(a, a); delegate to mul(a, 2)
    sMul(va, ((memory*)m)->vt, m);
  } else {
    //add(a, b)
    int sig = signum(((BigInteger*)va)->n[((BigInteger*)va)->count],
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count]);

    //normalize operators
    if (sig == 10)
      //a negative, b positive. Change "a" sign and perform addition
      ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;
    else if (sig == 1)
      //b negative, a positive. Change "b" sign and perform addition
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
    else if (sig == 11) {
      //a negative, b negative. Change signs and perform addition
      ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
    }

    //if both signs are the same we add, else we subtract
    if (sig == 0 || sig == 11)
      addition(va, vb);
    else
      //can't call subtraction as pSub does essential validations towards subtraction (i.e length of a vs b). #stackloop
      pSub(va, vb, m);

    if (sig == 10 || sig == 11)
      //switch back the sign
      ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;

    //denormalize b
    if (sig == 1)
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
    else if (sig == 11)
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
  }
}

/*
 * addition.
 *
 * performs a += b
 */
static void addition(void* va, void* vb) {
  int limit;
  int min;
  int swap;
  int move;
  int i;

  //assume "a" has the bigger lenght 
  limit = ((BigInteger*)va)->count;

  //assume "b" has the shortest length
  min = ((BigInteger*)vb)->count;

  //carry indicator 
  move = 0;
  i = 0;

  //if the assumption is wrong, rectify
  if (((BigInteger*)vb)->count > limit) {
    //switch limit and min
    swap = limit;
    limit = min;
    min = swap;

    move = 1;
  }

  //add all the common digits
  for (; i <= min; i++)
    ((BigInteger*)va)->n[i] += ((BigInteger*)vb)->n[i];

  //move the even digits
  if (move == 1) {
    for (; i <= limit; i++)
      ((BigInteger*)va)->n[i] = ((BigInteger*)vb)->n[i];

    ((BigInteger*)va)->count = limit;
  }

  //carry management
  carryAdd(va, 1, min);
}

/*
 * carryAdd.
 *
 * Manages addition carry.
 */
#if BI_STANDALONE == 1
static
#endif
 void carryAdd(void* va, int move, int min) {
  int i = 0;
  int acc;
  int limit;

  acc = 0;

  //move == 1 --> we know there's a non-common part. Min will be the common part threshold
  if (move == 1)
    limit = min;
  else
    limit = ((BigInteger*)va)->count;

  //iterate and manage carry on the common part
  for (; i <= limit; i++) {
    //add carry
    ((BigInteger*)va)->n[i] += acc;

    //as acc is int, we can divide by 10 and get the carry
    acc = ((BigInteger*)va)->n[i] / 10;

    if (acc > 0)
      //normalize the number
      ((BigInteger*)va)->n[i] = ((BigInteger*)va)->n[i] % 10;
  }

  if (move == 1) {
    //there's non-common part. Carry unitl acc = 0, as the non-common part is always normalized.
    while (acc > 0 && i <= ((BigInteger*)va)->count) {
      //add carry
      ((BigInteger*)va)->n[i] += acc;

      //as acc is int, we can divide by 10 and get the carry
      acc = ((BigInteger*)va)->n[i] / 10;

      if (acc > 0)
        //normalize the number
        ((BigInteger*)va)->n[i] = ((BigInteger*)va)->n[i] % 10;

      i++;
    }
  }

  //if there's a carry left, we move it to the end
  if (acc > 0) {
    if (((BigInteger*)va)->count == MAX_LENGTH) {
      showError(1);
      return;
    } else
      ((BigInteger*)va)->n[++(((BigInteger*)va)->count)] = acc;
  }
}

/*
 * pSub.
 *
 * Performs subtraction operation, having in count the number signs.
 * If signs are not the same, it performs addition.
 */
#if BI_STANDALONE == 1
static
#endif
 void pSub(void* va, void* vb, void* m) {
  int sig;
  int comp;
  int sp = 0;

  if (((memory*)m)->stmp == NULL) {
    showError(10);
    return;
  }

  if (va == vb) {
    //sub(a, a); result = 0
    BImemcpy(va, 0);
  } else {
    //sub(a, b);
    hardEquals(va, vb, &comp);

    sig = signum(((BigInteger*)va)->n[((BigInteger*)va)->count],
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count]);

    //if both are negative and comp = 1 means that a < b
    if ((comp == 2 && sig < 11) || (comp == 1 && sig == 11)) {
      sp = 1;

      memcpy(((memory*)m)->stmp, vb, sizeof(BigInteger)); //stmp = b

      //switch va and vb
      memcpy(vb, va, sizeof(BigInteger)); //vb = va
      memcpy(va, ((memory*)m)->stmp, sizeof(BigInteger)); //va = tmp

      //recalculate signum
      sig = signum(((BigInteger*)va)->n[((BigInteger*)va)->count],
        ((BigInteger*)vb)->n[((BigInteger*)vb)->count]);
    } else if (comp == 0)
      BImemcpy(va, 0);
    
    //normalize sign
    if (sig == 1)
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
    else if (sig == 10)
      ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;
    else if (sig == 11) {
      ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
    }

    //if they have same sign we subtract; else we add
    if (sig == 0 || sig == 11)
      subtract(va, vb);
    else
      addition(va, vb);

    if (sig == 10 || sig == 11)
      //on this cases, we switch sign
      ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;

    //denormalize b
    if (sig == 1)
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
    else if (sig == 11)
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;

    if (sp == 1) {
      //special things to be done
      //switch sign
      ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;

      //retrieve vb original value
      memcpy(vb, ((memory*)m)->stmp, sizeof(BigInteger));
    }
  }
}

/*
 * subrtact.
 *
 * Performs a -= b
 */
static void subtract(void* va, void* vb) {
  int i = 0;

  //subtract common digits
  for (; i <= ((BigInteger*)vb)->count; i++)
    ((BigInteger*)va)->n[i] -= ((BigInteger*)vb)->n[i];

  //if last digit is negative
  if (((BigInteger*)va)->n[((BigInteger*)va)->count] < 0)
    carrySub(va, 1);
  else
    carrySub(va, 0);
}

/*
 * carrySub.
 *
 * Manages subtraction carry. If carryType = 0, the carry
 * manages as a += 10; else, it invert the sign (except last digit)
 */
static void carrySub(void* va, int carryType) {
  int i = 0;
  int acc = 0;

  if (carryType == 0) {
    for (; i <= ((BigInteger*)va)->count; i++) {
      //subtract carry to number
      ((BigInteger*)va)->n[i] -= acc;

      if (((BigInteger*)va)->n[i] < 0) {
        //normalize the number
        ((BigInteger*)va)->n[i] += 10;
        acc = 1;
      } else
        acc = 0;
    }
  } else {
    for (i = 0; i < ((BigInteger*)va)->count; i++)
      if (((BigInteger*)va)->n[i] < 0)
        //normalize the number
        ((BigInteger*)va)->n[i] = ((BigInteger*)va)->n[i] * -1;
  }

  //count the digits again
  recount(va);
}

/*
 * recount.
 *
 * Count the digits, to check if count has to be diminished.
 */
static void recount(void* va) {
  while (((BigInteger*)va)->n[((BigInteger*)va)->count--] == 0);

  ++((BigInteger*)va)->count;

  if (((BigInteger*)va)->count < 0)
    ((BigInteger*)va)->count = 0;
}

/*
 * sMul.
 *
 * Simulates a *= b
 */
#if BI_STANDALONE == 1
static
#endif
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
    //we copy it to tmp to work without collapsing the data
    memcpy(((memory*)m)->mtmp, va, sizeof(BigInteger));

    sMul(va, ((memory*)m)->mtmp, m);
  } else {
    //mul(a, b)
    BImemcpy(((memory*)m)->mzero, 0);
    BImemcpy(((memory*)m)->mret, 0);
    BImemcpy(((memory*)m)->mone, 1);

    //if a = 0, no need to calculate
    hardEquals(va, ((memory*)m)->mzero, &comp);

    if (comp == 0)
      calc = 1;

    //if a = 0, no need to calculate
    hardEquals(vb, ((memory*)m)->mzero, &comp);

    if (comp == 0) {
      memcpy(va, ((memory*)m)->mzero, sizeof(BigInteger));
      calc = 1;
    }

    //normalize numbers
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

    //if |a| = 1, then a * b = b (sign will be later normalized)
    hardEquals(va, ((memory*)m)->mone, &comp);

    if (comp == 0) {
      memcpy(va, vb, sizeof(BigInteger));
      calc = 1;
    }

    //if |b| = 1, then a * b = a (sign will be later normalized)
    hardEquals(vb, ((memory*)m)->mone, &comp);

    if (comp == 0)
      calc = 1;

    //initialize BIT
    memcpy(&((BIT*)((memory*)m)->biBIT)->BI[0], ((memory*)m)->mzero, sizeof(BigInteger));
    memcpy(&((BIT*)((memory*)m)->biBIT)->BI[1], va, sizeof(BigInteger));

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

    //if the number is not calculated (a = 0, b = 0, |a| = 1, |b| = 1), we calculate it
    if (calc == 0) {
      //perform partial product for each digit of b
      for (i = 0; i <= ((BigInteger*)vb)->count; i++) {
        //validate if BIT[n] exists
        if (((BIT*)((memory*)m)->biBIT)->status[((BigInteger*)vb)->n[i]] == 0) {
          clean(((memory*)m)->mpart);

          //we don't have the BIT loaded, so we calculate it
          for (x = 0; x <= ((BigInteger*)va)->count; x++)
            ((BigInteger*)((memory*)m)->mpart)->n[x] = ((BigInteger*)va)->n[x] * ((BigInteger*)vb)->n[i];

          ((BigInteger*)((memory*)m)->mpart)->count = x - 1;
          carryAdd(((memory*)m)->mpart, 0, 0);

          //move the value to corresponding BIT
          memcpy(&((BIT*)((memory*)m)->biBIT)->BI[((BigInteger*)vb)->n[i]], ((memory*)m)->mpart, sizeof(BigInteger));
          ((BIT*)((memory*)m)->biBIT)->status[((BigInteger*)vb)->n[i]] = 1;
        } else
          //we have a loaded BIT, so we copy it
          memcpy(((memory*)m)->mpart, &((BIT*)((memory*)m)->biBIT)->BI[((BigInteger*)vb)->n[i]], sizeof(BigInteger));

        //ponderate the result with "i" 0's
        pMul(i, ((memory*)m)->mpart);

        //add it
        addition(((memory*)m)->mret, ((memory*)m)->mpart);

        //init again
        BImemcpy(((memory*)m)->mpart, 0);
      }

      //move the result
      memcpy(va, ((memory*)m)->mret, sizeof(BigInteger));
    }

    //if signs are even, we switch it
    if (sig == 1 || sig == 10)
      ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;

    //denormalize vb
    if (sig == 1)
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
    else if (sig == 11)
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
  }
}

/*
 * pMul.
 *
 * Ponderates vpart a pos number of positions, leaving pos 0's on the right side
 * That's a fancy way to say it performs vpart = vpart * 10^pos
 */
static void pMul(int pos, void* vpart) {
  int i;

  i = ((BigInteger*)vpart)->count + pos;

  if (i >= MAX_LENGTH) {
    showError(1);
    return;
  }

  //generate offset
  for (; i >= pos; i--)
    ((BigInteger*)vpart)->n[i] = ((BigInteger*)vpart)->n[i - pos];

  //normalize offset positions
  for (i = 0; i < pos; i++)
    ((BigInteger*)vpart)->n[i] = 0;

  ((BigInteger*)vpart)->count += pos;
}

/*
 * sDvs.
 *
 * Performs operation a /= b
 */
#if BI_STANDALONE == 1
static
#endif
 void sDvs(void* va, void* vb, void* m) {
  int sig;
  int comp;

  if (((memory*)m)->dtmp == NULL || ((memory*)m)->done == NULL) {
    showError(4);
    return;
  }

  //init decimal point
  BI_point = -1;

  if (va == vb) {
    //dvs(a, a)
    BImemcpy(va, 1);
  } else {
    //dvs(a, b)

    BImemcpy(((memory*)m)->dtmp, 0);
    BImemcpy(((memory*)m)->done, 1);

    sig = signum(((BigInteger*)va)->n[((BigInteger*)va)->count],
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count]);

    //normalize numbers
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
      //if a = b, a / b = 1
      hardEquals(va, ((memory*)m)->dtmp, &comp);

      //with this we indicate there's no decimal point
      BI_point = -2;

      if (comp == 0)
        //if a = 0, then b = 0 (as a = b), then a / b = 0
        memcpy(va, ((memory*)m)->dtmp, sizeof(BigInteger));
      else
        //otherwise, as a = b, a / b = 1
        memcpy(va, ((memory*)m)->done, sizeof(BigInteger));
    } else if (comp == 2)
      //if a < b, then a / b = 0 (as we're on integer)
      memcpy(va, ((memory*)m)->dtmp, sizeof(BigInteger));
    else if (comp == 1) {
      //if a > b, then a / b = n
      hardEquals(vb, ((memory*)m)->done, &comp);

      if (comp != 0)
        //only search n if b != 1
        divide(va, vb, m);
    }

    //if sign are even, we switch the sign
    if (sig == 1 || sig == 10)
      ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;

    //denormalize vb
    if (sig == 1)
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
    else if (sig == 11)
      ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
  }
}

/*
 * divide.
 *
 * Perform division via Bolzano
 */
static void divide(void* va, void* vb, void* m) {
  int len;
  int i = 0;
  int x = 0;
  int eq;
  int currentBIT;
  int res = 0;
  int added;
  int dlen;

  if (((memory*)m)->biBIT == NULL || ((memory*)m)->dTemp == NULL || ((memory*)m)->dret == NULL ||
    ((memory*)m)->biTemp == NULL) {
    showError(5);
    return;
  }

  BImemcpy(((memory*)m)->dret, 0);

  len = ((BigInteger*)va)->count - ((BigInteger*)vb)->count;

  //init BIT
  BImemcpy(&((BIT*)((memory*)m)->biBIT)->BI[0], 0);
  memcpy(&((BIT*)((memory*)m)->biBIT)->BI[1], vb, sizeof(BigInteger));

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

  //init dTemp
  BImemcpy(((memory*)m)->dTemp, 0);

  /*
   * Keep "b.count" first digits. If "b" has a single digit we don't move
   * anything, because we later will move a digit.
   */
  if (((BigInteger*)vb)->count > 0)
    for (; i < ((BigInteger*)vb)->count; i++)
      ((BigInteger*)((memory*)m)->dTemp)->n[((BigInteger*)vb)->count - i - 1] = ((BigInteger*)va)->n[((BigInteger*)va)->count - i];
  else
    ((BigInteger*)((memory*)m)->dTemp)->n[0] = 0;

  ((BigInteger*)((memory*)m)->dTemp)->count = ((BigInteger*)vb)->count - 1;

  if (((BigInteger*)((memory*)m)->dTemp)->count == -1)
    ++((BigInteger*)((memory*)m)->dTemp)->count;

  //keep dlen for double calculation
  dlen = len;

  //init decimal point
  BI_point = 0;

  if (((BigInteger*)va)->k == 'd') {
    if (len == 0)
      BI_point = len;
    else
      BI_point = len - 1;

    len = MAX_LENGTH - 1;

    if (((BigInteger*)vb)->count == 0)
      len = MAX_LENGTH - 2;
    else
      len = MAX_LENGTH - 1;
  }

  //for each digit we generated
  for (i = 0; i <= len; i++) {
    //make a temporal BI
    if (i <= dlen)
      //there are still digits
      pAppend(((memory*)m)->dTemp, ((BigInteger*)va)->n[dlen - i]);
    else
      //no more digits, just carry a 0
      pAppend(((memory*)m)->dTemp, 0);

    /*
     * if b.len == 0, there's a bug on which the result of append has {n0} form with len = 1
     * when it should have {n} form with len = 0 (it only happens during first iteration)
     */
    if (((BigInteger*)((memory*)m)->dTemp)->count == 1 && ((BigInteger*)((memory*)m)->dTemp)->n[1] == 0)
      --((BigInteger*)((memory*)m)->dTemp)->count;

    hardEquals(((memory*)m)->dTemp, &((BIT*)((memory*)m)->biBIT)->BI[currentBIT], &eq);

    x = currentBIT;

    if (eq == 1) {
      //if dTemp > BIT[x], we start from that value and keep going

      //retrieve the last BI
      memcpy(((memory*)m)->biTemp, &((BIT*)((memory*)m)->biBIT)->BI[x], sizeof(BigInteger));
      added = 0;

      for (; x < 10; x++) {
        //add the base (vb)
        pAdd(((memory*)m)->biTemp, vb, m);

        //add it to BIT, if there's still space (we move it before validation)
        if (currentBIT < 9) {
          memcpy(&((BIT*)((memory*)m)->biBIT)->BI[++currentBIT], ((memory*)m)->biTemp, sizeof(BigInteger));
          ((BIT*)((memory*)m)->biBIT)->status[currentBIT] = 1;
          added = 1;
        } else
          added = 0;

        //check if it fits
        hardEquals(((memory*)m)->dTemp, ((memory*)m)->biTemp, &eq);

        if (eq == 0) {
          //if dTemp = temp
          res = currentBIT;
          x = 99;
        } else if (eq == 2) {
          /*
           * if dTemp < temp, as Bolzano theoreme, we already went thru dTemp = temp.
           * so, the correct value was the previous.
           *
           * If we didn't reach the end of the table (added == 1), the index will be the previous
           * If we reached the end of the table (added == 0), the index will be the current
           */
          if (added == 1)
            res = (currentBIT - 1);
          else
            res = currentBIT;

          x = 99;
        }
      }
    } else if (eq == 0)
      //if dTemp = BIT[x], we already have the value
      res = currentBIT;
    else if (eq == 2) {
      //if dTemp < BIT[x], start for that value and go thru 0
      --x;

      for (; x >= 0; x--) {
        //check if it fits
        hardEquals(((memory*)m)->dTemp, &((BIT*)((memory*)m)->biBIT)->BI[x], &eq);

        if (eq == 0) {
          //if dTemp = temp
          res = x;
          x = -99;
        } else if (eq == 1) {
          /*
           * if dtemp > temp, as Bolzano theoreme, we already went thru dTemp = temp,
           * so, correct value is the next one.
           */
          res = x;
          x = -99;
        }
      }
    }

    //subtract. Can't call subtraction as pSub does main validations. #stackloop
    pSub(((memory*)m)->dTemp, &((BIT*)((memory*)m)->biBIT)->BI[res], m);

    if (i <= dlen)
      ((BigInteger*)((memory*)m)->dret)->n[len - i] = res;
    else {
      //we're on the decimal zone. Check if it worth to add the data
      if (((BigInteger*)((memory*)m)->dTemp)->count == 0 && ((BigInteger*)((memory*)m)->dTemp)->n[0] == 0) {
        //we found a division with remainder = 0. There's no more data to treat and we finish
        i = len + 1;
      } else
        //There's still data to treat, add it
        ((BigInteger*)((memory*)m)->dret)->n[len - i] = res;
    }
  }

  ((BigInteger*)((memory*)m)->dret)->count = len;

  memcpy(va, ((memory*)m)->dret, sizeof(BigInteger));

  recount(va);

  //avoid fake overflow error
  if (((BigInteger*)va)->n[((BigInteger*)va)->count + 1] > 0)
    ++((BigInteger*)va)->count;
}

/*
 * sNqrt.
 *
 * Perform nth root of a.
 */
#if BI_STANDALONE == 1
static
#endif
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

  //if root index is 0, return 0
  if (n <= 0) {
    memcpy(va, ((memory*)m)->szero, sizeof(BigInteger));

    return;
  }

  //as default behaviour, the nth root of a number will have half the digits #ToDo: review/optimize
  lmax = ((BigInteger*)va)->count / 2;

  //creat a base
  ((BigInteger*)((memory*)m)->sbase)->n[lmax] = 1;
  ((BigInteger*)((memory*)m)->sbase)->count = lmax;

  //use Bolzano to get an approximation
  memcpy(((memory*)m)->sret, ((memory*)m)->sbase, sizeof(BigInteger));
  memcpy(((memory*)m)->sraw, ((memory*)m)->sbase, sizeof(BigInteger));

  //calculate the power
  sBipow(((memory*)m)->sret, n, m);
  hardEquals(((memory*)m)->sret, va, &isEq);

  while (isEq != 0) {
    while (isEq == 2) {
      //ret < a. Increase and try again until overflow
      addition(((memory*)m)->sraw, ((memory*)m)->sbase);

      memcpy(((memory*)m)->sret, ((memory*)m)->sraw, sizeof(BigInteger));

      sBipow(((memory*)m)->sret, n, m);
      hardEquals(((memory*)m)->sret, va, &isEq);
    }

    //once here, it will always happens that ret >= a
    if (isEq == 1) {
      //if a > ret, get back 1 base position and go adjust the previous position. #stackloop
      pSub(((memory*)m)->sraw, ((memory*)m)->sbase, m);

      ((BigInteger*)((memory*)m)->sbase)->n[lmax] = 0;
      --lmax;

      if (lmax >= 0) {
        //if lmax is greater than 0, we still have digits to pivot
        ((BigInteger*)((memory*)m)->sbase)->n[lmax] = 1;
        --((BigInteger*)((memory*)m)->sbase)->count;

        //calculate power and compare (always will be a < ret, because a = ret case won't enter this if).
        hardEquals(((memory*)m)->sbase, ((memory*)m)->szero, &isEq);

        //if base > 0, we still have to iterate
        if (isEq == 1)
          isEq = 2;
        else
          //on the opposite case, we reached maximum possible aproximation
          isEq = 0;
      } else
        //if lmax is lower than 0, we reached maximum possible approximation
        isEq = 0;
    }
  }

  memcpy(va, ((memory*)m)->sraw, sizeof(BigInteger));
}

/*
 * sBipow.
 *
 * Performs a ^= p
 */
#if BI_STANDALONE == 1
static
#endif
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
   *  Factorize the power on base-2 so we can improve performance
   *  of the operation.
   *
   *  How it works
   *   - Convert exponent on base-2, and store data on an array
   *
   *   - For each iteration, we keep calculating a^2^i, that means
   *     a^0, a^2, a^4, a^8, a^16, ...
   *
   *   - If d2b position is 1, multiply, and this way we get the result
   *
   *     Example: 8^12
   *       12 = 1100 (but keeps stored as 0011) ==> 4 + 8
   *       8^12 = 8^4 * 8^8 = 8^(4+8)
   *
   *     With this way, we only do 8 multiplications instead of 12 (on this
   *     example). On larger numbers, the difference is still greater
   */
   //decimal to binary conversion
  while (t > 0 && d2bi < 10) {
    //divide by two
    x = t / 2;

    //keep the remainder on d2b
    d2b[d2bi++] = (t - x * 2);

    //reassign value
    t = x;
  }

  //if there's a really big number, we throw an error
  if (t > 0 && d2bi >= 10) {
    showError(8);
    return;
  }

  //normalize va
  if (((BigInteger*)va)->n[((BigInteger*)va)->count] < 0) {
    sig = 1;
    ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;
  }

  //iterate over binary number
  for (; i < d2bi; i++) {
    //calculate (a^(2^i))
    if (i == 0)
      memcpy(((memory*)m)->btmp, va, sizeof(BigInteger));
    else
      sMul(((memory*)m)->btmp, ((memory*)m)->btmp, m);

    if (d2b[i] == 1)
      sMul(((memory*)m)->bres, ((memory*)m)->btmp, m);
  }

  //revert va if exponent is even
  if (sig == 1 && p % 2 == 1)
    ((BigInteger*)va)->n[((BigInteger*)va)->count] *= -1;

  memcpy(va, ((memory*)m)->bres, sizeof(BigInteger));
}

#if BI_STANDALONE == 1
/*
 * add. Use it to add two numbers.
 */
void add(void* va, void* vb, void* m) {
  //validate data before treating
  if (validate == 1) {
    validateBI(va);
    validateBI(vb);
  }

  //delegate on static function
  pAdd(va, vb, m);
}

/*
 * sub. Use it to subtract two numbers.
 */
void sub(void* va, void* vb, void* m) {
  //validate data before treating
  if (validate == 1) {
    validateBI(va);
    validateBI(vb);
  }

  //delegate on static function
  pSub(va, vb, m);
}

/*
 * mul. Use it to multiply two numbers.
 */
void mul(void* va, void* vb, void* m) {
  //validate data before treating
  if (validate == 1) {
    validateBI(va);
    validateBI(vb);
  }

  //delegate on static function
  sMul(va, vb, m);
}

/*
 * dvs. Use it to divide two numbers.
 */
void dvs(void* va, void* vb, void* m) {
  //validate data before treating
  if (validate == 1) {
    validateBI(va);
    validateBI(vb);
  }

  //init the decimal point
  BI_point = 0;

  //delegate on static function
  sDvs(va, vb, m);
}

/*
 * nqrt. Use it to get the nth root of a number.
 */
void nqrt(void* va, int n, void* m) {
  //validate data before treating
  if (validate == 1)
    validateBI(va);

  //delegate on static function
  sNqrt(va, n, m);
}

/*
 * bipow. Use it to get the "p" power of a number.
 */
void bipow(void* va, int p, void* m) {
  //validate data before treating
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

/*
 * mod. Use it to get the "b" module of a number.
 */
void mod(void* va, void* vb, void* m) {
  //validate data before treating
  if (validate == 1) {
    validateBI(va);
    validateBI(vb);
  }

  //delegate on static function
  sDvs(va, vb, m);

  //copy the remainder of the divison
  memcpy(va, ((memory*)m)->dTemp, sizeof(BigInteger));
}

/*
 * toString. Gets the string representation of a BigInteger
 */
void toString(void* vb, char* dst) {
  int i = 0;
  int m = ((BigInteger*)vb)->count;
  int sig = 0;

  //validate data before treating
  if (validate == 1)
    validateBI(vb);

  //if first digit is negative, we turn it positive and flag the character
  if (((BigInteger*)vb)->n[m] < 0) {
    ((BigInteger*)vb)->n[m] *= -1;
    dst[i++] = '-';
    sig = 1;
  }

  for (; m >= 0; m--)
    dst[i++] = (char)(((BigInteger*)vb)->n[m] + 48);

  dst[i] = '\0';

  //as we're working on the pointer, check it and restore values if needed
  if (sig == 1)
    ((BigInteger*)vb)->n[((BigInteger*)vb)->count] *= -1;
}

/*
 * equals. Compares two numbers
 *
 * Return 0 if a = b
 *        1 if a > b
 *        2 if a < b
 */
void equals(void* va, void* vb, int* ret) {
  //validate data before treating
  if (validate == 1) {
    validateBI(va);
    validateBI(vb);
  }

  //delegate on static function
  hardEquals(va, vb, ret);
}

/*
 * iniStr. Allocates memory for a BigInteger string, to be used on toString function
 */
void iniStr(char** dst) {
  *dst = (char*)malloc(sizeof(char) * MAX_LENGTH);
}

/*
 * init. Starts BigInteger engine.
 */
void init(void** m) {
  //add
  ((memory*)m)->vt = malloc(sizeof(BigInteger));

  //subtract
  ((memory*)m)->stmp = malloc(sizeof(BigInteger));

  //multiplication
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

  //root
  ((memory*)m)->sret = malloc(sizeof(BigInteger));
  ((memory*)m)->sraw = malloc(sizeof(BigInteger));
  ((memory*)m)->sbase = malloc(sizeof(BigInteger));
  ((memory*)m)->szero = malloc(sizeof(BigInteger));

  //power
  ((memory*)m)->bres = malloc(sizeof(BigInteger));
  ((memory*)m)->btmp = malloc(sizeof(BigInteger));

  //append
  ((memory*)m)->aaux = malloc(sizeof(BigInteger));

  //BIT
  ((memory*)m)->biBIT = malloc(sizeof(BIT));

  //common values
  _BI_initialize();

  BImemcpy(((memory*)m)->vt, 2);
}

/*
 * getMemorySize. Returns memory size
 */
size_t getMemorySize() {
  return sizeof(memory);
}

/*
 * clean. Cleans a BigInteger, moving 0 as value
 */
void clean(void* va) {
  BImemcpy(va, 0);
}

/*
 * showError. Show or store an error on behalf of the given code.
 * 5x values are CUDA only.
 */
#if CUDA_ENABLED == 1
__host__ __device__
#endif
 static void showError(int k) {
#if BI_SERVICE == 1
  BIReturnCode = k;
#else
#ifdef  __CUDA_ARCH__
  cBIReturnCode = k;
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
#endif /* __CUDA_ARCH__ (Else) */
#endif /* BI_SERVICE (Else) */
}
#endif /* BI_STANDALONE */
