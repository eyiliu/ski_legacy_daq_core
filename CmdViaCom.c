#include <windows.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>
#include <rs232.h>
#include <utility.h>
#include <formatio.h>
#include <string.h>
#include <math.h>
#include "All.h"
#include "CountHit.h"
//unsigned char Response[100];
unsigned char Response[256]; // 20140320 Changqing Feng - 1V11  

void Check(int InitialData, unsigned char *ConBuf)											//累加校验函数
{
	ConBuf[0]=InitialData>>8;
	ConBuf[1]=(0xff)&InitialData;
}

int SendComData(int PortNumber, unsigned char *Command)										//命令发送函数
{
	int i;
	char timemsg[20];											   \
	double CurTime;
	int Length ;
	unsigned char TempMessage[100];
	unsigned char FileMessage[100];
	FlushOutQ(PortNumber);
	FlushInQ(PortNumber);
	GetCurrentDateTime(&CurTime);
	FormatDateTimeString(CurTime, "%Y-%m-%d-%H-%M-%S", timemsg, 20);
	Length = strlen ( timemsg ) ;
	WriteFile(fp_cmd_log, timemsg, Length ) ;
	for(i=0;i<12;i++)
	{
		sprintf(&TempMessage[3*i],"%02x ",Command[i]);
	}
	if((ComWrt(PortNumber,Command,12))<0)
		return(-1);
		SetCtrlVal(mainHandle,Main_Send,TempMessage);
	sprintf(FileMessage, "  %s\n", TempMessage );
	Length = strlen ( FileMessage );
	WriteFile(fp_cmd_log, FileMessage, Length ) ;
	
//////////////////////////////////////////////////////////////	
//	WriteFile(fp_cmd_log_1, Command, 12);					//
//	Command[6] = Command[6] + 16 ;
//	WriteFile(fp_cmd_log_2, Command, 12);					//
//	Command[6] = Command[6] + 16 ;
//	WriteFile(fp_cmd_log_3, Command, 12);					//
//	Command[6] = Command[6] + 16 ;
//	WriteFile(fp_cmd_log_4, Command, 12);					//
//////////////////////////////////////////////////////////////	
	
	return(0);
}

int ReadComCmdResp(int PortNumber)															//遥控命令应答包
{
	char timemsg[20];
	double CurTime;
	int File_Length ;
	unsigned char Byte ;
	char ErrorTitle[30];
	unsigned char TempMessage[300];
	unsigned char FileMessage[500];
	int Error,FeeID,ErrorValue;
	int i;
	int Length = 0 ;
	int n = 0 ;
	i = 0 ;
	Length = GetInQLen(PortNumber);
	while ( ( Length == 0 ) & ( n < 100 ) )
	{
		Sleep(10);
		Length = GetInQLen(PortNumber);
		n++;
	}
	if( Length > 100)
	 	return(0); 
	while ( Length != 0 )
	{
		if (i>=256)
	      continue;
		
		Byte = ComRdByte ( PortNumber );
		Response[i] = Byte ;
		Length = GetInQLen(PortNumber) ;
		i++ ;
		
		Sleep(5);
	}
	Sleep(5);
/*	while ( Length != 0 )
	{
		
		if (i>=256)
	      continue;
		
		Byte = ComRdByte ( PortNumber );
		Response[i] = Byte ;
		Length = GetInQLen(PortNumber) ;
		i++ ;
		Sleep(5);
	}  */
	Length = i ;
	GetCurrentDateTime(&CurTime);
	FormatDateTimeString(CurTime, "%Y-%m-%d-%H-%M-%S", timemsg, 20);
	File_Length = strlen ( timemsg ) ;
	WriteFile(fp_cmd_log, timemsg, File_Length ) ;   //StringLength(setupstr)
	for(i=0;i<Length;i++)
		sprintf(&TempMessage[3*i],"%02x ",Response[i]);
//	if(COM_Cmd_Status==1)
	
		SetCtrlVal(mainHandle,Main_Receive,TempMessage);
	
	sprintf(FileMessage, "  %s\n", TempMessage );
	File_Length = strlen ( FileMessage );
	WriteFile(fp_cmd_log, FileMessage, File_Length ) ;
	if(Hit_Status == 1)
	{
		Trig_Count = Response[5]*256*256*256 + Response[6]*256*256 + Response[7]*256 + Response[8]  ;
		Hit_Count  = Response[9]*256 + Response[10] ;
	}
	/*if((Response[0]!=0x55)||(Response[1]!=0xaa))
		return(-1);
	if((Response[2]!=0xeb)||(Response[3]!=0x90))
		return(-1);
	 FeeID=(int)Response[4];
	 ErrorValue=(int)Response[5];
	 for(i=0;i<8;i++)
	 {
		 if(FEEID[i]==FeeID)
			 break;
	 }
	 sprintf(ErrorTitle,"FEE%s Error:",FEENAME[i]);
	 switch(ErrorValue)
	 {
		 case 0xF1:
			 break;
		 case 0xF2:
			 MessagePopup(ErrorTitle,"Check Error");
			 return(-1);
			 break;
		 case 0xF4:
			 MessagePopup(ErrorTitle,"Invalid Command");
			 return(-1);
			 break;
	     default:
			 MessagePopup(ErrorTitle,"Invalid Response");
		 	 return(-1);
			 break;
	 }
	if((Response[6]!=0x5a)||(Response[7]!=0xa5))
		return(-1);*/		
	 if(Response[5] == 0x05)
		 Check_Current(Response);
	 else if(Response[5] == 0x06)
		 Check_Temp(Response);
		 
	 return(0);
}

