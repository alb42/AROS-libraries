unit ImageQuantLib;


{$MODE OBJFPC}{$H+}{$HINTS ON}


interface


Uses
  Exec, Ctypes;


Const
  IMAGEQUANTNAME    : PChar = 'imagequant.library';


Var
  ImageQuantBase    : PLibrary;


type
  Pliq_attr = ^Tliq_attr;
  Tliq_attr = record
  end;

  Pliq_image = ^Tliq_image;
  Tliq_image = record
  end;

  Pliq_result = ^Tliq_result;
  Tliq_result = record
  end;


  Pliq_color = ^Tliq_color;
  Tliq_color = record
    r,g,b,a  : cuchar;
  end;


  Pliq_palette = ^Tliq_palette;
  Tliq_palette = record
    count   : cuint;
    entries : array[0..256-1] of Tliq_color;
  end;


  liq_error = 
  (
    LIQ_OK                  = 0,
    LIQ_QUALITY_TOO_LOW     = 99,
    LIQ_VALUE_OUT_OF_RANGE  = 100,
    LIQ_OUT_OF_MEMORY,
    LIQ_NOT_READY,
    LIQ_BITMAP_NOT_AVAILABLE,
    LIQ_BUFFER_TOO_SMALL,
    LIQ_INVALID_POINTER
  );



/////////////////////////////////////////////////////////////////////////////
//
//  Callback functions definitions
//
/////////////////////////////////////////////////////////////////////////////



type 
  TMallocFunc                       = function(n: csize_t): Pointer; cdecl;
  TFreeFunc                         = procedure(n: Pointer); cdecl;

  Tliq_log_callback_function        = procedure(const attr: Pliq_attr; const msg: pchar; user_info: Pointer); cdecl;
  Tliq_log_flush_callback_function  = procedure(const attr: Pliq_attr; user_info: Pointer); cdecl;

  Tliq_image_get_rgba_row_callback  = procedure(row_out: array of Tliq_color; row: cint; width: cint; user_info: Pointer); cdecl;



