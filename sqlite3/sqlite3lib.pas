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
  SQLITE_INTERRUPT  =  9;  // Operation terminated by sqlite3_interrupt()
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
// Extended Result Codes
  SQLITE_IOERR_READ              = SQLITE_IOERR or (1 shl 8);
  SQLITE_IOERR_SHORT_READ        = SQLITE_IOERR or (2 shl 8);
  SQLITE_IOERR_WRITE             = SQLITE_IOERR or (3 shl 8);
  SQLITE_IOERR_FSYNC             = SQLITE_IOERR or (4 shl 8);
  SQLITE_IOERR_DIR_FSYNC         = SQLITE_IOERR or (5 shl 8);
  SQLITE_IOERR_TRUNCATE          = SQLITE_IOERR or (6 shl 8);
  SQLITE_IOERR_FSTAT             = SQLITE_IOERR or (7 shl 8);
  SQLITE_IOERR_UNLOCK            = SQLITE_IOERR or (8 shl 8);
  SQLITE_IOERR_RDLOCK            = SQLITE_IOERR or (9 shl 8);
  SQLITE_IOERR_DELETE            = SQLITE_IOERR or (10 shl 8);
  SQLITE_IOERR_BLOCKED           = SQLITE_IOERR or (11 shl 8);
  SQLITE_IOERR_NOMEM             = SQLITE_IOERR or (12 shl 8);
  SQLITE_IOERR_ACCESS            = SQLITE_IOERR or (13 shl 8);
  SQLITE_IOERR_CHECKRESERVEDLOCK = SQLITE_IOERR or (14 shl 8);
  SQLITE_IOERR_LOCK              = SQLITE_IOERR or (15 shl 8);
  SQLITE_IOERR_CLOSE             = SQLITE_IOERR or (16 shl 8);
  SQLITE_IOERR_DIR_CLOSE         = SQLITE_IOERR or (17 shl 8);
  SQLITE_IOERR_SHMOPEN           = SQLITE_IOERR or (18 shl 8);
  SQLITE_IOERR_SHMSIZE           = SQLITE_IOERR or (19 shl 8);
  SQLITE_IOERR_SHMLOCK           = SQLITE_IOERR or (20 shl 8);
  SQLITE_LOCKED_SHAREDCACHE      = SQLITE_LOCKED or (1 shl 8);
  SQLITE_BUSY_RECOVERY           = SQLITE_BUSY or (1 shl 8);
  SQLITE_CANTOPEN_NOTEMPDIR      = SQLITE_CANTOPEN or (1 shl 8);
// Flags for Open Operations
  SQLITE_OPEN_READONLY        = $00000001;  // Ok for sqlite3_open_v2()
  SQLITE_OPEN_READWRITE       = $00000002;  // Ok for sqlite3_open_v2()
  SQLITE_OPEN_CREATE          = $00000004;  // Ok for sqlite3_open_v2()
  SQLITE_OPEN_DELETEONCLOSE   = $00000008;  // VFS only
  SQLITE_OPEN_EXCLUSIVE       = $00000010;  // VFS only
  SQLITE_OPEN_AUTOPROXY       = $00000020;  // VFS only
  SQLITE_OPEN_MAIN_DB         = $00000100;  // VFS only
  SQLITE_OPEN_TEMP_DB         = $00000200;  // VFS only
  SQLITE_OPEN_TRANSIENT_DB    = $00000400;  // VFS only
  SQLITE_OPEN_MAIN_JOURNAL    = $00000800;  // VFS only
  SQLITE_OPEN_TEMP_JOURNAL    = $00001000;  // VFS only
  SQLITE_OPEN_SUBJOURNAL      = $00002000;  // VFS only
  SQLITE_OPEN_MASTER_JOURNAL  = $00004000;  // VFS only
  SQLITE_OPEN_NOMUTEX         = $00008000;  // Ok for sqlite3_open_v2()
  SQLITE_OPEN_FULLMUTEX       = $00010000;  // Ok for sqlite3_open_v2()
  SQLITE_OPEN_SHAREDCACHE     = $00020000;  // Ok for sqlite3_open_v2()
  SQLITE_OPEN_PRIVATECACHE    = $00040000;  // Ok for sqlite3_open_v2()
  SQLITE_OPEN_WAL             = $00080000;  // VFS only
