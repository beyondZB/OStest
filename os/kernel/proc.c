#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"



//================================================
//		sysGetTicks
//================================================
PUBLIC int sysGetTicks()
{
	return kernelGetTicks();
}


//================================================
//		kernelGetTicks
//================================================
PUBLIC int kernelGetTicks()
{
	return ticks;
}



