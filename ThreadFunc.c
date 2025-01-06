/* #include <windows.h> */
/* #include <rs232.h> */
/* #include <utility.h> */
/* #include <ansi_c.h> */
/* #include <formatio.h> */
#include "fix.hh"

#include <ni-visa/visa.h>
/* #include <userint.h> */
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include "All.h"
int AcqPackCnt;
FILE* fp_TAConfig;
int BeepFlag=0;
FILE *fp_temperature;
FILE *fp_current;
unsigned char CommandWord[2]; 

unsigned char High_Cmd[6] = {0x51,0x52,0x52,0x53,0x53,0x54};
unsigned char Low_Cmd[6]  = {0x98,0x21,0xa9,0x31,0xb7,0x40};	
int CVICALLBACK DataAcq(void *functionData)
{
  //	unsigned char dat[512], TempMessage[100];
  unsigned char dat[2048];
  char TempMessage[100];  
  int i, j;
  unsigned int n;
  int WaitCnt=0;
  AcqPackCnt=0;
  int Cnt_AcqPackage = 0;
  while(GetData_Status==1)
    {
      status=viRead(instr,dat,2048,&n);
      if (n!=0)
	{
	  WaitCnt=0;
	  AcqPackCnt++;
	  //WriteFile (fp_write, dat,n);
	  if(AcqPackCnt%1000==0)
	    {
	      sprintf(TempMessage,"采集到 %d 个数据包(每个数据包512Byte)...\n",AcqPackCnt);        
	      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	    }
	}
      else
	{   
	  if(GetData_Status==1)
	    {
	      SetCtrlVal(mainHandle,Main_TEXTBOX,"等待数据……\n");
	      //Beep();
	      //	Sleep(100);
	      WaitCnt++;
	    }
			
	}
      //这里可以添加函数进行采集不到数据的操作
    }
  sprintf(TempMessage,"采集停止，共采集到 %d 个数据包...\n",AcqPackCnt);
  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
  Cnt_AcqPackage		=		  AcqPackCnt / 7.56;
  sprintf(TempMessage,"共采集到 %d 个完整数据...\n",Cnt_AcqPackage);
  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
  return(0);
}

int CVICALLBACK Acqing(void *functionData)
{
  int Cnt_AcqPack = AcqPackCnt;
  int Cnt_AcqPack_Delay = Cnt_AcqPack;
  while(GetData_Status==1)
    {
      Cnt_AcqPack = AcqPackCnt;
      if(Cnt_AcqPack_Delay == Cnt_AcqPack)
	{
	  SetCtrlVal(mainHandle,Main_Acq_ing,0);
	}
      else
	{
	  SetCtrlVal(mainHandle,Main_Acq_ing,1);
	}
      sleep(200);
      Cnt_AcqPack_Delay = Cnt_AcqPack;
    }
  SetCtrlVal(mainHandle,Main_Acq_ing,0); 
  return(0);
}
int CVICALLBACK USBStatus (void* functionData)						  //查询USB端口状态线程
{
  char desc[256];
  int i = 0;
  while(USB_Status==1)
    {
      status = viFindRsrc(defaultRM, "USB[0-9]::0x04B4::0x1004::NI-VISA-[0-9]000[0-9]::RAW", VI_NULL, VI_NULL, desc);		
      if (status < VI_SUCCESS)
	{
	  status = viFindRsrc(defaultRM, "USB[0-9]::0x04B4::0x1004::NI-VISA-[0-9]::RAW", VI_NULL, VI_NULL, desc);
	  if(status<VI_SUCCESS)
	    {
	      USB_Status=0;
	      GetData_Status=0;
	      AcqPackCnt=0; 
	      SetCtrlVal(mainHandle,Main_USB_Status,0);
	      SetCtrlVal(mainHandle,Main_Acq_Status,0);
	      i = 0;
	      while(1)
		{
				
		  //Beep();
		  sleep(500);
					
		}
				
			
	    }
	}
      sleep(1000);
    }
  return 0;
}


