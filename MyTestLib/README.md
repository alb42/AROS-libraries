# How to use
 Test library to create a own Library

## Important Files:
<ul>
<li>Makefile     - define the compile properties
<li>lib_header.c - definition of Basic library function and parameter
<li>lib.h        - definition of own library base structure (and later other needed for the functions)
<li>testlibrary.pas - example freepascal unit how to use this Library
</ul>

## Makefile
Define the name of your library (2 times). If you need more objects to compile add them to "OBJS", additional link libs add to "LIBS"

## lib_header.c
At begin of file you can define name, version, date and name of LIBBASETYPE and add some additional includes if needed.

### InitLib
<pre>
  (AROS_UFH3 (LIBBASETYPEPTR, InitLib,
    AROS_UFHA(LIBBASETYPEPTR, Base, D0),
    AROS_UFHA(BPTR, seglist, A0),
    AROS_UFHA(struct ExecBase *, sysbase, A6)))
</pre>
Called when the Library is opened the very first time. Good place to initialize all the things of your library and librarybase (e.g. Mutexes, additional Libraries, additional memory, lists, initial Values in your LibBase).

### LibOpen
<pre>
  AROS_LH1(LIBBASETYPEPTR, LibOpen,
    AROS_LHA (ULONG, version, D0),
    LIBBASETYPEPTR, Base, 1, LIBHEADNAME)  
</pre>
Called when a program tries to open the Library (via OpenLibrary) If the first time, InitLib is called before 

### LibClose
<pre>
  AROS_LH0(BPTR, LibClose,
    LIBBASETYPEPTR, Base, 2, LIBHEADNAME)
</pre>    
Called when a program closes the Library (via CloseLibrary). If this was the last open Library (denoted by Base-&gt;My_Test_Lib.lib_OpenCnt) The Library will be also unloaded (via LibExpunge)

### LibExpunge
<pre>
  AROS_LH1(BPTR, LibExpunge,
    AROS_LHA(LIBBASETYPEPTR, Base, D0),
    struct ExecBase *, sysBase, 3, LIBHEADNAME)
</pre>    
Calls LibExpungeInternal (still do not know when this function is called, maybe on resident unload)    

### LibExpungeInternal
<pre>
__saveds APTR LibExpungeInternal(LIBBASETYPE *Base reg(a6))
</pre>
Remove the Library from the Library List and free the memory. Goodplace to free the resources of your LibBase (e.g. Mutexes, additional Libraries, additional memory, lists, initial Values in your LibBase)

### LibReserved
<pre>
AROS_LH0(LIBBASETYPEPTR, LibReserved,
    LIBBASETYPEPTR, Base, 4, LIBHEADNAME)
</pre>    
Reserved function, I have no idea what it does.

### FuncTable
<pre>
const APTR FuncTable[]
</pre>
List of all functions in the Library in the right order the List is terminated with a -1.

## How to add a function to the List
Create a new Entry like:<br>
<pre>
AROS_LH1(int, TestFunction,
    AROS_LHA(int, TestValue, D0),
    LIBBASETYPEPTR, Base, 5, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("TestFunction\n");
    
    return 0
    
    AROS_LIBFUNC_EXIT
}
</pre>
<br><br>
Add Entry to FunctTable<br>
<pre>
 &AROS_SLIB_ENTRY(TestFunction,LIBHEADNAME,5),
</pre>
Set the right function name and number as defined in the AROS_LHx Header.

### The Header
<ul>
<li><pre>AROS_LHx(int, Testfunction)</pre>
  <ul>
    <li>x = number of parameters for the function (must match the number of AROS_LHA)
    <li>int = return type (if no return value = void)
    <li>Testfunction = name of function
  </ul>
<li><pre>AROS_LHA(int, TestValue, D0),</pre> - a typical parameter entry
  <ul> 
    <li>int = Type of the parameter
    <li>TestValue = name of the parameter
    <li>D0 = a m68k register to hold the parameter must be unique in the function (for Numbers: D0-D7, for Pointers: A0-A5))
  </ul>
<li><pre>LIBBASETYPEPTR, Base, 5, LIBHEADNAME</pre>
 <ul>
   <li>5 - Function offset for this function (must match the FuncTable)
 </ul>
</ul>

### The Body
<ul>
  <li><pre>AROS_LIBFUNC_INIT</pre> - Every function must begin with this macro
  <li><pre>AROS_LIBFUNC_EXIT</pre> - Every function must end with this macro
  <li><pre>USESYSBASE</pre> - get the sysbase (ExecBase) e.g. for functions like bug() (see lib.h doe exact definition)
</ul>

### Freepascal use
</pre>function TestFunction(TestValue: Integer): Integer; syscall MyNewLib 5;</pre>
<ul>
  <li> function (with return value) / procedure (without return value)
  <li> TestFunction - Name of Function
  <li> TestValue: Integer -  Name and type of First Parameter
  <li> : Integer - Returntype if a function
  <li> MyNewLib - LibraryBase opened before with OpenLibrary();
  <li> 5 - Library Offset (from Lib_header.c definition/ FuncTable)
</ul>