/*int ReadComTestResp(int PortNumber)														//遥测命令应答包
{
	return(0);
}*/

int LoopbackTest(int PortNumber, int FeeID)
{
	unsigned char ConBuf[12];
	unsigned char CheckBuffer[2];
	int  CheckTemp=0;
	FlushInQ(PortNumber);
	ConBuf[0]=0x55;
	ConBuf[1]=0xaa;
	ConBuf[2]=0xeb;
	ConBuf[3]=0x90;
	ConBuf[4]=(unsigned char)FeeID;
	CheckTemp=CheckTemp+(int)ConBuf[4];						
	ConBuf[5]=0x02;
	CheckTemp=CheckTemp+(int)ConBuf[5];
	ConBuf[6]=0x00;
	CheckTemp=CheckTemp+(int)ConBuf[6];						  
	ConBuf[7]=0x00;
	CheckTemp=CheckTemp+(int)ConBuf[7];	
	Check(CheckTemp,CheckBuffer);
	ConBuf[8]=CheckBuffer[0];
	ConBuf[9]=CheckBuffer[1];
	ConBuf[10]=0x5a;
	ConBuf[11]=0xa5;
	if(SendComData(PortNumber, ConBuf)<0)
		return(-1);
	Sleep(10);
	if(ReadComCmdResp(PortNumber)<0)
		return(-1);
	Sleep(10);
	return(0);
}

int ResetFPGA(int PortNumber, int FeeID)
{
	unsigned char ConBuf[12];
	unsigned char CheckBuffer[2];
	int CheckTemp=0;
	FlushInQ(PortNumber);
	ConBuf[0]=0x55;
	ConBuf[1]=0xaa;
	ConBuf[2]=0xeb;
	ConBuf[3]=0x90;
	ConBuf[4]=(unsigned char)FeeID;
	CheckTemp=CheckTemp+(int)ConBuf[4];						
	ConBuf[5]=0x02;
	CheckTemp=CheckTemp+(int)ConBuf[5];	
	ConBuf[6]=0x00;
	CheckTemp=CheckTemp+(int)ConBuf[6];						
	ConBuf[7]=0x00;
	CheckTemp=CheckTemp+(int)ConBuf[7];
	Check(CheckTemp,CheckBuffer);
	ConBuf[8]=CheckBuffer[0];
	ConBuf[9]=CheckBuffer[1];
	ConBuf[10]=0x5a;
	ConBuf[11]=0xa5;
	if(SendComData(PortNumber, ConBuf)<0)
		return(-1);
	Sleep(10);
	if(ReadComCmdResp(PortNumber)<0)
		return(-1);
	Sleep(10);
	return(0);
}

int ClearTrig(int PortNumber, int FeeID)
{
	unsigned char ConBuf[12];
	unsigned char CheckBuffer[2];
	int CheckTemp=0;
	FlushInQ(PortNumber);
	ConBuf[0]=0x55;
	ConBuf[1]=0xaa;
	ConBuf[2]=0xeb;
	ConBuf[3]=0x90;
	ConBuf[4]=(unsigned char)FeeID;
	CheckTemp=CheckTemp+(int)ConBuf[4];						
	ConBuf[5]=0x02;
	CheckTemp=CheckTemp+(int)ConBuf[5];
	ConBuf[6]=0x04;
	CheckTemp=CheckTemp+(int)ConBuf[6];						
	ConBuf[7]=0x00;
	CheckTemp=CheckTemp+(int)ConBuf[7];
	Check(CheckTemp,CheckBuffer);
	ConBuf[8]=CheckBuffer[0];
	ConBuf[9]=CheckBuffer[1];
	ConBuf[10]=0x5a;
	ConBuf[11]=0xa5;
	if(SendComData(PortNumber, ConBuf)<0)
		return(-1);
	Sleep(20);
	if(ReadComCmdResp(PortNumber)<0)
		return(-1);
	Sleep(10);
	return(0);
}
	