int CVICALLBACK MonitorFunction (void* functionData)
{
  int i;
  int n=0;
  char TemperatureFile[300];
  char CurrentFile[300];
  sprintf(TemperatureFile,"%s\\Temperature.csv",WrDataPath);
  sprintf(CurrentFile,"%s\\Current.csv",WrDataPath);
  fp_temperature=fopen(TemperatureFile,"a");
  if(!fp_temperature)
    MessagePopup("错误","没有成功创建TemperatureFile！");
  fp_current=fopen(CurrentFile,"a");
  while(Monitor_Status==1)
    {
      if(n%10==0)
	{
	  for(i=0;i<16;i++)
	    {
	      if(FEE[i]==0)
		continue;
	      if(CheckCurrent(1,FEEID[i],i,fp_current)==-1)
		{
		  MessagePopup("错误","电流遥测错误！");
		  return(-1);
		}
	    } 
	  SetCtrlVal(mainHandle,Main_TEXTBOX,"\n完成所有FEE的电流遥测......\n");
	}
		
      if(n%100==0)
	{
	  for(i=0;i<16;i++)
	    {
	      if(FEE[i]==0)
		continue;
	      if(CheckTemperature(1,FEEID[i],i,fp_temperature)==-1)
		{
		  MessagePopup("错误","温度遥测错误！");
		  return(-1);
		}
	    }
	  SetCtrlVal(mainHandle,Main_TEXTBOX,"完成所有FEE的温度遥测......\n");
	}
		
      if(n%100==0)
	{
	  for(i=0;i <16;i++)
	    {
	      if(FEE[i] == 0)
		continue;
	      if(CheckFPGALogic(1, FEEID[i]) == -1)
		{
		  MessagePopup("错误","FPGA状态参数遥测错误！");
		  return(-1);
		}
	    }
	  SetCtrlVal(mainHandle,Main_TEXTBOX,"完成所有FEE的FPGA状态参数遥测......\n");
			
	}
		
      if(n%100==0)
	{
	  for(i=0;i<32;i++)
	    {
	      if(Hit[i]==0)
		continue;
	      if(CheckHit( 1 , i)==-1)
		{
		  MessagePopup("错误","Sub-DAQ状态寄存器遥测错误！");
		  return(-1);
		}
	    }
	  SetCtrlVal(mainHandle,Main_TEXTBOX,"完成Sub-DAQ状态寄存器（Trig、Hit计数）遥测......\n\n");
	  n=1;
	}
		
      sleep(100);
      n++;
    }
  fclose(fp_temperature);
  fclose(fp_current);
  return 0;
}

