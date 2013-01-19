/****************************************************************************
*
*文件(File):         D12.h
*
*修改(Modify):       2011/8/4 16:07:17
*
*作者(Author):       USER
*
*编绎(Compile):      智能平台(Smart Platform)
*
*描述(Description):
*
*
*
----------------------------------------------------------------------------
|
| Version | Datetime             |   Author    | Description
| --------+----------------------+-------------+----------------------------
|
| V1.00  2011/8/4 16:07:17           USER 
----------------------------------------------------------------------------
****************************************************************************/


 #ifndef   D12_h_2011_08_04_19_07_17_INCLUDED
 #define   D12_h_2011_08_04_19_07_17_INCLUDED
 #define D12_NOLAZYCLOCK			0x02
#define D12_CLOCKRUNNING        0x04
#define D12_INTERRUPTMODE		0x08
#define D12_SOFTCONNECT			0x10
#define D12_ENDP_NONISO			0x00
#define D12_ENDP_ISOOUT			0x40
#define D12_ENDP_ISOIN			0x80
#define D12_ENDP_ISOIO			0xC0

#define D12_CLOCK_12M			0x03
#define D12_CLOCK_4M			0x0b
#define D12_SETTOONE            0x40
#define D12_SOFONLY				0x80

#define D12_DMASINGLE			0x00
#define D12_BURST_4				0x01
#define D12_BURST_8				0x02
#define D12_BURST_16			0x03
#define D12_DMAENABLE           0x04
#define D12_DMA_INTOKEN			0x08
#define D12_AUTOLOAD			0x10
#define D12_NORMALPLUSSOF		0x20
#define D12_ENDP4INTENABLE		0x40
#define D12_ENDP5INTENABLE		0x80

#define D12_INT_ENDP0OUT		0x01
#define D12_INT_ENDP0IN			0x02
#define D12_INT_ENDP1OUT		0x04
#define D12_INT_ENDP1IN			0x08
#define D12_INT_ENDP2OUT		0x10
#define D12_INT_ENDP2IN			0x20
#define D12_INT_BUSRESET		0x40
#define D12_INT_SUSPENDCHANGE	0x80
#define D12_INT_EOT				0x0100

#define D12_SETUPPACKET			0x20

#define D12_BUFFER0FULL			0x20
#define D12_BUFFER1FULL			0x40

#define D12_FULLEMPTY			0x01
#define D12_STALL				0x02



 #endif   /*D12_h_2011_08_04_19_07_17_INCLUDED*/
