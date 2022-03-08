# BigInteger.c
C Based model to work with Big Integers

Correctly tested working with up to 4096 digits integer and double (on the master branch).

This architecture was made during my Degree final paper (TFG, in Spain), to cover an RSA cryptoanalsys.


## BigInteger.c Changelog
 * v1.1 (beta)
   * Review variables and functions, making it "static" when convenient.
   * Changing all pointer references to "void" to avoid memory leakage.
   * Added new validations on BI creation.
   * New function "BImemcpy" to retreive useful values of BigInteger.
   * New function "validateBI" to validate the internal structure of a BI void pointer.
 * v1.11 (beta)
   * Added memory cleansing before "return" statements.
 * v1.2 (beta)
   * Added new behaviour for better performance on "pow" function.
 * v1.3 (beta)
   * Patch to avoid memory leackage.
   * Deleted "time.h" library (unnecesary).
 * v1.4 (beta)
   * Changed "BIQSRT" function for "nqrt", that makes "n-root"
   * Delete "static" from BIT (unnecesary).
   * Created wrapper function for "add" ,"sub", and "mul" for internal use.
   * Arranged header variable names to naming convention.
   * Function "signum" modified to increase performance. It now works with two int.
     * 90,566% memory usage reduction on nqrt via wrappers.
     * 53,656% performance increase on nqrt via wrappers.
     * (nqrt is used to measurement as nqrt -> pow -> mul -> pmul -> add is the longest function chain available).
 * v1.5 (beta)
   * Changes on toString function to move data to char* instead of printing.
 * v1.6 (beta)
   * Bugfix on validateBI on negative numbers.
   * Bugfix on toString on negative numbers.
 * v2.0 (Release)
   * SIT (324 diferent operations) done. Fixes done.
     * Remove validateBI from pSub.
     * Bugfix on add if b < 0 and |a| < |b|.
     * Bugfix on add if |a| < |b| and a, b < 0.
     * Bugfix on division if a / b, b < 0.
   * Code cleansing
 * v2.1 (Release)
   * Bugfix on "division" function.
 * v2.2 (Release)
   * Bugfix on "validateBI" function. Avoid "t" data leackage.
 * v2.3 (Release)
   * Naming change on "pow" function. Now it's called "bipow" to avoid internal errors.
 * v2.4 (Release)
   * Bugfix on division when len(a) - len(b) = 1.
 * v2.5 (Release)
   * Change on "nqrt" function for performance improvement. Root calculation via Bolzano.
 * v2.51 (Release)
   * Bugfix on "validateBI".
 * v3.0 (Major Release)
   * Deleted unnecesary commented lines.
   * Changed the signature of "subtraction" for "subtract(a, b)".
   * Deleted function "rev" (unnecesary).
   * Deleted "min" variable from multiplication.
   * Deleted "a = -1" and "b = -1" validation on multiplication (unnecesary).
   * Added length on "pMul" function to avoid heap overflow.
   * Change on "sMul" function. Instead of doing all the products and then all the add, now it make product-addition loop. This way, there are less memory consumption and better performance.
   * Change on "carryAdd" function. Deleted "Do/While" loop (unncesary).
   * Deleted function "makeBase" (unnecesary).
   * Deleted function "shift" (unnecesary).
   * New function iniStr. It allocate memory for "toString" function.
   * General code cleansing.
 * v3.01 (Release)
   * Deleted "Do/While" loop on "carrySub" function.
 * v3.02 (Release)
   * Optimization on "carryAdd" function. Carry is made in two times.
     * Common part: it's all carried.
     * Non-common part: it's carried unti acc = 0.
     * 3.448% performance increase tested with 10.000 additions of 100 digit numbers.
 * v3.1 (Release)
   * Function "nqrt" modified.
     * Change on "add" calls by "pAdd".
     * Change on "equals" calls by "hardEquals".
     * Change on "sub" calls by "pSub".
     * Change on "bipow" calls by "sBipow".
     * Performance increase of 38'758% based on 10.000 nqrt calls.
   * New function "sBipow", static wrapper for "bipow".
   * Bugfix on "carryAdd".
 * v3.11 (Release)
   * Deleted unused variables.
 * v4.0 (Major Release)
   * Review for CUDA codification.
     * Changes on all functionalities. We operate directly upon the pointer, saving a bunch of performance (e.g: up to 25'957% on addition operation). Also reduce RAM consumption to 50%.
     * Additional validations on "nqrt" function.
     * General code cleansing.
     * Review and removal of "showError" codes.
     * Overall, 201 code lines were removed on the optimisation.
   * Bugfix on "validateBI".
   * Changes on "dvs" to delegate on static function "sDvs".
   * Changes on "nqrt" to delegate on static funcion "sNqrt".
   * New function "free" to massive memory free.
   * Bugfix on "toString" with negative numbers.
   * Some variables are changed to static.
   * Deleted "limits.h" library.
   * Component Test performed. Changes derived of it:
     * Added default value on "BImemcpy": 0.
     * Added a "clean" call on pAppend.
     * Bugfix on "append" with negative numbers.
     * Bugfix on division (0 / 0 = 1).
     * Bugifx on 0 or below index root.
     * Bugfix on below 0 index power.
 * v4.1 (Release)
   * Bugfix on subtraction when a < b.
   * Added cast to int on strlen.
 * v4.2 (Release)
   * Bugfix on "b" when a < b.
 * v4.3 (Release)
   * Bugfix on one-digit division.
   * Bugfix on bipow when va < 0.
 * v4.4 (Release)
   * New static BI data.
   * Improvement on BI_initialize.
   * Bugfix on same-operator functions.
     * add(a, a).
     * sub(a, a).
     * mul(a, a).
     * dvs(a, a).
 * v4.5 (Release)
   * carryAdd is no longer static.
 * v4.6 (Release)
   * add new field "k" on BigInteger, as preparation for Major Release BigInteger v5.0.
 * v4.7 (Release)
   * "validateBI" changes to improve performance.
   * Deleted cast on malloc to imporve performance.
   * Bugfix on sNqrt when data lenght < 0.
   * Performance improvement on clean function.
 * v4.71 (Release)
   * Added function iniBIT to allocate BIT memory.
   * Now multiplication and division works with external BIT to increase performance.
 * v5.0 (Major Release)
   * Changed "n" type from int to char, reducing up to 75% of memory usage.
   * Use of external variables to increase performance (gaining memory usage).
   * "free" function has been removed.
   * New precompiler parameter added for setting BI length (C_MAX_LENGTH). If not present, 4096 will be set as length.
   * New precompiler parameter added for making BI validations optional (CVALIDATE). If it have a value of 1, validations will be made.
   * New precompiler parameter added or Standalone BI (BI_STANDALONE).
   * "iniBIT" function has been removed, as it has been integrated on "init" function.
   * Comment and identation revision.
   * Typedef created in order to remove "struct" references (code clarify).
   * Improvement made on "add" in order to improve performance.
   * Improvement made on "subtract". Now we call "addition" instead of "pAdd". Performance improvement.
   * Improvement made on "subtract". "CarryType" has been removed.
   * Improvement made on "mul". Now we call "addition" instead of "pAdd". Performance improvement.
   * Performance improvement made on "division" as "append" functionality has been rewritten.
      * "dtmp" variable removed.
      * "pAppend" is now an internal (static) function for "division" exclusive use.
   * Improvement made on "nqrt". Now we call "addition" instead of "pAdd". Performance improvement.
   * Performance improvement on "hardEquals" when "va" and "vb" are the same memory pointer.
   * "Clean" function is now public.
   * "BOperation" interface now created. Shall be used when BI_STANDALONE has not been declared.
     * Common functions have been refactorized.
     * _BI_Initialize not longer static. 
   * New interface BigDouble created. Now you can use Integer and Double operations
     * "nqrt" and "bipow" are still integer-only operations.
 * v5.01 (Release)
   * Bugfix on hardEquals function 
 * v5.02 (Release)
   * Improvement on "n" variable. Now it's signed char
 * v5.1 (Release)
   * Included service mode by using precompiler directive "BI_SERVICE". If "BI_SERVICE == 1", program won't stop if error found.
   * Improved dynamically BigDouble creation by using precompiler directives
   * Bugfixes  
 * v5.2 (Release)
   * Bugfix on division while "b" have a signle digit
 * v5.22 (Bugfix)
   * General bugfix on division
 * v5.3 (Release)
   * Added new "mod" function that performs a %= b
 * v5.31 (Bugfix)
   * Bugfix on "getMemorySize" function


## BigDouble.c Changelog
 * v1.0 (Release)
   * Basic functional model based on BigOperation
 * v1.1 (Release)
   * Added length control via C_MAX_LENGTH
   * Removed D_MAX_LENGTH control
   * "signed" added to "n" (inside struct)
   * Added a return after showError (service mode)


## BOperation.c Changelog
 * v1.0 (Release)
   * Basic functional model
 * v1.1 (Release)
   * Return added after showError (service mode)
 * v1.2 (Release)
   * Bugfix on division when "b" have only one digit
 * v1.22 (Bugfix)
   * Final bugfix on division  
 * v1.23 (Bugfix)
   * Bugfix on getMemorySize


## How To Configure it?
* Download both header and source code file, or go to "Release" section and download the latest release.

There are several configurations to make
* If you want to set a custom digit length, use `C_MAX_LENGTH` precompiler option (i.e. `-D C_MAX_LENGTH=512`).
* If you trust your data input and want to gain some performance, you can deactivate most of the data validation. Use `CVALIDATE` precompiler option
  * Use `-D CVALIDATE=0` to avoid validations
  * Use `-D CVALIDATE=1` or do not define `CVALIDATE` if you want to ensure the validations
* If you want to work only with Integers, use `BI_STANDALONE` precompiler option (`-D BI_STANDALONE=1`)
  * When using BigInteger Standalone, just `#include "BigInteger.h"`
  * When not using BigInteger Standalone, `#indlude "BOperation.h"` and do not `#include "BigInteger.h"`
* If you are using BigInteger or BOperation as a service (i. e. don't want to stop execution on fail), use `BI_SERVICE` precompiler option (`-D BI_SERVICE=1`).
  * On that case, you may want to use `getReturnCode()` function to check the result of each operation (being `0` an OK, otherwise means operation failed).


## How it works?
* BigInteger (or just BI) works on the basis of 1:1 integer - digit relation. It means that every digit of a BI reaches the space of an integer.
* This is made to allow operations works without problems, and in a parallel trend (the source code was initially designed to work under CUDA).
* Have in mind that, in order to allow easy growing of results, all numbers are stored backwards!
* BI most operations store the results on the first BI passed to calculation function (that's it, it makes "a = a + b", e.g).


## What does BI provides?
* BI provides basic operations such as
  * Addition (add function)
  * Subtraction (sub function)
  * Multiplication (mul function)
  * Division (dvs function)
* BI also provides two advanced operations
  * n-root (nqrt function)
  * Powering (bipow function)
* Debugging and others
  * Memory allocaion for display (iniStr function)
  * Display function (toString function)
  * Useful variables (via BImemcpy function)
    * Minus One (_MIN)
    * Zero (_ZERO)
    * One (_ONE)
    * Two (_TWO)
    * Three (_THREE)
    * Four (_FOUR)
    * ...
    * Ten (_TEN)
    * One Hundred (_HUN)
  * Appending function (append function)
  * Compare function (equals function)
  * Initialize and create BigInteger (newBI function)


## How about BD (BigDouble)?
* BD (BigDouble) is an extension for BigInteger, so it works the same way.
* You can perform the same operations on both structures, as they share most of their information.


## Want to know more?
Just reach me an email at doscar.sole@gmail.com, or tweet me @DoHITB. Will be happy to talk to you!
I have full documentation about CUDA possible implementation, and some fun facts to share!


## What's Next?
There is a plan for several releases, that will be ongoing on further months.

* Make "nqrt" and "bipow" works on non-integer data
* Make "operate" functionality works (pending unit test)
* Translate all comments to English
