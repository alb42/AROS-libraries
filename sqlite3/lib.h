#ifndef LIB_H
#define LIB_H

#define __NOLIBBASE__

#include <graphics/text.h>
#include <exec/libraries.h>
#include <exec/semaphores.h>
#include <dos/dos.h>

#ifdef __AROS__
//#include <aros/debug.h>
#define reg(x)
#define __saveds
#endif

#define USESYSBASE struct ExecBase *SysBase = Base->sqlite3_SysBase;

struct sqlite3base
{
  struct Library   sqlite3_Lib;
  struct ExecBase *sqlite3_SysBase;
  APTR             sqlite3_SegList;
};


#endif
