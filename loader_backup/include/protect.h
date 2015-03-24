#ifndef _LESSOS_PROTECT_H_
#define _LESSOS_PROTECT_H_

//系统段描述符
typedef struct s_descriptor	/*共8个字节*/
{
	u16	limit_low;		/* Limit Low */
	u16	base_low;		/* Base Low */
	u8	base_mid;		/* Base Mid */
	u8	attr1;			/* P(1) DPL(2) DT(1) TYPE(4) */
	u8	limit_high_attr2;	/* G(1) D(1) 0(1) AVL(1) LimitHigh(4) */
	u8	base_high;		/* Base */
}DESCRIPTOR;

#endif	/*_LESSOS_PROTECT_H_*/
