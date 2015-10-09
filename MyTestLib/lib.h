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

#define USESYSBASE struct ExecBase *SysBase = Base->My_SysBase;


struct MyTestBase
{
  struct Library   My_Test_Lib;
	struct ExecBase *My_SysBase;
	APTR             My_SegList;
  int              testint;
};


#endif
