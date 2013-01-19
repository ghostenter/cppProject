/****************************************************************************
*
*�ļ�(File):         mifare.c
*
*�޸�(Modify):       2011/7/28 13:34:51
*
*����(Author):       USER
*
*����(Compile):      ����ƽ̨(Smart Platform)
*
*����(Description):
*
*
*
----------------------------------------------------------------------------
|
| Version | Datetime             |   Author    | Description
| --------+----------------------+-------------+----------------------------
|
| V1.00  2011/7/28 13:34:51           USER 
----------------------------------------------------------------------------
****************************************************************************/
#include "in_call.h"
int M1_start(unsigned char* PICCsnr, unsigned char* ATQ, unsigned char* ATS);	//����M1��
/***********************************************************************
		module		:	[M1����д]
		function		:	[10���ڶ�M1��]
  		return		:	[0:�ɹ�
  						-1:ʧ��]
		comment	:	[ȫ����ͨ����]
		machine		:	[EH-0818]
		language	:	[CHN]
 		keyword		:	[CPU_CARD]
		date			:	[11/07/28]
 		author		:	[chen-zhengkai]
************************************************************************/
char readM1Card(char (*card_r)())
{
	char dspBuf[20] = {0};	//��ʾ�ڴ�
	char retStr[] = "�����������";
	char str[20] = {0};
	unsigned char cnt = 0;	//������ʱ��
	char read_ok = 0;	//������� 0:�ɹ� 	1:ʧ��
	char TempStatus = 0;
	
	if( RCX_Init(CARD_TYPE_14443A) != RCX_OK ) {	//ISO14443Э��TypeA �Ϳ���ʼ��
		DispStr_CE(0,6,"�򿪽ӿڴ���",DISP_POSITION|DISP_CLRSCR);
		DispStr_CE(0,10,retStr,DISP_POSITION);
		delay_and_wait_key( 3, EXIT_KEY_ALL, 0 );
		return -1;
	}
	
	DispStr_CE(0,2,"����10����ˢ��",DISP_CENTER|DISP_CLRSCR);
	Sys_Timer_Start(SYS_TIMER1,64);	//����ϵͳ��ʱ�������Ϊ1��
	for (cnt = READ_CARD_SECOND, read_ok = 1; cnt && read_ok; ) {
		if (!Sys_Timer_Read(SYS_TIMER1)) {	//1���Ӽ�ʱ������ʱ���Զ��ر�
			cnt--;
			sprintf(str, "����%d����ˢ��", cnt);
			DispStr_CE(0,2,str,DISP_CENTER|DISP_CLRLINE);
			Sys_Timer_Start(SYS_TIMER1,64);	//����������ʱ��
		}
		TempStatus = card_r();	//��������
		if (!TempStatus) {	//�����ɹ�
			read_ok = 0;
		}
		if (KEY_Read()==KEY_F1) {
			break;
		}		
	}
	RCX_Close();
	if ( read_ok ) {	//����ʧ��
		if (TempStatus == -1) {
			strcpy(dspBuf, "δ�����Ϸ���");
		}
		else  if (TempStatus == -2) {
			strcpy(dspBuf, "EEPROM��дʧ��");
		}
		DispStr_CE(0, 6, dspBuf, DISP_CENTER|DISP_CLRSCR);
		DispStr_CE(0, 10, retStr, DISP_CENTER);
		delay_and_wait_key( 3, EXIT_KEY_ALL, 0 );
		return -1;
	}
	return 0;
}


