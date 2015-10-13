#ifndef TTY
#define TTY

#include "type.h"
#include "const.h"
#include "buffer.h"

#define TTY_ROW 25
#define TTY_COLUMN 80

typedef struct
{
    TerminateBuffer* buffer;
    int startRow;
}Teletype ;

typedef struct
{
    int curRow;
    int curColumn;
} Cursor;

void InitTty();
void CreateTeletype(Teletype* tty);
TerminateBuffer* GetTerminateBuffer();
Buff GetBuffer();
void GetCursor(Cursor* cursor);
void StartNewRow();
void WriteCharacter(VirtualKey key);
void DeleteLastCharacter();
void KeyPressed(VirtualKey key,VirtualKey modifyKey);
bool CheckIsPrintf();
bool IsBuffFull();
void SwitchTty(VirtualKey Key);
void PageUp();
void PageDown();
void Home();
void End();
void FreeTty();
void refreshTty();
u32 sysTeletypeOutputStr(const char* str);

#endif // TTY