int ConfigReg(int PortNumber, int FeeID, int HitEnable, int HitChoose, int TrigChoose, int DataMode, int WorkMode, int Trig)
{
	unsigned char ConBuf[12];
	unsigned char CheckBuffer[2];
	int CheckTemp=0;
	int Trig_r;
	
	// because of the changes of the logic of FPGA, the 0 bit of CtrlReg of FEE invets
	if (Trig == 1)
		Trig_r = 0;
	else
		Trig_r = 1;
	
	FlushInQ(PortNumber);
	ConBuf[0]=0x55;
	ConBuf[1]=0xaa;
	ConBuf[2]=0xeb;
	ConBuf[3]=0x90;
	ConBuf[4]=(unsigned char)FeeID;
	CheckTemp=CheckTemp+(int)ConBuf[4];						
	ConBuf[5]=0x04;
	CheckTemp=CheckTemp+(int)ConBuf[5];
	ConBuf[6]=(unsigned char)(HitEnable*64+HitChoose*32+TrigChoose*16+DataMode*4+WorkMode*2+Trig_r);
	CheckTemp=CheckTemp+(int)ConBuf[6];						
	ConBuf[7]=0x00;
	CheckTemp=CheckTemp+(int)ConBuf[7];	
	Check(CheckTemp,CheckBuffer);
	ConBuf[8]=CheckBuffer[0];
	ConBuf[9]=CheckBuffer[1];
	ConBuf[10]=0x5a;
	ConBuf[11]=0xa5;
	if(SendComData(PortNumber, ConBuf)<0)
		return(-1);
	Sleep(10);
	if(ReadComCmdResp(PortNumber)<0)
		return(-1);
	Sleep(10);
	return(0);
}

int CheckCurrent(int PortNumber, int FeeID, int Fee, FILE *fp)
{
	unsigned char Response[50];
	unsigned char ConBuf[12];
	unsigned char CheckBuffer[2];
	unsigned char TempMessage[100];
	char timemsg[20];
	int CheckTemp=0;
	int Length;
	int i;
	int data;
	double current;
	double CurTime;
	FlushInQ(PortNumber);
	ConBuf[0]=0x55;
	ConBuf[1]=0xaa;
	ConBuf[2]=0xeb;
	ConBuf[3]=0x90;
	ConBuf[4]=(unsigned char)FeeID;
	CheckTemp=CheckTemp+(int)ConBuf[4];						
	ConBuf[5]=0x05;
	CheckTemp=CheckTemp+(int)ConBuf[5];
	ConBuf[6]=0x00;
	CheckTemp=CheckTemp+(int)ConBuf[6];						
	ConBuf[7]=0x00;
	CheckTemp=CheckTemp+(int)ConBuf[7];
	Check(CheckTemp,CheckBuffer);
	ConBuf[8]=CheckBuffer[0];
	ConBuf[9]=CheckBuffer[1];
	ConBuf[10]=0x5a;
	ConBuf[11]=0xa5;
	if(SendComData(PortNumber, ConBuf)<0)
		return(-1);
	Sleep(20);
	if(ReadComCmdResp(PortNumber)<0)
		return(-1);
	Sleep(10);
	return(0);
}

int CheckTemperature(int PortNumber, int FeeID, int Fee, FILE *fp)
{
	unsigned char Response[50];
	unsigned char ConBuf[12];
	unsigned char CheckBuffer[2];
	unsigned char TempMessage[100];
	char timemsg[20];
	double CurTime;
	double tempdata;
	double temperature1,temperature2;
	int i;
	int Length;
	int data1, data2;
	int CheckTemp=0;
	FlushInQ(PortNumber);
	ConBuf[0]=0x55;
	ConBuf[1]=0xaa;
	ConBuf[2]=0xeb;
	ConBuf[3]=0x90;
	ConBuf[4]=(unsigned char)FeeID;
	CheckTemp=CheckTemp+(int)ConBuf[4];						
	ConBuf[5]=0x06;
	CheckTemp=CheckTemp+(int)ConBuf[5];	
	ConBuf[6]=0x02;
	CheckTemp=CheckTemp+(int)ConBuf[6];						
	ConBuf[7]=0x00;
	CheckTemp=CheckTemp+(int)ConBuf[7];
	Check(CheckTemp,CheckBuffer);
	ConBuf[8]=CheckBuffer[0];
	ConBuf[9]=CheckBuffer[1];
	ConBuf[10]=0x5a;
	ConBuf[11]=0xa5;
	if(SendComData(PortNumber, ConBuf)<0)
		return(-1);
	Sleep(20);
	if(ReadComCmdResp(PortNumber)<0)
		return(-1);
	Sleep(10);
	return(0);
}