char readCpu7Card(char (*card_r)())
{
	char dspBuf[20] = {0};	//��ʾ�ڴ�
	char retStr[] = "�����������";
	char str[20] = {0};
	unsigned char cnt = 0;	//������ʱ��
	char read_ok = 0;	//������� 0:�ɹ� 	1:ʧ��
	char TempStatus = 0;
	
	if( RCX_Init(CARD_TYPE_14443A) != RCX_OK ) {	//ISO14443Э��TypeA �Ϳ���ʼ��
		DispStr_CE(0,6,"�򿪽ӿڴ���",DISP_POSITION|DISP_CLRSCR);
		DispStr_CE(0,10,retStr,DISP_POSITION);
		delay_and_wait_key( 3, EXIT_KEY_ALL, 0 );
		return -1;
	}
	
	DispStr_CE(0,2,"����10����ˢ��",DISP_CENTER|DISP_CLRSCR);
	Sys_Timer_Start(SYS_TIMER1,64);	//����ϵͳ��ʱ�������Ϊ1��
	for (cnt = READ_CARD_SECOND, read_ok = 1; cnt && read_ok; ) {
		if (!Sys_Timer_Read(SYS_TIMER1)) {	//1���Ӽ�ʱ������ʱ���Զ��ر�
			cnt--;
			sprintf(str, "����%d����ˢ��", cnt);
			DispStr_CE(0,2,str,DISP_CENTER|DISP_CLRLINE);
			Sys_Timer_Start(SYS_TIMER1,64);	//����������ʱ��
		}
		TempStatus = card_r();	//��������
		if (!TempStatus) {	//�����ɹ�
			read_ok = 0;
		}
		if (KEY_Read()==KEY_F1) {
			break;
		}		
	}
	RCX_Close();
	if ( read_ok ) {	//����ʧ��
		if (TempStatus == -1) {
			strcpy(dspBuf, "δ�����Ϸ���");
		}
		else  if (TempStatus == -2) {
			strcpy(dspBuf, "EEPROM��дʧ��");
		}
		DispStr_CE(0, 6, dspBuf, DISP_CENTER|DISP_CLRSCR);
		DispStr_CE(0, 10, retStr, DISP_CENTER);
		delay_and_wait_key( 3, EXIT_KEY_ALL, 0 );
		return -1;
	}
	return 0;
}
/***********************************************************************
		module		:	[M1����д]
		function		:	[����Ȩ��M1����ȡ��Կ��]
  		return		:	[0:�����ɹ�
						-1:����ʧ��
						-2:eepromд��ʧ��]
		comment	:	[ȫ����ͨ����]
		machine		:	[EH-0818]
		language	:	[CHN]
 		keyword		:	[CPU_CARD]
		date			:	[11/07/28]
 		author		:	[chen-zhengkai]
************************************************************************/
char readAuthorizeCard()
{
	int	ret = RCX_OK;
	unsigned char 	ATQ[3] = {0};		//����Ӧ��(2 bytes)
	unsigned char 	PICCsnr[5] = {0};	//��������(4 bytes) 
	unsigned char 	ATS[2] = {0};		//ѡ����Ӧ�𣬶���mifare1:0x08(1 byte)
	unsigned char 	rdata[16] = {0};	//��ȡBUFFER
	unsigned char keya[6] = {0};		//keyA��Կ
	char temp[4] = {0};					//�����ŵ�����ID��
	unsigned char main_key[8] = {0};	//����Կ
	int sectno = 0;				//������
	int blockno = 0;			//���
	int key_block = 0;			//
	int key_pos = 0;			//

	ret = M1_start(PICCsnr, ATQ, ATS);

	if (ret == RCX_OK) {	//�������������Կ
		int i = 0;
		for (i = 0; i < 4; i++) {
			temp[i] = PICCsnr[3-i];
		};
		sectno = 7;
		memcpy(keya,temp,4);
		sprintf(temp,"%02X",sectno);
		memcpy(keya+4,temp,2);
		ret = CardMFCAuthKey(PICC_AUTHENT1A,PICCsnr,keya,(4*sectno+0));	//��Կ��֤
	}
	if (ret == RCX_OK) {
		blockno = sectno * 4 + 1;
		ret = CardMFCRead16Bytes(blockno,rdata);	//��ȡ����������һ������ݣ���������0��
		key_block = rdata[6]-'0';
		key_pos = rdata[7] - '0';
	}
	if (ret == RCX_OK) {		//�����ʮ��������Կ
		int i = 0;
		for (i = 0; i < 4; i++) {
			temp[i] = PICCsnr[3-i];
		};
		sectno = 15;
		memcpy(keya,temp,4);
		sprintf(temp,"%02X",sectno);
		memcpy(keya+4,temp,2);
		ret = CardMFCAuthKey(PICC_AUTHENT1A,PICCsnr,keya,(4*sectno+0));	//��Կ��֤
	}
	if (ret == RCX_OK) {
		blockno = sectno * 4 + 2;
		ret = CardMFCRead16Bytes(blockno,rdata);	//��ȡ��ʮ�������ڶ�������ݣ���������0��
	}
	if (ret == RCX_OK) {		//��ȡ����Կ
		memset(temp,0,sizeof(temp));
		memcpy(temp,rdata+14,2);
		sectno = strtoul(temp,NULL,16);
		memcpy(keya,rdata+8,6);
		ret = CardMFCAuthKey(PICC_AUTHENT1A,PICCsnr,keya,(4*sectno+0));	//��Կ��֤
	}
	if (ret == RCX_OK) {
		blockno = 4 * sectno + key_block;
		ret = CardMFCRead16Bytes(blockno,rdata);	//��ȡ����
		memcpy(main_key,rdata+key_pos,8);			//�������Կ
	}
	if (ret == RCX_OK) {
		ret = eeprom_write(main_key, 8, EEPROM_OFFSET_MAIN_KEY);
		if (ret) {
			return -2;		//eepromд��ʧ��
		}
	}
	else {
		return -1;		//��ȡ����Կʧ��
	}
	return 0;
}
/***********************************************************************
		module		:	[M1����д]
		function		:	[����M1��]
  		return		:	[RCX_OK: �ɹ�
  						����: ʧ��]
		comment	:	[ȫ����ͨ����]
		machine		:	[EH-0818]
		language	:	[CHN]
 		keyword		:	[CPU_CARD]
		date			:	[11/08/01]
 		author		:	[chen-zhengkai]
************************************************************************/
int M1_start(unsigned char* PICCsnr, unsigned char* ATQ, unsigned char* ATS)
{
	int ret = RCX_OK;
	ret = CardTypeARequest(PICC_REQALL,ATQ); //�п��ܴ��� Halt ״̬
	if ( ret != RCX_OK) {
		ret = CardTypeARequest(PICC_REQALL,ATQ);//�п��ܴ���active״̬
	}
	if ( ret == RCX_OK ) {
		ret = CardTypeAAnticollLevel1(PICCsnr);	//ִ�з���ͻ����1
	}
	if ( ret == RCX_OK ) {
		ret = CardTypeASelectLevel1(PICCsnr,ATS);	//ѡ�񼤻Ƭ
	}
	return ret;
}

