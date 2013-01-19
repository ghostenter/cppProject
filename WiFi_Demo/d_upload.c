/****************************************************************************
*
*文件(File):         d_upload.c
*
*修改(Modify):       2011/7/22 15:34:23
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
| V1.00  2011/7/22 15:34:23           USER 
----------------------------------------------------------------------------
****************************************************************************/
#include "in_call.h"
#include "D12.h"
//void usb_upload();								//USB数据上传
//int USBDownLongString(unsigned char DataBuffer[], unsigned int *DataLen);	//接收从PC端传输过来的数据（移植）
//int USBUpLongString(unsigned char DataBuffer[], unsigned int DataLen);		//向PC端发送数据（移植）
void myitoa(int cnt, char* temp);		//整型转字符串
/***********************************************************************
		module		:	[USB数据上传操作]
		function	:	[usb上传菜单]
  		return		:	[无]
		comment		:	[全局普通函数]
		machine		:	[EH-0818]
		language	:	[CHN]
 		keyword		:	[UPLOAD]
		date		:	[11/07/22]
 		author		:	[chen-zhengkai]
************************************************************************/

void usb_upload_menu()
{
	int select = -1;
	char db_menu_str[] =	"1. USB上传数据";
	BROWINFO	info;
	info.iStr = db_menu_str;		//浏览内容指针
	info.lPtr = 0;					//显示内容iStr的起始显示行
	info.cPtr = 0;					//当前选择行

	while (1) {
		//以下BROWINFO结构成员变量必须参与循环，有可能会被EXT_Brow_Select函数改变
		info.startLine = 0;				//在LCD上的显示起始行
		info.dispLines = 1;				//在LCD上的显示行数
		info.mInt = 1;					//显示内容的总行数
		info.lineMax = 14;				//每行最大字符数
		info.sFont = 0;					//7x9大字体显示
		info.numEnable = 0;				//是否允许数字键代替方向控制
		info.qEvent = EXIT_KEY_F1|EXIT_AUTO_QUIT|EXIT_KEY_POWER|EXIT_KEY_CANCEL;    //可导致函数退出的事件标志
		info.autoexit = 1200;			//自动退出的时间
		//菜单
		Disp_Clear();
		DispStr_CE(0, 0, "usb传输菜单", DISP_CENTER);
		select = EXT_Brow_Select(&info);
		switch (select) {
			case 0:		//usb上传数据
				if(USB_Init()) {
					DispStr_CE(0,2,"未连接",DISP_CENTER|DISP_CLRSCR);
					delay_and_wait_key(20, EXIT_KEY_ALL, 30);															
				}
				if ( !Usb_Data_Transmit() ) {
					DispStr_CE(0,6,"数据已成功上传",DISP_CENTER|DISP_CLRSCR);
					DispStr_CE(0,8,"按任意键返回",DISP_CENTER);
					delay_and_wait_key( 3, EXIT_KEY_ALL, 0 );
				}
				break;
			default:	//降低CPU占用率，降低能耗
				if (info.qEvent == EXIT_KEY_F1  || EXIT_AUTO_QUIT 
                                || EXIT_KEY_POWER || EXIT_KEY_CANCEL) { //返回上级菜单 
					return;
				}
				Sys_Power_Sleep(3);
				break;
		}
	}
}

