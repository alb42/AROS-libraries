program testlib;

uses
  Exec;

type
  TMyLib = record
    My_Test_Lib: TLibrary;
    My_SysBase: PLibrary;
    My_SegList: APTR;
    TestInt: Integer;
  end;
  PMyLib = ^TMyLib;
var
  MyNewLib: PMyLib;

function TestFunction(TestValue: Integer): Integer; syscall MyNewLib 5;

procedure StartMe;
var
  a: Integer;
begin
  MyNewLib := PMyLib(OpenLibrary('mytest.library', 0));
  if Assigned(MyNewLib) then
  begin
    writeln('Successfully Open');
    a := Random(100);
    writeln('Start: a= ', a, ' intern ', MyNewLib^.TestInt);
    a := TestFunction(a);
    writeln('First Call a= ', a, ' intern ', MyNewLib^.TestInt);
    a := TestFunction(a);
    writeln('Second Call a= ', a, ' intern ', MyNewLib^.TestInt);
    writeln('enter to close lib');
    Readln;
    CloseLibrary(PLibrary(MyNewLib));
    writeln('Lib Closed');
  end else
    writeln('Lib Not Open');
end;

begin
  StartMe;
end.