int CheckFPGALogic(int PortNumber, int FeeID)
{
	unsigned char ConBuf[12];
	unsigned char CheckBuffer[2];
	int CheckTemp=0;
	FlushInQ(PortNumber);
	ConBuf[0]=0x55;
	ConBuf[1]=0xaa;
	ConBuf[2]=0xeb;
	ConBuf[3]=0x90;
	ConBuf[4]=(unsigned char)FeeID;
	CheckTemp=CheckTemp+(int)ConBuf[4];						
	ConBuf[5]=0x07;
	CheckTemp=CheckTemp+(int)ConBuf[5];
	ConBuf[6]=0x00;
	CheckTemp=CheckTemp+(int)ConBuf[6];						
	ConBuf[7]=0x00;
	CheckTemp=CheckTemp+(int)ConBuf[7];	
	Check(CheckTemp,CheckBuffer);
	ConBuf[8]=CheckBuffer[0];
	ConBuf[9]=CheckBuffer[1];
	ConBuf[10]=0x5a;
	ConBuf[11]=0xa5;
	if(SendComData(PortNumber, ConBuf)<0)
		return(-1);
	Sleep(20);
	if(ReadComCmdResp(PortNumber)<0)
		return(-1);
	Sleep(10);
	return(0);
}	

int SetDelay(int PortNumber, int FeeID, int DelayValue)
{
	unsigned char ConBuf[12];
	unsigned char CheckBuffer[2];
	int CheckTemp=0;
	FlushInQ(PortNumber);
	FlushOutQ(PortNumber);
	ConBuf[0]=0x55;
	ConBuf[1]=0xaa;
	ConBuf[2]=0xeb;
	ConBuf[3]=0x90;
	ConBuf[4]=(unsigned char)FeeID;
	CheckTemp=CheckTemp+(int)ConBuf[4];						
	ConBuf[5]=0x08;
	CheckTemp=CheckTemp+(int)ConBuf[5];
	ConBuf[6]=DelayValue;
	CheckTemp=CheckTemp+(int)ConBuf[6];						
	ConBuf[7]=0x00;
	CheckTemp=CheckTemp+(int)ConBuf[7];
	Check(CheckTemp,CheckBuffer);
	ConBuf[8]=CheckBuffer[0];
	ConBuf[9]=CheckBuffer[1];
	ConBuf[10]=0x5a;
	ConBuf[11]=0xa5;
	if(SendComData(PortNumber, ConBuf)<0)
		return(-1);
	Sleep(20);
	if(ReadComCmdResp(PortNumber)<0)
		return(-1);
	Sleep(10);
	return(0);
}

int SetDACCode(int PortNumber, int FeeID, int Value)
{
	unsigned char ConBuf[12];
	unsigned char CheckBuffer[2];
	int CheckTemp=0;
	FlushInQ(PortNumber);
	ConBuf[0]=0x55;
	ConBuf[1]=0xaa;
	ConBuf[2]=0xeb;
	ConBuf[3]=0x90;
	ConBuf[4]=(unsigned char)FeeID;
	CheckTemp=CheckTemp+(int)ConBuf[4];						
	ConBuf[5]=0x0a;
	CheckTemp=CheckTemp+(int)ConBuf[5];
	ConBuf[7]=(unsigned char)(Value&0xff);						                              //还要插入DACCode和Value之间的线性换算关系    
	ConBuf[6]=(unsigned char)(((Value-(int)ConBuf[7])>>8)&(0x0f))+0x80;
	CheckTemp=CheckTemp+(int)ConBuf[6];						
	CheckTemp=CheckTemp+(int)ConBuf[7];
	Check(CheckTemp,CheckBuffer);
	ConBuf[8]=CheckBuffer[0];
	ConBuf[9]=CheckBuffer[1];
	ConBuf[10]=0x5a;
	ConBuf[11]=0xa5;
	if(SendComData(PortNumber, ConBuf)<0)
		return(-1);
	Sleep(10);
	if(ReadComCmdResp(PortNumber)<0)
		return(-1);
	Sleep(10);
	return(0);
}

int SetCaliTestChn(int PortNumber, int FeeID, int Chn)
{
	unsigned char ConBuf[12];
	unsigned char CheckBuffer[2];
	int CheckTemp=0;
	FlushInQ(PortNumber);
	ConBuf[0]=0x55;
	ConBuf[1]=0xaa;
	ConBuf[2]=0xeb;
	ConBuf[3]=0x90;
	ConBuf[4]=(unsigned char)FeeID;
	ConBuf[5]=0x0c;
	CheckTemp=CheckTemp+(int)ConBuf[4];						
	CheckTemp=CheckTemp+(int)ConBuf[5];
	ConBuf[6]=(unsigned char)Chn;
	ConBuf[7]=0x00;
	CheckTemp=CheckTemp+(int)ConBuf[6];						
	CheckTemp=CheckTemp+(int)ConBuf[7];
	Check(CheckTemp,CheckBuffer);
	ConBuf[8]=CheckBuffer[0];
	ConBuf[9]=CheckBuffer[1];
	ConBuf[10]=0x5a;
	ConBuf[11]=0xa5;
	if(SendComData(PortNumber, ConBuf)<0)
		return(-1);
	Sleep(10);
	if(ReadComCmdResp(PortNumber)<0)
		return(-1);
	Sleep(10);
	return(0);
}

