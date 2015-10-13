program test;


{$MODE OBJFPC}{$H+}{$HINTS ON}{$ASSERTIONS ON}


Uses
  exec, imagequantlib;



procedure test_fixed_colors;
var
  attr  : Pliq_attr;
  dummy : array[0..4-1] of byte = (0,0,0,0);
  img   : Pliq_image;
  res   : Pliq_result;
  pal   : Pliq_palette;
begin
  attr := liq_attr_create();

  img   := liq_image_create_rgba(attr, @dummy, 1, 1, 0);
  assert(img <> nil);

  liq_image_add_fixed_color(img, MACRO_liq_color(0,0,0,0));

  res := liq_quantize_image(attr, img);
  assert(res <> nil);

  pal := liq_get_palette(res);
  assert(pal <> nil);
  assert(pal^.count = 1);

  liq_result_destroy(res);
  liq_image_destroy(img);
  liq_attr_destroy(attr);
end;



procedure test_fixed_colors_order;
var
  attr      : Pliq_attr;
  dummy     : array[0..4-1] of byte = (0,0,0,0);
  img       : Pliq_image;
  colors    : array[0..17-1] of Tliq_color = 
  (
    (r: 0;g: 0;b: 0;a: 0), (r: 1;g: 1;b: 1;a: 1), (r: 2;g: 2;b: 2;a: 2), 
    (r: 3;g: 3;b: 3;a: 3), (r: 4;g: 4;b: 4;a: 4), (r: 5;g: 4;b: 4;a: 4), 
    (r: 6;g: 4;b: 4;a: 4), (r: 6;g: 7;b: 4;a: 4), (r: 6;g: 7;b: 8;a: 4), 
    (r: 6;g: 7;b: 8;a: 9), (r:10;g: 7;b: 8;a: 9), (r:10;g:11;b: 8;a: 9),
    (r:10;g:11;b:12;a: 9), (r:10;g:11;b:12;a:13), (r:14;g:11;b:12;a:13), 
    (r:14;g:15;b:12;a:13), (r:14;g:15;b:16;a:13)
  );
  i         : integer;
  res       : Pliq_result;
  pal       : Pliq_palette;
begin
  attr := liq_attr_create();

  img := liq_image_create_rgba(attr, @dummy, 1, 1, 0);
  assert(img <> nil);

  for i:=0 to Pred(17) do
  begin
    liq_image_add_fixed_color(img, colors[i]);
  end;

  res := liq_quantize_image(attr, img);
  assert(res <> nil);

  pal := liq_get_palette(res);
  assert(pal <> nil);
  assert(pal^.count = 17);

  for i:=0 to Pred(17) do
  begin
    assert(pal^.entries[i].r = colors[i].r);
    assert(pal^.entries[i].g = colors[i].g);
    assert(pal^.entries[i].b = colors[i].b);
    assert(pal^.entries[i].a = colors[i].a);
  end;

  liq_result_destroy(res);
  liq_image_destroy(img);
  liq_attr_destroy(attr);
end;



Function main: Integer;
begin
  test_fixed_colors();
  test_fixed_colors_order();
  WriteLn('OK');
  result := 0;
end;



begin
  Writeln('enter');
  
  ImageQuantBase := OpenLibrary(IMAGEQUANTNAME, 0);
  if Assigned(ImageQuantBase) then
  begin
    WriteLn('Suceeded in opening ', IMAGEQUANTNAME);

    ExitCode := main;

    CloseLibrary(ImageQuantBase);
  end
  else WriteLn('Failed to open ', IMAGEQUANTNAME);

  Writeln('leave');
end.
