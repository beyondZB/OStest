#include "buffer.h"

void CreateTerminateBuffer(TerminateBuffer* buffer)
{
    buffer->curRow = 0;
    buffer->curColumn = 0;

    int i = 0;
    for(i;i<BUFF_ROW;i++){
	int j = 0;
        for(j;j<BUFF_COLUMN;j++){
            buffer->buffer[i][j] = ' ';
        }
    }
}

void FreeTerminateBuffer(TerminateBuffer* buffer)
{
}