int SetMaxValue(int PortNumber, int FeeID, int Chn, int MaxValue)
{
	unsigned char ConBuf[12];
	unsigned char CheckBuffer[2];
	int CheckTemp=0;
	FlushInQ(PortNumber);
	ConBuf[0]=0x55;
	ConBuf[1]=0xaa;
	ConBuf[2]=0xeb;
	ConBuf[3]=0x90;
	ConBuf[4]=(unsigned char)FeeID;
	ConBuf[5]=0x0e;
	CheckTemp=CheckTemp+(int)ConBuf[4];						
	CheckTemp=CheckTemp+(int)ConBuf[5];
	ConBuf[6]=(unsigned char)Chn;
	ConBuf[7]=(unsigned char)MaxValue;
	CheckTemp=CheckTemp+(int)ConBuf[6];						
	CheckTemp=CheckTemp+(int)ConBuf[7];
	Check(CheckTemp,CheckBuffer);
	ConBuf[8]=CheckBuffer[0];
	ConBuf[9]=CheckBuffer[1];
	ConBuf[10]=0x5a;
	ConBuf[11]=0xa5;
	if(SendComData(PortNumber, ConBuf)<0)
		return(-1);
	Sleep(10);
	if(ReadComCmdResp(PortNumber)<0)
		return(-1);
	Sleep(10);
	return(0);
}

int SetMinValue(int PortNumber, int FeeID, int Chn, int MinValue)
{
	unsigned char ConBuf[12];
	unsigned char CheckBuffer[2];
	int CheckTemp=0;
	FlushInQ(PortNumber);
	ConBuf[0]=0x55;
	ConBuf[1]=0xaa;
	ConBuf[2]=0xeb;
	ConBuf[3]=0x90;
	ConBuf[4]=(unsigned char)FeeID;
	ConBuf[5]=0x10;
	CheckTemp=CheckTemp+(int)ConBuf[4];						
	CheckTemp=CheckTemp+(int)ConBuf[5];	
	ConBuf[6]=(unsigned char)Chn;
	ConBuf[7]=(unsigned char)MinValue;
	CheckTemp=CheckTemp+(int)ConBuf[6];						
	CheckTemp=CheckTemp+(int)ConBuf[7];
	Check(CheckTemp,CheckBuffer);
	ConBuf[8]=CheckBuffer[0];
	ConBuf[9]=CheckBuffer[1];
	ConBuf[10]=0x5a;
	ConBuf[11]=0xa5;
	if(SendComData(PortNumber, ConBuf)<0)
		return(-1);
	Sleep(10);
	if(ReadComCmdResp(PortNumber)<0)
		return(-1);
	Sleep(10);
	return(0);
}

int SetTA_Config(int PortNumber, int FeeID, int Address, int Parameter)
{
	unsigned char ConBuf[12];
	unsigned char CheckBuffer[2];
	int CheckTemp=0;
	FlushInQ(PortNumber);
	ConBuf[0]=0x55;
	ConBuf[1]=0xaa;
	ConBuf[2]=0xeb;
	ConBuf[3]=0x90;
	ConBuf[4]=(unsigned char)FeeID;
	ConBuf[5]=0x28;
	CheckTemp=CheckTemp+(int)ConBuf[4];						
	CheckTemp=CheckTemp+(int)ConBuf[5];	
	ConBuf[6]=(unsigned char)Address;
	ConBuf[7]=(unsigned char)Parameter;
	CheckTemp=CheckTemp+(int)ConBuf[6];						
	CheckTemp=CheckTemp+(int)ConBuf[7];
	Check(CheckTemp,CheckBuffer);
	ConBuf[8]=CheckBuffer[0];
	ConBuf[9]=CheckBuffer[1];
	ConBuf[10]=0x5a;
	ConBuf[11]=0xa5;
	if(SendComData(PortNumber, ConBuf)<0)
		return(-1);
//	Sleep(10);
//	if(ReadComCmdResp(PortNumber)<0)
		return(-1);
//	Sleep(10);					
//	WriteFile(fp_TAConfig,ConBuf,12);
	return(0);
}

