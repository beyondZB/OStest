#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"


#define NR_sysGetTicks	0
#define NR_sysReadKey	1

//================================================
//		getTicks
//================================================
PUBLIC int getTicks()
{
	return system_call(NR_sysGetTicks, 0, 0, 0, 0, 0);
}

PUBLIC VirtualKey readKey(){
	return (VirtualKey)( system_call(NR_sysReadKey, 0, 0, 0, 0, 0) );
}
