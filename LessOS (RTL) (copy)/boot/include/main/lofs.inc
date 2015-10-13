RootDirSectors		equ	1	; 根目录占用空间,这里暂时设置为1，即根目录最多存放32个文件.
SecPerTrk		equ	18	;每磁道扇区数
BlockSize 		equ	512	;每块512字节
InodeSize		equ	32	;每个inode大小为32字节 

INDEX_DBIDX_COUNT		equ	4 	;4个直接索引块
INDEX_L1IDBIDX_COUNT	equ	2 	;2个一级间接索引块
INDEX_L2IDBIDX_COUNT	equ	1 	;1个二级间接索引块
PARTITION_BEGIN_BLOCK 	equ	3 	;从第三个扇区开始，1号扇区为mbr，2-3号扇区为嵌入式扇区
EMBEDDED_SECTOR		equ 	1 	;1-2号扇区为嵌入式扇区