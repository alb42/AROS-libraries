unit sqlite3lib;
{$mode delphi}{$H+}
interface

uses
  Exec;

const
  SQLITE3NAME = 'sqlite3.library';

  SQLITE_OK         =  0;  // Successful result
  SQLITE_ERROR      =  1;  // SQL error or missing database
  SQLITE_INTERNAL   =  2;  // Internal logic error in SQLite
  SQLITE_PERM       =  3;  // Access permission denied
  SQLITE_ABORT      =  4;  // Callback routine requested an abort
  SQLITE_BUSY       =  5;  // The database file is locked
  SQLITE_LOCKED     =  6;  // A table in the database is locked
  SQLITE_NOMEM      =  7;  // A malloc() failed
  SQLITE_READONLY   =  8;  // Attempt to write a readonly database
  SQLITE_INTERRUPT  =  9;  // Operation terminated by sqlite3_interrupt()*/
  SQLITE_IOERR      = 10;  // Some kind of disk I/O error occurred
  SQLITE_CORRUPT    = 11;  // The database disk image is malformed
  SQLITE_NOTFOUND   = 12;  // NOT USED. Table or record not found
  SQLITE_FULL       = 13;  // Insertion failed because database is full
  SQLITE_CANTOPEN   = 14;  // Unable to open the database file
  SQLITE_PROTOCOL   = 15;  // Database lock protocol error
  SQLITE_EMPTY      = 16;  // Database is empty
  SQLITE_SCHEMA     = 17;  // The database schema changed
  SQLITE_TOOBIG     = 18;  // String or BLOB exceeds size limit
  SQLITE_CONSTRAINT = 19;  // Abort due to constraint violation
  SQLITE_MISMATCH   = 20;  // Data type mismatch
  SQLITE_MISUSE     = 21;  // Library used incorrectly
  SQLITE_NOLFS      = 22;  // Uses OS features not supported on host
  SQLITE_AUTH       = 23;  // Authorization denied
  SQLITE_FORMAT     = 24;  // Auxiliary database format error
  SQLITE_RANGE      = 25;  // 2nd parameter to sqlite3_bind out of range
  SQLITE_NOTADB     = 26;  // File opened that is not a database file
  SQLITE_ROW        = 100; // sqlite3_step() has another row ready
  SQLITE_DONE       = 101; // sqlite3_step() has finished executing

// for sqlite3_Limit
  SQLITE_LIMIT_LENGTH              = 0;
  SQLITE_LIMIT_SQL_LENGTH          = 1;
  SQLITE_LIMIT_COLUMN              = 2;
  SQLITE_LIMIT_EXPR_DEPTH          = 3;
  SQLITE_LIMIT_COMPOUND_SELECT     = 4;
  SQLITE_LIMIT_VDBE_OP             = 5;
  SQLITE_LIMIT_FUNCTION_ARG        = 6;
  SQLITE_LIMIT_ATTACHED            = 7;
  SQLITE_LIMIT_LIKE_PATTERN_LENGTH = 8;
  SQLITE_LIMIT_VARIABLE_NUMBER     = 9;
  SQLITE_LIMIT_TRIGGER_DEPTH       = 10;

// basic SQLITE type codes

  SQLITE_INTEGER = 1;
  SQLITE_FLOAT   = 2;
  SQLITE_BLOB    = 4;
  SQLITE_NULL    = 5;
  SQLITE_TEXT    = 3;
  SQLITE3_TEXT   = 3;

// Text Encodings

  SQLITE_UTF8          = 1;
  SQLITE_UTF16LE       = 2;
  SQLITE_UTF16BE       = 3;
  SQLITE_UTF16         = 4; // Use native byte order
  SQLITE_ANY           = 5; // sqlite3_create_function only
  SQLITE_UTF16_ALIGNED = 8; // sqlite3_create_collation only

type
  SQLite3_Int64 = Int64;

  SQLite3 = record end;
  PSQLite3 = ^SQLite3;
  PPSQLite3 = ^PSQLite3;

  SQLite3_Stmt = record end;
  PSQLite3_Stmt = ^SQLite3_Stmt;
  PPSQLite3_Stmt = ^PSQLite3_Stmt;

  SQLite3_Value = record end;
  PSQLite3_Value = ^SQLite3_Value;
  PPSQLite3_Value = ^PSQLite3_Value;

  SQLite3_Context = record end;
  PSQLite3_Context = ^SQLite3_Context;
  PPSQLite3_Context = ^PSQLite3_Context;

 TSQLite3Base = record
    SQLite3_Lib: TLibrary;
    SQLite3_SysBase: PExecBase;
    SQLite3_SegList: APTR;
  end;
  PSQLite3Base = ^TSQLite3Base;

  TSQLite3_Destructor_Type = procedure(n: Pointer); cdecl;