// device characteristics
  SQLITE_IOCAP_ATOMIC                = $00000001;
  SQLITE_IOCAP_ATOMIC512             = $00000002;
  SQLITE_IOCAP_ATOMIC1K              = $00000004;
  SQLITE_IOCAP_ATOMIC2K              = $00000008;
  SQLITE_IOCAP_ATOMIC4K              = $00000010;
  SQLITE_IOCAP_ATOMIC8K              = $00000020;
  SQLITE_IOCAP_ATOMIC16K             = $00000040;
  SQLITE_IOCAP_ATOMIC32K             = $00000080;
  SQLITE_IOCAP_ATOMIC64K             = $00000100;
  SQLITE_IOCAP_SAFE_APPEND           = $00000200;
  SQLITE_IOCAP_SEQUENTIAL            = $00000400;
  SQLITE_IOCAP_UNDELETABLE_WHEN_OPEN = $00000800;
// File Locking levels
  SQLITE_LOCK_NONE         = 0;
  SQLITE_LOCK_SHARED       = 1;
  SQLITE_LOCK_RESERVED     = 2;
  SQLITE_LOCK_PENDING      = 3;
  SQLITE_LOCK_EXCLUSIVE    = 4;
// Synchronization Type Flags
  SQLITE_SYNC_NORMAL       = $00002;
  SQLITE_SYNC_FULL         = $00003;
  SQLITE_SYNC_DATAONLY     = $00010;



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
// autorizer return codes

  SQLITE_DENY   = 1; // Abort the SQL statement with an error
  SQLITE_IGNORE = 2; // Don't allow access, but don't generate an error
// Authorizer Action Codes
  SQLITE_CREATE_INDEX       =  1; // Index Name      Table Name
  SQLITE_CREATE_TABLE       =  2; // Table Name      NULL
  SQLITE_CREATE_TEMP_INDEX  =  3; // Index Name      Table Name
  SQLITE_CREATE_TEMP_TABLE  =  4; // Table Name      NULL
  SQLITE_CREATE_TEMP_TRIGGER=  5; // Trigger Name    Table Name
  SQLITE_CREATE_TEMP_VIEW   =  6; // View Name       NULL
  SQLITE_CREATE_TRIGGER     =  7; // Trigger Name    Table Name
  SQLITE_CREATE_VIEW        =  8; // View Name       NULL
  SQLITE_DELETE             =  9; // Table Name      NULL
  SQLITE_DROP_INDEX         = 10; // Index Name      Table Name
  SQLITE_DROP_TABLE         = 11; // Table Name      NULL
  SQLITE_DROP_TEMP_INDEX    = 12; // Index Name      Table Name
  SQLITE_DROP_TEMP_TABLE    = 13; // Table Name      NULL
  SQLITE_DROP_TEMP_TRIGGER  = 14; // Trigger Name    Table Name
  SQLITE_DROP_TEMP_VIEW     = 15; // View Name       NULL
  SQLITE_DROP_TRIGGER       = 16; // Trigger Name    Table Name
  SQLITE_DROP_VIEW          = 17; // View Name       NULL
  SQLITE_INSERT             = 18; // Table Name      NULL
  SQLITE_PRAGMA             = 19; // Pragma Name     1st arg or NULL
  SQLITE_READ               = 20; // Table Name      Column Name
  SQLITE_SELECT             = 21; // NULL            NULL
  SQLITE_TRANSACTION        = 22; // Operation       NULL
  SQLITE_UPDATE             = 23; // Table Name      Column Name
  SQLITE_ATTACH             = 24; // Filename        NULL
  SQLITE_DETACH             = 25; // Database Name   NULL
  SQLITE_ALTER_TABLE        = 26; // Database Name   Table Name
  SQLITE_REINDEX            = 27; // Index Name      NULL
  SQLITE_ANALYZE            = 28; // Table Name      NULL
  SQLITE_CREATE_VTABLE      = 29; // Table Name      Module Name
  SQLITE_DROP_VTABLE        = 30; // Table Name      Module Name
  SQLITE_FUNCTION           = 31; // NULL            Function Name
  SQLITE_SAVEPOINT          = 32; // Operation       Savepoint Name
  SQLITE_COPY               =  0; // No longer used

