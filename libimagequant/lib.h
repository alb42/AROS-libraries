#ifndef LIB_H
#define LIB_H

#define __NOLIBBASE__

#include <exec/libraries.h>
#include <exec/semaphores.h>
#include <dos/dos.h>

#ifdef __AROS__
//#include <aros/debug.h>
#define reg(x)
#define __saveds
#endif

#define USESYSBASE struct ExecBase *SysBase = Base->imagequant_SysBase;

struct imagequantbase
{
    struct Library   imagequant_Lib;
    struct ExecBase *imagequant_SysBase;
    APTR            imagequant_SegList;
    int              testint;
};


#endif