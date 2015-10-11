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
#include <aros/symbolsets.h>
#include "lib.h"
#include <sqlite3.h>

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

AROS_LH1(int, sqlite3_errcode,
    AROS_LHA(sqlite3 *, db, A0),
    LIBBASETYPEPTR, Base, 27, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_errcode \n");

    return sqlite3_errcode(db);

    AROS_LIBFUNC_EXIT
}


AROS_LH1(int, sqlite3_extended_errcode,
    AROS_LHA(sqlite3 *, db, A0),
    LIBBASETYPEPTR, Base, 28, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_extended_errcode \n");

    return sqlite3_extended_errcode(db);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(const char *, sqlite3_errmsg,
    AROS_LHA(sqlite3 *, db, A0),
    LIBBASETYPEPTR, Base, 29, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_errmsg \n");

    return sqlite3_errmsg(db);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(const void *, sqlite3_errmsg16,
    AROS_LHA(sqlite3 *, db, A0),
    LIBBASETYPEPTR, Base, 30, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_errmsg16 \n");

    return sqlite3_errmsg16(db);

    AROS_LIBFUNC_EXIT
}

AROS_LH3(int, sqlite3_limit,
    AROS_LHA(sqlite3 *, db, A0),
    AROS_LHA(int , id, D0),
    AROS_LHA(int , newVal, D1),
    LIBBASETYPEPTR, Base, 31, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_limit \n");

    return sqlite3_limit(db, id, newVal);

    AROS_LIBFUNC_EXIT
}

AROS_LH5(int, sqlite3_bind_blob,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int , idx, D0),
    AROS_LHA(const void *, value, A1),    
    AROS_LHA(int, n, A2),
    AROS_LHA(void(*destructor),(void*), A3),
    LIBBASETYPEPTR, Base, 32, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_bind_blob \n");

    return sqlite3_bind_blob(pStmt, idx, value, n, destructor);

    AROS_LIBFUNC_EXIT
}

AROS_LH3(int, sqlite3_bind_double,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int , idx, D0),
    AROS_LHA(double , value, A1),    
    LIBBASETYPEPTR, Base, 33, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_bind_double \n");

    return sqlite3_bind_double(pStmt, idx, value);

    AROS_LIBFUNC_EXIT
}

AROS_LH3(int, sqlite3_bind_int,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int , idx, D0),
    AROS_LHA(int , value, D1),    
    LIBBASETYPEPTR, Base, 34, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_bind_int \n");

    return sqlite3_bind_int(pStmt, idx, value);

    AROS_LIBFUNC_EXIT
}

AROS_LH3(int, sqlite3_bind_int64,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int , idx, D0),
    AROS_LHA(sqlite3_int64 , value, D1),    
    LIBBASETYPEPTR, Base, 35, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_bind_int64 \n");

    return sqlite3_bind_int64(pStmt, idx, value);

    AROS_LIBFUNC_EXIT
}

AROS_LH2(int, sqlite3_bind_null,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int , idx, D0),
    LIBBASETYPEPTR, Base, 36, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_bind_null \n");

    return sqlite3_bind_null(pStmt, idx);

    AROS_LIBFUNC_EXIT
}

AROS_LH5(int, sqlite3_bind_text,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int , idx, D0),
    AROS_LHA(const char *, value, A1),    
    AROS_LHA(int, n, A2),
    AROS_LHA(void(*destructor),(void*), A3),
    LIBBASETYPEPTR, Base, 37, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_bind_text \n");

    return sqlite3_bind_text(pStmt, idx, value, n, destructor);

    AROS_LIBFUNC_EXIT
}

AROS_LH5(int, sqlite3_bind_text16,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int , idx, D0),
    AROS_LHA(const void *, value, A1),    
    AROS_LHA(int, n, A2),
    AROS_LHA(void(*destructor),(void*), A3),
    LIBBASETYPEPTR, Base, 38, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_bind_text16 \n");

    return sqlite3_bind_text16(pStmt, idx, value, n, destructor);

    AROS_LIBFUNC_EXIT
}

AROS_LH3(int, sqlite3_bind_value,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int , idx, D0),
    AROS_LHA(sqlite3_value *, value, A1),    
    LIBBASETYPEPTR, Base, 39, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_bind_value \n");

    return sqlite3_bind_value(pStmt, idx, value);

    AROS_LIBFUNC_EXIT
}

