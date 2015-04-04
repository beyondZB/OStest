#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"


PUBLIC void init_8259A()
{
	/* Master 8259, ICW1(Initialization Command Word 1)
	   0x11代表需要ICW4,级联的8259,8字节中断向量,edge triggered模式 */
	out_byte(INT_M_CTL, 0x11);

	/* Slave 8259, ICW1
	   0x11代表需要ICW4,级联的8259,8字节中断向量,edge triggered模式 */
	out_byte(INT_S_CTL, 0x11);

	/* Master 8259, ICW2. 设置主8259的中断入口地址为0x20 */
	out_byte(INT_M_CTLMASK, INT_VECTOR_IRQ0);

	/* Slave 8259, ICW2. 设置从8259的中断入口地址为0x28 */
	out_byte(INT_S_CTLMASK, INT_VECTOR_IRQ8);

	/* Master 8259, ICW3. 
	   设置参数为是否有级联从片的位图, 0x40代表IRQ2有级联从片*/
	out_byte(INT_M_CTLMASK, 0x4);

	/* Slave 8259, ICW3.  
	   设置参数为从8259链接主8259的IR号, 0x2代表主片的IRQ2有级联从片*/
	out_byte(INT_S_CTLMASK, 0x2);

	/* Masster 8259, ICW4
	   0x1代表:8086模式,正常OEI,sequential模式*/
	out_byte(INT_M_CTLMASK, 0x1);

	/* Slave 8259, ICW4
	   0x1代表:8086模式,正常OEI,sequential模式*/
	out_byte(INT_S_CTLMASK, 0x1);

	/* Master 8259, OCW1 */
	out_byte(INT_M_CTLMASK, 0xFE);

	/* Slave 8259, OCW1 */
	out_byte(INT_S_CTLMASK, 0xFF);
}


PUBLIC void spurious_irq(int irq)
{
	disp_str("spurious_irq: ");
	disp_int(irq);
	disp_str("\n");
}
