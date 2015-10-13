/*--------------------------------------------------------------------------*/
/* Resident header written for sqlite3.library */
/*--------------------------------------------------------------------------*/

#define __NOLIBBASE__

#define VERSION                     1
#define REVISION                    0

#define COMPDATE                 "11.10.2015"
#define VERS                           "1.0"
#define LIBHEADNAME           imagequant
#define LIBHEADNAMESTR    "imagequant"

#include <aros/debug.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <exec/resident.h>
#include <exec/nodes.h>
#include <exec/libraries.h>

// #include <sqlite3.h>
#include "libimagequant.h"

#include <aros/symbolsets.h>
#include "lib.h"

const UBYTE lib_name[] = LIBHEADNAMESTR ".library";

const UBYTE lib_id[] = "$VER: " LIBHEADNAMESTR ".library " VERS " (" COMPDATE ") by magorium\n";

#define LIBBASETYPE             struct imagequantbase
#define LIBBASETYPEPTR      LIBBASETYPE *

extern const APTR FuncTable[];



AROS_UFP3 (LIBBASETYPEPTR, InitLib,
    AROS_UFPA(LIBBASETYPEPTR, Base, D0),
    AROS_UFPA(BPTR, seglist, A0),
    AROS_UFPA(struct ExecBase *, sysbase, A6)
);


static struct LibInitStruct
{
    IPTR                    LibSize;
    const APTR              *FuncTable;
    const struct DataTable   *DataTable;
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
    RTC_MATCHWORD,                  /* match word */
    (APTR)&romtag,                 /* back pointer */
    (APTR)(&romtag + 1),           /* skip pointer */
    RTF_AUTOINIT | RTF_EXTENDED,    /* flags */
    VERSION,                        /* version */
    NT_LIBRARY,                     /* type of module */
    0,                              /* init priority */
    (STRPTR)lib_name,               /* module name */
    (STRPTR)lib_id + 6,
    (APTR)&LibInitStruct,
    REVISION, 
    NULL
};



AROS_UFH3 (LIBBASETYPEPTR, InitLib,
    AROS_UFHA(LIBBASETYPEPTR, Base, D0),
    AROS_UFHA(BPTR, seglist, A0),
    AROS_UFHA(struct ExecBase *, sysbase, A6)
    )
{
    AROS_USERFUNC_INIT

    Base->imagequant_SegList = seglist;
    Base->imagequant_SysBase = (APTR)sysbase;
    Base->testint = 0;

    USESYSBASE
    bug("InitLib\n");

    if (!set_open_libraries())
    {
      set_close_libraries();
      return NULL;
    }

    return Base;

    AROS_USERFUNC_EXIT
}


AROS_LH1(LIBBASETYPEPTR, LibOpen,
    AROS_LHA (ULONG, version, D0),
    LIBBASETYPEPTR, Base, 1, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("LibOpen\n");

    (void)version;
    
    Base->imagequant_Lib.lib_OpenCnt++;
    return Base;

    AROS_LIBFUNC_EXIT
    
}


__saveds APTR LibExpungeInternal(LIBBASETYPE *Base reg(a6))
{
  USESYSBASE
  APTR seglist;
  bug("LibExpungeInternal\n");

  if (Base->imagequant_Lib.lib_OpenCnt)
  {
    return 0;
  }

  seglist = Base->imagequant_SegList;

  Forbid();
  Remove((struct Node*)Base);
  Permit();
  FreeMem((APTR)Base - Base->imagequant_Lib.lib_NegSize, (LONG)Base->imagequant_Lib.lib_PosSize +
    (LONG)Base->imagequant_Lib.lib_NegSize);
  
  set_close_libraries();
  return seglist;
}



