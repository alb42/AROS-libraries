program testsqlite3;

{$mode delphi}{$H+}
uses
  Exec, StrUtils, sqlite3lib;

procedure MyCallback(Data: Pointer; NumCol: Integer; Cols: PPChar; ColNames: PPChar); cdecl;
var
  i: Integer;
begin
  writeln('-->Callback function');
  for i := 0 to NumCol - 1 do
  begin
    writeln(ColNames[i],' = ', ifthen(Assigned(Cols[i]), Cols[i], 'NULL'));
  end;
  writeln('<--Callback function');
end;

procedure StartMe;
var
  db: Psqlite3;
  rc: Integer;
  sql: string;
  ErrMsg: PChar;
  isOK: Boolean;
  Ver: PChar;
  v: LongWord;
begin
  if Assigned(sqlite3Base) then
  begin
    writeln('Successfully Open');
    Ver := sqlite3_libversion;
    if Assigned(Ver) then
    begin
      writeln('Version: ' + Ver);
    end;
    writeln('Integer Version:', sqlite3_libversion_number());
    writeln('Open/create db-file');
    //rc := sqlite3_open('test1.sqlite', db); // in file db
    rc := sqlite3_open(':memory:', db);       // in memory db
    if rc = 0 then
    begin
      isOK := True;
      writeln('OK');
      writeln('Create a table');
      ErrMsg := nil;
      sql := 'CREATE TABLE COMPANY(' +
         'ID INT PRIMARY KEY     NOT NULL,' +
         'NAME           TEXT    NOT NULL,' +
         'AGE            INT     NOT NULL,' +
         'ADDRESS        CHAR(50),' +
         'SALARY         REAL );';
      rc := sqlite3_exec(db, PChar(sql), nil, nil, ErrMsg);
      if rc <> 0 then
      begin
        writeln('Error on create Table: ', ErrMsg);
        sqlite3_free(ErrMsg);
        isOK := False;
      end else
      begin
        writeln('OK');
      end;
      if isOK then
      begin
        sql := 'INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) ' +
           'VALUES (1, "Paul", 32, "California", 20000.00 ); ' +
           'INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) ' +
           'VALUES (2, "Allen", 25, "Texas", 15000.00 ); ' +
           'INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)' +
           'VALUES (3, "Teddy", 23, "Norway", 20000.00 );' +
           'INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)' +
           'VALUES (4, "Mark", 25, "Rich-Mond", 65000.00 );';

        rc := sqlite3_exec(db, PChar(sql), nil, nil, ErrMsg);
        if rc <> 0 then
        begin
          writeln('Error on Insert Data: ', ErrMsg);
          sqlite3_free(ErrMsg);
          isOK := False;
        end else
        begin
          writeln('Successfully inserted Data');
        end;
      end;

      if isOK then
      begin
        sql := 'SELECT * from COMPANY';

        rc := sqlite3_exec(db, PChar(sql), @MyCallback, nil, ErrMsg);
        if rc <> 0 then
        begin
          writeln('Error on select Data: ', ErrMsg);
          sqlite3_free(ErrMsg);
          isOK := False;
        end else
        begin
          writeln('Successfully Select Data');
        end;
      end;
      sqlite3_close(db);
    end else
    begin
      writeln('Error on open/create DataBaseFile ', rc);
    end;
    writeln('Lib Closed');
  end else
    writeln('Lib Not Open');
end;

begin
  StartMe;
end.