/***********************************************************************
		module		:	[M1����д]
		function		:	[���ڼ�¼(M1�������ƴ�)]
  		return		:	[0:�����ɹ�
						-1:����ʧ��
						-2:eeprom��дʧ��]
		comment	:	[ȫ����ͨ����]
		machine		:	[EH-0818]
		language	:	[CHN]
 		keyword		:	[CPU_CARD]
		date			:	[11/07/28]
 		author		:	[chen-zhengkai]
************************************************************************/
#if 0
char workAttendance_xian()
{
	int	ret = RCX_OK;
	unsigned char 	ATQ[3] = {0};		//����Ӧ��(2 bytes)
	unsigned char 	PICCsnr[5] = {0};	//��������(4 bytes) 
	unsigned char	phyNo[9] = {0};	//��������(8 bytes)		//add chen
	unsigned char 	ATS[2] = {0};		//ѡ����Ӧ�𣬶���mifare1:0x08(1 byte)
	unsigned char 	rdata[16] = {0};	//��ȡBUFFER
	unsigned char keya[6] = {0};		//keyA��Կ
	unsigned char main_key[8] = {0};	//����Կ
	unsigned char num[20] = {0};	//��ʾѧ��
	unsigned char name[10] = {0};	//��ʾ����
	int sectno = 0;				//������
	int blockno = 0;			//���
	char keya_0[6] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};	//��0������keya�ǹ̶��ģ�123456
	USER_INFO	tempInfo;
	typ_TIME		ttime;	//ʱ��ṹ
	typ_DATE	tdate;	//���ڽṹ

	
	ret = M1_start(PICCsnr, ATQ, ATS);
	if (ret == RCX_OK) {
		if (eeprom_read(main_key, sizeof(main_key), EEPROM_OFFSET_MAIN_KEY)) {
			return -2;		//��ȡeepromʧ��
		}
		CalKeyA(PICCsnr, main_key, keya);	//����õ�KEYA
		sectno = 0;
		ret = CardMFCAuthKey(PICC_AUTHENT1A, PICCsnr, keya_0, sectno*4 + 3);	//��֤��0����
	}
	if (ret == RCX_OK) {
		//��ȡѧ�ţ���0������1��͵�2��ǰ4�ֽ���ѧ��
		sectno = 0;
		blockno = sectno*4+1;
		ret = CardMFCRead16Bytes(blockno,rdata);
		memcpy(num, rdata, 16);
		blockno = sectno*4+1;
		ret = CardMFCRead16Bytes(blockno,rdata);
		memcpy(num+16, rdata, 4);
	}
	if (ret == RCX_OK) {
		sectno = 2;
		ret = CardMFCAuthKey(PICC_AUTHENT1A, PICCsnr, keya, sectno*4 + 3);	//��֤��2����
		if (ret == RCX_OK) {
			//��ȡ��������2������0���ǰ8�ֽ�������
			blockno = sectno*4+0;
			ret = CardMFCRead16Bytes(blockno,rdata);
			memcpy(name, rdata, 8);
		}
	}
	//���Ӽ�¼�����ݿ�
	if (ret == RCX_OK) {
		dec2hex(PICCsnr, 4, phyNo);
		memcpy(tempInfo.phyNo, phyNo, 8);
		RTC_Get_Time(&ttime);		//��ȡ����ʱ��
		RTC_Get_Date(&tdate);		//��ȡ��ǰ����
		sprintf(tempInfo.time, "%04d%02d%02d%02d%02d%02d", tdate.year, tdate.month, tdate.day, ttime.hour, ttime.min, ttime.sec);
		ret = DB_add_record(ATTENDANCE_INDEX,  (void *)(&tempInfo));	//�Ѽ�¼���ӵ����ݿ���
		if ( !ret ) {
			return -1;		//��¼����ʧ��
		}
	}
	else {
		return -1;
	}
	warning_beep();
	DispStr_CE(0, 6, (char*)name, DISP_CENTER|DISP_CLRSCR);
	DispStr_CE(0, 10, (char*)num, DISP_CENTER);
	return 0;
}
/***********************************************************************
		module		:	[M1����д������������ʱ�棩]
		function		:	[���ڼ�¼]
  		return		:	[0:�����ɹ�
						-1:����ʧ��
						-2:eeprom��дʧ��]
		comment	:	[ȫ����ͨ����]
		machine		:	[EH-0818]
		language	:	[CHN]
 		keyword		:	[CPU_CARD]
		date			:	[11/09/03]
 		author		:	[chen-zhengkai]
************************************************************************/
char workAttendanceTemp()
{
	int	ret = RCX_OK;
	unsigned char 	ATQ[3] = {0};		//����Ӧ��(2 bytes)
	unsigned char 	PICCsnr[5] = {0};	//��������(4 bytes) 
	unsigned char	phyNo[9] = {0};	//��������(8 bytes)		//add chen
	unsigned char 	ATS[2] = {0};		//ѡ����Ӧ�𣬶���mifare1:0x08(1 byte)
	unsigned char 	rdata[16] = {0};	//��ȡBUFFER
	unsigned char keya[6] = {0};		//keyA��Կ
	unsigned char main_key[8] = {0};	//����Կ
	unsigned char num[20] = {0};	//��ʾѧ��
	unsigned char name[10] = {0};	//��ʾ����
	int sectno = 0;				//������
	int blockno = 0;			//���
	int idx = 0;
	char keya_0[6] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};	//��0������keya�ǹ̶��ģ�123456
	USER_INFO	tempInfo;
	typ_TIME		ttime;	//ʱ��ṹ
	typ_DATE	tdate;	//���ڽṹ

	
	ret = M1_start(PICCsnr, ATQ, ATS);