AROS_LH0(BPTR, LibClose,
    LIBBASETYPEPTR, Base, 2, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT
    USESYSBASE
    bug("LibClose\n");

    if (!(--Base->imagequant_Lib.lib_OpenCnt))
    {
      return LibExpungeInternal(Base);          
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

    return LibExpungeInternal(Base);

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


// Space for your own functions
// do not forget to update the FuncTable as well



AROS_LH0(liq_attr*, liq_attr_create,
    LIBBASETYPEPTR, Base, 5, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_attr_create\n");

    return liq_attr_create();
    
    AROS_LIBFUNC_EXIT
}


/*
type 
  TMallocFunc : function(n: size_t): Pointer
  TFreeFunc   : procedure(n: Pointer);
*/


AROS_LH2(liq_attr*, liq_attr_create_with_allocator,
    AROS_LHA(void *, mallocfunc, A0),
    AROS_LHA(void *, freefunc  , A1),
    LIBBASETYPEPTR, Base, 6, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_attr_create_with_allocator\n");

    return liq_attr_create_with_allocator(mallocfunc, freefunc);
    
    AROS_LIBFUNC_EXIT
}



AROS_LH1(liq_attr*, liq_attr_copy,
    AROS_LHA(liq_attr *, orig, A0),
    LIBBASETYPEPTR, Base, 7, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_attr_copy\n");

    return liq_attr_copy(orig);
    
    AROS_LIBFUNC_EXIT
}



AROS_LH1(void , liq_attr_destroy,
    AROS_LHA(liq_attr *, attr, A0),
    LIBBASETYPEPTR, Base, 8, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_attr_destroy\n");

    liq_attr_destroy(attr);
    
    AROS_LIBFUNC_EXIT
}



AROS_LH2(liq_error, liq_set_max_colors,
    AROS_LHA(liq_attr *, attr, A0),
    AROS_LHA(int, colors  , D0),
    LIBBASETYPEPTR, Base, 9, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_set_max_colors\n");

    return liq_set_max_colors(attr, colors);
    
    AROS_LIBFUNC_EXIT
}



AROS_LH1(int , liq_get_max_colors,
    AROS_LHA(const liq_attr *, attr, A0),
    LIBBASETYPEPTR, Base, 10, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_get_max_colors\n");

    return liq_get_max_colors(attr);
    
    AROS_LIBFUNC_EXIT
}



AROS_LH2(liq_error, liq_set_speed,
    AROS_LHA(liq_attr *, attr, A0),
    AROS_LHA(int, speed  , D0),
    LIBBASETYPEPTR, Base, 11, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_set_speed\n");

    return liq_set_speed(attr, speed);
    
    AROS_LIBFUNC_EXIT
}



AROS_LH1(int , liq_get_speed,
    AROS_LHA(const liq_attr *, attr, A0),
    LIBBASETYPEPTR, Base, 12, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_get_speed\n");

    return liq_get_speed(attr);
    
    AROS_LIBFUNC_EXIT
}



AROS_LH2(liq_error, liq_set_min_opacity,
    AROS_LHA(liq_attr *, attr, A0),
    AROS_LHA(int, min  , D0),
    LIBBASETYPEPTR, Base, 13, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_set_min_opacity\n");

    return liq_set_min_opacity(attr, min);
    
    AROS_LIBFUNC_EXIT
}



AROS_LH1(int , liq_get_min_opacity,
    AROS_LHA(const liq_attr *, attr, A0),
    LIBBASETYPEPTR, Base, 14, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_get_min_opacity\n");

    return liq_get_min_opacity(attr);
    
    AROS_LIBFUNC_EXIT
}




AROS_LH2(liq_error, liq_set_min_posterization,
    AROS_LHA(liq_attr *, attr, A0),
    AROS_LHA(int, bits  , D0),
    LIBBASETYPEPTR, Base, 15, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_set_min_posterization\n");

    return liq_set_min_posterization(attr, bits);
    
    AROS_LIBFUNC_EXIT
}



AROS_LH1(int , liq_get_min_posterization,
    AROS_LHA(const liq_attr *, attr, A0),
    LIBBASETYPEPTR, Base, 16, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_get_min_posterization\n");

    return liq_get_min_posterization(attr);
    
    AROS_LIBFUNC_EXIT
}



AROS_LH3(liq_error, liq_set_quality,
    AROS_LHA(liq_attr *, attr, A0),
    AROS_LHA(int, minimum  , D0),
    AROS_LHA(int, maximum  , D1),
    LIBBASETYPEPTR, Base, 17, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_set_quality\n");

    return liq_set_quality(attr, minimum, maximum);
    
    AROS_LIBFUNC_EXIT
}



AROS_LH1(int , liq_get_min_quality,
    AROS_LHA(const liq_attr *, attr, A0),
    LIBBASETYPEPTR, Base, 18, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_get_min_quality\n");

    return liq_get_min_quality(attr);
    
    AROS_LIBFUNC_EXIT
}


AROS_LH1(int , liq_get_max_quality,
    AROS_LHA(const liq_attr *, attr, A0),
    LIBBASETYPEPTR, Base, 19, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_get_max_quality\n");

    return liq_get_max_quality(attr);
    
    AROS_LIBFUNC_EXIT
}



AROS_LH2(void, liq_set_last_index_transparent,
    AROS_LHA(liq_attr *, attr, A0),
    AROS_LHA(int, is_last  , D0),
    LIBBASETYPEPTR, Base, 20, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_set_last_index_transparent\n");

    liq_set_last_index_transparent(attr, is_last);
    
    AROS_LIBFUNC_EXIT
}


typedef void liq_log_callback_function(const liq_attr*, const char *message, void* user_info);
typedef void liq_log_flush_callback_function(const liq_attr*, void* user_info);



AROS_LH3(void, liq_set_log_callback,
    AROS_LHA(liq_attr *, attr, A0),
    AROS_LHA(liq_log_callback_function *, func , A1),
    AROS_LHA(void *, user_info, A2),
    LIBBASETYPEPTR, Base, 21, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_set_log_callback\n");

    liq_set_log_callback(attr, func, user_info);
    
    AROS_LIBFUNC_EXIT
}



AROS_LH3(void, liq_set_log_flush_callback,
    AROS_LHA(liq_attr *, attr, A0),
    AROS_LHA(liq_log_flush_callback_function *, func , A1),
    AROS_LHA(void *, user_info, A2),
    LIBBASETYPEPTR, Base, 22, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_set_log_flush_callback\n");

    liq_set_log_flush_callback(attr, func, user_info);
    
    AROS_LIBFUNC_EXIT
}



AROS_LH5(liq_image *, liq_image_create_rgba_rows,
    AROS_LHA(liq_attr *, attr, A0),
    AROS_LHA(void *, rows[], A1),
    AROS_LHA(int, width  , D0),
    AROS_LHA(int, height  , D1),
    AROS_LHA(double, gamma , D2),
    LIBBASETYPEPTR, Base, 23, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_image_create_rgba_rows\n");

    return liq_image_create_rgba_rows(attr, rows, width, height, gamma);
    
    AROS_LIBFUNC_EXIT
}



AROS_LH5(liq_image *, liq_image_create_rgba,
    AROS_LHA(liq_attr *, attr, A0),
    AROS_LHA(void *, bitmap, A1),
    AROS_LHA(int, width  , D0),
    AROS_LHA(int, height  , D1),
    AROS_LHA(double, gamma , D2),
    LIBBASETYPEPTR, Base, 24, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_image_create_rgba\n");

    return liq_image_create_rgba(attr, bitmap, width, height, gamma);
    
    AROS_LIBFUNC_EXIT
}


typedef void liq_image_get_rgba_row_callback(liq_color row_out[], int row, int width, void* user_info);



AROS_LH6(liq_image *, liq_image_create_custom,
    AROS_LHA(liq_attr *, attr, A0),
    AROS_LHA(liq_image_get_rgba_row_callback *, row_callback, A1),
    AROS_LHA(void *, user_info, A2),
    AROS_LHA(int, width  , D0),
    AROS_LHA(int, height  , D1),
    AROS_LHA(double, gamma , D2),
    LIBBASETYPEPTR, Base, 25, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_image_create_custom\n");

    return liq_image_create_custom(attr, row_callback, user_info, width, height, gamma);
    
    AROS_LIBFUNC_EXIT
}




AROS_LH2(liq_error, liq_image_set_memory_ownership,
    AROS_LHA(liq_image * , image, A0),
    AROS_LHA(int , ownership_flags, D0),
    LIBBASETYPEPTR, Base, 26, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_image_set_memory_ownership\n");

    return liq_image_set_memory_ownership(image, ownership_flags);
    
    AROS_LIBFUNC_EXIT
}



AROS_LH2(liq_error, liq_image_add_fixed_color,
    AROS_LHA(liq_image * , img, A0),
    AROS_LHA(liq_color , color, A1),  // ??
    LIBBASETYPEPTR, Base, 27, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_image_add_fixed_color\n");

    return liq_image_add_fixed_color(img, color);
    
    AROS_LIBFUNC_EXIT
}




AROS_LH1(int , liq_image_get_width,
    AROS_LHA(const liq_image * , img, A0),
    LIBBASETYPEPTR, Base, 28, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_image_get_width\n");

    return liq_image_get_width(img);
    
    AROS_LIBFUNC_EXIT
}



AROS_LH1(int , liq_image_get_height,
    AROS_LHA(const liq_image * , img, A0),
    LIBBASETYPEPTR, Base, 29, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_image_get_height\n");

    return liq_image_get_height(img);
    
    AROS_LIBFUNC_EXIT
}



AROS_LH1(void , liq_image_destroy,
    AROS_LHA(const liq_image * , img, A0),
    LIBBASETYPEPTR, Base, 30, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_image_destroy\n");

    liq_image_destroy(img);
    
    AROS_LIBFUNC_EXIT
}



AROS_LH2(liq_result *, liq_quantize_image,
    AROS_LHA(liq_attr * , options, A0),
    AROS_LHA(liq_image * , input_image, A1),
    LIBBASETYPEPTR, Base, 31, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_quantize_image\n");

    return liq_quantize_image(options, input_image);
    
    AROS_LIBFUNC_EXIT
}




AROS_LH2(liq_error, liq_set_dithering_level,
    AROS_LHA(liq_result * , res, A0),
    AROS_LHA(float , dither_level, D0),  // ??
    LIBBASETYPEPTR, Base, 32, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_set_dithering_level\n");

    return liq_set_dithering_level(res, dither_level);
    
    AROS_LIBFUNC_EXIT
}


AROS_LH2(liq_error, liq_set_output_gamma,
    AROS_LHA(liq_result * , res, A0),
    AROS_LHA(double , gamma, D0),  // ??
    LIBBASETYPEPTR, Base, 33, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_set_dithering_level\n");

    return liq_set_dithering_level(res, gamma);
    
    AROS_LIBFUNC_EXIT
}


//AROS_LH1(int, liq_get_output_gamma,
AROS_LH1(double, liq_get_output_gamma,
    AROS_LHA(const liq_result * , result, A0),
    LIBBASETYPEPTR, Base, 34, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_get_output_gamma\n");

    return liq_get_output_gamma(result);
    
    AROS_LIBFUNC_EXIT
}



AROS_LH1(const liq_palette *, liq_get_palette,
    AROS_LHA(liq_result * , result, A0),
    LIBBASETYPEPTR, Base, 35, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_get_palette\n");

    return liq_get_palette(result);
    
    AROS_LIBFUNC_EXIT
}



AROS_LH4(liq_error , liq_write_remapped_image,
    AROS_LHA(liq_result * , result, A0),
    AROS_LHA(liq_image * , input_image, A1),
    AROS_LHA(void * , buffer, A2),
    AROS_LHA(size_t , buffer_size, A3),
    LIBBASETYPEPTR, Base, 36, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_write_remapped_image\n");

    return liq_write_remapped_image(result, input_image, buffer, buffer_size);
    
    AROS_LIBFUNC_EXIT
}



AROS_LH3(liq_error , liq_write_remapped_image_rows,
    AROS_LHA(liq_result * , result, A0),
    AROS_LHA(liq_image * , input_image, A1),
    AROS_LHA(unsigned char ** , row_pointers, A2),
    LIBBASETYPEPTR, Base, 37, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_write_remapped_image_rows\n");

    return liq_write_remapped_image_rows(result, input_image, row_pointers);
    
    AROS_LIBFUNC_EXIT
}






AROS_LH1(double , liq_get_quantization_error,
    AROS_LHA(liq_result * , result, A0),
    LIBBASETYPEPTR, Base, 38, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_get_quantization_error\n");

    return liq_get_quantization_error(result);
    
    AROS_LIBFUNC_EXIT
}

AROS_LH1(int , liq_get_quantization_quality,
    AROS_LHA(liq_result * , result, A0),
    LIBBASETYPEPTR, Base, 39, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_get_quantization_quality\n");

    return liq_get_quantization_quality(result);
    
    AROS_LIBFUNC_EXIT
}

AROS_LH1(double , liq_get_remapping_error,
    AROS_LHA(liq_result * , result, A0),
    LIBBASETYPEPTR, Base, 40, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_get_remapping_error\n");

    return liq_get_remapping_error(result);
    
    AROS_LIBFUNC_EXIT
}

AROS_LH1(int , liq_get_remapping_quality,
    AROS_LHA(liq_result * , result, A0),
    LIBBASETYPEPTR, Base, 41, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_get_remapping_quality\n");

    return liq_get_remapping_quality(result);
    
    AROS_LIBFUNC_EXIT
}


AROS_LH1(void , liq_result_destroy,
    AROS_LHA(liq_result * , result, A0),
    LIBBASETYPEPTR, Base, 42, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("liq_result_destroy\n");

    liq_result_destroy(result);
    
    AROS_LIBFUNC_EXIT
}



AROS_LH0(int, liq_version,
    LIBBASETYPEPTR, Base, 43, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("imagequant_version\n");

    return liq_version();
    
    AROS_LIBFUNC_EXIT
}






/*
LIQ_EXPORT LIQ_USERESULT liq_attr* liq_attr_create(void);
LIQ_EXPORT LIQ_USERESULT liq_attr* liq_attr_create_with_allocator(void* (*malloc)(size_t), void (*free)(void*));
LIQ_EXPORT LIQ_USERESULT liq_attr* liq_attr_copy(liq_attr *orig) LIQ_NONNULL;
LIQ_EXPORT void liq_attr_destroy(liq_attr *attr) LIQ_NONNULL;

LIQ_EXPORT liq_error liq_set_max_colors(liq_attr* attr, int colors) LIQ_NONNULL;
LIQ_EXPORT LIQ_USERESULT int liq_get_max_colors(const liq_attr* attr) LIQ_NONNULL;
LIQ_EXPORT liq_error liq_set_speed(liq_attr* attr, int speed) LIQ_NONNULL;
LIQ_EXPORT LIQ_USERESULT int liq_get_speed(const liq_attr* attr) LIQ_NONNULL;
LIQ_EXPORT liq_error liq_set_min_opacity(liq_attr* attr, int min) LIQ_NONNULL;
LIQ_EXPORT LIQ_USERESULT int liq_get_min_opacity(const liq_attr* attr) LIQ_NONNULL;
LIQ_EXPORT liq_error liq_set_min_posterization(liq_attr* attr, int bits) LIQ_NONNULL;
LIQ_EXPORT LIQ_USERESULT int liq_get_min_posterization(const liq_attr* attr) LIQ_NONNULL;
LIQ_EXPORT liq_error liq_set_quality(liq_attr* attr, int minimum, int maximum) LIQ_NONNULL;
LIQ_EXPORT LIQ_USERESULT int liq_get_min_quality(const liq_attr* attr) LIQ_NONNULL;
LIQ_EXPORT LIQ_USERESULT int liq_get_max_quality(const liq_attr* attr) LIQ_NONNULL;
LIQ_EXPORT void liq_set_last_index_transparent(liq_attr* attr, int is_last) LIQ_NONNULL;

typedef void liq_log_callback_function(const liq_attr*, const char *message, void* user_info);
typedef void liq_log_flush_callback_function(const liq_attr*, void* user_info);
LIQ_EXPORT void liq_set_log_callback(liq_attr*, liq_log_callback_function*, void* user_info);
LIQ_EXPORT void liq_set_log_flush_callback(liq_attr*, liq_log_flush_callback_function*, void* user_info);

LIQ_EXPORT LIQ_USERESULT liq_image *liq_image_create_rgba_rows(liq_attr *attr, void* rows[], int width, int height, double gamma) LIQ_NONNULL;
LIQ_EXPORT LIQ_USERESULT liq_image *liq_image_create_rgba(liq_attr *attr, void* bitmap, int width, int height, double gamma) LIQ_NONNULL;

typedef void liq_image_get_rgba_row_callback(liq_color row_out[], int row, int width, void* user_info);
LIQ_EXPORT LIQ_USERESULT liq_image *liq_image_create_custom(liq_attr *attr, liq_image_get_rgba_row_callback *row_callback, void* user_info, int width, int height, double gamma);

LIQ_EXPORT liq_error liq_image_set_memory_ownership(liq_image *image, int ownership_flags) LIQ_NONNULL;
LIQ_EXPORT liq_error liq_image_add_fixed_color(liq_image *img, liq_color color) LIQ_NONNULL;
LIQ_EXPORT LIQ_USERESULT int liq_image_get_width(const liq_image *img) LIQ_NONNULL;
LIQ_EXPORT LIQ_USERESULT int liq_image_get_height(const liq_image *img) LIQ_NONNULL;
LIQ_EXPORT void liq_image_destroy(liq_image *img) LIQ_NONNULL;

LIQ_EXPORT LIQ_USERESULT liq_result *liq_quantize_image(liq_attr *options, liq_image *input_image) LIQ_NONNULL;

LIQ_EXPORT liq_error liq_set_dithering_level(liq_result *res, float dither_level) LIQ_NONNULL;
LIQ_EXPORT liq_error liq_set_output_gamma(liq_result* res, double gamma) LIQ_NONNULL;
LIQ_EXPORT LIQ_USERESULT double liq_get_output_gamma(const liq_result *result) LIQ_NONNULL;

LIQ_EXPORT LIQ_USERESULT const liq_palette *liq_get_palette(liq_result *result) LIQ_NONNULL;

LIQ_EXPORT liq_error liq_write_remapped_image(liq_result *result, liq_image *input_image, void *buffer, size_t buffer_size) LIQ_NONNULL;
LIQ_EXPORT liq_error liq_write_remapped_image_rows(liq_result *result, liq_image *input_image, unsigned char **row_pointers) LIQ_NONNULL;

LIQ_EXPORT double liq_get_quantization_error(liq_result *result) LIQ_NONNULL;
LIQ_EXPORT int liq_get_quantization_quality(liq_result *result) LIQ_NONNULL;
LIQ_EXPORT double liq_get_remapping_error(liq_result *result) LIQ_NONNULL;
LIQ_EXPORT int liq_get_remapping_quality(liq_result *result) LIQ_NONNULL;

LIQ_EXPORT void liq_result_destroy(liq_result *) LIQ_NONNULL;
*/



/*
LIQ_USERESULT liq_attr* liq_attr_create(void);
LIQ_USERESULT liq_attr* liq_attr_create_with_allocator(void* (*malloc)(size_t), void (*free)(void*));
LIQ_USERESULT liq_attr* liq_attr_copy(liq_attr *orig) LIQ_NONNULL;
void liq_attr_destroy(liq_attr *attr) LIQ_NONNULL;

liq_error liq_set_max_colors(liq_attr* attr, int colors) LIQ_NONNULL;
LIQ_USERESULT int liq_get_max_colors(const liq_attr* attr) LIQ_NONNULL;
liq_error liq_set_speed(liq_attr* attr, int speed) LIQ_NONNULL;
LIQ_USERESULT int liq_get_speed(const liq_attr* attr) LIQ_NONNULL;
liq_error liq_set_min_opacity(liq_attr* attr, int min) LIQ_NONNULL;
LIQ_USERESULT int liq_get_min_opacity(const liq_attr* attr) LIQ_NONNULL;
liq_error liq_set_min_posterization(liq_attr* attr, int bits) LIQ_NONNULL;
LIQ_USERESULT int liq_get_min_posterization(const liq_attr* attr) LIQ_NONNULL;
liq_error liq_set_quality(liq_attr* attr, int minimum, int maximum) LIQ_NONNULL;
LIQ_USERESULT int liq_get_min_quality(const liq_attr* attr) LIQ_NONNULL;
LIQ_USERESULT int liq_get_max_quality(const liq_attr* attr) LIQ_NONNULL;
void liq_set_last_index_transparent(liq_attr* attr, int is_last) LIQ_NONNULL;

typedef void liq_log_callback_function(const liq_attr*, const char *message, void* user_info);
typedef void liq_log_flush_callback_function(const liq_attr*, void* user_info);
void liq_set_log_callback(liq_attr*, liq_log_callback_function*, void* user_info);
void liq_set_log_flush_callback(liq_attr*, liq_log_flush_callback_function*, void* user_info);

LIQ_USERESULT liq_image *liq_image_create_rgba_rows(liq_attr *attr, void* rows[], int width, int height, double gamma) LIQ_NONNULL;
 LIQ_USERESULT liq_image *liq_image_create_rgba(liq_attr *attr, void* bitmap, int width, int height, double gamma) LIQ_NONNULL;

typedef void liq_image_get_rgba_row_callback(liq_color row_out[], int row, int width, void* user_info);
LIQ_USERESULT liq_image *liq_image_create_custom(liq_attr *attr, liq_image_get_rgba_row_callback *row_callback, void* user_info, int width, int height, double gamma);

liq_error liq_image_set_memory_ownership(liq_image *image, int ownership_flags) LIQ_NONNULL;
liq_error liq_image_add_fixed_color(liq_image *img, liq_color color) LIQ_NONNULL;
LIQ_USERESULT int liq_image_get_width(const liq_image *img) LIQ_NONNULL;
LIQ_USERESULT int liq_image_get_height(const liq_image *img) LIQ_NONNULL;
void liq_image_destroy(liq_image *img) LIQ_NONNULL;

LIQ_EXPORT LIQ_USERESULT liq_result *liq_quantize_image(liq_attr *options, liq_image *input_image) LIQ_NONNULL;

LIQ_EXPORT liq_error liq_set_dithering_level(liq_result *res, float dither_level) LIQ_NONNULL;
LIQ_EXPORT liq_error liq_set_output_gamma(liq_result* res, double gamma) LIQ_NONNULL;
LIQ_EXPORT LIQ_USERESULT double liq_get_output_gamma(const liq_result *result) LIQ_NONNULL;

LIQ_EXPORT LIQ_USERESULT const liq_palette *liq_get_palette(liq_result *result) LIQ_NONNULL;

LIQ_EXPORT liq_error liq_write_remapped_image(liq_result *result, liq_image *input_image, void *buffer, size_t buffer_size) LIQ_NONNULL;
LIQ_EXPORT liq_error liq_write_remapped_image_rows(liq_result *result, liq_image *input_image, unsigned char **row_pointers) LIQ_NONNULL;

LIQ_EXPORT double liq_get_quantization_error(liq_result *result) LIQ_NONNULL;
LIQ_EXPORT int liq_get_quantization_quality(liq_result *result) LIQ_NONNULL;
LIQ_EXPORT double liq_get_remapping_error(liq_result *result) LIQ_NONNULL;
LIQ_EXPORT int liq_get_remapping_quality(liq_result *result) LIQ_NONNULL;

LIQ_EXPORT void liq_result_destroy(liq_result *) LIQ_NONNULL;
*/





/*
AROS_LH1(int, TestFunction,
    AROS_LHA(int, TestValue, D0),
    LIBBASETYPEPTR, Base, 10, LIBHEADNAME
    )
{
    AROS_LIBFUNC_INIT

    USESYSBASE
    bug("TestFunction\n");

    Base->testint = TestValue + Base->testint;
    
    return Base->testint;
    
    AROS_LIBFUNC_EXIT
}
*/

// Functable -> Table of all functions in the Library, in right order!


const APTR FuncTable[] = 
{
    &AROS_SLIB_ENTRY(LibOpen                        , LIBHEADNAME,  1),
    &AROS_SLIB_ENTRY(LibClose                       , LIBHEADNAME,  2),
    &AROS_SLIB_ENTRY(LibExpunge                     , LIBHEADNAME,  3),
    &AROS_SLIB_ENTRY(LibReserved                    , LIBHEADNAME,  4),

    &AROS_SLIB_ENTRY(liq_attr_create                , LIBHEADNAME,  5),
    &AROS_SLIB_ENTRY(liq_attr_create_with_allocator , LIBHEADNAME,  6),
    &AROS_SLIB_ENTRY(liq_attr_copy                  , LIBHEADNAME,  7),
    &AROS_SLIB_ENTRY(liq_attr_destroy               , LIBHEADNAME,  8),

    &AROS_SLIB_ENTRY(liq_set_max_colors             , LIBHEADNAME,  9),
    &AROS_SLIB_ENTRY(liq_get_max_colors             , LIBHEADNAME, 10),
    &AROS_SLIB_ENTRY(liq_set_speed                  , LIBHEADNAME, 11),
    &AROS_SLIB_ENTRY(liq_get_speed                  , LIBHEADNAME, 12),
    &AROS_SLIB_ENTRY(liq_set_min_opacity            , LIBHEADNAME, 13),
    &AROS_SLIB_ENTRY(liq_get_min_opacity            , LIBHEADNAME, 14),
    &AROS_SLIB_ENTRY(liq_set_min_posterization      , LIBHEADNAME, 15),
    &AROS_SLIB_ENTRY(liq_get_min_posterization      , LIBHEADNAME, 16),
    &AROS_SLIB_ENTRY(liq_set_quality                , LIBHEADNAME, 17),
    &AROS_SLIB_ENTRY(liq_get_min_quality            , LIBHEADNAME, 18),
    &AROS_SLIB_ENTRY(liq_get_max_quality            , LIBHEADNAME, 19),
    &AROS_SLIB_ENTRY(liq_set_last_index_transparent , LIBHEADNAME, 20),

    &AROS_SLIB_ENTRY(liq_set_log_callback           , LIBHEADNAME, 21),
    &AROS_SLIB_ENTRY(liq_set_log_flush_callback     , LIBHEADNAME, 22),

    &AROS_SLIB_ENTRY(liq_image_create_rgba_rows     , LIBHEADNAME, 23),
    &AROS_SLIB_ENTRY(liq_image_create_rgba          , LIBHEADNAME, 24),

    &AROS_SLIB_ENTRY(liq_image_create_custom        , LIBHEADNAME, 25),

    &AROS_SLIB_ENTRY(liq_image_set_memory_ownership , LIBHEADNAME, 26),
    &AROS_SLIB_ENTRY(liq_image_add_fixed_color      , LIBHEADNAME, 27),
    &AROS_SLIB_ENTRY(liq_image_get_width            , LIBHEADNAME, 28),
    &AROS_SLIB_ENTRY(liq_image_get_height           , LIBHEADNAME, 29),
    &AROS_SLIB_ENTRY(liq_image_destroy              , LIBHEADNAME, 30),

    &AROS_SLIB_ENTRY(liq_quantize_image             , LIBHEADNAME, 31),

    &AROS_SLIB_ENTRY(liq_set_dithering_level        , LIBHEADNAME, 32),
    &AROS_SLIB_ENTRY(liq_set_output_gamma           , LIBHEADNAME, 33),
    &AROS_SLIB_ENTRY(liq_get_output_gamma           , LIBHEADNAME, 34),

    &AROS_SLIB_ENTRY(liq_get_palette                , LIBHEADNAME, 35),

    &AROS_SLIB_ENTRY(liq_write_remapped_image       , LIBHEADNAME, 36),
    &AROS_SLIB_ENTRY(liq_write_remapped_image_rows  , LIBHEADNAME, 37),

    &AROS_SLIB_ENTRY(liq_get_quantization_error     , LIBHEADNAME, 38),
    &AROS_SLIB_ENTRY(liq_get_quantization_quality   , LIBHEADNAME, 39),
    &AROS_SLIB_ENTRY(liq_get_remapping_error        , LIBHEADNAME, 40),
    &AROS_SLIB_ENTRY(liq_get_remapping_quality      , LIBHEADNAME, 41),

    &AROS_SLIB_ENTRY(liq_result_destroy             , LIBHEADNAME, 42),

    &AROS_SLIB_ENTRY(liq_version                    , LIBHEADNAME, 43),
//    &AROS_SLIB_ENTRY(TestFunction       , LIBHEADNAME, 11),
    (void *)-1
};



// AutoInit stuff
void *__PROGRAM_ENTRIES__symbol_set_handler_missing;
void *__LIBS__symbol_set_handler_missing;
// end of AutoInitStuff