int CVICALLBACK CaliAcq(void *functionData)
{
  unsigned char CommandBuffer[2];
  char TempMessage[100];
  int i;
  int Volt = 0;
  int FileLength;
 
  char timemsg[20]="0000-00-00-00-00-00";
  /* double CurTime; */
  
  char WrDataFile[300];
  char File[100];

  
  CommandBuffer[1]  = 0x00;
  CommandBuffer[0]  = 0x11;
  if(SendUsbData(instr, CommandBuffer) ==-1)
    {
      MessagePopup ("错误", "发送USB配置命令失败！！");
      return -1;
    }
  else
    {
      SetCtrlVal(mainHandle,Main_TEXTBOX,"进入刻度模式\n");
    }
  /* GetCurrentDateTime(&CurTime); */

  /* FormatDateTimeString(CurTime, "%Y-%m-%d-%H-%M-%S", timemsg, 20); */

  
  sprintf(WrDataFile,"%s\\%s.dat",WrDataPath,timemsg);		 
  FileLength=strlen(WrDataFile);
  WrDataFile[FileLength-4]='\0';
  
  for( i = 0 ; i <8 ; i++)
    {	  
      sleep(100);
      /*-------Creat file------------------------------*/
      Volt  =  i*100;
      sprintf(File,"%s_%d.dat",WrDataFile,Volt);
      fp_write=fopen (File, "wb+");
      if(fp_write==NULL)
	{
	  MessagePopup ("错误", "新建数据保存文件失败！");
	  return -1;
	}
      /*--------------Set Cali Voltage-------------*/
      CommandBuffer[1]  = Cali_DAC_Code_High[i];
      CommandBuffer[0]  = Cali_DAC_Code_Low[i];
      if(SendUsbData(instr, CommandBuffer) ==-1)
	{
	  MessagePopup ("错误", "发送USB配置命令失败！！");
	  return -1;
	}
      else
	{
	  SetCtrlVal(mainHandle,Main_TEXTBOX,"设置刻度电压\n");
	}
      /*------------Start Acq----------------*/
      GetData_Status=1;
      //std::thread
      /* CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, DataAcq, &mainHandle, &threadID1);        */
      SetCtrlVal(mainHandle,Main_Acq_Status,1);
      SetCtrlVal(mainHandle,Main_TEXTBOX,"开始采数\n");
      CommandBuffer[1]  = 0x00;
      CommandBuffer[0]  = 0xa1;
      if(SendUsbData(instr, CommandBuffer) ==-1)
	{
	  MessagePopup ("错误", "发送USB配置命令失败！！");
	  return -1;
	}
      else
	{
	  SetCtrlVal(mainHandle,Main_TEXTBOX,"打开触发\n");
	}
      
      while( AcqPackCnt  < 300)
	{
	  sleep(300);
	}				
      
      /*-----Stop Acq-----*/
      CommandBuffer[1]  = 0x00;
      CommandBuffer[0]  = 0xa0;
      if(SendUsbData(instr, CommandBuffer) ==-1)
	{
	  MessagePopup ("错误", "发送USB配置命令失败！！");
	  return -1;
	}
      else
	{
	  SetCtrlVal(mainHandle,Main_TEXTBOX,"关闭触发\n");
	}
      
      GetData_Status=0;
      //std::thread
      //CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,threadID1,OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
      SetCtrlVal(mainHandle,Main_TEXTBOX,"停止采数\n");
      AcqPackCnt = 0;
      SetCtrlVal(mainHandle,Main_Acq_Status,0);
    }    
	
  SetCtrlVal(mainHandle,Main_TEXTBOX,"完成既定刻度测试\n");
  CommandBuffer[1]  = 0x00;
  CommandBuffer[0]  = 0x10;
  if(SendUsbData(instr, CommandBuffer) ==-1)
    {
      MessagePopup ("错误", "发送USB配置命令失败！！");
      return -1;
    }
  else
    {
      SetCtrlVal(mainHandle,Main_TEXTBOX,"返回正常采数模式\n");
    }
  
  return (0);
	
}	  

