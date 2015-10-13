#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "proc.h"
#include "global.h"
#include "keyboard.h"
#include "tty.h"
#include "vkey.h"

void InitTty(){
	int i =0;
        for(i;i<TTY_NUM;i++){
                 tty_list[i].buffer = &buffer_list[i];
                 tty_list[i].startRow = 0;
                 CreateTeletype(&tty_list[i]);
        }
        tty = tty_list[0];

}

void CreateTeletype(Teletype* tty){
    tty->startRow = 0;
    CreateTerminateBuffer(tty->buffer);
}

TerminateBuffer* GetTerminateBuffer(){
    return tty.buffer;
}

Buff GetBuffer(){
    return tty.buffer->buffer;
}

void GetCursor(Cursor *cursor){
    cursor->curRow = disp_pos / 160;
    cursor->curColumn = (disp_pos % 160) / 2 + 1; 
}

void StartNewRow(){
    TerminateBuffer* terminateBuffer = tty.buffer;
    if(tty.startRow>0 && terminateBuffer->curRow-tty.startRow>=TTY_ROW){
        tty.startRow = terminateBuffer->curRow-TTY_ROW+1;
        return;
    }

    if(terminateBuffer->curRow>=TTY_ROW-1){
        int startRow = tty.startRow;
        if(startRow + TTY_ROW >=BUFF_ROW){
            return;
        }
        tty.startRow = startRow+1;
    }

    terminateBuffer->curRow++;
    terminateBuffer->curColumn = 0;
}

void WriteCharacter(VirtualKey k){
    TerminateBuffer* terminateBuffer = tty.buffer;
    terminateBuffer->buffer[terminateBuffer->curRow][terminateBuffer->curColumn] = k;
    if(terminateBuffer->curColumn>=BUFF_COLUMN-1){
        terminateBuffer->curRow ++;
        terminateBuffer->curColumn = 0;
    }else{
        terminateBuffer->curColumn ++;
    }
    if(tty.startRow>0 && terminateBuffer->curRow-tty.startRow>=TTY_ROW){
        tty.startRow = terminateBuffer->curRow-TTY_ROW+1;
    }
}

void DeleteLastCharacter(){
    TerminateBuffer* buffer = tty.buffer;
    if(buffer->curRow==0 && buffer->curColumn==0){
        return;
    }
    if(buffer->curColumn==0){
        buffer->curRow--;
        int newColumn = BUFF_COLUMN-1;
        while(buffer->buffer[buffer->curRow][newColumn]==' ' && newColumn!=-1){
            newColumn--;
        }
        buffer->curColumn = newColumn+1;
        if(tty.startRow>0){
            tty.startRow--;
        }
        return;
    }
    if(buffer->curColumn!=0){
        buffer->buffer[buffer->curRow][--buffer->curColumn]= ' ';
        return;
    }
}

void TeletypeOutputChar(VirtualKey key)
{
	//3. new row
	if(key == '\n'){
	    if(!CheckIsPrintf()){
	        StartNewRow();
	    }
	    return;
	}

	//9. normal routine
	WriteCharacter(key);
}

PUBLIC void refreshTty(){
	disp_pos = 0;
	Buff p = GetBuffer(&tty);
	int startRow = tty.startRow;
	int i = startRow;

    	for(i;i<TTY_ROW+startRow;i++){

		int j = 0;
	        for(j;j<TTY_COLUMN;j++){
	
	        	VirtualKey key  = p[i][j];
			char outs[2] = {' ','\0'};
			outs[0] = key;
			disp_str(outs);
        	}
    	}
}

PUBLIC void TeletypeOutputStr(const char* str)
{
	disp_str(str);
	int i = 0;
	for(i = 0; str[i] != '\0'; i++){
		TeletypeOutputChar(str[i]);
	}
}


PUBLIC u32 sysTeletypeOutputStr(const char* str)
{
	TeletypeOutputStr(str);
	refreshTty();
	return 0;
}

void TeletypeKeyPressed(VirtualKey key, VirtualKey modifyKey){
    // 1. buff full
    if(IsBuffFull()){
        return;
    }

    // 2. delete a buff
    if(IsKeyBackspace(key)){
        DeleteLastCharacter();
        return;
    }

    //3. new row
    if(IsKeyReturn(key)){
        if(!CheckIsPrintf()){
            StartNewRow();
        }
        return;
    }

    //4. home
    if(IsKeyHome(key)){
        Home();
        return;
    }

    //5. end
    if(IsKeyEnd(key)){
        End();
        return;
    }
    
//    //6. switch tty
//    if(IsKeySwitchTty(key,modifyKey)){
//        SwitchTty(key);
//	return;
//    }     

    //7. page up
    if(IsKeyPageUp(key) && IsKeyControlModifiers(modifyKey) ){
        PageUp();
        return;
    }

    //8. page down
    if(IsKeyPageDown(key) && IsKeyControlModifiers(modifyKey) ){
        PageDown();
        return;
    }

    //9. normal routine
    WriteCharacter(key);
}

bool CheckIsPrintf(){
    TerminateBuffer* buffer = tty.buffer;
    if(buffer->curColumn<6){
        return false;
    }
    u8* rowBuff = buffer->buffer[buffer->curRow];
    int index = 0;
    while(rowBuff[index]==32){
        index ++;
    }
    if(buffer->curColumn-index<6){
        return false;
    }
    if(!(rowBuff[index]=='p' && rowBuff[index+1]=='r'
         && rowBuff[index+2]=='i' && rowBuff[index+3]=='n'
         &&rowBuff[index+4]=='t' && rowBuff[index+5]=='f')){
        return false;
    }

    index = index + 6;
    while(rowBuff[index]==32){
        index ++;
    }
    if(index<buffer->curColumn){
        int column = 0;
        int row = buffer->curRow + 1;
	int i = index;
        for(i;i<buffer->curColumn;i++){
            buffer->buffer[row][column++] = buffer->buffer[buffer->curRow][i];
        }

        buffer->curRow += 2;
        buffer->curColumn = 0;
    }
    return true;
}

bool IsBuffFull(){
    TerminateBuffer* terminateBuffer = tty.buffer;
    if(terminateBuffer->curColumn>=BUFF_COLUMN-1 && terminateBuffer->curRow>=BUFF_ROW-1){
        return true;
    }

    return false;
}

void SwitchTty(VirtualKey key){
    if(key==F1){
	tty = tty_list[0];
    }else if(key==F2){
	tty = tty_list[1];
    }else if(key==F3){
	tty = tty_list[2];
    }else if(key==F4){
	tty = tty_list[3];
    }else if(key==F5){
	tty = tty_list[4];
    }else if(key==F6){
	tty = tty_list[5];
    }else if(key==F7){
	tty = tty_list[6];
    }	
}

void PageUp(){
    int startRow = tty.startRow;
    if(startRow<=0){
        return;
    }
    tty.startRow = startRow-1;
}

void PageDown(){
    int startRow = tty.startRow;
    if(startRow>=TTY_ROW){
        return;
    }
    if(tty.buffer->curRow-startRow<=TTY_ROW-1){
        return;
    }
    tty.startRow = startRow+1;
}

void Home(){
    tty.startRow = 0;
}

void End(){
    int startRow = tty.startRow;
    int curRow = tty.buffer->curRow;
    if(startRow==0 && curRow<=TTY_ROW-1){
        return;
    }
    int dis = curRow-(tty.startRow+TTY_ROW-1);
    while(dis>0){
        dis--;
        tty.startRow++;
    }
}

void FreeTty(){
    // TODO
}