//	if (ret == RCX_OK) {
//		if (eeprom_read(main_key, sizeof(main_key), EEPROM_OFFSET_MAIN_KEY)) {
//			return -2;		//��ȡeepromʧ��
//		}
//		CalKeyA(PICCsnr, main_key, keya);	//����õ�KEYA
//		sectno = 0;
//		ret = CardMFCAuthKey(PICC_AUTHENT1A, PICCsnr, keya_0, sectno*4 + 3);	//��֤��0����
//	}
//	if (ret == RCX_OK) {
//		//��ȡѧ�ţ���0������1��͵�2��ǰ4�ֽ���ѧ��
//		sectno = 0;
//		blockno = sectno*4+1;
//		ret = CardMFCRead16Bytes(blockno,rdata);
//		memcpy(num, rdata, 16);
//		blockno = sectno*4+1;
//		ret = CardMFCRead16Bytes(blockno,rdata);
//		memcpy(num+16, rdata, 4);
//	}
//	if (ret == RCX_OK) {
//		sectno = 2;
//		ret = CardMFCAuthKey(PICC_AUTHENT1A, PICCsnr, keya, sectno*4 + 3);	//��֤��2����
//		if (ret == RCX_OK) {
//			//��ȡ��������2������0���ǰ8�ֽ�������
//			blockno = sectno*4+0;
//			ret = CardMFCRead16Bytes(blockno,rdata);
//			memcpy(name, rdata, 8);
//		}
//	}
	//���Ӽ�¼�����ݿ�
	if (ret == RCX_OK) {
		dec2hex(PICCsnr, 4, phyNo);
		memcpy(tempInfo.phyNo, phyNo, 8);
		RTC_Get_Time(&ttime);		//��ȡ����ʱ��
		RTC_Get_Date(&tdate);		//��ȡ��ǰ����
		sprintf(tempInfo.time, "%04d%02d%02d%02d%02d%02d", tdate.year, tdate.month, tdate.day, ttime.hour, ttime.min, ttime.sec);
		ret = DB_add_record(ATTENDANCE_INDEX,  (void *)(&tempInfo));	//�Ѽ�¼���ӵ����ݿ���
		if ( !ret ) {
			return -1;		//��¼����ʧ��
		}
	}
	else {
		return -1;
	}
	warning_beep();