type
  SQLite3_Int64 = Int64;

  SQLite3_mutex = record end;
  PSQLite3_Mutex = ^SQLite3_Mutex;

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

  SQLite3_Module = record end;
  PSQLite3_Module = ^SQLite3_Module;
  PPSQLite3_Module = ^PSQLite3_Module;

  SQLite3_Blob = record end;
  PSQLite3_Blob = ^SQLite3_Blob;
  PPSQLite3_Blob = ^PSQLite3_Blob;


 TSQLite3Base = record
    SQLite3_Lib: TLibrary;
    SQLite3_SysBase: PExecBase;
    SQLite3_SegList: APTR;
  end;
  PSQLite3Base = ^TSQLite3Base;

  TSQLite3_Destructor_Type = procedure(n: Pointer); cdecl;
  TSQLite3_Callback = function(Data: Pointer; NumCol: Integer; Cols: PPAnsiChar; ColNames: PPAnsiChar): Integer; cdecl;
  TSQLite3_BusyCallback = function(Data: Pointer; NumCalls: Integer): Integer; cdecl;
  TSQLite3_AuthCallback = function(pUserData: Pointer; Code: Integer; s1, s2, s3, s4: PAnsiChar): Integer; cdecl;
  TSQLite3_ProgressCallback = function(pUserData: Pointer): Integer; cdecl;
  TSQLite3_FunctionCallback = procedure(Ctx: PSQLite3_context; N: Integer; V: PPSQLite3_value); cdecl;
  TSQLite3_StepCallback = procedure(Ctx: PSQLite3_context; N: Integer; V: PPSQLite3_value); cdecl;
  TSQLite3_FinalCallback = procedure(Ctx: PSQLite3_context); cdecl;
  TSQLite3_CompareCallback = function(User: Pointer; A: Integer; B: Pointer; C: Integer; D: Pointer): Integer; cdecl;
  TSQLite3_Collation_Needed_CallBack = function(User: Pointer; Db: PSQLite3; eTextRep: Integer; s: PAnsiChar): Pointer; cdecl;
  TSQLite3_Collation_Needed_CallBack16 = function(User: Pointer; Db: PSQLite3; eTextRep: Integer; s: PWideChar): Pointer; cdecl;
  TSQLite3_CommitCallback = function(User: Pointer): Integer; cdecl;
  TSQLite3_RollbackCallback = procedure(User: Pointer); cdecl;
  TSQLite3_UpdateCallback = procedure(User: Pointer; Event: Integer; Database, Table: PAnsiChar; RowID: SQLite3_Int64); cdecl;
const
  SQLITE_STATIC    = 0;
  SQLITE_TRANSIENT = -1;

var
  SQLite3Base: PSQLite3Base;