AROS_LH3(int, sqlite3_bind_zeroblob,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int , idx, D0),
    AROS_LHA(int , n, D1),    
    LIBBASETYPEPTR, Base, 40, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_bind_zeroblob \n");

    return sqlite3_bind_zeroblob(pStmt, idx, n);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(int, sqlite3_bind_parameter_count,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),   
    LIBBASETYPEPTR, Base, 41, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_bind_parameter_count \n");

    return sqlite3_bind_parameter_count(pStmt);

    AROS_LIBFUNC_EXIT
}

AROS_LH2(const char *, sqlite3_bind_parameter_name,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int , n, D0),
    LIBBASETYPEPTR, Base, 42, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_bind_parameter_name \n");

    return sqlite3_bind_parameter_name(pStmt, n);

    AROS_LIBFUNC_EXIT
}

AROS_LH2(int, sqlite3_bind_parameter_index,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(const char *, zName, A1),
    LIBBASETYPEPTR, Base, 43, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_bind_parameter_index \n");

    return sqlite3_bind_parameter_index(pStmt, zName);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(int, sqlite3_clear_bindings,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    LIBBASETYPEPTR, Base, 44, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_clear_bindings \n");

    return sqlite3_clear_bindings(pStmt);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(int, sqlite3_column_count,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    LIBBASETYPEPTR, Base, 45, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_column_count \n");

    return sqlite3_column_count(pStmt);

    AROS_LIBFUNC_EXIT
}

AROS_LH2(const char *, sqlite3_column_name,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int , N, D0),
    LIBBASETYPEPTR, Base, 46, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_column_name \n");

    return sqlite3_column_name(pStmt, N);

    AROS_LIBFUNC_EXIT
}

AROS_LH2(const void *, sqlite3_column_name16,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int , N, D0),
    LIBBASETYPEPTR, Base, 47, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_column_name16 \n");

    return sqlite3_column_name16(pStmt, N);

    AROS_LIBFUNC_EXIT
}

AROS_LH2(const char *, sqlite3_column_decltype,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int , N, D0),
    LIBBASETYPEPTR, Base, 48, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_column_decltype \n");

    return sqlite3_column_decltype(pStmt, N);

    AROS_LIBFUNC_EXIT
}