//	DispStr_CE(0, 6, (char*)name, DISP_CENTER|DISP_CLRSCR);
//	DispStr_CE(0, 10, (char*)num, DISP_CENTER);
	DispStr_CE(0, 6, "�򿨳ɹ�", DISP_CENTER|DISP_CLRSCR);
	DispStr_CE(0, 10, "��ǩ��", DISP_CENTER);
	return 0;
}
#endif
/***********************************************************************
		module		:	[M1����д]
		function		:	[����ʶ��]
  		return		:	[0:�����ɹ�
						-1:����ʧ��
						-2:eeprom��дʧ��]
		comment	:	[ȫ����ͨ����]
		machine		:	[EH-0818]
		language	:	[CHN]
 		keyword		:	[CPU_CARD]
		date			:	[11/07/28]
 		author		:	[chen-zhengkai]
************************************************************************/
char discernID()
{
	int	ret = RCX_OK;
	unsigned char 	ATQ[3] = {0};		//����Ӧ��(2 bytes)
	unsigned char 	PICCsnr[5] = {0};	//��������(4 bytes) 
	unsigned char 	ATS[2] = {0};		//ѡ����Ӧ�𣬶���mifare1:0x08(1 byte)
	unsigned char 	rdata[16] = {0};	//��ȡBUFFER
	unsigned char keya[6] = {0};		//keyA��Կ
	unsigned char main_key[8] = {0};	//����Կ
	unsigned char num[20] = {0};	//��ʾѧ��
	unsigned char name[10] = {0};	//��ʾ����
	int sectno = 0;				//������
	int blockno = 0;			//���
	char keya_0[6] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};		//��0������keya�ǹ̶��ģ�123456

	ret = M1_start(PICCsnr, ATQ, ATS);
	if (ret == RCX_OK) {
		if (eeprom_read(main_key, sizeof(main_key), EEPROM_OFFSET_MAIN_KEY)) {
			return -2;		//��ȡeepromʧ��
		}
		CalKeyA(PICCsnr, main_key, keya);	//����õ�KEYA
		sectno = 0;
		ret = CardMFCAuthKey(PICC_AUTHENT1A, PICCsnr, keya_0, sectno*4 + 3);	//��֤��0����
	}
	if (ret == RCX_OK) {
		//��ȡѧ�ţ���0������1��͵�2��ǰ4�ֽ���ѧ��
		sectno = 0;
		blockno = sectno*4+1;
		ret = CardMFCRead16Bytes(blockno,rdata);
		memcpy(num, rdata, 16);
		blockno = sectno*4+1;
		ret = CardMFCRead16Bytes(blockno,rdata);
		memcpy(num+16, rdata, 4);
	}
	if (ret == RCX_OK) {
		sectno = 2;
		ret = CardMFCAuthKey(PICC_AUTHENT1A, PICCsnr, keya, sectno*4 + 3);	//��֤��2����
		if (ret == RCX_OK) {
			//��ȡ��������2������0���ǰ8�ֽ�������
			blockno = sectno*4+0;
			ret = CardMFCRead16Bytes(blockno,rdata);
			memcpy(name, rdata, 8);
		}
	}
	if (ret != RCX_OK) {
		return -1;
	}
	warning_beep();
	DispStr_CE(0, 6, (char*)name, DISP_CENTER|DISP_CLRSCR);
	DispStr_CE(0, 10, (char*)num, DISP_CENTER);
	return 0;
}