function SQLite3_Open(FileName: PAnsiChar; var db: PSQLite3): Integer; syscall SQLite3Base 5;
function SQLite3_Close(db: PSQLite3): Integer; syscall SQLite3Base 6;
function SQLite3_Exec(db: PSQLite3; sql: PAnsiChar; Callback: TSQLite3_Callback; data: Pointer; var ErrMsg: PAnsiChar): Integer; syscall SQLite3Base 7;
procedure SQLite3_Free(Mem: Pointer); syscall SQLite3Base 8;
function SQLite3_LibVersion(): PAnsiChar; syscall SQLite3Base 9;
function SQLite3_LibVersion_Number(): Integer; syscall SQLite3Base 10;
function SQLite3_Prepare(db: PSQLite3; zSQL: PAnsiChar; nByte: Integer; var pStmt: PSQLite3_Stmt; var pzTail: PAnsiChar): Integer; syscall SQLite3Base 11;
function SQLite3_Prepare_v2(db: PSQLite3; zSQL: PAnsiChar; nByte: Integer; var pStmt: PSQLite3_Stmt; var pzTail: PAnsiChar): Integer; syscall SQLite3Base 12;
function SQLite3_SQL(pStmt: PSQLite3_Stmt): PAnsiChar; syscall SQLite3Base 13;
function SQLite3_Step(pStmt: PSQLite3_Stmt): Integer; syscall SQLite3Base 14;
function SQLite3_Column_Blob(pStmt: PSQLite3_Stmt; iCol: Integer): Pointer; syscall SQLite3Base 15;
function SQLite3_Column_Bytes(pStmt: PSQLite3_Stmt; iCol: Integer): Integer; syscall SQLite3Base 16;
function SQLite3_Column_Bytes16(pStmt: PSQLite3_Stmt; iCol: Integer): Integer; syscall SQLite3Base 17;
function SQLite3_Column_Double(pStmt: PSQLite3_Stmt; iCol: Integer): Double; syscall SQLite3Base 18;
function SQLite3_Column_Int(pStmt: PSQLite3_Stmt; iCol: Integer): Integer; syscall SQLite3Base 19;
function SQLite3_Column_Int64(pStmt: PSQLite3_Stmt; iCol: Integer): SQLite3_Int64; syscall SQLite3Base 20;
function SQLite3_Column_Text(pStmt: PSQLite3_Stmt; iCol: Integer): PAnsiChar; syscall SQLite3Base 21;
function SQLite3_Column_Text16(pStmt: PSQLite3_Stmt; iCol: Integer): PWideChar; syscall SQLite3Base 22;
function SQLite3_Column_Type(pStmt: PSQLite3_Stmt; iCol: Integer): Integer; syscall SQLite3Base 23;
function SQLite3_Column_Value(pStmt: PSQLite3_Stmt; iCol: Integer): PSQLite3_Value; syscall SQLite3Base 24;
function SQLite3_Finalize(pStmt: PSQLite3_Stmt): Integer; syscall SQLite3Base 25;
function SQLite3_Reset(pStmt: PSQLite3_Stmt): Integer; syscall SQLite3Base 26;
function SQLite3_ErrCode(db: PSQLite3): Integer; syscall SQLite3Base 27;
function SQLite3_Extended_ErrCode(db: PSQLite3): Integer; syscall SQLite3Base 28;
function SQLite3_ErrMsg(db: PSQLite3): PAnsiChar; syscall SQLite3Base 29;
function SQLite3_ErrMsg16(db: PSQLite3): PWideChar; syscall SQLite3Base 30;
function SQLite3_Limit(db: PSQLite3; ID: Integer; NewVal: Integer): Integer; syscall SQLite3Base 31;
function SQLite3_Bind_Blob(Stmt: PSQLite3_Stmt; Idx: Integer; Value: Pointer; n: Integer; Destroyer: TSQLite3_Destructor_Type): Integer; syscall SQLite3Base 32;
function SQLite3_Bind_Double(Stmt: PSQLite3_Stmt; Idx: Integer; Value: Double): Integer; syscall SQLite3Base 33;
function SQLite3_Bind_Int(Stmt: PSQLite3_Stmt; Idx: Integer; Value: Integer): Integer; syscall SQLite3Base 34;
function SQLite3_Bind_Int64(Stmt: PSQLite3_Stmt; Idx: Integer; Value: SQLite3_Int64): Integer; syscall SQLite3Base 35;
function SQLite3_Bind_Null(Stmt: PSQLite3_Stmt; Idx: Integer): Integer; syscall SQLite3Base 36;
function SQLite3_Bind_Text(Stmt: PSQLite3_Stmt; Idx: Integer; Value: PAnsiChar; n: Integer; Destroyer: TSQLite3_Destructor_Type): Integer; syscall SQLite3Base 37;
function SQLite3_Bind_Text16(Stmt: PSQLite3_Stmt; Idx: Integer; Value: PWideChar; n: Integer; Destroyer: TSQLite3_Destructor_Type): Integer; syscall SQLite3Base 38;
function SQLite3_Bind_Value(Stmt: PSQLite3_Stmt; Idx: Integer; Value: PSQLite3_Value): Integer; syscall SQLite3Base 39;
function SQLite3_Bind_ZeroBlob(Stmt: PSQLite3_Stmt; Idx: Integer; n: Integer): Integer; syscall SQLite3Base 40;
function SQLite3_Bind_Parameter_Count(Stmt: PSQLite3_Stmt): Integer; syscall SQLite3Base 41;
function SQLite3_Bind_Parameter_Name(Stmt: PSQLite3_Stmt; n: Integer): PAnsiChar; syscall SQLite3Base 42;
function SQLite3_Bind_Parameter_Index(Stmt: PSQLite3_Stmt; zName: PAnsiChar): Integer; syscall SQLite3Base 43;
function SQLite3_Clear_Bindings(Stmt: PSQLite3_Stmt): Integer; syscall SQLite3Base 44;
function SQLite3_Column_Count(Stmt: PSQLite3_Stmt): Integer; syscall SQLite3Base 45;
function SQLite3_Column_Name(Stmt: PSQLite3_Stmt; N: Integer): PAnsiChar; syscall SQLite3Base 46;
function SQLite3_Column_Name16(Stmt: PSQLite3_Stmt; N: Integer): PWideChar; syscall SQLite3Base 47;
function SQLite3_Column_Decltype(Stmt: PSQLite3_Stmt; N: Integer): PAnsiChar; syscall SQLite3Base 48;
function SQLite3_Column_Database_Decltype16(Stmt: PSQLite3_Stmt; N: Integer): PWideChar; syscall SQLite3Base 49;
function SQLite3_Data_Count(Stmt: PSQLite3_Stmt): Integer; syscall SQLite3Base 50;
function SQLite3_Value_Blob(Value: PSQLite3_Value): Pointer; syscall SQLite3Base 51;
function SQLite3_Value_Bytes(Value: PSQLite3_Value): Integer; syscall SQLite3Base 52;
function SQLite3_Value_Bytes16(Value: PSQLite3_Value): Integer; syscall SQLite3Base 53;
function SQLite3_Value_Double(Value: PSQLite3_Value): Double; syscall SQLite3Base 54;
function SQLite3_Value_Int(Value: PSQLite3_Value): Integer; syscall SQLite3Base 55;
function SQLite3_Value_Int64(Value: PSQLite3_Value): SQLite3_Int64; syscall SQLite3Base 56;
function SQLite3_Value_Text(Value: PSQLite3_Value): PAnsiChar; syscall SQLite3Base 57;
function SQLite3_Value_Text16(Value: PSQLite3_Value): PWideChar; syscall SQLite3Base 58;
function SQLite3_Value_Text16LE(Value: PSQLite3_Value): PAnsiChar; syscall SQLite3Base 59;
function SQLite3_Value_Text16BE(Value: PSQLite3_Value): PAnsiChar; syscall SQLite3Base 60;
function SQLite3_Value_Type(Value: PSQLite3_Value): Integer; syscall SQLite3Base 61;
function SQLite3_Value_Numeric_Type(Value: PSQLite3_Value): Integer; syscall SQLite3Base 62;
function SQLite3_Aggregate_Context(Context: PSQLite3_Context; nBytes: Integer): Pointer; syscall SQLite3Base 63;
function SQLite3_User_Data(Context: PSQLite3_Context): Pointer; syscall SQLite3Base 64;
function SQLite3_Get_Auxdata(Context: PSQLite3_Context; N: Integer): Pointer; syscall SQLite3Base 65;
procedure SQLite3_Set_Auxdata(Context: PSQLite3_Context; N: Integer; Data: Pointer; Destroyer: TSQLite3_Destructor_Type); syscall SQLite3Base 66;
function SQLite3_SourceID(): PAnsiChar; syscall SQLite3Base 67;
function SQLite3_Extended_Result_Codes(db: PSQLite3; OnOff: Integer): Integer; syscall SQLite3Base 68;
function SQLite3_Last_Insert_RowID(db: PSQLite3): SQLite3_Int64; syscall SQLite3Base 69;
function SQLite3_Changes(db: PSQLite3): Integer; syscall SQLite3Base 70;
function SQLite3_Total_Changes(db: PSQLite3): Integer; syscall SQLite3Base 71;
function SQLite3_Interrupt(db: PSQLite3): Integer; syscall SQLite3Base 72;
function SQLite3_Complete(SQL: PAnsiChar): Integer; syscall SQLite3Base 73;
function SQLite3_Complete16(SQL: PWideChar): Integer; syscall SQLite3Base 74;
function SQLite3_Busy_Handler(db: PSQLite3; CallBack: TSQLite3_BusyCallback; Data: Pointer): Integer; syscall SQLite3Base 75;
function SQLite3_Busy_Timeout(db: PSQLite3; ms: Integer): Integer; syscall SQLite3Base 76;
function SQLite3_Get_Table(db: PSQLite3; zSQL: PAnsiChar;var azResult: PPAnsiChar; var nRow: Integer; var nColumn: Integer; var zErrMsg: PAnsiChar): Integer; syscall SQLite3Base 77;
procedure SQLite3_Free_Table(azResult: PPAnsiChar); syscall SQLite3Base 78;
function SQLite3_malloc(Size: Integer): Pointer; syscall SQLite3Base 79;
function SQLite3_Realloc(Ptr: Pointer; Size: Integer): Pointer; syscall SQLite3Base 80;
function SQLite3_Memory_Used(): SQLite3_Int64; syscall SQLite3Base 81;
function SQLite3_Memory_Highwater(ResetFlag: Integer): SQLite3_Int64; syscall SQLite3Base 82;
procedure SQLite3_Randomness(N: Integer; P: Pointer); syscall SQLite3Base 83;
function SQLite3_Set_Authorizer(db: PSQLite3; xAuth: TSQLite3_AuthCallback; pUserData: Pointer): Integer; syscall SQLite3Base 84;
procedure SQLite3_Progress_Handler(db: PSQLite3; cb: TSQLite3_ProgressCallback; pUserData: Pointer); syscall SQLite3Base 85;
function SQLite3_Open16(FileName: PWideChar; var db: PSQLite3): Integer; syscall SQLite3Base 86;
function SQLite3_Open_v2(FileName: PAnsiChar; var db: PSQLite3; Flags: Integer; zVfs: PAnsiChar): Integer; syscall SQLite3Base 87;
function SQLite3_Prepare16(db: PSQLite3; zSQL: PWideChar; nByte: Integer; var pStmt: PSQLite3_Stmt; var pzTail: PWideChar): Integer; syscall SQLite3Base 88;
function SQLite3_Prepare16_v2(db: PSQLite3; zSQL: PWideChar; nByte: Integer; var pStmt: PSQLite3_Stmt; var pzTail: PWideChar): Integer; syscall SQLite3Base 89;
procedure SQLite3_Result_Blob(pCtx: PSQLite3_Context; Value: Pointer; n: Integer; Destroyer: TSQLite3_Destructor_Type); syscall SQLite3Base 90;
procedure SQLite3_Result_Double(pCtx: PSQLite3_Context; Value: Double); syscall SQLite3Base 91;
procedure SQLite3_Result_Error(pCtx: PSQLite3_Context; ErrMsg: PAnsiChar; n: Integer); syscall SQLite3Base 92;
procedure SQLite3_Result_Error16(pCtx: PSQLite3_Context; ErrMsg: PWideChar; n: Integer); syscall SQLite3Base 93;
procedure SQLite3_Result_Error_Toobig(pCtx: PSQLite3_Context); syscall SQLite3Base 94;
procedure SQLite3_Result_Error_NoMem(pCtx: PSQLite3_Context); syscall SQLite3Base 95;
procedure SQLite3_Result_Error_Code(pCtx: PSQLite3_Context; Code: Integer); syscall SQLite3Base 96;
procedure SQLite3_Result_Int(pCtx: PSQLite3_Context; Value: Integer); syscall SQLite3Base 97;
procedure SQLite3_Result_Int64(pCtx: PSQLite3_Context; Value: SQLite3_Int64); syscall SQLite3Base 98;
procedure SQLite3_Result_Null(pCtx: PSQLite3_Context); syscall SQLite3Base 99;
procedure SQLite3_Result_Text(pCtx: PSQLite3_Context; Value: PAnsiChar; n: Integer; Destroyer: TSQLite3_Destructor_Type); syscall SQLite3Base 100;
procedure SQLite3_Result_Text16(pCtx: PSQLite3_Context; Value: PWideChar; n: Integer; Destroyer: TSQLite3_Destructor_Type); syscall SQLite3Base 101;
procedure SQLite3_Result_Text16LE(pCtx: PSQLite3_Context; Value: PWideChar; n: Integer; Destroyer: TSQLite3_Destructor_Type); syscall SQLite3Base 102;
procedure SQLite3_Result_Text16BE(pCtx: PSQLite3_Context; Value: PWideChar; n: Integer; Destroyer: TSQLite3_Destructor_Type); syscall SQLite3Base 103;
procedure SQLite3_Result_Value(pCtx: PSQLite3_Context; Value: PSQLite3_Value); syscall SQLite3Base 104;
procedure SQLite3_Result_Zeroblob(pCtx: PSQLite3_Context; n: Integer); syscall SQLite3Base 105;
function SQLite3_Sleep(ms: Integer): Integer; syscall SQLite3Base 106;
function SQLite3_Get_Autocommit(db: PSQLite3): Integer; syscall SQLite3Base 107;
function SQLite3_Db_Handle(Stmt: PSQLite3_Stmt): PSQLite3; syscall SQLite3Base 108;
function SQLite3_Next_Stmt(db: PSQLite3; Stmt: PSQLite3_Stmt): PSQLite3_Stmt; syscall SQLite3Base 109;
function SQLite3_Create_Function(db: PSQLite3; zFunctionName: PAnsiChar; nArg: Integer; eTextRep: Integer; pApp: Pointer; xFunc: TSQLite3_FunctionCallback; xStep: TSQLite3_StepCallback; xFinal: TSQLite3_FinalCallback): Integer; syscall SQLite3Base 110;
function SQLite3_Create_Function16(db: PSQLite3; zFunctionName: PWideChar; nArg: Integer; eTextRep: Integer; pApp: Pointer; xFunc: TSQLite3_FunctionCallback; xStep: TSQLite3_StepCallback; xFinal: TSQLite3_FinalCallback): Integer; syscall SQLite3Base 111;
function SQLite3_Create_Function_v2(db: PSQLite3; zFunctionName: PAnsiChar; nArg: Integer; eTextRep: Integer; pApp: Pointer; xFunc: TSQLite3_FunctionCallback; xStep: TSQLite3_StepCallback; xFinal: TSQLite3_FinalCallback; xDestroy: TSQLite3_Destructor_Type): Integer; syscall SQLite3Base 112;
function SQLite3_Create_Collation(db: PSQLite3; zName: PAnsiChar; eTextRep: Integer; pArg: Pointer; xCompare: TSQLite3_CompareCallback): Integer; syscall SQLite3Base 113;
function SQLite3_Create_Collation_v2(db: PSQLite3; zName: PAnsiChar; eTextRep: Integer; pArg: Pointer; xCompare: TSQLite3_CompareCallback; xDestroy: TSQLite3_Destructor_Type): Integer; syscall SQLite3Base 114;
function SQLite3_Create_Collation16(db: PSQLite3; zName: PWideChar; eTextRep: Integer; pArg: Pointer; xCompare: TSQLite3_CompareCallback): Integer; syscall SQLite3Base 115;
function SQLite3_Collation_Needed(db: PSQLite3; User: Pointer; Callback: TSQLite3_Collation_Needed_CallBack): Integer; syscall SQLite3Base 116;
function SQLite3_Collation_Needed16(db: PSQLite3; User: Pointer; Callback: TSQLite3_Collation_Needed_CallBack16): Integer; syscall SQLite3Base 117;
function SQLite3_Commit_Hook(db: PSQLite3; Callback: TSQLite3_CommitCallBack; User: Pointer): Pointer; syscall SQLite3Base 118;
function SQLite3_Rollback_Hook(db: PSQLite3; Callback: TSQLite3_RollbackCallBack; User: Pointer): Pointer; syscall SQLite3Base 119;
function SQLite3_Update_Hook(db: PSQLite3; Callback: TSQLite3_UpdateCallback; User: Pointer): Pointer; syscall SQLite3Base 120;
function SQLite3_Create_Module(db: PSQLite3; zName: PAnsiChar; P: PSQLite3_Module; pClientData: Pointer): Integer; syscall SQLite3Base 121;
function SQLite3_Create_Module_v2(db: PSQLite3; zName: PAnsiChar; P: PSQLite3_Module; pClientData: Pointer; xDestroy: TSQLite3_Destructor_Type): Integer; syscall SQLite3Base 122;
function SQLite3_Declare_VTab(db: PSQLite3; zSQL: PAnsiChar): Integer; syscall SQLite3Base 123;
function SQLite3_Overload_Function(db: PSQLite3; zFuncName: PAnsiChar; nArg: Integer): Integer; syscall SQLite3Base 124;
function SQLite3_Blob_Open(db: PSQLite3; zDb: PAnsiChar; zTable: PAnsiChar; zColumn: PAnsiChar; iRow: SQLite3_Int64; Flags: Integer; var pBlob: PSQLite3_Blob): Integer; syscall SQLite3Base 125;
function SQLite3_Blob_Close(pBlob: PSQLite3_Blob): Integer; syscall SQLite3Base 126;
function SQLite3_Blob_Bytes(pBlob: PSQLite3_Blob): Integer; syscall SQLite3Base 127;
function SQLite3_Blob_Read(pBlob: PSQLite3_Blob; Z: Pointer; N: Integer; iOffset: Integer): Integer; syscall SQLite3Base 128;
function SQLite3_Blob_Write(pBlob: PSQLite3_Blob; Z: Pointer; N: Integer; iOffset: Integer): Integer; syscall SQLite3Base 129;

implementation

initialization
  SQLite3Base := PSQLite3Base(OpenLibrary(SQLITE3NAME, 0));
finalization
  CloseLibrary(PLibrary(SQLite3Base));
end.