int CVICALLBACK SignalSweepAcq(void *functionData)
{
  int WaitCnt=0;
  int ChnID;
  int i;
  unsigned int n;
  int flag=0;
  int FileLength;
  int Volt;
  int S_Volt, E_Volt, Step;
  int WatiCount;
  char TempMessage[100];
  char WrDataFile[300];
  char timemsg[20]="0000-00-00-00-00-00";
  /* double CurTime; */
  int WaitCount;
  unsigned char CommandWord[2];
  char File[100];
  unsigned char dat[512];
  char IP[100];
  ViSession rm;
  ViSession SignalSource;
  int IPLength;
  int four_Cmd,HitEnable,HitChoose,TrigChoose;
  void OpenSignal(ViSession devise);
  void CloseSignal(ViSession devise);
  void SetSignal(ViSession devise, int voltage );
  BeepFlag=0;
  GetCtrlVal(signalsweepHandle, PANEL_Sig_InitVolt, &S_Volt);
  GetCtrlVal(signalsweepHandle, PANEL_Sig_FinalVolt, &E_Volt);
  GetCtrlVal(signalsweepHandle, PANEL_Sig_Step, &Step);
  GetCtrlVal(signalsweepHandle, PANEL_Sig_ChnID,&ChnID);
  //GetCurrentDateTime(&CurTime);
  GetCtrlVal(mainHandle,Main_HitEnable,&HitEnable);
  GetCtrlVal(mainHandle,Main_TrigChoose,&TrigChoose);
  //FormatDateTimeString(CurTime, "%Y-%m-%d-%H-%M-%S", timemsg, 20);
  sprintf(TempMessage,"%s\\VA%d",WrDataPath,ChnID);
  //MakeDir(TempMessage);
  sprintf(WrDataFile,"%s\\VA%d\\%s.dat",WrDataPath,ChnID,timemsg);
  IPLength=strlen((char*)IPAddress);
  sprintf(IP,"%s",(char*)IPAddress);
  IP[IPLength+14]='\0';
  status = viOpenDefaultRM(&rm);
  if (status < VI_SUCCESS) 
    {
      MessagePopup ("错误", "信号源打开失败！");
      return -1;
    }
  status = viOpen(rm,IP,VI_NULL,VI_NULL,&SignalSource);
  if (status < VI_SUCCESS) 
    {
      MessagePopup ("错误", "网络信号源不存在！");
      return -1;
    }
  //	CloseSignal(SignalSource);
  SetCtrlVal(mainHandle,Main_TEXTBOX,"开始进行信号源扫描测试...\n");
  if(S_Volt>=E_Volt)
    {
      MessagePopup("错误","刻度电压的起始值应小于终止值！");
      return(-1);
    }
  if(Step<=0)
    {
      MessagePopup("错误","步长应为正值！");
      return(-1);
    }
  FileLength=strlen(WrDataFile);
  WrDataFile[FileLength-4]='\0';
  if(Cmd2DAQ_Reset(instr)==-1)
    {
      MessagePopup("错误","发送USB配置命令失败！");
      return(-1);
    }
  sleep(10);
  for(i=0;i<16;i++)
    {
      if(FEE[i]==0)
	continue;
      if(Cmd2DAQ_FEESet(instr,i,FEEID[i])==-1)
	{
	  MessagePopup("错误","发送USB配置命令失败！");
	  return(-1);
	}
      else
	{
	  sprintf(TempMessage,"FEE%d挂载为FEE %s\n",i+1,FEENAME[i]);
	  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	}
    }
  CommandWord[0] = 0x03;
  CommandWord[1] = 0x3f;
  if(SendUsbData(instr, CommandWord) ==-1)
    {
      MessagePopup ("错误", "发送USB配置命令失败！！");
      return -1;
    }
  else
    SetCtrlVal(mainHandle,Main_TEXTBOX,"所有通道选择为主份...\n");
  CommandWord[0] = 0x02;
  CommandWord[1] = 0x2f;
  if(SendUsbData(instr, CommandWord)==-1)								 
    {
      MessagePopup ("错误", "发送USB配置命令失败！");
      return -1;
    }
  else
    SetCtrlVal(mainHandle,Main_TEXTBOX,"关闭所有通道...\n");
  CommandWord[0] = 0x0a;
  CommandWord[1] = 0xaf;
  if(SendUsbData(instr, CommandWord)==-1)
    {
      MessagePopup("错误","发送USB配置命令失败！");
      return(-1);
    }
  else
    SetCtrlVal(mainHandle,Main_TEXTBOX,"关闭所有命令返回通道...\n");
  for(i=0;i<16;i++)
    {
      if(FEE[i]==0)
	continue;
      if((Cmd2DAQ_EnableFEECmdChn(instr,i))==-1)
	{
	  MessagePopup("错误","发送USB配置命令失败！");
	  return(-1);
	}
      else
	{
	  sprintf(TempMessage,"FEE %s命令返回通道打开...\n",FEENAME[i]);
	  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	}
    }	  
  Cmd2DAQ_ChooseTrigMode(instr,4);									 
  SetCtrlVal(mainHandle,Main_TEXTBOX,"Sub_DAQ选择为外触发...\n");
  CloseSignal(SignalSource);
  Cmd2DAQ_ClrFifo(instr);				//1013
  //	SetCtrlVal(mainHandle,Main_TEXTBOX,"clrfifo执行完了\n");  
  status=viRead(instr,dat,512,&n);		
	  
  sleep(10);

  Cmd2DAQ_SetFifo(instr);			//	  1013
  for(i=0;i<16;i++)
    {
      if(FEE[i]==0)
	continue;
      if(Cmd2DAQ_EnableFEEChn(instr,i)==-1)
	{
	  MessagePopup("错误","发送USB配置命令失败！");
	  return(-1);
	}
      sprintf(TempMessage,"FEE %s通道打开...\n",FEENAME[i]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
    }
  if(Cmd2DAQ_StartAcq(instr)==-1)
    {
      MessagePopup("错误","发送USB配置命令失败！");
      return(-1);
    }
  if(Cmd2DAQ_EnableTrig(instr)==-1)
    {
      MessagePopup ("错误", "发送USB配置命令失败！");
      return -1;
    }
  SetSignal(SignalSource,0);
  for(Volt=S_Volt;Volt<=E_Volt;Volt=Volt+Step)
    {
      AcqPackCnt=0;
		 
      TrigDelay = 0x1D;
      for(i=0;i<16;i++)
	{
	  if(FEE[i]==0)
	    continue;
	  if((SetDelay(1,FEEID[i],TrigDelay))==-1)
	    {
	      MessagePopup("错误","发送COM配置命令失败！");
	      return(-1);
	    }
	  sprintf(TempMessage,"设置FEE %s的触发延迟值为%d...\n",FEENAME[i],TrigDelay);
	  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	}
      for(i=0;i<16;i++)
	{
	  if(FEE[i]==0)
	    continue;
	  if ( (ConfigReg(1,FEEID[i],0,0,0,0,1,0))==-1)
	    {
	      MessagePopup("错误","发送COM配置命令失败！");
	      return -1;
	    }
	  else
	    {
	      sprintf(TempMessage,"%s<%s%d%s","FEE",FEE[i],"触发接收关闭...\n");
	      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	    }
	}

      sprintf(File,"%s_%dmV.dat",WrDataFile,Volt);
      fp_write=fopen(File, "wb+");
      if(fp_write==NULL)
	{
	  MessagePopup ("错误", "新建数据保存文件失败！");
	  return -1;
	}
      SetSignal(SignalSource,Volt);
      sprintf(TempMessage,"设置信号发生器产生的触发电压为%dmV...\n",Volt);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
      for(i=0;i<16;i++)
	{
	  if(FEE[i]==0)
	    continue;
	  if ( (ClearTrig(1,FEEID[i]))==-1)
	    {
	      MessagePopup("错误","发送COM配置命令失败！");
	      return -1;
	    }
	  else
	    {
	      sprintf(TempMessage,"FEE %s触发计数清零...\n",FEENAME[i]);
	      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	    }
	  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	}
		
      OpenSignal(SignalSource);
      sleep(100);
      for(i=0;i<16;i++)
	{
	  if(FEE[i]==0)
	    continue;
	  if(ConfigReg(1,FEEID[i],HitEnable,0,TrigChoose,0,0,1)==-1)
	    {
	      MessagePopup("错误","发送COM配置命令失败！");
	      return(-1);
	    }
	  sprintf(TempMessage,"完成对FEE %s的配置...\n",FEENAME[i]);
	  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	}
      sprintf(TempMessage,"开始%dmV信号源扫描测试...\n",Volt);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
      SetCtrlVal(mainHandle,Main_Acq_Status,1);
      //SetCtrlAttribute(mainHandle,Main_Open_SaveFile,ATTR_DIMMED,1);
      GetData_Status=1;
      //std::thread
      //CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, DataAcq, &mainHandle, &threadID1);
      SetCtrlVal(mainHandle,Main_TEXTBOX,"开始采集\n"); 
      while(AcqPackCnt<200)
	{
	  if(SignalSweep_Status==0)
	    {   
	      GetData_Status=0;
	      //std::thread
	      //	      CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,threadID1,OPT_TP_PROCESS_EVENTS_WHILE_WAITING);

	      if(Cmd2DAQ_DisableTrig(instr)==-1)
		{
		  MessagePopup("错误","发送USB配置命令失败！");
		  return(-1);
		}
	      else
		SetCtrlVal(mainHandle,Main_TEXTBOX,"Sub_DAQ触发使能关闭...\n");
	      for(i=0;i<16;i++)
		{
		  if(Cmd2DAQ_DisableTrig(instr)==-1)
		    {
		      MessagePopup ("错误", "发送USB配置命令失败！");
		      return -1;
		    }
		  if(FEE[i]==0)
		    continue;
		  if ( (ConfigReg(1,FEEID[i],0,0,0,0,0,0))==-1)
		    {
		      MessagePopup("错误","发送COM配置命令失败！");
		      return -1;
		    }
		  else
		    {
		      sprintf(TempMessage,"%s<%s%d%s","FEE",FEE[i],"触发接收关闭...\n");
		      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		    }
		}
	      CommandWord[0] = 0x02;
	      CommandWord[1] = 0x2f;
	      if(SendUsbData(instr, CommandWord)==-1)									 
		{
		  MessagePopup ("错误", "发送USB配置命令失败！");
		  return -1;
		}
	      else
		SetCtrlVal(mainHandle,Main_TEXTBOX,"关闭所有通道...\n");
	      if(Cmd2DAQ_StopAcq(instr)==-1)
		{
		  MessagePopup ("错误", "发送USB配置命令失败！");
		  return -1;
		}
	      else
		SetCtrlVal(mainHandle,Main_TEXTBOX,"读取剩余数据...\n");  
	      sleep(500); 
	      GetData_Status=0;
	      sleep(100); 
	      SetCtrlVal(mainHandle,Main_Acq_Status,0);
	      SetCtrlVal(mainHandle,Main_TEXTBOX,"结束采集命令发出...\n");
	      fclose(fp_write);
	      /* SetCtrlAttribute(mainHandle,Main_Open_SaveFile,ATTR_DIMMED,0); */
	      SetCtrlVal(mainHandle,Main_Acq_Status,0);
	      fclose(fp_write);
	      return(-1);
	    }
	  else
	    sleep(1000);
				
	}
      for(i=0;i<16;i++)
	{
	  if(FEE[i]==0)
	    continue;
	  if ( (ConfigReg(1,FEEID[i],0,0,0,0,1,0))==-1)
	    {
	      MessagePopup("错误","发送COM配置命令失败！");
	      return -1;
	    }
	  else
	    {
	      sprintf(TempMessage,"%s<%s%d%s","FEE",FEE[i],"触发接收关闭...\n");
	      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	    }
	}
      CloseSignal(SignalSource);
      if(Cmd2DAQ_ClrFifo(instr)==-1)
	{
	  MessagePopup("错误","发送USB配置命令失败！");
	  return(-1);
	}
      sleep(500);
      GetData_Status=0;
      //std::thread
      //CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,threadID1,OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
      sprintf(TempMessage,"完成%dmV信号源测试...\n",Volt);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
      SetCtrlVal(mainHandle,Main_Acq_Status,0);
      fclose(fp_write);
      if(Cmd2DAQ_SetFifo(instr)==-1)
	{
	  MessagePopup("错误","发送USB配置命令失败！");
	  return(-1);
	}
      SetCtrlVal(mainHandle,Main_Acq_Status,0);
      fclose(fp_write);
      /* SetCtrlAttribute(mainHandle,Main_Save_Path,ATTR_DIMMED,0); */
    }
  if(Cmd2DAQ_DisableTrig(instr)==-1)
    {
      MessagePopup ("错误", "发送USB配置命令失败！");
      return -1;
    }
  if(Cmd2DAQ_StopAcq(instr)==-1)
    {
      MessagePopup ("错误", "发送USB配置命令失败！");
      return -1;
    }
  CommandWord[0] = 0x02;
  CommandWord[1] = 0x2f;
  if(SendUsbData(instr, CommandWord)==-1)									 
    {
      MessagePopup ("错误", "发送USB配置命令失败！");
      return -1;
    }
  else
    SetCtrlVal(mainHandle,Main_TEXTBOX,"关闭所有通道...\n");
  SetCtrlVal(mainHandle,Main_TEXTBOX,"完成信号源扫描测试...\n");
  SignalSweep_Status=0;
  /* SetCtrlAttribute(mainHandle,Main_Open_SaveFile,ATTR_DIMMED,0); */
  SetCtrlVal(signalsweepHandle, PANEL_Sig_ChnID,(ChnID+1));
  BeepFlag=1;
  //std::thread
  //CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE,BeepFunction, &mainHandle, &threadID7);
  MessagePopup("完成","请手动切换到下一块芯片！！！");
  BeepFlag=0;
  return(0);
}

int CVICALLBACK BeepFunction(void *functionData)
{
  while(BeepFlag==1)
    {
      /* Beep(); */
      sleep(800);
    }
  return(0);
}

ViUInt32 retCnt = 0, itemCnt = 0;

void OpenSignal(ViSession devise)											    //打开信号源输出
{
  char TempMessage[100];
  sprintf(TempMessage,"OUTPut2:STATe ON");
  status = viWrite(devise, (ViBuf)TempMessage, 17, &retCnt);
  if (status < VI_SUCCESS)
    {	
      MessagePopup ("错误", "未能打开信号源！");
      return;
    }
  sprintf(TempMessage,"OUTPut1:STATe ON");
  status = viWrite(devise, (ViBuf)TempMessage, 17, &retCnt);
  if (status < VI_SUCCESS)
    {	
      MessagePopup ("错误", "未能打开信号源！");
      return;
    }
  SetCtrlVal(mainHandle,Main_TEXTBOX,"打开信号源！\n");
  return;
}

void CloseSignal(ViSession devise)												//关闭信号源输出
{
  char TempMessage[100];
  sprintf(TempMessage,"OUTPut1:STATe OFF");
  status = viWrite(devise, (ViBuf)TempMessage, 18, &retCnt);
  if (status < VI_SUCCESS)
    {	
      MessagePopup ("错误", "未能关闭信号源！");
      return;
    }
  sprintf(TempMessage,"OUTPut2:STATe OFF");
  status = viWrite(devise, (ViBuf)TempMessage, 18, &retCnt);
  if (status < VI_SUCCESS)
    {	
      MessagePopup ("错误", "未能关闭信号源！");
      return;
    }
  SetCtrlVal(mainHandle,Main_TEXTBOX,"关闭信号源！\n");
  return;
}

void SetSignal(ViSession devise, int voltage )									//设置信号源的幅度和偏置
{
  float v, o;
  char Vol[10], Off[10];
  char VolStr[60];
  char OffStr[60];
  sprintf(VolStr,"SOURce2:VOLTage:LEVel:IMMediate:AMPLitude ");
  sprintf(OffStr,"SOURce2:VOLTage:LEVel:IMMediate:OFFSet ");
  v=(float)voltage/1000;
  o=v/2;
  sprintf(Vol,"%.3f",v);
  strcat(VolStr,Vol);
  status = viWrite(devise, (ViBuf) VolStr, strlen(VolStr) + 1, &retCnt);
  if (status < VI_SUCCESS)
    {
      MessagePopup ("错误", "电压幅度设置失败！");
      return;
    }
  sprintf(Off,"%.3f",o);
  strcat(OffStr,Off);
  status = viWrite(devise, (ViBuf) OffStr, strlen(OffStr) + 1, &retCnt);
  if (status < VI_SUCCESS)
    {
      MessagePopup ("错误", "偏置电压设置失败！");
      return;
    }
  return;
}

int CVICALLBACK TestA(void *functionData)
{

  int i, j, n=0;
  char TempMessage[1000];

  while(TestA_Status)
    {
      if(n%10==0)
	{
	  SetCtrlVal(mainHandle,Main_TEXTBOX,"测试线程\n"); 
	}
      n++;
      sleep(100);
			
      //这里可以添加函数进行采集不到数据的操作
    }
  sprintf(TempMessage,"测试线程结束...\n");
  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
  return(0);
}


