/****************************************************************************
*
*文件(File):         db.c
*
*修改(Modify):       2011/8/9 11:49:28
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
| V1.00  2011/8/9 11:49:28           USER 
----------------------------------------------------------------------------
****************************************************************************/
//*********************************************************************************************
//File    : db.c
//Function: define database table struct and size
//          format database and give some simple data for test
//          backup data manage, handset parameters manage
//*********************************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <api.h>
#include <database.h>
#include "in_call.h"
#include "extra.h"
//#include "hd_dt_wl.h"
//#include "common.h"


//how many blocks of every db table 
short g_blocknum[NUMOFDBTABLE] = {10};
//every record's size of db table  

unsigned short g_rec_size[NUMOFDBTABLE] = {sizeof(USER_INFO)};

char *g_strTableName[NUMOFDBTABLE] = {"备份区1"};
		    
 //======================================================================
// Name：Database_Init
// Function：Database Init progress
// Parameters：None	
// Return：
//	0	success
//	other	failure
// Note: 
//======================================================================
int	Database_Init(void)
{
	char	disbuf[40];
	short ret;
	int i;
	
     
	DispStr_CE(0,0,"Database Init",DISP_CENTER|DISP_CLRSCR);
	DispStr_CE(0,2,"Maybe longtime",DISP_CENTER);
	DispStr_CE(0,4,"Please Wait.....",DISP_CENTER);
	
	//Erase entire database first!
	DB_erase_db_sys();
	
	//Creata each tables
	for(i=0; i<NUMOFDBTABLE; i++)
	{
		ret = DB_format_db(i, g_blocknum[i], g_rec_size[i]);
		if(ret)
		{
			sprintf(disbuf,"Create Err:%d",ret);
			DispStr_CE(0,6,disbuf,DISP_CENTER);			
			ret = -1;
			goto end;
			
		}
	}
	
	//check the whole system
	ret = DB_init_sys_param(0);
	if(ret != NUMOFDBTABLE) 
	{
		DispStr_CE(0,6,"Check DB Error1",DISP_CENTER);		
		ret = -2;
		goto end;
	}
	else
	{
		for(i=0; i<NUMOFDBTABLE; i++)
		{
			ret = DB_check_format(i,g_blocknum[i],g_rec_size[i]);
			if(ret)
			{
				sprintf(disbuf,"Check Table %d Err:%d",i,ret);
				DispStr_CE(0,6,disbuf,DISP_CENTER|DISP_5x7);
				goto end;		        			        	
			}
		}
		
		DispStr_CE(0,6,"DB Init OK",DISP_CENTER);		
		ret = 0;
		goto end;
	}

end:
	delay_and_wait_key(0,EXIT_KEY_ALL,0);
	return ret;	
}


int count_db_toal(int fsid, unsigned int *p_total_valid, unsigned int *p_total_all)
{
	unsigned int counter = 0;
	unsigned int counter1 = 0;
	unsigned int i = 0;
	void *pRec = NULL;
	char flag = 0;

	counter = DB_count_records(fsid);
	for( i = 0; i < counter; i++)
	{
		pRec = DB_jump_to_record(fsid, i, &flag);
		if(flag) 
		{
			continue;
		}
		else 
		{
			counter1++;
		}
	}
	if(p_total_valid) 
		*p_total_valid = counter1;
	
	if(p_total_all) 
		*p_total_all = counter;
	
	return SUCCESSFUL;
}

//------------------------------------------------------------------------------------------------------
//Function     : chk_sys_db_format
//Descritiption: check the handset database struct, and decide whether to format the handset database or not
//		 用于手持机第一次启功时判断是否需要初始化数据库
//Input	       : None
//Output       : None
//Return       :
//Note         : Please apply the function when handset reset. two functions:get_db_sys_param and DB_check_db 
//		 must be applied when handset reset, otherwise problem will occur when to read and write database
//---------------------------------------------------------------------------------------------------------------
int chk_sys_db_format(void)
{
	int i = 0;
	char	disbuf[40];
	short retshort = 0;
	long retlong = 0;
	int need_init = 0;
	
	//first: check the database
	if(DB_init_sys_param(0x000A0000) != NUMOFDBTABLE)
	{
		DispStr_CE(0, 0, "Table Num Error", DISP_CENTER);	
		need_init = 1;		
	}
	else
	{
		//second: check the table format
		for(i = 0; i < DATA_TABLE_NUM; i++)
		{
			retshort = DB_check_format(i, g_blocknum[i], g_rec_size[i]);
			if(retshort)
			{
				sprintf(disbuf, "Table %d Err:%d", i, retshort);
				DispStr_CE(0, 0, disbuf, DISP_CENTER | DISP_5x7);
				need_init = 1;
				break;		        	
			}
		}
	}
	if(need_init)
	{		
		DispStr_CE(0,2,(char*)"DB need init",DISP_CENTER);
		DispStr_CE(0,4,(char*)"Enter - Init  ",DISP_CENTER);
		DispStr_CE(0,6,(char*)"Other - Cancel",DISP_CENTER);
		retlong = delay_and_wait_key(0, EXIT_KEY_ALL, 0);
		if((retlong == (long)EXIT_KEY_ENTER) || (retlong == (long)EXIT_KEY_OK))
		{		   
			//call the init function
			if(Database_Init()  == 0)
			{
				return(SUCCESSFUL);
			}
			else
			{
				return(FAILURE);
			}
		}
		else
		{
			return(SUCCESSFUL);
		}		
	}	
	return(SUCCESSFUL);
}