int TA_Self_Test(int PortNumber, int FeeID, int Channel)
{
	unsigned char ConBuf[12];
	unsigned char CheckBuffer[2];
	int CheckTemp=0;
	FlushInQ(PortNumber);
	ConBuf[0]=0x55;
	ConBuf[1]=0xaa;
	ConBuf[2]=0xeb;
	ConBuf[3]=0x90;
	ConBuf[4]=(unsigned char)FeeID;
	ConBuf[5]=0x04;
	CheckTemp=CheckTemp+(int)ConBuf[4];						
	CheckTemp=CheckTemp+(int)ConBuf[5];	
	ConBuf[6]=0x80;
	ConBuf[7]=(unsigned char)Channel;
	CheckTemp=CheckTemp+(int)ConBuf[6];						
	CheckTemp=CheckTemp+(int)ConBuf[7];
	Check(CheckTemp,CheckBuffer);
	ConBuf[8]=CheckBuffer[0];
	ConBuf[9]=CheckBuffer[1];
	ConBuf[10]=0x5a;
	ConBuf[11]=0xa5;
	if(SendComData(PortNumber, ConBuf)<0)
		return(-1);
	Sleep(10);
	if(ReadComCmdResp(PortNumber)<0)
		return(-1);
	Sleep(10);					
//	WriteFile(fp_TAConfig,ConBuf,12);
	return(0);
}

int TA_Thr(int PortNumber, int FeeID, int Ch, int Thr)
{
	unsigned char ConBuf[12];
	unsigned char CheckBuffer[2];
	int CheckTemp=0;
	FlushInQ(PortNumber);
	ConBuf[0]=0x55;
	ConBuf[1]=0xaa;
	ConBuf[2]=0xeb;
	ConBuf[3]=0x90;
	ConBuf[4]=(unsigned char)FeeID;
	if ( ( Ch == 0 ) || ( Ch == 1 ) )
		ConBuf[5]=0x22;
	else
		ConBuf[5]=0x26;
	CheckTemp=CheckTemp+(int)ConBuf[4];						
	CheckTemp=CheckTemp+(int)ConBuf[5];	
	ConBuf[6]=(unsigned char)((Thr&3840)>>8);
	if ( ( Ch == 0 ) || ( Ch == 2 ) )
		ConBuf[6] = ConBuf[6] + 0x40 ;
	else
		ConBuf[6] = ConBuf[6] + 0x80 ;	
	ConBuf[7]=(unsigned char)(Thr&255);
	CheckTemp=CheckTemp+(int)ConBuf[6];						
	CheckTemp=CheckTemp+(int)ConBuf[7];
	Check(CheckTemp,CheckBuffer);
	ConBuf[8]=CheckBuffer[0];
	ConBuf[9]=CheckBuffer[1];
	ConBuf[10]=0x5a;
	ConBuf[11]=0xa5;
	if(SendComData(PortNumber, ConBuf)<0)
		return(-1);
	Sleep(10);
	if(ReadComCmdResp(PortNumber)<0)
		return(-1);
	Sleep(10);					
//	WriteFile(fp_TAConfig,ConBuf,12);
	return(0);
}

int TA_DAC(int PortNumber, int FeeID, int DAC)
{
	unsigned char ConBuf[12];
	unsigned char CheckBuffer[2];
	int temp;
	int CheckTemp=0;
	FlushInQ(PortNumber);
	ConBuf[0]=0x55;
	ConBuf[1]=0xaa;
	ConBuf[2]=0xeb;
	ConBuf[3]=0x90;
	ConBuf[4]=(unsigned char)FeeID;
	ConBuf[5]=0x0a;
	CheckTemp=CheckTemp+(int)ConBuf[4];						
	CheckTemp=CheckTemp+(int)ConBuf[5];	
	temp = DAC&3840 ;
	temp = temp>>8 ;
	ConBuf[6]=(unsigned char)(temp);
	ConBuf[6] = ConBuf[6] + 0x80 ;
	temp = DAC&255;
	ConBuf[7]=(unsigned char)(temp);
	CheckTemp=CheckTemp+(int)ConBuf[6];						
	CheckTemp=CheckTemp+(int)ConBuf[7];
	Check(CheckTemp,CheckBuffer);
	ConBuf[8]=CheckBuffer[0];
	ConBuf[9]=CheckBuffer[1];
	ConBuf[10]=0x5a;
	ConBuf[11]=0xa5;
	if(SendComData(PortNumber, ConBuf)<0)
		return(-1);
	Sleep(10);
	if(ReadComCmdResp(PortNumber)<0)
		return(-1);
	Sleep(10);					
//	WriteFile(fp_TAConfig,ConBuf,12);
	return(0);
}

int TA_load(int PortNumber, int FeeID )
{
	unsigned char ConBuf[12];
	unsigned char CheckBuffer[2];
	int CheckTemp=0;
	FlushInQ(PortNumber);
	ConBuf[0]=0x55;
	ConBuf[1]=0xaa;
	ConBuf[2]=0xeb;
	ConBuf[3]=0x90;
	ConBuf[4]=(unsigned char)FeeID;
	ConBuf[5]=0x30;
	CheckTemp=CheckTemp+(int)ConBuf[4];						
	CheckTemp=CheckTemp+(int)ConBuf[5];
	ConBuf[6]=0x01;
	ConBuf[7]=0x00;
	CheckTemp=CheckTemp+(int)ConBuf[6];						
	CheckTemp=CheckTemp+(int)ConBuf[7];
	Check(CheckTemp,CheckBuffer);
	ConBuf[8]=CheckBuffer[0];
	ConBuf[9]=CheckBuffer[1];
	ConBuf[10]=0x5a;
	ConBuf[11]=0xa5;
	if(SendComData(PortNumber, ConBuf)<0)
		return(-1);
	Sleep(10);
	if(ReadComCmdResp(PortNumber)<0)
		return(-1);
	Sleep(10);					
//	WriteFile(fp_TAConfig,ConBuf,12);
	return(0);
}

