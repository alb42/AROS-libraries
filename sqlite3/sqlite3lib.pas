unit sqlite3lib;
{$mode delphi}{$H+}
interface

uses
  Exec;

const
  SQLITE3NAME = 'sqlite3.library';

type
  SQLite3 = record end;
  PSQLite3 = ^SQLite3;
  PPSQLite3 = ^PSQLite3;

  SQLite3_Int64 = Int64;


 TSQLite3Base = record
    SQLite3_Lib: TLibrary;
    SQLite3_SysBase: PExecBase;
    SQLite3_SegList: APTR;
  end;
  PSQLite3Base = ^TSQLite3Base;

var
  SQLite3Base: PSQLite3Base;

function SQLite3_Open(Name: PChar; var db: Psqlite3): Integer; syscall SQLite3Base 5;
procedure SQLite3_Close(db: Psqlite3); syscall SQLite3Base 6;
function SQLite3_Exec(db: Psqlite3; sql: PChar; callback: Pointer; data: Pointer; var ErrMsg: PChar): Integer; syscall SQLite3Base 7;
procedure SQLite3_Free(Mem: Pointer); syscall SQLite3Base 8;
function SQLite3_LibVersion(): PChar; syscall SQLite3Base 9;
function SQLite3_LibVersion_Number(): Integer; syscall SQLite3Base 10;

implementation

initialization
  SQLite3Base := PSQLite3Base(OpenLibrary(SQLITE3NAME, 0));
finalization
  CloseLibrary(PLibrary(SQLite3Base));
end.