AROS_LH2(const void *, sqlite3_column_decltype16,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    AROS_LHA(int , N, D0),
    LIBBASETYPEPTR, Base, 49, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_column_decltype16 \n");

    return sqlite3_column_decltype16(pStmt, N);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(int , sqlite3_data_count,
    AROS_LHA(sqlite3_stmt *, pStmt, A0),
    LIBBASETYPEPTR, Base, 50, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_data_count \n");

    return sqlite3_data_count(pStmt);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(const void *, sqlite3_value_blob,
    AROS_LHA(sqlite3_value *, value, A0),
    LIBBASETYPEPTR, Base, 51, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_value_blob \n");

    return sqlite3_value_blob(value);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(int, sqlite3_value_bytes,
    AROS_LHA(sqlite3_value *, value, A0),
    LIBBASETYPEPTR, Base, 52, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_value_bytes \n");

    return sqlite3_value_bytes(value);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(int, sqlite3_value_bytes16,
    AROS_LHA(sqlite3_value *, value, A0),
    LIBBASETYPEPTR, Base, 53, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_value_bytes16 \n");

    return sqlite3_value_bytes16(value);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(double, sqlite3_value_double,
    AROS_LHA(sqlite3_value *, value, A0),
    LIBBASETYPEPTR, Base, 54, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_value_double \n");

    return sqlite3_value_double(value);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(int, sqlite3_value_int,
    AROS_LHA(sqlite3_value *, value, A0),
    LIBBASETYPEPTR, Base, 55, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_value_int \n");

    return sqlite3_value_int(value);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(sqlite3_int64, sqlite3_value_int64,
    AROS_LHA(sqlite3_value *, value, A0),
    LIBBASETYPEPTR, Base, 56, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_value_int64 \n");

    return sqlite3_value_int64(value);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(const unsigned char *, sqlite3_value_text,
    AROS_LHA(sqlite3_value *, value, A0),
    LIBBASETYPEPTR, Base, 57, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_value_text \n");

    return sqlite3_value_text(value);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(const void *, sqlite3_value_text16,
    AROS_LHA(sqlite3_value *, value, A0),
    LIBBASETYPEPTR, Base, 58, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_value_text16 \n");

    return sqlite3_value_text16(value);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(const void *, sqlite3_value_text16le,
    AROS_LHA(sqlite3_value *, value, A0),
    LIBBASETYPEPTR, Base, 59, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_value_text16le \n");

    return sqlite3_value_text16le(value);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(const void *, sqlite3_value_text16be,
    AROS_LHA(sqlite3_value *, value, A0),
    LIBBASETYPEPTR, Base, 60, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_value_text16be \n");

    return sqlite3_value_text16be(value);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(int, sqlite3_value_type,
    AROS_LHA(sqlite3_value *, value, A0),
    LIBBASETYPEPTR, Base, 61, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_value_type \n");

    return sqlite3_value_type(value);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(int, sqlite3_value_numeric_type,
    AROS_LHA(sqlite3_value *, value, A0),
    LIBBASETYPEPTR, Base, 62, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_value_numeric_type \n");

    return sqlite3_value_numeric_type(value);

    AROS_LIBFUNC_EXIT
}

AROS_LH2(void *, sqlite3_aggregate_context,
    AROS_LHA(sqlite3_context *, pCtx, A0),
    AROS_LHA(int, nBytes, D0),
    LIBBASETYPEPTR, Base, 63, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_aggregate_context \n");

    return sqlite3_aggregate_context(pCtx, nBytes);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(void *, sqlite3_user_data,
    AROS_LHA(sqlite3_context *, pCtx, A0),
    LIBBASETYPEPTR, Base, 64, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_user_data \n");

    return sqlite3_user_data(pCtx);

    AROS_LIBFUNC_EXIT
}

AROS_LH2(void *, sqlite3_get_auxdata,
    AROS_LHA(sqlite3_context *, pCtx, A0),
    AROS_LHA(int , N, D0),
    LIBBASETYPEPTR, Base, 65, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_get_auxdata \n");

    return sqlite3_get_auxdata(pCtx, N);

    AROS_LIBFUNC_EXIT
}

AROS_LH4(void , sqlite3_set_auxdata,
    AROS_LHA(sqlite3_context *, pCtx, A0),
    AROS_LHA(int , N, D0),
    AROS_LHA(void *, auxdata, A1),
    AROS_LHA(void(*destructor),(void*), A2),
    LIBBASETYPEPTR, Base, 66, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_set_auxdata \n");

    return sqlite3_set_auxdata(pCtx, N, auxdata, destructor);

    AROS_LIBFUNC_EXIT
}

AROS_LH0(const char *, sqlite3_sourceid,
    LIBBASETYPEPTR, Base, 67, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_sourceid \n");

    return sqlite3_sourceid();

    AROS_LIBFUNC_EXIT
}

AROS_LH2(int , sqlite3_extended_result_codes,
    AROS_LHA(sqlite3 *, pDb, A0),
    AROS_LHA(int , OnOff, D0),
    LIBBASETYPEPTR, Base, 68, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_extended_result_codes \n");

    return sqlite3_extended_result_codes(pDb, OnOff);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(sqlite3_int64 , sqlite3_last_insert_rowid,
    AROS_LHA(sqlite3 *, pDb, A0),
    LIBBASETYPEPTR, Base, 69, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_last_insert_rowid \n");

    return sqlite3_last_insert_rowid(pDb);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(int , sqlite3_changes,
    AROS_LHA(sqlite3 *, pDb, A0),
    LIBBASETYPEPTR, Base, 70, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_changes \n");

    return sqlite3_changes(pDb);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(int , sqlite3_total_changes,
    AROS_LHA(sqlite3 *, pDb, A0),
    LIBBASETYPEPTR, Base, 71, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_total_changes \n");

    return sqlite3_total_changes(pDb);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(void , sqlite3_interrupt,
    AROS_LHA(sqlite3 *, pDb, A0),
    LIBBASETYPEPTR, Base, 72, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_interrupt \n");

    return sqlite3_interrupt(pDb);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(int , sqlite3_complete,
    AROS_LHA(const char *, sql, A0),
    LIBBASETYPEPTR, Base, 73, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_complete \n");

    return sqlite3_complete(sql);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(int , sqlite3_complete16,
    AROS_LHA(const void *, sql, A0),
    LIBBASETYPEPTR, Base, 74, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_complete16 \n");

    return sqlite3_complete16(sql);

    AROS_LIBFUNC_EXIT
}

AROS_LH3(int , sqlite3_busy_handler,
    AROS_LHA(sqlite3 *, pDb, A0),
    AROS_LHA(int(*callback),(void*,int), A1),
    AROS_LHA(void *, data, A2),
    LIBBASETYPEPTR, Base, 75, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_busy_handler \n");

    return sqlite3_busy_handler(pDb, callback, data);

    AROS_LIBFUNC_EXIT
}

AROS_LH2(int , sqlite3_busy_timeout,
    AROS_LHA(sqlite3 *, pDb, A0),
    AROS_LHA(int , ms, D0),
    LIBBASETYPEPTR, Base, 76, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_busy_timeout \n");

    return sqlite3_busy_timeout(pDb, ms);

    AROS_LIBFUNC_EXIT
}

AROS_LH6(int , sqlite3_get_table,
    AROS_LHA(sqlite3 *, pDb, A0),
    AROS_LHA(const char *, zSql, A1),
    AROS_LHA(char ***, pazResult, A2),
    AROS_LHA(int *, pnRow, A3),
    AROS_LHA(int *, pnColumn, A4),
    AROS_LHA(char **, pzErrmsg, A5),
    LIBBASETYPEPTR, Base, 77, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_get_table \n");

    return sqlite3_get_table(pDb, zSql, pazResult, pnRow, pnColumn, pzErrmsg);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(void , sqlite3_free_table,
    AROS_LHA(char **, Result, A2),
    LIBBASETYPEPTR, Base, 78, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_free_table \n");

    sqlite3_free_table(Result);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(void *, sqlite3_malloc,
    AROS_LHA(int, size, D0),
    LIBBASETYPEPTR, Base, 79, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_malloc \n");

    return sqlite3_malloc(size);

    AROS_LIBFUNC_EXIT
}

AROS_LH2(void *, sqlite3_realloc,
    AROS_LHA(void *, ptr, A0),
    AROS_LHA(int, size, D0),
    LIBBASETYPEPTR, Base, 80, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_realloc \n");

    return sqlite3_realloc(ptr, size);

    AROS_LIBFUNC_EXIT
}

AROS_LH0(sqlite3_int64 , sqlite3_memory_used,
    LIBBASETYPEPTR, Base, 81, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_memory_used \n");

    return sqlite3_memory_used();

    AROS_LIBFUNC_EXIT
}

AROS_LH1(sqlite3_int64 , sqlite3_memory_highwater,
    AROS_LHA(int, resetflag, D0),
    LIBBASETYPEPTR, Base, 82, LIBHEADNAME)
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("sqlite3_memory_highwater \n");

    return sqlite3_memory_highwater(resetflag);

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
    &AROS_SLIB_ENTRY(sqlite3_errcode,LIBHEADNAME,27),
    &AROS_SLIB_ENTRY(sqlite3_extended_errcode,LIBHEADNAME,28),
    &AROS_SLIB_ENTRY(sqlite3_errmsg,LIBHEADNAME,29),
    &AROS_SLIB_ENTRY(sqlite3_errmsg16,LIBHEADNAME,30),
    &AROS_SLIB_ENTRY(sqlite3_limit,LIBHEADNAME,31),
    &AROS_SLIB_ENTRY(sqlite3_bind_blob,LIBHEADNAME,32),
    &AROS_SLIB_ENTRY(sqlite3_bind_double,LIBHEADNAME,33),
    &AROS_SLIB_ENTRY(sqlite3_bind_int,LIBHEADNAME,34),
    &AROS_SLIB_ENTRY(sqlite3_bind_int64,LIBHEADNAME,35),
    &AROS_SLIB_ENTRY(sqlite3_bind_null,LIBHEADNAME,36),
    &AROS_SLIB_ENTRY(sqlite3_bind_text,LIBHEADNAME,37),
    &AROS_SLIB_ENTRY(sqlite3_bind_text16,LIBHEADNAME,38),
    &AROS_SLIB_ENTRY(sqlite3_bind_value,LIBHEADNAME,39),
    &AROS_SLIB_ENTRY(sqlite3_bind_zeroblob,LIBHEADNAME,40),
    &AROS_SLIB_ENTRY(sqlite3_bind_parameter_count,LIBHEADNAME,41),
    &AROS_SLIB_ENTRY(sqlite3_bind_parameter_name,LIBHEADNAME,42),
    &AROS_SLIB_ENTRY(sqlite3_bind_parameter_index,LIBHEADNAME,43),
    &AROS_SLIB_ENTRY(sqlite3_clear_bindings,LIBHEADNAME,44),
    &AROS_SLIB_ENTRY(sqlite3_column_count,LIBHEADNAME,45),
    &AROS_SLIB_ENTRY(sqlite3_column_name,LIBHEADNAME,46),
    &AROS_SLIB_ENTRY(sqlite3_column_name16,LIBHEADNAME,47),
    &AROS_SLIB_ENTRY(sqlite3_column_decltype,LIBHEADNAME,48),
    &AROS_SLIB_ENTRY(sqlite3_column_decltype16,LIBHEADNAME,49),
    &AROS_SLIB_ENTRY(sqlite3_data_count,LIBHEADNAME,50),
    &AROS_SLIB_ENTRY(sqlite3_value_blob,LIBHEADNAME,51),
    &AROS_SLIB_ENTRY(sqlite3_value_bytes,LIBHEADNAME,52),
    &AROS_SLIB_ENTRY(sqlite3_value_bytes16,LIBHEADNAME,53),
    &AROS_SLIB_ENTRY(sqlite3_value_double,LIBHEADNAME,54),
    &AROS_SLIB_ENTRY(sqlite3_value_int,LIBHEADNAME,55),
    &AROS_SLIB_ENTRY(sqlite3_value_int64,LIBHEADNAME,56),
    &AROS_SLIB_ENTRY(sqlite3_value_text,LIBHEADNAME,57),
    &AROS_SLIB_ENTRY(sqlite3_value_text16,LIBHEADNAME,58),
    &AROS_SLIB_ENTRY(sqlite3_value_text16le,LIBHEADNAME,59),
    &AROS_SLIB_ENTRY(sqlite3_value_text16be,LIBHEADNAME,60),
    &AROS_SLIB_ENTRY(sqlite3_value_type,LIBHEADNAME,61),
    &AROS_SLIB_ENTRY(sqlite3_value_numeric_type,LIBHEADNAME,62),
    &AROS_SLIB_ENTRY(sqlite3_aggregate_context,LIBHEADNAME,63),
    &AROS_SLIB_ENTRY(sqlite3_user_data,LIBHEADNAME,64),
    &AROS_SLIB_ENTRY(sqlite3_get_auxdata,LIBHEADNAME,65),
    &AROS_SLIB_ENTRY(sqlite3_set_auxdata,LIBHEADNAME,66),
    &AROS_SLIB_ENTRY(sqlite3_sourceid,LIBHEADNAME,67),
    &AROS_SLIB_ENTRY(sqlite3_extended_result_codes,LIBHEADNAME,68),
    &AROS_SLIB_ENTRY(sqlite3_last_insert_rowid,LIBHEADNAME,69),
    &AROS_SLIB_ENTRY(sqlite3_changes,LIBHEADNAME,70),
    &AROS_SLIB_ENTRY(sqlite3_total_changes,LIBHEADNAME,71),
    &AROS_SLIB_ENTRY(sqlite3_interrupt,LIBHEADNAME,72),
    &AROS_SLIB_ENTRY(sqlite3_complete,LIBHEADNAME,73),
    &AROS_SLIB_ENTRY(sqlite3_complete16,LIBHEADNAME,74),
    &AROS_SLIB_ENTRY(sqlite3_busy_handler,LIBHEADNAME,75),
    &AROS_SLIB_ENTRY(sqlite3_busy_timeout,LIBHEADNAME,76),
    &AROS_SLIB_ENTRY(sqlite3_get_table,LIBHEADNAME,77),
    &AROS_SLIB_ENTRY(sqlite3_free_table,LIBHEADNAME,78),
    &AROS_SLIB_ENTRY(sqlite3_malloc,LIBHEADNAME,79),
    &AROS_SLIB_ENTRY(sqlite3_realloc,LIBHEADNAME,80),
    &AROS_SLIB_ENTRY(sqlite3_memory_used,LIBHEADNAME,81),
    &AROS_SLIB_ENTRY(sqlite3_memory_highwater,LIBHEADNAME,82),
    (void *)-1
};

void *__PROGRAM_ENTRIES__symbol_set_handler_missing;
void *__LIBS__symbol_set_handler_missing;


