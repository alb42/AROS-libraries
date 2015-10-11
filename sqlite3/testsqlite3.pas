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
  Res: PSQLite3_Stmt;
  Rest: PChar;
  RestW: PWideChar;
  SQLVal: PSQLite3_Value;
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
        writeln('Insert Data ...');
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
          writeln('OK');
        end;
      end;

      if isOK then
      begin
        writeln('Select Data');
        sql := 'SELECT * from COMPANY';

        rc := sqlite3_exec(db, PChar(sql), @MyCallback, nil, ErrMsg);
        if rc <> 0 then
        begin
          writeln('Error on select Data: ', ErrMsg);
          sqlite3_free(ErrMsg);
          isOK := False;
        end else
        begin
          writeln('OK');
        end;
      end;
      if isOK then
      begin
        writeln('Test sqlite_prepare_v2 API');
        sql := 'SELECT SQLITE_VERSION()';

        rc := sqlite3_prepare_v2(db, PChar(sql), -1, res, Rest);
        if rc <> 0 then
        begin
          writeln('Error on select Data ', rc);
          isOK := False;
        end else
        begin
          writeln('OK');
          writeln('make a step');
          rc := sqlite3_step(Res);
          if rc = SQLITE_ROW then
          begin
            writeln('Number of results: ', sqlite3_Data_Count(Res));
            writeln('Num of Colums: ', sqlite3_Column_Count(res));
            Rest := sqlite3_Column_Name(res, 0);
            if Assigned(Rest) then
            begin
              writeln('name of column 0: ', Rest);
            end;
            RestW := sqlite3_Column_Name16(res, 0);
            if Assigned(RestW) then
            begin
              writeln('name of column as WideChar 0: ', WideString(RestW));
            end;
            writeln('Version got by prepare API: ', sqlite3_Column_Text(res, 0));
            writeln('get as SQLite3_Value');
            SQLVal := SQLite3_Column_Value(Res, 0);
            if Assigned(SQLVal) then
            begin
              writeln('Type of SQLite3_Value (should be 3 = Text): ', SQLite3_Value_Type(SQLVal));
              writeln('Result from SQLite3_Value: ', SQLite3_Value_Text(SQLVal));
            end;
          end else
          begin
            writeln('Error: Step Result <> SQLITE_ROW but ', rc);
            isOK := False;
          end;
          sqlite3_finalize(Res);
        end;
      end;
      sqlite3_close(db);
    end else
    begin
      writeln('Error on open/create DataBaseFile ', rc);
    end;
    writeln();
    if IsOK then
      writeln('### All test completed ###')
    else
      writeln('### ERROR, tests stopped ###');
    writeln();
    writeln('Lib Closed');
  end else
    writeln('Lib Not Open');
end;

begin
  StartMe;
end.
