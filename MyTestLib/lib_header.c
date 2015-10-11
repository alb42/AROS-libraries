/*--------------------------------------------------------------------------*/
/* Resident header written for mytest.library */
/*--------------------------------------------------------------------------*/

#define __NOLIBBASE__

#define VERSION         1
#define REVISION        0

#define LIBHEADNAME     mytest
#define LIBHEADNAMESTR "mytest"
#define COMPDATE        "04.10.2015"
#define VERS            "1.0"

#define LIBBASETYPE 	struct MyTestBase
#define LIBBASETYPEPTR  LIBBASETYPE *

#include <aros/debug.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <exec/resident.h>
#include <exec/nodes.h>
#include <exec/libraries.h>
#include <aros/symbolsets.h>
#include "lib.h"

const UBYTE lib_name[] = LIBHEADNAMESTR ".library";

const UBYTE lib_id[] = "$VER: " LIBHEADNAMESTR ".library " VERS " (" COMPDATE ") by ALB42\n";

extern const APTR FuncTable[];

AROS_UFP3 (LIBBASETYPEPTR, InitLib,
    AROS_UFPA(LIBBASETYPEPTR, Base, D0),
    AROS_UFPA(BPTR, seglist, A0),
    AROS_UFPA(struct ExecBase *, sysbase, A6)
);

static struct LibInitStruct
{
    IPTR                   LibSize;
    const APTR             *FuncTable;
    const struct DataTable *DataTable;
    APTR                    InitFunc;
}
const LibInitStruct =
{
    sizeof(LIBBASETYPE),
    FuncTable,
    NULL,
    (APTR)InitLib
};


const struct Resident romtag =
{
  RTC_MATCHWORD,              /* match word */
  (APTR)&romtag,              /* back pointer */
  (APTR)(&romtag + 1),        /* skip pointer */
  RTF_AUTOINIT | RTF_EXTENDED,/* flags */
  VERSION,                    /* version */
  NT_LIBRARY,                 /* type of module */
  0,                          /* init priority */
  (STRPTR)lib_name,           /* module name */
  (STRPTR)lib_id + 6,
  (APTR)&LibInitStruct,
  REVISION, NULL
};

AROS_UFH3 (LIBBASETYPEPTR, InitLib,
    AROS_UFHA(LIBBASETYPEPTR, Base, D0),
    AROS_UFHA(BPTR, seglist, A0),
    AROS_UFHA(struct ExecBase *, sysbase, A6)
    )
{
    AROS_USERFUNC_INIT
    
    Base->My_SegList = seglist;
    Base->My_SysBase = (APTR)sysbase;
    Base->testint = 0;
    
    USESYSBASE
    bug("InitLib\n");
    
    if (!set_open_libraries())
    {
      set_close_libraries();
      return NULL;
    }
    
    return Base;
    
    AROS_USERFUNC_EXIT
}

AROS_LH1(LIBBASETYPEPTR, LibOpen,
    AROS_LHA (ULONG, version, D0),
    LIBBASETYPEPTR, Base, 1, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT
    
    USESYSBASE
    bug("LibOpen\n");

    (void)version;
    
    Base->My_Test_Lib.lib_OpenCnt++;
    return Base;

    AROS_LIBFUNC_EXIT
    
}

__saveds APTR LibExpungeInternal(LIBBASETYPE *Base reg(a6))
{
  USESYSBASE
  APTR seglist;
  bug("LibExpungeInternal\n");

  if (Base->My_Test_Lib.lib_OpenCnt)
  {
    return 0;
  }

  seglist = Base->My_SegList;

  Forbid();
  Remove((struct Node*)Base);
  Permit();
  FreeMem((APTR)Base - Base->My_Test_Lib.lib_NegSize, (LONG)Base->My_Test_Lib.lib_PosSize +
    (LONG)Base->My_Test_Lib.lib_NegSize);
  
  set_close_libraries();
  return seglist;
}

AROS_LH0(BPTR, LibClose,
    LIBBASETYPEPTR, Base, 2, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("LibClose\n");

    if (!(--Base->My_Test_Lib.lib_OpenCnt))
    {
      return LibExpungeInternal(Base);          
    }
    return 0;
	
    AROS_LIBFUNC_EXIT
}

AROS_LH1(BPTR, LibExpunge,
    AROS_LHA(LIBBASETYPEPTR, Base, D0),
    struct ExecBase *, sysBase, 3, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    (void)sysBase;
    
    USESYSBASE
    bug("LibExpunge\n");

    return LibExpungeInternal(Base);
	
    AROS_LIBFUNC_EXIT
}

AROS_LH0(LIBBASETYPEPTR, LibReserved,
    LIBBASETYPEPTR, Base, 4, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("LibReserved\n");

    return 0;
    //return (APTR)LibReserved();
    
    AROS_LIBFUNC_EXIT
}


// Space for your own functions
// do not forget to update the FuncTable as well

AROS_LH1(int, TestFunction,
    AROS_LHA(int, TestValue, D0),
    LIBBASETYPEPTR, Base, 5, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("TestFunction\n");

    Base->testint = TestValue + Base->testint;
    
    return Base->testint;
    
    AROS_LIBFUNC_EXIT
}

// Functable -> Table of all functions in the Library, in right order!

const APTR FuncTable[] = 
{
    &AROS_SLIB_ENTRY(LibOpen,LIBHEADNAME,1),
    &AROS_SLIB_ENTRY(LibClose,LIBHEADNAME,2),
    &AROS_SLIB_ENTRY(LibExpunge,LIBHEADNAME,3),
    &AROS_SLIB_ENTRY(LibReserved,LIBHEADNAME,4),
    &AROS_SLIB_ENTRY(TestFunction,LIBHEADNAME,5),
    (void *)-1
};