/////////////////////////////////////////////////////////////////////////////
//
//  Library functions definitions
//
/////////////////////////////////////////////////////////////////////////////



  function  liq_attr_create: Pliq_attr;                                                                                                                                             syscall ImageQuantBase  5;
  function  liq_attr_create_with_allocator(mallocfunc: TMallocFunc; freefunc: TFreeFunc): Pliq_attr;                                                                                syscall ImageQuantBase  6;
  function  liq_attr_create(orig: Pliq_attr): Pliq_attr;                                                                                                                            syscall ImageQuantBase  7;
  procedure liq_attr_destroy(attr: Pliq_attr);                                                                                                                                      syscall ImageQuantBase  8;

  function  liq_set_max_colors(attr: Pliq_attr; colors: cint): liq_error;                                                                                                           syscall ImageQuantBase  9;
  function  liq_get_max_colors(const attr: Pliq_attr): cint;                                                                                                                        syscall ImageQuantBase 10;
  function  liq_set_speedliq_set_speed(attr: Pliq_attr; speed: cint): liq_error;                                                                                                    syscall ImageQuantBase 11;
  function  liq_get_speed(const attr: Pliq_attr): cint;                                                                                                                             syscall ImageQuantBase 12;
  function  liq_set_min_opacity(attr: Pliq_attr; min: cint): liq_error;                                                                                                             syscall ImageQuantBase 13;
  function  liq_get_min_opacity(const attr: Pliq_attr): cint;                                                                                                                       syscall ImageQuantBase 14;
  function  liq_set_min_posterization(attr: Pliq_attr; bits: cint): liq_error;                                                                                                      syscall ImageQuantBase 15;
  function  liq_get_min_posterization(const attr: Pliq_attr): cint;                                                                                                                 syscall ImageQuantBase 16;
  function  liq_set_quality(attr: Pliq_attr; minimum: cint; maximum: cint): liq_error;                                                                                              syscall ImageQuantBase 17;
  function  liq_get_min_quality(const attr: Pliq_attr): cint;                                                                                                                       syscall ImageQuantBase 18;
  function  liq_get_max_quality(const attr: Pliq_attr): cint;                                                                                                                       syscall ImageQuantBase 19;
  procedure liq_set_last_index_transparent(attr: Pliq_attr; is_last: cint);                                                                                                         syscall ImageQuantBase 20;

  procedure liq_set_log_callback(attr: Pliq_attr; func: Tliq_log_callback_function; user_info: Pointer);                                                                             syscall ImageQuantBase 21;
  procedure liq_set_log_flush_callback(attr: Pliq_attr; func: Tliq_log_flush_callback_function; user_info: Pointer);                                                                 syscall ImageQuantBase 22;

  function  liq_image_create_rgba_rows(attr: Pliq_attr; rows: array of Pointer; width: cint; height: cint; gamma: cdouble): Pliq_image;                                              syscall ImageQuantBase 23;
  function  liq_image_create_rgba(attr: Pliq_attr; bitmap: Pointer; width: cint; height: cint; gamma: cdouble): Pliq_image;                                                          syscall ImageQuantBase 24;

  function  liq_image_create_custom(attr: Pliq_attr; row_callback: Tliq_image_get_rgba_row_callback; user_info: Pointer; width: cint;  height: cint; gamma: cdouble): Pliq_image;    syscall ImageQuantBase 25;

  function  liq_image_set_memory_ownership(image: Pliq_image; ownership_flags: cint): liq_error;                                                                                    syscall ImageQuantBase 26;
  function  liq_image_add_fixed_color(img: Pliq_image; color: Tliq_color): liq_error;                                                                                               syscall ImageQuantBase 27;
  function  liq_image_get_width(const img: Pliq_image): cint;                                                                                                                       syscall ImageQuantBase 28;
  function  liq_image_get_height(const img: Pliq_image): cint;                                                                                                                      syscall ImageQuantBase 29;
  procedure liq_image_destroy(img: Pliq_image);                                                                                                                                     syscall ImageQuantBase 30;

  function  liq_quantize_image(options: Pliq_attr; input_image: Pliq_image): Pliq_result;                                                                                           syscall ImageQuantBase 31;

  function  liq_set_dithering_level(res: Pliq_result; dither_level: cfloat): liq_error;                                                                                             syscall ImageQuantBase 32;
  function  liq_set_output_gamma(res: Pliq_result; gamma: cdouble): liq_error;                                                                                                      syscall ImageQuantBase 33;
  function  liq_get_output_gamma(const result: Pliq_result): cdouble;                                                                                                               syscall ImageQuantBase 34;

  function  liq_get_palette(result: Pliq_result): Pliq_palette;                                                                                                                     syscall ImageQuantBase 35;

  function  liq_write_remapped_image(result: Pliq_result; input_image: Pliq_image; buffer: Pointer; buffer_size: csize_t): liq_error;                                                syscall ImageQuantBase 36;
  function  liq_write_remapped_image_rows(result: Pliq_result; input_image: Pliq_image; row_pointers: PPChar): liq_error;                                                           syscall ImageQuantBase 37;

  function  liq_get_quantization_error(result: Pliq_result): cdouble;                                                                                                               syscall ImageQuantBase 38;
  function  liq_get_quantization_quality(result: Pliq_result): cint;                                                                                                                syscall ImageQuantBase 39;
  function  liq_get_remapping_error(result: Pliq_result): cdouble;                                                                                                                  syscall ImageQuantBase 40;
  function  liq_get_remapping_quality(result: Pliq_result): cint;                                                                                                                   syscall ImageQuantBase 41;

  procedure liq_result_destroy(result: Pliq_result);                                                                                                                                syscall ImageQuantBase 42;

  function  liq_version: cint;                                                                                                                                                      syscall ImageQuantBase 43;



/////////////////////////////////////////////////////////////////////////////
//
//  Macro definitions
//
/////////////////////////////////////////////////////////////////////////////



  function  MACRO_liq_color(r,g,b,a: cuchar): Tliq_color; inline;
  



implementation



/////////////////////////////////////////////////////////////////////////////
//
//  Macro implementations
//
/////////////////////////////////////////////////////////////////////////////



function  MACRO_liq_color(r,g,b,a: cuchar): Tliq_color;
begin
  Result.r := r;
  Result.g := g;
  Result.b := b;
  Result.a := a;
end;


end.
