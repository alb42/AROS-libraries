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

AROS_LH5(int, sqlite3_prepare,
    AROS_LHA(sqlite3 *, db, A0),
    AROS_LHA(const char *, zSql, A1),
    AROS_LHA(int, nByte, D0),
    AROS_LHA(sqlite3_stmt **, ppStmt, A2),
    AROS_LHA(const char **, pzTail, A3),
    LIBBASETYPEPTR, Base, 11, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_prepare \n");

    return sqlite3_prepare(db, zSql, nByte, ppStmt, pzTail);

    AROS_LIBFUNC_EXIT
}

AROS_LH5(int, sqlite3_prepare_v2,
    AROS_LHA(sqlite3 *, db, A0),
    AROS_LHA(const char *, zSql, A1),
    AROS_LHA(int, nByte, D0),
    AROS_LHA(sqlite3_stmt **, ppStmt, A2),
    AROS_LHA(const char **, pzTail, A3),
    LIBBASETYPEPTR, Base, 12, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_prepare_v2 \n");

    return sqlite3_prepare_v2(db, zSql, nByte, ppStmt, pzTail);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(const char *, sqlite3_sql,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    LIBBASETYPEPTR, Base, 13, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_sql \n");

    return sqlite3_sql(pStmt);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(int, sqlite3_step,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    LIBBASETYPEPTR, Base, 14, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_step \n");

    return sqlite3_step(pStmt);

    AROS_LIBFUNC_EXIT
}

AROS_LH2(const void *, sqlite3_column_blob,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int, iCol, D0),
    LIBBASETYPEPTR, Base, 15, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_column_blob \n");

    return sqlite3_column_blob(pStmt, iCol);

    AROS_LIBFUNC_EXIT
}

AROS_LH2(int , sqlite3_column_bytes,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int, iCol, D0),
    LIBBASETYPEPTR, Base, 16, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_column_bytes \n");

    return sqlite3_column_bytes(pStmt, iCol);

    AROS_LIBFUNC_EXIT
}

AROS_LH2(int , sqlite3_column_bytes16,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int, iCol, D0),
    LIBBASETYPEPTR, Base, 17, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_column_bytes16 \n");

    return sqlite3_column_bytes16(pStmt, iCol);

    AROS_LIBFUNC_EXIT
}

AROS_LH2(double , sqlite3_column_double,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int, iCol, D0),
    LIBBASETYPEPTR, Base, 18, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_column_double \n");

    return sqlite3_column_double(pStmt, iCol);

    AROS_LIBFUNC_EXIT
}

AROS_LH2(int , sqlite3_column_int,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int, iCol, D0),
    LIBBASETYPEPTR, Base, 19, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_column_int \n");

    return sqlite3_column_int(pStmt, iCol);

    AROS_LIBFUNC_EXIT
}

AROS_LH2(sqlite3_int64 , sqlite3_column_int64,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int, iCol, D0),
    LIBBASETYPEPTR, Base, 20, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_column_int64 \n");

    return sqlite3_column_int64(pStmt, iCol);

    AROS_LIBFUNC_EXIT
}

AROS_LH2(const unsigned char *, sqlite3_column_text,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int, iCol, D0),
    LIBBASETYPEPTR, Base, 21, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_column_text \n");

    return sqlite3_column_text(pStmt, iCol);

    AROS_LIBFUNC_EXIT
}

AROS_LH2(const void *, sqlite3_column_text16,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int, iCol, D0),
    LIBBASETYPEPTR, Base, 22, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_column_text16 \n");

    return sqlite3_column_text16(pStmt, iCol);

    AROS_LIBFUNC_EXIT
}

AROS_LH2(int , sqlite3_column_type,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int, iCol, D0),
    LIBBASETYPEPTR, Base, 23, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_column_type \n");

    return sqlite3_column_type(pStmt, iCol);

    AROS_LIBFUNC_EXIT
}

AROS_LH2(sqlite3_value *, sqlite3_column_value,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int, iCol, D0),
    LIBBASETYPEPTR, Base, 24, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_column_value \n");

    return sqlite3_column_value(pStmt, iCol);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(int, sqlite3_finalize,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    LIBBASETYPEPTR, Base, 25, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_finalize \n");

    return sqlite3_finalize(pStmt);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(int, sqlite3_reset,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    LIBBASETYPEPTR, Base, 26, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_reset \n");

    return sqlite3_reset(pStmt);

    AROS_LIBFUNC_EXIT
}


const APTR FuncTable[] =
{
    &AROS_SLIB_ENTRY(LibOpen,LIBHEADNAME,1),
    &AROS_SLIB_ENTRY(LibClose,LIBHEADNAME,2),
    &AROS_SLIB_ENTRY(LibExpunge,LIBHEADNAME,3),
    &AROS_SLIB_ENTRY(LibReserved,LIBHEADNAME,4),
    &AROS_SLIB_ENTRY(sqlite3_open,LIBHEADNAME,5),
    &AROS_SLIB_ENTRY(sqlite3_close,LIBHEADNAME,6),
    &AROS_SLIB_ENTRY(sqlite3_exec,LIBHEADNAME,7),
    &AROS_SLIB_ENTRY(sqlite3_free,LIBHEADNAME,8),
    &AROS_SLIB_ENTRY(sqlite3_libversion,LIBHEADNAME,9),
    &AROS_SLIB_ENTRY(sqlite3_libversion_number,LIBHEADNAME,10),
    &AROS_SLIB_ENTRY(sqlite3_prepare,LIBHEADNAME,11),
    &AROS_SLIB_ENTRY(sqlite3_prepare_v2,LIBHEADNAME,12),
    &AROS_SLIB_ENTRY(sqlite3_sql,LIBHEADNAME,13),
    &AROS_SLIB_ENTRY(sqlite3_step,LIBHEADNAME,14),
    &AROS_SLIB_ENTRY(sqlite3_column_blob,LIBHEADNAME,15),
    &AROS_SLIB_ENTRY(sqlite3_column_bytes,LIBHEADNAME,16),
    &AROS_SLIB_ENTRY(sqlite3_column_bytes16,LIBHEADNAME,17),
    &AROS_SLIB_ENTRY(sqlite3_column_double,LIBHEADNAME,18),
    &AROS_SLIB_ENTRY(sqlite3_column_int,LIBHEADNAME,19),
    &AROS_SLIB_ENTRY(sqlite3_column_int64,LIBHEADNAME,20),
    &AROS_SLIB_ENTRY(sqlite3_column_text,LIBHEADNAME,21),
    &AROS_SLIB_ENTRY(sqlite3_column_text16,LIBHEADNAME,22),
    &AROS_SLIB_ENTRY(sqlite3_column_type,LIBHEADNAME,23),
    &AROS_SLIB_ENTRY(sqlite3_column_value,LIBHEADNAME,24),
    &AROS_SLIB_ENTRY(sqlite3_finalize,LIBHEADNAME,25),
    &AROS_SLIB_ENTRY(sqlite3_reset,LIBHEADNAME,26),
    (void *)-1
};

void *__PROGRAM_ENTRIES__symbol_set_handler_missing;
void *__LIBS__symbol_set_handler_missing;


