# /voidMemory/BigInteger.c
C Based model to work with Big Integers

Correctly tested working with up to 4096 digits integer (on the master branch).

This architecture was made during my Degree final paper (TFG, in Spain), to cover an RSA cryptoanalsys.

## Changelog
 * v1.1 (beta)
   * Review variables and functions, making it "static" when convenient.
   * Changing all pointer references to "void" to avoid memory leakage.
   * Added new validations on BI creation.
   * New function "BImemcpy" to retreive useful values of BigInteger.
   * New function "validateBI" to validate the internal structure of a BI void pointer.
 * v1.11 (beta)
   * Added memory cleansing before "return" statements

## How To Configure it?
* Download both header and source code file
* Change "int MAX_LENGTH = 512;" to adjust desired max length on source code file. 
* Change "int n[512];" to same value as previous point to allow storage of digits
* Initially, it worked with 4096 digits, so there will be no problem to use it - it was downgraded to 512 in order to preserve som RAM.

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
  * Square root (BISQRT function)
  * Powering (pow function)
* Debugging and others
  * Display function (toString function)
  * Useful variables (via BImemcpy function)
    * Minus One (_MIN)
    * Zero (_ZERO)
    * One (_ONE)
    * Ten (_TEN)
    * One Hundred (_HUN)
  * Appending function (append function)
  * Compare function (equals function)
  * Initialize and create BigInteger (newBI function)
  
  
## Disclamer
This version is on beta and has not been tested yet. Use at your own risk


## Want to know more?
Just reach me an email at doscar.sole@gmail.com, or tweet me @DoHITB. Will be happy to talk to you!
I have full documentation about CUDA possible implementation, and some fun facts to share!


## What's Next?
There is a plan for several releases, that will be ongoing on further months.
