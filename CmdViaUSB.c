#include <ni-visa/visa.h>
//#include <userint.h>
/* #include "All.h" */


#include <stdio.h>


int SendUsbData(ViSession instr, unsigned char CommandWord[2])
{
  unsigned int n;
  unsigned char Temp_Command[2];						//待定
  Temp_Command[0]=CommandWord[1];				//待定
  Temp_Command[1]=CommandWord[0];				//待定
  if(VI_SUCCESS==viWrite(instr,Temp_Command,2,&n))
    return(0);
  else
    return(-1);
}


int Cmd2DAQ_StartAcq(ViSession instr)
{
  unsigned char Command[2];												   
  Command[0]=0x00;
  Command[1]=0x01;
  if(SendUsbData(instr,Command)<0)
    return(-1);
  return(0);
}
//added by gss0627   begin

int Cmd2DAQ_ClrFifo(ViSession instr)
{
  unsigned char Command[2];
  Command[0]=0x00;
  Command[1]=0xc1;
  if(SendUsbData(instr,Command)<0)
    return(-1);
  return(0);
}
int Cmd2DAQ_SetFifo(ViSession instr)
{
  unsigned char Command[2];
  Command[0]=0x00;
  Command[1]=0xc2;
  if(SendUsbData(instr,Command)<0)
    return(-1);
  return(0);
}
//added by gss0627   end 

int Cmd2DAQ_StopAcq(ViSession instr)
{
  unsigned char Command[2];
  Command[0]=0x00;
  Command[1]=0x02;
  if(SendUsbData(instr,Command)<0)
    return(-1);
  return(0);
}

int Cmd2DAQ_Reset(ViSession instr)
{
  unsigned char Command[2];
  Command[0]=0x00;
  Command[1]=0x0f;
  if(SendUsbData(instr,Command)<0)
    return(-1);
  return(0);
}

int Cmd2DAQ_EnableFEEChn(ViSession instr,int FEEID)
{
  unsigned char Command[2];
  Command[0]=0x00;
  Command[1]=0x10+FEEID;
  if(SendUsbData(instr,Command)<0)
    return(-1);
  return(0);
}

int Cmd2DAQ_DisableFEEChn(ViSession instr, int FEEID)
{
  unsigned char Command[2];
  Command[0]=0x00;
  Command[1]=0x20+FEEID;
  if(SendUsbData(instr,Command)<0)
    return(-1);
  return(0);
}

int Cmd2DAQ_ChooseMasterFEEChn(ViSession instr, int FEEID)
{
  unsigned char Command[2];
  Command[0]=0x00;
  Command[1]=0x30+FEEID;
  if(SendUsbData(instr,Command)<0)
    return(-1);
  return(0);
}
	
int Cmd2DAQ_ChooseBackupFEEChn(ViSession instr, int FEEID)
{
  unsigned char Command[2];
  Command[0]=0x00;
  Command[1]=0x40+FEEID;
  if(SendUsbData(instr,Command)<0)
    return(-1);
  return(0);
}
	
int Cmd2DAQ_ChooseTrigMode(ViSession instr, int Mode)
{
  unsigned char Command[2];
  Command[0]=0x00;
  Command[1]=0x50+Mode;
  if(SendUsbData(instr,Command)<0)
    return(-1);
  return(0);
}
	
int Cmd2DAQ_CaliInternal(ViSession instr, int Time)
{
  unsigned char Command[2];
  int LowBits, HighBits;
  LowBits=Time&0xff;
  HighBits=(Time>>8)&0x0f;
  Command[0]=HighBits+0x10;
  Command[1]=LowBits;
  if(SendUsbData(instr,Command)<0)
    return(-1);
  return(0);
}

int Cmd2DAQ_FEESet(ViSession instr, int M, int N)
{
  unsigned char Command[2];
  Command[0]=0x20+M;
  Command[1]=N;
  if(SendUsbData(instr,Command)<0)
    return(-1);
  return(0);
}

int Cmd2DAQ_EnableFEECmdChn(ViSession instr, int FEEID)
{
  unsigned char Command[2];
  Command[0]=0x00;
  Command[1]=0x90+FEEID;
  if(SendUsbData(instr,Command)<0)
    return(-1);
  return(0);
}
	
int Cmd2DAQ_DisableFEECmdChn(ViSession instr, int FEEID)
{
  unsigned char Command[2];
  Command[0]=0x00;
  Command[1]=0xa0+FEEID;
  if(SendUsbData(instr,Command)<0)
    return(-1);
  return(0);
}

int Cmd2DAQ_EnableTrig(ViSession instr)
{
  unsigned char  Command[2];
  Command[0]=0x00;
  Command[1]=0xD1;
  if(SendUsbData(instr,Command)<0)
    return(-1);
  return(0);
}

int Cmd2DAQ_DisableTrig(ViSession instr)
{
  unsigned char Command[2];
  Command[0]=0x00;
  Command[1]=0xD0;
  if(SendUsbData(instr,Command)<0)
    return(-1);
  return(0);
}
int Cmd2DAQ_HitChnChoose(ViSession instr, int HitChnPara)
{
  unsigned char Command[2];
  Command[0]=0x0E;
  Command[1]=HitChnPara;
  if(SendUsbData(instr,Command)<0)
    return(-1);
  return(0);
}

int Cmd2DAQ_Trig_Sel(ViSession instr,int TrigChoos)	//Modified by Siyuan 20141224		For A or B Channel	
{
  unsigned char Command[2];												   
  if(TrigChoos == 0)
    {
      Command[0]=0x07;
      Command[1]=0x7f;
      if(SendUsbData(instr,Command)<0)
	return(-1);
    }
  else
    {
      Command[0]=0x08;
      Command[1]=0x8f;
      if(SendUsbData(instr,Command)<0)
	return(-1);
    }
  return(0);
}
int Cmd2DAQ_CMD_Sel(ViSession instr,int CMD_Sel)	//Modified by Siyuan 20141224		For A or B Channel	
{
  unsigned char Command[2];												   
  if(CMD_Sel == 0)
    {
      Command[0]=0x05;
      Command[1]=0x5f;
      if(SendUsbData(instr,Command)<0)
	return(-1);
    }
  else
    {
      Command[0]=0x06;
      Command[1]=0x6f;
      if(SendUsbData(instr,Command)<0)
	return(-1);
    }
  return(0);
}


int Cmd2DAQ_Hit_Sel(ViSession instr,int HitChoose)	//Modified by Siyuan 20141224		For A or B Channel	
{
  unsigned char Command[2];												   
  if(HitChoose == 0)
    {
      Command[0]=0x30;
      Command[1]=0xf0;
      if(SendUsbData(instr,Command)<0)
	return(-1);
    }
  else
    {
      Command[0]=0x30;
      Command[1]=0xff;
      if(SendUsbData(instr,Command)<0)
	return(-1);
    }
  return(0);
}