int SetTHR(int PortNumber, int FeeID, unsigned char THR_value[2] ,unsigned char type)   //value[0] high  and [1] low;tpye: command type
{
	unsigned char ConBuf[12];
	unsigned char CheckBuffer[2];
	int CheckTemp=0;
	FlushInQ(PortNumber);
	FlushOutQ(PortNumber);
	ConBuf[0]=0x55;
	ConBuf[1]=0xaa;
	ConBuf[2]=0xeb;
	ConBuf[3]=0x90;
	ConBuf[4]=(unsigned char)FeeID;
	CheckTemp=CheckTemp+(int)ConBuf[4];						
	ConBuf[5]=type;
	CheckTemp=CheckTemp+(int)ConBuf[5];
	ConBuf[6]=THR_value[0];
	CheckTemp=CheckTemp+(int)ConBuf[6];						
	ConBuf[7]=THR_value[1];
	CheckTemp=CheckTemp+(int)ConBuf[7];
	Check(CheckTemp,CheckBuffer);
	ConBuf[8]=CheckBuffer[0];
	ConBuf[9]=CheckBuffer[1];
	ConBuf[10]=0x5a;
	ConBuf[11]=0xa5;
	if(SendComData(PortNumber, ConBuf)<0)
		return(-1);
	Sleep(10);
	if(ReadComCmdResp(PortNumber)<0)
		return(-1);
	Sleep(10);
	return(0);
}
int CheckHit(int PortNumber, int HitID)
{
	unsigned char Response[50];
	unsigned char ConBuf[12];
	unsigned char CheckBuffer[2];
	unsigned char TempMessage[100];
	char timemsg[20];
	int i;
	int Length;
	int data1, data2;
	int CheckTemp=0;
	Hit_Status = 1;
	FlushInQ(PortNumber);
	ConBuf[0]=0x55;
	ConBuf[1]=0xaa;
	ConBuf[2]=0xeb;
	ConBuf[3]=0x90;
	ConBuf[4]=0x30;
	CheckTemp=CheckTemp+(int)ConBuf[4];						
	ConBuf[5]=(unsigned char)HitID;
	CheckTemp=CheckTemp+(int)ConBuf[5];	
	ConBuf[6]=0x00;
	CheckTemp=CheckTemp+(int)ConBuf[6];						
	ConBuf[7]=0x00;
	CheckTemp=CheckTemp+(int)ConBuf[7];
	Check(CheckTemp,CheckBuffer);
	ConBuf[8]=CheckBuffer[0];
	ConBuf[9]=CheckBuffer[1];
	ConBuf[10]=0x5a;
	ConBuf[11]=0xa5;
	if(SendComData(PortNumber, ConBuf)<0)
		return(-1);
	Sleep(10);
	if(ReadComCmdResp(PortNumber)<0)
		return(-1);
	if(countHandle)
	{
		SetCtrlVal(countHandle , CountHit_Trig_Count, Trig_Count);	
		switch (HitID)
		{
			case 0 : SetCtrlVal(countHandle , CountHit_PX1_B8, Hit_Count); ;break;
			case 1 : SetCtrlVal(countHandle , CountHit_PX1_T8, Hit_Count) ;break;
			case 2 : SetCtrlVal(countHandle , CountHit_PX1_B5, Hit_Count) ;break;
			case 3 : SetCtrlVal(countHandle , CountHit_PX1_T5, Hit_Count) ;break;
			case 4 : SetCtrlVal(countHandle , CountHit_PX4_B8, Hit_Count) ;break;
			case 5 : SetCtrlVal(countHandle , CountHit_PX4_T8, Hit_Count) ;break;
			case 6 : SetCtrlVal(countHandle , CountHit_PX4_B5, Hit_Count) ;break;
			case 7 : SetCtrlVal(countHandle , CountHit_PX4_T5, Hit_Count) ;break;
			case 8 : SetCtrlVal(countHandle , CountHit_NX1_B8, Hit_Count) ;break;
			case 9 : SetCtrlVal(countHandle , CountHit_NX1_T8, Hit_Count) ;break;
			case 10 : SetCtrlVal(countHandle , CountHit_NX1_B5, Hit_Count) ;break;
			case 11 : SetCtrlVal(countHandle , CountHit_NX1_T5, Hit_Count) ;break;
			case 12 : SetCtrlVal(countHandle , CountHit_NX4_B8, Hit_Count) ;break;
			case 13 : SetCtrlVal(countHandle , CountHit_NX4_T8, Hit_Count) ;break;
			case 14 : SetCtrlVal(countHandle , CountHit_NX4_B5, Hit_Count) ;break;
			case 15 : SetCtrlVal(countHandle , CountHit_NX4_T5, Hit_Count) ;break;
			case 16 : SetCtrlVal(countHandle , CountHit_PY1_B8, Hit_Count) ;break;
			case 17 : SetCtrlVal(countHandle , CountHit_PY1_T8, Hit_Count) ;break;
			case 18 : SetCtrlVal(countHandle , CountHit_PY1_B5, Hit_Count) ;break;
			case 19 : SetCtrlVal(countHandle , CountHit_PY1_T5, Hit_Count) ;break;
			case 20 : SetCtrlVal(countHandle , CountHit_PY4_B8, Hit_Count) ;break;
			case 21 : SetCtrlVal(countHandle , CountHit_PY4_T8, Hit_Count) ;break;
			case 22 : SetCtrlVal(countHandle , CountHit_PY4_B5, Hit_Count) ;break;
			case 23 : SetCtrlVal(countHandle , CountHit_PY4_T5, Hit_Count) ;break;
			case 24 : SetCtrlVal(countHandle , CountHit_NY1_B8, Hit_Count) ;break;
			case 25 : SetCtrlVal(countHandle , CountHit_NY1_T8, Hit_Count) ;break;
			case 26 : SetCtrlVal(countHandle , CountHit_NY1_B5, Hit_Count) ;break;
			case 27 : SetCtrlVal(countHandle , CountHit_NY1_T5, Hit_Count) ;break;
			case 28 : SetCtrlVal(countHandle , CountHit_NY4_B8, Hit_Count) ;break;
			case 29 : SetCtrlVal(countHandle , CountHit_NY4_T8, Hit_Count) ;break;
			case 30 : SetCtrlVal(countHandle , CountHit_NY4_B5, Hit_Count) ;break;
			case 31 : SetCtrlVal(countHandle , CountHit_NY4_T5, Hit_Count) ;break;
			
		}
	}
	Hit_Status = 0;
	Sleep(10);
	return(0);
}
int Set_Current_Thr(void)
{
	char Config_File[300];
	char File_Path[300];
	FILE *fp_read;
	unsigned char TempBuffer[12];
	GetDir(File_Path);
	strcat(File_Path,"\\Current_Thr_Config.dat");
//	printf("%s \n",File_Path);
	fp_read=fopen(File_Path,"r");
	if(fp_read == NULL)
		MessagePopup("错误","打开文件失败！");
	while(!feof(fp_read))
	{
		fread(TempBuffer,1,12,fp_read);
		if((TempBuffer[2]!=0xeb)||(TempBuffer[3]!=0x90)||(TempBuffer[10]!=0x5a)||(TempBuffer[11]!=0xa5))
		{
			MessagePopup("错误","配置文件有误！");
		}
		SendComData(1,TempBuffer);
		Sleep(15);
		ReadComCmdResp(1);
		Sleep(10);
	}
	fclose(fp_read);
	SetCtrlVal(mainHandle,Main_TEXTBOX,"配置电流阈值成功...\n");
	return(0);
}
int Check_Current(unsigned char *Response)
{	
	int FEEID = Response[4];
	int Plus =   Response[7] + 256*Response[6] ;
	
	Current_Remote[FEEID-16] = 2000*Plus/4096 - 208 ;
	Plus = Response[9] + 256*Response[8] ;
	Current_Remote[FEEID]    = 2000*Plus/4096 - 150 ;
	
	return 0;
	
}
int Check_Temp(unsigned char*Response)
{
	int FEEID = Response[4]; 
	double Plus =  Response[7] + 256*Response[6] ;
	Temp_Remote[FEEID-16] = 273.15/(1+273.15/4100*log(10*Plus/(17.25*(4096 - 0.9524*Plus)))) -273.15 ; 
	Plus =  Response[9] + 256*Response[8] ; 
	Temp_Remote[FEEID] = 273.15/(1+273.15/4100*log(10*Plus/(17.25*(4096 - 0.9524*Plus)))) -273.15 ; 
	Plus =  Response[11] + 256*Response[10] ; 
	Temp_Remote[FEEID+16] = 273.15/(1+273.15/4100*log(10*Plus/(17.25*(4096 - 0.9524*Plus)))) -273.15 ; 
	Plus =  Response[13] + 256*Response[12] ; 
	Temp_Remote[FEEID+32] = 273.15/(1+273.15/4100*log(10*Plus/(17.25*(4096 - 0.9524*Plus)))) -273.15 ; 

	return 0;
}