/***********************************************************************
		module		:	[USB数据上传操作]
		function		:	[usb上传数据，带界面显示]
  		return		:	[无]
		comment	:	[全局普通函数]
		machine		:	[EH-0818]
		language	:	[CHN]
 		keyword		:	[UPLOAD]
		date			:	[11/07/22]
 		author		:	[chen-zhengkai]
************************************************************************/
void usb_upload()
{

	unsigned long	send_len = 0;			//发送字符串的长度
	unsigned char 	send_buf[100] = {0};	//发送字符串
	unsigned char	ret_buf[5] = {0};	//等待应答的缓存
	unsigned int		ret_buf_len = 0;	//等待应带缓存长度
//	char	comState = 0x00;			//通信状态:未连接
//	char state_len = 1;			//字节长度
	int ret = 0;		//表示结果
	char disBuf[20] = {0};	//显示内存1
	char dspBuf[20] = {0};	//显示内存2
	char* p_temp = NULL;
	char retBuf[] = "按任意键返回";
	unsigned long	db_cnt = 0;	//数据库已记录的条数(包括已删除)
	unsigned long	red_cnt = 0;	//有效数据的数目
	char red_flag = 0;				//记录是否有效
	unsigned long idx = 1;			//正在处理的数据序号
	unsigned long cnt = 0;			//计数
	unsigned char temp[20] = {0};	//临时缓存，数据库记录数
	unsigned int temp_len = 0;

	db_cnt = DB_count_records(ATTENDANCE_INDEX);	//返回数据库已记录的条数(包括已删除)
	if (!db_cnt) {
		strcpy(disBuf, "数据库无记录");
		DispStr_CE(0,6,disBuf,DISP_CENTER|DISP_CLRSCR);
		DispStr_CE(0,8,retBuf,DISP_CENTER);
		delay_and_wait_key( 3, EXIT_KEY_ALL, 0 );
		return;
	}
	else {
		if ( USB_Init() ) {	//USB没有连接
			strcpy(disBuf, "USB未连接");
			DispStr_CE(0,6,disBuf,DISP_CENTER|DISP_CLRSCR);
			DispStr_CE(0,8,retBuf,DISP_CENTER);
			delay_and_wait_key( 3, EXIT_KEY_ALL, 0 );
			return;
		}
		else {
#if 1
			strcpy(disBuf, "USB已连接");
			DispStr_CE(0,6,disBuf,DISP_CENTER|DISP_CLRSCR);
			DispStr_CE(0,8,retBuf,DISP_CENTER);
			//Sys_Delay_Sec(1);
			//计算未被删除的记录数
			for (cnt = 0; cnt < db_cnt; cnt++) {
				DB_jump_to_record(0, cnt, &red_flag);
				if ( !red_flag ) {	//记录存在并且未被删除
					red_cnt++;
				}
			}
			while (1) {
				if ( KEY_Read() == KEY_F1 ) {
					return;
				}
				if ( !USBDownLongString(temp, &temp_len) ) {	//接受开始讯号
					ret =1;		//收到信息
				}
				if ( !strncmp(temp, "start", 5) && ret == 1) {
					ret = 2;		//开始上传
				}
				//上传记录
				if ( ret == 2 ) {
					DispStr_CE(0,6,"OYEAH",DISP_CENTER|DISP_CLRSCR);
					DispStr_CE(0,8,"按任意键继续",DISP_CENTER);
					delay_and_wait_key( 3, EXIT_KEY_ALL, 0 );
					sprintf(disBuf, "共%d条记录", red_cnt);
					DispStr_CE(0,6,disBuf,DISP_CENTER|DISP_CLRSCR);

//					for ( cnt = 0; cnt < db_cnt && ret == 2; cnt++ ) {
						//显示
					sprintf(dspBuf, "正在处理第%d条", idx);
					DispStr_CE(0,8,dspBuf,DISP_CENTER|DISP_CLRLINE);
						//上传记录
					cnt = 0;	//debug
					p_temp = (char*)DB_jump_to_record(0, cnt, &red_flag);
					if (!red_flag) {	//记录存在并且未被删除
						memcpy(send_buf, p_temp, sizeof(USER_INFO));		//存到发送缓存
						send_len = strlen(send_buf);	//发送数据长度
//						char s[20] = {0};
//						myitoa(send_len, s);
//						debugFun(s);		//等待时间过长会蓝屏
						if ( USBUpLongString(send_buf, send_len) ) {	//上传到PC失败
							ret = 0;
							break;
						}
						else {	//上传到PC成功
							idx++;
							ret = 3;	//等待状态，等待下一次发送命令
						}
					}
				}
				if ( idx >= red_cnt ) {
					DispStr_CE(0,6,"数据上传成功",DISP_CENTER|DISP_CLRSCR);
					DispStr_CE(0,8,"按任意键返回",DISP_CENTER);
					delay_and_wait_key( 3, EXIT_KEY_ALL, 0 );
					return;
				}
				else {
					DispStr_CE(0,6,"连接中断",DISP_CENTER|DISP_CLRSCR);
					DispStr_CE(0,8,"按任意键返回",DISP_CENTER);
					delay_and_wait_key( 3, EXIT_KEY_ALL, 0 );
					return;
				}
			Sys_Delay_MS(3);
		}
#else
			while (1) {
				char data[10] = {0};
				int dataLen = 0;
				if ( KEY_Read() == KEY_F1 ) {
					return;
				}
				if ( !USBDownLongString(data, &dataLen) ) {
					DispStr_CE(0,6,"OYEAH",DISP_CENTER|DISP_CLRSCR);
					DispStr_CE(0,8,data,DISP_CENTER);
					delay_and_wait_key( 3, EXIT_KEY_ALL, 0 );
					if ( !USBUpLongString("1234567890", 10) ) {
						break;	
					}
				}
				Sys_Delay_MS(3);
			}
#endif
		}
	}
}
/***********************************************************************
		module		:	[USB数据上传操作]
		function		:	[数据库记录转字符串]
  		return		:	[无]
		comment	:	[全局普通函数]
		machine		:	[EH-0818]
		language	:	[CHN]
 		keyword		:	[UPLOAD]
		date			:	[11/07/25]
 		author		:	[chen-zhengkai]
************************************************************************/
void db_to_str(unsigned char* buf, int bufMax, int cnt)	/*buf：输出字符串	bufmax：字符串最大容量	cnt：记录数*/
{
	int recIdx = 0;
	char* p_rec = NULL;
	char	rec_flag = 0;
	int idx_record = 0;
	
	for (; recIdx <cnt; recIdx++) {
		p_rec = (char*)DB_jump_to_record(0, recIdx, &rec_flag);
		if (!rec_flag) {	//记录存在并且未被删除
			memcpy(buf+idx_record, p_rec, sizeof(USER_INFO));
			idx_record += sizeof(USER_INFO);
			if (idx_record >= bufMax) {
				return;
			}
		}
	}
}
#if 0
/***********************************************************************
		module		:	[USB数据上传操作]
		function		:	[向PC端发送数据]
  		return		:	[0 操作成功
						-1 F1键返回
						-2 没有联接上PC]
		comment	:	[全局普通函数]
		machine		:	[EH-0818]
		language	:	[CHN]
 		keyword		:	[UPLOAD]
		date			:	[11/07/25]
 		author		:	[深圳]
************************************************************************/
int USBUpLongString(unsigned char DataBuffer[], unsigned int DataLen)
{
	int blk_cnt;						// 块数量
	int blk_res;						// 不足1024字节的剩余字节数
	int blk_idx=0;					// 循环变量
	int cnt_now;						// 本次发送字节数
	int chr_idx;						// 块内循环变量
	short ret = -1;					// 函数返回值		
	unsigned short i_st;					// 中断类型
	unsigned char recvbuf[4];		// 接收缓冲区
	/*
	if(USB_Init())
	{
		ret = -2;
		return ret;
	}
	*/
	blk_cnt = (DataLen / 64);						// 计算参数
	blk_res = (DataLen % 64);
	blk_cnt += ( (blk_res>0) ? 1 : 0 );
	blk_res = ((blk_res==0) ? 64 : blk_res);
	
	for(;;)
	{
		if(KEY_Read() == KEY_F1)	
		{
			ret = -1;
			goto end;
		}
		i_st = D12_Read_Interrupt_Register();				// 读取PDIUSBD12中断寄存器值
		if(i_st != 0) 
		{
			if(i_st & D12_INT_SUSPENDCHANGE)
				USB_Suspend();								// 总线挂起改变
			if(i_st & D12_INT_ENDP0IN)
				USB_EP0_Txdone();							// 控制端点发送数据处理
			if(i_st & D12_INT_ENDP0OUT)
				USB_EP0_Rxdone();							// 控制端点接收数据处理
			if(i_st & D12_INT_ENDP1OUT)
			{
				D12_Read_Last_Status(2);
				D12_Read_Endpoint(2,1,recvbuf);
				if(recvbuf[0] == 'S')
				{					
					D12_Read_Last_Status(3);
					D12_Write_Endpoint(3, 4, (unsigned char*)&DataLen);			
				}
				if(recvbuf[0] == 'E') 
				{
					break;
				}				
			}
			if(i_st & D12_INT_ENDP2OUT)
			{	
				D12_Read_Last_Status(4);
				D12_Read_Endpoint(4, 1, recvbuf);
				if(recvbuf[0] == 'S')
				{
					for(chr_idx = 0; chr_idx < 2; chr_idx++)	// 一次发送 128 byte
					{
						if(blk_idx >= blk_cnt)
							break;
							 		
						cnt_now = ((blk_idx != (blk_cnt - 1)) ? 64 : blk_res );
						D12_Read_Last_Status(5);
						D12_Write_Endpoint(5, cnt_now, (DataBuffer + 64 * blk_idx));
						blk_idx++;	
					}
				}
			}
		}
	}
	ret = 0;
end:
//	USB_Disconnect();
	return ret;
}
#endif
/***********************************************************************
		module		:	[USB数据上传操作]
		function		:	[整型数转字符串]
  		return		:	[无]
		comment	:	[全局普通函数]
		machine		:	[EH-0818]
		language	:	[CHN]
 		keyword		:	[UPLOAD]
		date			:	[11/08/05]
 		author		:	[chenzhengkai]
************************************************************************/
void myitoa(int cnt, char* temp)
{
	char t[20] = {0};
	int i = 0;
	int n = 0;
	if (cnt == 0) {
		temp[0] = '0';
		temp[1] = 0;
	}
	for (i = 0; cnt>0; i++) {
		t[i] = cnt%10 + '0';
		cnt /= 10;
		n++;
	}
	for (i = 0; i < n; i++) {
		temp[i] = t[n-1-i];
	}
}


