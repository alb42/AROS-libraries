# libimagequant


In this repository you'll find a wrapper library written in c and unit for Free Pascal accompanied with some test examples for AROS.

Documentation regarding libimagequant library function calls can be found [here](https://github.com/pornel/pngquant/blob/master/lib/MANUAL.md).



## How to create the shared library


Follow the instructions written beneath.

Assuming knowledge on git or download and extracting archives.

A working build environment able to target AROS is required (both gcc and FPC).



### Step 1: Preparations


- Git clone this repo or download the archive
- Git clone the pngquant repository or [download the archive](https://github.com/pornel/pngquant/archive/master.zip)
- Inside the libimagequant directroy of this project create a new directory (clib)
- copy all files from pngquant/lib to AROS-libraries/libimagequant/clib

In case not cross-compiling, you need to transfer all files from AROS-libraries over to AROS



### Step 2: Creating the library objects and archive


The default makefile doesn't do its job properly on AROS, so instead we manually compile the library.

- Start AROS shell
- Travel to AROS-libraries/libimagequant/clib
- invoke sh
- Create library object files with gcc -std=c99 -c -lgcc -DNDEBUG *.c
- Create library archive with ar -rf libimagequant.a blur.o libimagequant.o mediacut.o mempool.o nearest.o pam.o viter.o

That should result in a library archive named libimagequant.a



### Step 3: Creating the AROS library wrapper


- copy or move the file libimagequant.a from AROS-libraries/libimagequant/clib to AROS-libraries/libimagequant
- type make

This should result in the creation of file imagequant.library



### Done


You have now sucessfully created the AROS library for libimagequant, and is ready for usage.

- close sh by typing exit.

In order to test, make sure you are inside AROS shell (not sh shell). Also a working FPC setup is required.

- make sure you are inside drawer AROS-libraries/libimagequant
- Type FPC test.pas

You should now have a new executable file named test, which will do some simple checks of the library and should print "OK" to the console if the checks passed successful.



## ToDo



- Since the library is only a wrapper, prevent from opening again
- create a proper testcase, preferably porting pngquant project (requires FPC AROS headers for libpng)
- Create a proper makefile able to create libimagequant and library archive
- include proper copyright information regarding libimagequant


## Copyright


Please note that libimagequant is copyrighted by its respective owner(s). The presented work inside AROS-libraries/libimagequant/ as well as this documentation can be used freely, but remains copyrighted by its respective owner(s).
