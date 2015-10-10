/*--------------------------------------------------------------------------*/
/* Resident header written for sqlite3.library */
/*--------------------------------------------------------------------------*/

#define __NOLIBBASE__

#define VERSION         1
#define REVISION        0

#define COMPDATE        "04.10.2015"
#define VERS            "1.0"
#define LIBHEADNAME     sqlite3
#define LIBHEADNAMESTR "sqlite3"

#include <aros/debug.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <exec/resident.h>
#include <exec/nodes.h>
#include <exec/libraries.h>
#include <sqlite3.h>
#include <aros/symbolsets.h>
#include "lib.h"

THIS_PROGRAM_HANDLES_SYMBOLSETS

void *__startup_entries_next;
void *__includelibrarieshandling;
void *__startup_error;

const UBYTE lib_name[] = LIBHEADNAMESTR ".library";

const UBYTE lib_id[] = "$VER: " LIBHEADNAMESTR ".library " VERS " (" COMPDATE ") by ALB42\n";

#define LIBBASETYPE   struct sqlite3base
#define LIBBASETYPEPTR  LIBBASETYPE *

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

LONG dummy_f(VOID)
{
    return -1;
}

AROS_UFH3 (LIBBASETYPEPTR, InitLib,
    AROS_UFHA(LIBBASETYPEPTR, Base, D0),
    AROS_UFHA(BPTR, seglist, A0),
    AROS_UFHA(struct ExecBase *, sysbase, A6)
)
{
    AROS_USERFUNC_INIT

    Base->sqlite3_SegList = seglist;
    Base->sqlite3_SysBase = sysbase;

    USESYSBASE
    bug("InitLib\n");

    if (!set_open_libraries())
    {
      set_close_libraries();
      return NULL;
    }
    sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
    sqlite3_initialize();
    return Base;

    AROS_USERFUNC_EXIT
}

AROS_LH1(LIBBASETYPEPTR, LibOpen,
    AROS_LHA (ULONG, version, D0),
    LIBBASETYPEPTR, Base, 1, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("LibOpen\n");

    (void)version;

    Base->sqlite3_Lib.lib_OpenCnt++;
    return Base;

    AROS_LIBFUNC_EXIT

}

__saveds APTR LibExpungeIntern (LIBBASETYPE *Base reg(a6))
{
  USESYSBASE
  APTR seglist;
  bug("Expunge internal\n");

  if (Base->sqlite3_Lib.lib_OpenCnt)
  {
    return 0;
  }

  seglist = Base->sqlite3_SegList;

  Forbid();
  Remove((struct Node*)Base);
  Permit();
  FreeMem((APTR)Base - Base->sqlite3_Lib.lib_NegSize, (LONG)Base->sqlite3_Lib.lib_PosSize +
    (LONG)Base->sqlite3_Lib.lib_NegSize);
  sqlite3_shutdown();
  set_close_libraries();
  return seglist;
}

AROS_LH0(BPTR, LibClose,
    LIBBASETYPEPTR, Base, 2, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("LibClose\n");

    if (!(--Base->sqlite3_Lib.lib_OpenCnt))
    {
      return LibExpungeIntern(Base);
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

    return LibExpungeIntern(Base);

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

AROS_LH2(int, sqlite3_open,
    AROS_LHA(const char*, Filename, A0),
    AROS_LHA(sqlite3 **, ppDb, A1),
    LIBBASETYPEPTR, Base, 5, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_open %s\n",Filename);

    return sqlite3_open(Filename, ppDb);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(void, sqlite3_close,
    AROS_LHA(sqlite3 *, pDb, A0),
    LIBBASETYPEPTR, Base, 6, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_close \n");

    sqlite3_close(pDb);

    AROS_LIBFUNC_EXIT
}

AROS_LH5(int, sqlite3_exec,
    AROS_LHA(sqlite3 *, pDb, A0),
    AROS_LHA(const char*, sql, A1),
    AROS_LHA(int(*callback),(void*,int,char**,char**), A2),
    AROS_LHA(void *, data, A3),
    AROS_LHA(char**, errmsg, A4),
    LIBBASETYPEPTR, Base, 7, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_exec\n");

    return sqlite3_exec(pDb, sql, callback, data, errmsg);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(void, sqlite3_free,
    AROS_LHA(void *, mem, A0),
    LIBBASETYPEPTR, Base, 8, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_free \n");

    sqlite3_free(mem);

    AROS_LIBFUNC_EXIT
}

AROS_LH0(const char *, sqlite3_libversion,
    LIBBASETYPEPTR, Base, 9, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_libversion \n");

    return sqlite3_libversion();

    AROS_LIBFUNC_EXIT
}


AROS_LH0(int, sqlite3_libversion_number,
    LIBBASETYPEPTR, Base, 10, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_libversion_number %d\n");    

    return sqlite3_libversion_number();

    AROS_LIBFUNC_EXIT
}



const APTR FuncTable[] =
{
    &AROS_SLIB_ENTRY(LibOpen,LIBHEADNAME),
    &AROS_SLIB_ENTRY(LibClose,LIBHEADNAME),
    &AROS_SLIB_ENTRY(LibExpunge,LIBHEADNAME),
    &AROS_SLIB_ENTRY(LibReserved,LIBHEADNAME),
    &AROS_SLIB_ENTRY(sqlite3_open,LIBHEADNAME),
    &AROS_SLIB_ENTRY(sqlite3_close,LIBHEADNAME),
    &AROS_SLIB_ENTRY(sqlite3_exec,LIBHEADNAME),
    &AROS_SLIB_ENTRY(sqlite3_free,LIBHEADNAME),
    &AROS_SLIB_ENTRY(sqlite3_libversion,LIBHEADNAME),
    &AROS_SLIB_ENTRY(sqlite3_libversion_number,LIBHEADNAME),
    (void *)-1
};

ADD2INITLIB(InitLib,0)
ADD2EXPUNGELIB(LibExpungeIntern, 0)