/***********************************************************************
		module		:	[M1����д]
		function		:	[���ڼ�¼(M1������������)]
  		return		:	[0:�����ɹ�
						-1:����ʧ��
						-2:eeprom��дʧ��]
		comment	:	[ȫ����ͨ����]
		machine		:	[EH-0818]
		language	:	[CHN]
 		keyword		:	[CPU_CARD]
		date			:	[11/09/05]
 		author		:	[chen-zhengkai]
************************************************************************/
#if 0
char workAttendance_Dalian()
{
	int	ret = RCX_OK;
	unsigned char 	ATQ[3] = {0};		//����Ӧ��(2 bytes)
	unsigned char 	PICCsnr[5] = {0};	//��������(4 bytes) 
	unsigned char	phyNo[9] = {0};	//��������(8 bytes)		//add chen
	unsigned char 	ATS[2] = {0};		//ѡ����Ӧ�𣬶���mifare1:0x08(1 byte)
	unsigned char 	rdata[16] = {0};	//��ȡBUFFER
	unsigned char keya[6] = {0};		//keyA��Կ
	//unsigned char keyA[8] = {0};	//����Կ
	unsigned char num[20] = {0};	//��ʾѧ��
	unsigned char name[10] = {0};	//��ʾ����
	int sectno = 0;				//������
	int blockno = 0;			//���
	char keya_0[6] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};	//��0������keya�ǹ̶��ģ�123456
	USER_INFO	tempInfo;
	typ_TIME		ttime;	//ʱ��ṹ
	typ_DATE	tdate;	//���ڽṹ

	
	ret = M1_start(PICCsnr, ATQ, ATS);
	if (ret == RCX_OK) {
		if ( calkeyAfromPSAM(keya, PICCsnr) ) {	//����õ�KEYA
			return -1;		//��ȡkeyaʧ��
		}
		//CalKeyA(PICCsnr, main_key, keya);	//����õ�KEYA
		sectno = 0;
		ret = CardMFCAuthKey(PICC_AUTHENT1A, PICCsnr, keya_0, sectno*4 + 3);	//��֤��0����
	}
	if (ret == RCX_OK) {
		//��ȡѧ�ţ���0������1��͵�2��ǰ4�ֽ���ѧ��
		sectno = 0;
		blockno = sectno*4+1;
		ret = CardMFCRead16Bytes(blockno,rdata);
		memcpy(num, rdata, 16);
		blockno = sectno*4+1;
		ret = CardMFCRead16Bytes(blockno,rdata);
		memcpy(num+16, rdata, 4);
	}
	if (ret == RCX_OK) {
		sectno = 2;
		ret = CardMFCAuthKey(PICC_AUTHENT1A, PICCsnr, keya, sectno*4 + 3);	//��֤��2����
		if (ret == RCX_OK) {
			//��ȡ��������2������0���ǰ8�ֽ�������
			blockno = sectno*4+0;
			ret = CardMFCRead16Bytes(blockno,rdata);
			memcpy(name, rdata, 8);
		}
	}
	//���Ӽ�¼�����ݿ�
	if (ret == RCX_OK) {
		dec2hex(PICCsnr, 4, phyNo);
		memcpy(tempInfo.phyNo, phyNo, 8);
		RTC_Get_Time(&ttime);		//��ȡ����ʱ��
		RTC_Get_Date(&tdate);		//��ȡ��ǰ����
		sprintf(tempInfo.time, "%04d%02d%02d%02d%02d%02d", tdate.year, tdate.month, tdate.day, ttime.hour, ttime.min, ttime.sec);
		ret = DB_add_record(ATTENDANCE_INDEX,  (void *)(&tempInfo));	//�Ѽ�¼���ӵ����ݿ���
		if ( !ret ) {
			return -1;		//��¼����ʧ��
		}
	}
	else {
		return -1;
	}
	warning_beep();
	DispStr_CE(0, 6, (char*)name, DISP_CENTER|DISP_CLRSCR);
	DispStr_CE(0, 8, (char*)num, DISP_CENTER);
	return 0;
}
#endif
/***********************************************************************
		module		:	[M1����д]
		function		:	[��ȡPSAM��������KEY_A(M1������������)]
  		return		:	[0����ȡ�ɹ�
						��������ȡʧ��]
		comment	:	[ȫ����ͨ����]
		machine		:	[EH-0818]
		language	:	[CHN]
 		keyword		:	[CPU_CARD]
		date			:	[11/09/05]
 		author		:	[chen-zhengkai]
************************************************************************/
int calkeyAfromPSAM(char* keya, const char* phyNo)
{
	unsigned char szCmd[256] = {0};	//���͵�����
	unsigned char uRev[256] = {0};	//���ܵ�����
	short revLen = 0;		//���ܳ���
	short sendLen = 0;	//���ͳ���
	unsigned char sPlain[8];	//������������
	int tempLen = 0;
	//char adfName[50] = {0};	//Ŀ¼����
	char atr[200] = {0};
	int atrLen = 0;
	int ret = 0;
	unsigned short SW = 0;
	const unsigned char statickey[17]="3230303530313331";	//"20050131"
	char debugTemp[30] = {0};
	int i = 0;
//	static unsigned char prePhyNo[4] = {0};
//	static unsigned char preCipherData[16] = {0};
	unsigned char cipherData[17] = {0};
	unsigned int startpos = 0;
	
//	if(memcmp(prePhyNo, phyNo,4) == 0)
//	{
//		memcpy(cipherData,preCipherData,8);
//		return 0;
//	}
	//���������������
	for(i = 0; i < 4; i++) {
		sPlain[i] = phyNo[i];
		sPlain[i+4] = ~phyNo[i];
	}
	
	ICC_interface_power(ICC_POWER_ON);
	ret = ICC_select_sock(ICC_SOCK_AUX1);
	if (ret) {
		debugFun("�Ҳ�������");
		return -1;
	}
	//��Ƭ�ϵ�
	ret = ICC_cpucard_ATR(atr, &atrLen, ICC_CLOCK_4M, ICC_VOLTAGE_3, 0x11, ICC_CPUC_ATR_FLAG_LONGATR);
	if (ret) {
		debugFun("��Ƭ�ϵ�ʧ��");
		return -1;
	}
	
	//ѡ��Ŀ¼
	//strcpy(adfName, "D1560001018000000000000100000000");
	//sprintf(szCmd,"00A40400%02X%s",strlen(adfName)/2,adfName);
	//hex2dec(adfName, strlen(adfName), (unsigned char*)szCmd, &tempLen);
	if ((ICC_cpucard_get_param())->isT1) {
		debugFun("��Ƭ��t1Э��");
		return -2;
	}
	else {
		//ѡ��Ŀ¼
		hex2dec("DF04", 4, szCmd, &tempLen);
		ret = ICC_T0_TPDU(0 , 0x00 ,0xA4, 0x00, 0x00 , 2,  szCmd, &SW);
		if (ret) {
			debugFun("ѡ�������ʧ��");
			return -1;
		}
		if ( !(SW == 0x9000 || (SW >> 8) == 0x61) ) {	//ִ�в��ɹ�
			debugFun("ѡ��Ŀ¼����");
			return -1;
		}
		//��ɢ��Կ
		//sprintf(szCmd,"801A270208%s",statickey);
		hex2dec(statickey, 16, szCmd, &tempLen);
		ret = ICC_T0_TPDU(0 , 0x80 ,0x1A, 0x27, 0x02 , 0x08,  szCmd, &SW);
		if (ret) {
			debugFun("��ɢ�����ʧ��");
			return -1;
		}
		if ( !(SW == 0x9000 || (SW >> 8) == 0x61) ) {	//ִ�в��ɹ�
			debugFun("��ɢ��Կ����");
//			sprintf(debugTemp, "%d %d", SW>>8,  SW&0xff);
//			debugFun(debugTemp);
			return -1;
		}
		//��������
		memset(szCmd,0,sizeof(szCmd));
		//dec2hex(sPlain, 8, szCmd);
		strncpy(szCmd, sPlain, 8);
		ret = ICC_T0_TPDU(0 , 0x80 ,0xFA, 0x00, 0x00 , 0x08,  szCmd, &SW);
		if (ret) {
			debugFun("���������ʧ��");
			return -1;
		}
		if ( !(SW == 0x9000 || (SW >> 8) == 0x61) ) {	//ִ�в��ɹ�
			debugFun("������Կ����");
			return -1;
		}
		else {
			revLen = 0x0000;
			revLen = SW&0x00ff;
		}
		//��ȡ����
		//Sys_Delay_MS(20);
		 ret = ICC_T0_TPDU(1,0x00,0xC0,0x00,0x00, revLen ,uRev, &SW);
		 if (ret) {
			debugFun("��ȡ����ʧ��");
			return -1;
		}
		if ( !(SW == 0x9000 || (SW >> 8) == 0x61) ) {	//ִ�в��ɹ�
			debugFun("��ȡ��Կʧ��");
			return -1;
		}
//		memcpy(prePhyNo,phyNo,4);
		memcpy(cipherData, uRev, 8);
//		memcpy(preCipherData,cipherData,8);
		
		memcpy(cipherData + 8, cipherData, 8);
		startpos=(cipherData[0] + cipherData[1] + cipherData[2] + cipherData[3] + cipherData[4] + cipherData[5] + cipherData[6] + cipherData[7]) % 7;      // sCipher[17]6079813C4F1A2F92, startpos = 4
		memcpy(keya, cipherData + startpos, 6);
//		sprintf(debugTemp, "%d %d, %d,%d,%d,%d,%d,%d,%d,%d", startpos, revLen, uRev[0] , uRev[1] , uRev[2] , uRev[3] , uRev[4] , uRev[5] , uRev[6], uRev[7]);
//		debugFun(debugTemp);
	}
	ICC_interface_power(ICC_POWER_OFF);
	return 0;
}
/***********************************************************************
		module		:	[M1����д]
		function		:	[������]
  		return		:	[��]
		comment	:	[ȫ����ͨ����]
		machine		:	[EH-0818]
		language	:	[CHN]
 		keyword		:	[CPU_CARD]
		date			:	[11/08/02]
 		author		:	[chen-zhengkai]
************************************************************************/
void	warning_beep()
{
	int	j;
	for ( j=0; j<1000; j++ ) {
		Beep_Sound(BEEP_ON,6,50);
	}
	Sys_Delay_MS(200);		
	Beep_Sound(BEEP_OFF,6,50);
}




