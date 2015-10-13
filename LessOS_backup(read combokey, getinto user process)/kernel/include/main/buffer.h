#ifndef BUFF_H
#define BUFF_H

#include "type.h"
#include "const.h"

#define BUFF_ROW  200
#define BUFF_COLUMN 80

typedef u8(*Buff)[BUFF_COLUMN];

typedef struct
{
    u8 buffer[BUFF_ROW][BUFF_COLUMN];
    int curRow;
    int curColumn;
} TerminateBuffer;

void CreateTerminateBuffer(TerminateBuffer* buffer);
void FreeTerminateBuffer(TerminateBuffer* buffer);

#endif // BUFF_H
