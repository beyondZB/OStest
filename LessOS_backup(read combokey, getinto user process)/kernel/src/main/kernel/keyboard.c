#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "tty.h"
#include "lofs.h"
#include "proc.h"
#include "global.h"
#include "keyboard.h"
#include "keymap.h"

PRIVATE KB_INPUT	kb_in;	//扫描码缓冲区
PRIVATE KB_INPUT	vk;	//virtualkey缓冲区	

PRIVATE bool	ctrl_l_status = FALSE;
PRIVATE bool	ctrl_r_status = FALSE;
PRIVATE bool	shift_l_status = FALSE;
PRIVATE bool	shift_r_status = FALSE;
PRIVATE bool	alt_l_status = FALSE;
PRIVATE bool	alt_r_status = FALSE;

PRIVATE VirtualKey decorativeKey[NUM_DECORATIVE_KEY] = {CTRL_L, CTRL_R, ALT_L, ALT_R, SHIFT_L, SHIFT_R};
PRIVATE bool* decorativeKeyStatus[NUM_DECORATIVE_KEY] = {&(ctrl_l_status), &(ctrl_r_status), &(alt_l_status), 
					&(alt_r_status), &(shift_l_status), &(shift_r_status)};
PUBLIC ComboKey* kernelReadComboKey();
ComboKey combokey;

PRIVATE bool insert_kbuf(u8 scan_code)
{
	if(kb_in.count < KB_IN_BYTES){
		*(kb_in.p_head) = scan_code;
		kb_in.p_head++;
		if(kb_in.p_head == kb_in.buf + KB_IN_BYTES){
			kb_in.p_head = kb_in.buf;
		}
		kb_in.count++;
		return TRUE;
	}
	return FALSE;
}

PRIVATE bool insert_vkbuf(VirtualKey key){
	if(vk.count < KB_IN_BYTES){
		*(vk.p_head) = key;
		vk.p_head++;
		if(vk.p_head == vk.buf + KB_IN_BYTES){
			vk.p_head = vk.buf;
		}
		vk.count++;
		return TRUE;
	}
	return FALSE;
}

PRIVATE bool insert_kbuf_revocation(){
	if(kb_in.count > 0){
		kb_in.p_head--;
		if(kb_in.p_head == kb_in.buf - 1){
			kb_in.p_head = kb_in.buf + KB_IN_BYTES - 1;
		}
		kb_in.count--;
		return TRUE;
	}
	return FALSE;
}

PRIVATE u8 get_head_pre(){
	char*	pre_head;
	//往前读一个字符看是否为0xe0,判断左右ctrl
	pre_head = kb_in.p_head - 1;
	if(pre_head == kb_in.buf - 1)
		pre_head = kb_in.buf + KB_IN_BYTES - 1;
	return *pre_head;	
}

PRIVATE bool is_decorative_key(VirtualKey key){
	int i = 0;
	for(i = 0; i < NUM_DECORATIVE_KEY; i++){
		if(key == decorativeKey[i])
			return true;
	}
	return false;
}

PUBLIC void keyboard_handler(int irq)
{
	u8 scan_code = in_byte(KB_DATA);
//	disp_int(scan_code);
//	disp_str(" ");
	VirtualKey key = '\0';
	bool make = !(scan_code & FLAG_BREAK);
	bool two_byte_code = FALSE;
	
	//若扫描码为0xe0则则直接插入缓冲区,然后返回
	if(scan_code == 0xe0){
		insert_kbuf(scan_code);
		return;
	}

	//先得到virtualkey
	if(keymap[(scan_code & 0x7f) * MAP_COLS + 2] == 0)
	{
		key = keymap[(scan_code & 0x7f) * MAP_COLS];
	}
	else{
		if(get_head_pre() == 0xe0){
			key = keymap[(scan_code & 0x7f) * MAP_COLS + 2];
			two_byte_code = TRUE;
		}
		else
			key = keymap[(scan_code & 0x7f) * MAP_COLS];
	}

	//只有普通键(非装饰键)的扫描码装入vk缓冲区
	if(make && !is_decorative_key(key)){
		insert_vkbuf(key);
	}
	if(two_byte_code)
		insert_kbuf_revocation();
	

//若为装饰键则要修改状态
	int i = 0;
	for(i = 0; i < NUM_DECORATIVE_KEY; i++){
		if(key == decorativeKey[i]){
			if(make)
				*(decorativeKeyStatus[i]) = VKEY_STATE_PRESSED;
			else
				*(decorativeKeyStatus[i]) = VKEY_STATE_RELEASED;
		}
	}

	kernelUnBlock(readKeyBlockEip);
//	u8 scan_code = in_byte(KB_DATA);
//
//	if (kb_in.count < KB_IN_BYTES) {
//		*(kb_in.p_head) = scan_code;
//		kb_in.p_head++;
//		if(kb_in.p_head == kb_in.buf + KB_IN_BYTES){
//			kb_in.p_head = kb_in.buf;
//		}
//		kb_in.count++;
//	}
}

PUBLIC void clearScreen(){
	disp_pos = 0;
	int i = 0;
	for(i = 0; i < (80 * 25 + 0) * 2; i++)
	{
		disp_str(" ");
	}
	disp_pos = 80 * 2 * 3;

}

PUBLIC void init_keyboard()
{
	kb_in.count = 0;
	kb_in.p_head = kb_in.p_tail = kb_in.buf;

	vk.count = 0;
	vk.p_head = vk.p_tail = vk.buf;

	put_irq_handler(KEYBOARD_IRQ, keyboard_handler);	//设定键盘的中断处理程序
	enable_irq(KEYBOARD_IRQ);				//打开键盘中断
	
	//清屏
	clearScreen();
}

PUBLIC VirtualKey kernelReadKey()       /* 从vk缓冲区中读取下一个字节 */
{
        VirtualKey key;

        while (vk.count <= 0){
		readKeyBlockEip = (p_proc_running - proc_table);
		kernelBlock();
	}

        key = *(vk.p_tail);
        vk.p_tail++;
        if (vk.p_tail == vk.buf + KB_IN_BYTES) {
                vk.p_tail = vk.buf;
        }
        vk.count--;

        return key;
}

PUBLIC int sysReadKey(){
	ComboKey* ck = kernelReadComboKey();
	VirtualKey modifer = 0;
	if(ck->ctrl_l_status || ck->ctrl_r_status){
		modifer = CTRL_L;
	}else if(ck->shift_l_status || ck->shift_r_status){
		modifer = SHIFT_L;
	}else if(ck->alt_l_status || ck->alt_r_status){
		modifer = ALT_L;
	}	
	TeletypeKeyPressed(ck->key,modifer);
	return (int)ck->key;
}

PUBLIC bool kernelGetModifierKeyStatus(VirtualKey key){
	int i = 0;
	for(i = 0; i < NUM_DECORATIVE_KEY; i++){
		if(key == decorativeKey[i]){
			return *(decorativeKeyStatus[i]);
		}
	}
	return FALSE;
}


PUBLIC u32 sysReadComboKey()
{
	return (u32)kernelReadComboKey();
}


PUBLIC ComboKey* kernelReadComboKey()
{
	combokey.key = kernelReadKey();
	combokey.ctrl_l_status = ctrl_l_status;
	combokey.ctrl_r_status = ctrl_r_status;
	combokey.shift_l_status = shift_l_status;
	combokey.shift_r_status = shift_r_status;
	combokey.alt_l_status = alt_l_status;
	combokey.alt_r_status = alt_r_status;
	return &combokey;
}