const
  SQLITE_STATIC    = 0;
  SQLITE_TRANSIENT = -1;

var
  SQLite3Base: PSQLite3Base;

function SQLite3_Open(Name: PChar; var db: PSQLite3): Integer; syscall SQLite3Base 5;
procedure SQLite3_Close(db: PSQLite3); syscall SQLite3Base 6;
function SQLite3_Exec(db: PSQLite3; sql: PChar; callback: Pointer; data: Pointer; var ErrMsg: PChar): Integer; syscall SQLite3Base 7;
procedure SQLite3_Free(Mem: Pointer); syscall SQLite3Base 8;
function SQLite3_LibVersion(): PChar; syscall SQLite3Base 9;
function SQLite3_LibVersion_Number(): Integer; syscall SQLite3Base 10;
function SQLite3_Prepare(db: PSQLite3; zSQL: PChar; nByte: Integer; var pStmt: PSQLite3_Stmt; var pzTail: PChar): Integer; syscall SQLite3Base 11;
function SQLite3_Prepare_v2(db: PSQLite3; zSQL: PChar; nByte: Integer; var pStmt: PSQLite3_Stmt; var pzTail: PChar): Integer; syscall SQLite3Base 12;
function SQLite3_SQL(pStmt: PSQLite3_Stmt): PChar; syscall SQLite3Base 13;
function SQLite3_Step(pStmt: PSQLite3_Stmt): Integer; syscall SQLite3Base 14;
function SQLite3_Column_Blob(pStmt: PSQLite3_Stmt; iCol: Integer): Pointer; syscall SQLite3Base 15;
function SQLite3_Column_Bytes(pStmt: PSQLite3_Stmt; iCol: Integer): Integer; syscall SQLite3Base 16;
function SQLite3_Column_Bytes16(pStmt: PSQLite3_Stmt; iCol: Integer): Integer; syscall SQLite3Base 17;
function SQLite3_Column_Double(pStmt: PSQLite3_Stmt; iCol: Integer): Double; syscall SQLite3Base 18;
function SQLite3_Column_Int(pStmt: PSQLite3_Stmt; iCol: Integer): Integer; syscall SQLite3Base 19;
function SQLite3_Column_Int64(pStmt: PSQLite3_Stmt; iCol: Integer): SQLite3_Int64; syscall SQLite3Base 20;
function SQLite3_Column_Text(pStmt: PSQLite3_Stmt; iCol: Integer): PChar; syscall SQLite3Base 21;
function SQLite3_Column_Text16(pStmt: PSQLite3_Stmt; iCol: Integer): PWideChar; syscall SQLite3Base 22;
function SQLite3_Column_Type(pStmt: PSQLite3_Stmt; iCol: Integer): Integer; syscall SQLite3Base 23;
function SQLite3_Column_Value(pStmt: PSQLite3_Stmt; iCol: Integer): PSQLite3_Value; syscall SQLite3Base 24;
function SQLite3_Finalize(pStmt: PSQLite3_Stmt): Integer; syscall SQLite3Base 25;
function SQLite3_Reset(pStmt: PSQLite3_Stmt): Integer; syscall SQLite3Base 26;
function SQLite3_ErrCode(db: PSQLite3): Integer; syscall SQLite3Base 27;
function SQLite3_Extended_ErrCode(db: PSQLite3): Integer; syscall SQLite3Base 28;
function SQLite3_ErrMsg(db: PSQLite3): PChar; syscall SQLite3Base 29;
function SQLite3_ErrMsg16(db: PSQLite3): PWideChar; syscall SQLite3Base 30;
function SQLite3_Limit(db: PSQLite3; ID: Integer; NewVal: Integer): Integer; syscall SQLite3Base 31;
function SQLite3_Bind_Blob(Stmt: PSQLite3_Stmt; Idx: Integer; Value: Pointer; n: Integer; Destroyer: Pointer): Integer; syscall SQLite3Base 32;
function SQLite3_Bind_Double(Stmt: PSQLite3_Stmt; Idx: Integer; Value: Double): Integer; syscall SQLite3Base 33;
function SQLite3_Bind_Int(Stmt: PSQLite3_Stmt; Idx: Integer; Value: Integer): Integer; syscall SQLite3Base 34;
function SQLite3_Bind_Int64(Stmt: PSQLite3_Stmt; Idx: Integer; Value: SQLite3_Int64): Integer; syscall SQLite3Base 35;
function SQLite3_Bind_Null(Stmt: PSQLite3_Stmt; Idx: Integer): Integer; syscall SQLite3Base 36;
function SQLite3_Bind_Text(Stmt: PSQLite3_Stmt; Idx: Integer; Value: PChar; n: Integer; Destroyer: Pointer): Integer; syscall SQLite3Base 37;
function SQLite3_Bind_Text16(Stmt: PSQLite3_Stmt; Idx: Integer; Value: PWideChar; n: Integer; Destroyer: Pointer): Integer; syscall SQLite3Base 38;
function SQLite3_Bind_Value(Stmt: PSQLite3_Stmt; Idx: Integer; Value: PSQLite3_Value): Integer; syscall SQLite3Base 39;
function SQLite3_Bind_ZeroBlob(Stmt: PSQLite3_Stmt; Idx: Integer; n: Integer): Integer; syscall SQLite3Base 40;
function SQLite3_Bind_Parameter_Count(Stmt: PSQLite3_Stmt): Integer; syscall SQLite3Base 41;
function SQLite3_Bind_Parameter_Name(Stmt: PSQLite3_Stmt; n: Integer): PChar; syscall SQLite3Base 42;
function SQLite3_Bind_Parameter_Index(Stmt: PSQLite3_Stmt; zName: PChar): Integer; syscall SQLite3Base 43;
function SQLite3_Clear_Bindings(Stmt: PSQLite3_Stmt): Integer; syscall SQLite3Base 44;
function SQLite3_Column_Count(Stmt: PSQLite3_Stmt): Integer; syscall SQLite3Base 45;
function SQLite3_Column_Name(Stmt: PSQLite3_Stmt; N: Integer): PChar; syscall SQLite3Base 46;
function SQLite3_Column_Name16(Stmt: PSQLite3_Stmt; N: Integer): PWideChar; syscall SQLite3Base 47;
function SQLite3_Column_Decltype(Stmt: PSQLite3_Stmt; N: Integer): PChar; syscall SQLite3Base 48;
function SQLite3_Column_Database_Decltype16(Stmt: PSQLite3_Stmt; N: Integer): PWideChar; syscall SQLite3Base 49;
function SQLite3_Data_Count(Stmt: PSQLite3_Stmt): Integer; syscall SQLite3Base 50;
function SQLite3_Value_Blob(Value: PSQLite3_Value): Pointer; syscall SQLite3Base 51;
function SQLite3_Value_Bytes(Value: PSQLite3_Value): Integer; syscall SQLite3Base 52;
function SQLite3_Value_Bytes16(Value: PSQLite3_Value): Integer; syscall SQLite3Base 53;
function SQLite3_Value_Double(Value: PSQLite3_Value): Double; syscall SQLite3Base 54;
function SQLite3_Value_Int(Value: PSQLite3_Value): Integer; syscall SQLite3Base 55;
function SQLite3_Value_Int64(Value: PSQLite3_Value): SQLite3_Int64; syscall SQLite3Base 56;
function SQLite3_Value_Text(Value: PSQLite3_Value): PChar; syscall SQLite3Base 57;
function SQLite3_Value_Text16(Value: PSQLite3_Value): PWideChar; syscall SQLite3Base 58;
function SQLite3_Value_Text16LE(Value: PSQLite3_Value): PChar; syscall SQLite3Base 59;
function SQLite3_Value_Text16BE(Value: PSQLite3_Value): PChar; syscall SQLite3Base 60;
function SQLite3_Value_Type(Value: PSQLite3_Value): Integer; syscall SQLite3Base 61;
function SQLite3_Value_Numeric_Type(Value: PSQLite3_Value): Integer; syscall SQLite3Base 62;
function SQLite3_Aggregate_Context(Context: PSQLite3_Context; nBytes: Integer): Pointer; syscall SQLite3Base 63;
function SQLite3_User_Data(Context: PSQLite3_Context): Pointer; syscall SQLite3Base 64;
function SQLite3_Get_Auxdata(Context: PSQLite3_Context; N: Integer): Pointer; syscall SQLite3Base 65;
procedure SQLite3_Set_Auxdata(Context: PSQLite3_Context; N: Integer; Data: Pointer; Destroyer: TSQLite3_Destructor_Type); syscall SQLite3Base 66;

implementation

initialization
  SQLite3Base := PSQLite3Base(OpenLibrary(SQLITE3NAME, 0));
finalization
  CloseLibrary(PLibrary(SQLite3Base));
end.
