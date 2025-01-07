#include "fix.hh"
#include "Main.h"
#include "All.h"
#include <unistd.h>
#include <cstdint>
#include <math.h>

#include "Hv_Uir.h"
#include "Mask_Channels.h"
#include "Duty_Mode.h"
#include "CountHit.h"
#include "Help.h"
#include "Duty_Mode.h"
#include "MutiTHR.h"
#include "MutiHit.h"
#include "TAConfig.h"
#include "MutiUSB.h"
#include "SignalSweep.h"
/* #include "COM_Cmd_Status.h" */
/* #include <windows.h> */
/* #include <udpsupp.h> */
/* #include <windows.h> */
/* #include <mmsystem.h>  //这个和上面一行是多媒体库，引用这两个库，然后用PlaySound函数播放wav格式 */
/* #include <ansi_c.h> */
/* #include <formatio.h> */
/* #include <rs232.h> */
/* #include <utility.h> */
/* #include <cvirte.h>		 */
/* #include <userint.h> */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "TAConfig.h"
#include "PX.h"

unsigned char Response[256];

ViSession defaultRM, instr;
ViSession rm, SignalSource;
ViStatus status;

unsigned char 	Cali_DAC_Code_High[8] = {0x51,0x51,0x51,0x51,0x51,0x51,0x51,0x51};
unsigned char 	Cali_DAC_Code_Low[8]  = {0x98,0xa6,0xb3,0xc1,0xcf,0xdc,0xea,0xf8};    

/*-----Relation of TA THR -----*/
double Relationship_TA1_10[12][2] = {{14,0x88E},{22,0X896},{26,0X89A},{32,0X89F},{52,0X8B4},{86,0X8D4},{141,0X906},{192,0X92A},{261,0X94E},{350,0X976},{450,0X995},{600,0X9C0}};
double Relationship_TA1_11[12][2] = {{16,0x890},{22,0X892},{27,0X896},{36,0X89F},{55,0X8B0},{95,0X8D1},{140,0X8F6},{205,0X920},{318,0X953},{450,0X983},{640,0X9B6},{780,0X9D1}};
double Relationship_TA2_10[12][2] = {{11,0x883},{21,0X88C},{35,0X898},{55,0X8AD},{85,0X8C6},{114,0X8E2},{160,0X906},{309,0X949},{440,0X981},{660,0X9BE},{760,0X9CE},{900,0X9D8}};
double Relationship_TA2_11[12][2] = {{15,0x884},{21,0X889},{29,0X890},{49,0X8A0},{93,0X8C6},{156,0X8F5},{250,0X92A},{370,0X95C},{510,0X987},{760,0X9BE},{910,0X9D3},{1010,0X9D9}};
double Relationship_TA3_10[12][2] = {{6 ,0x877},{9,0X87A}, {18,0X883},{43,0X89B},{68,0X8B5},{115,0X8E2},{220,0X922},{300,0X95B},{420,0X98A},{530,0X9A6},{700,0X9CC},{780,0X9D7}};
double Relationship_TA3_11[12][2] = {{5,0x877}, {10,0X87A},{14,0X882},{21,0X883},{40,0X895},{83,0X8B9},{120,0X8D9},{164,0X906},{270,0X942},{460,0X985},{720,0X9C0},{830,0X9D2}};



int USB_Status=0,GetData_Status=0,datasaveflag=1,COM_Status=0,Monitor_Status=0,LoadFEE_Status=0,COM_Cmd_Status=0,Cali_Status=0,SignalSweep_Status=0;
int Data_Mode_Status=1,Cali_Mode_Status=0,Hit_Status = 0,TestA_Status,Data_Chn_Choose = 1;
int DAQTrig_Mode_Status=1;
int Error;
int mainHandle, Status1Handle, Status2Handle, Status3Handle, Status4Handle, Status5Handle, Status6Handle, Status7Handle, Status8Handle, comcmdHandle,signalsweepHandle,mutiusbHandle,muticmdHandle,mutihitHandle,TAconfigHandle,mutiTHRHandle,HelpHandle,countHandle = 0,dutyHandle,PX_Handle;
int Mask_Handle,Hv_Uir_Handle;

int FEE[16]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},Hit[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int HitChnParaM[8] = {0};
int Show_FEE_Status[16]={0};
int FEEID[17]={PX_1,PX_2,PX_3,PX_4,NX_1,NX_2,NX_3,NX_4,PY_1,PY_2,PY_3,PY_4,NY_1,NY_2,NY_3,NY_4,SUB_DAQ};
char FEENAME[16][6]={"P_X_1","P_X_2","P_X_3","P_X_4","N_X_1","N_X_2","N_X_3","N_X_4","P_Y_1","P_Y_2","P_Y_3","P_Y_4","N_Y_1","N_Y_2","N_Y_3","N_Y_4"};
int threadID1,threadID2,threadID3,threadID4,threadID5,threadID6,threadID7;
FILE* fp_write;
int Time_Length,Log_Length;
unsigned int Trig_Count;
int Current_Remote[32] = {0},Temp_Remote[64] ={0};
int Hit_Count;
double CurTime;
char timemsg[20]="0000-00-00-00-00-00";
char text_log[300];
int TrigDelay=0x18;
//FILE* fp_TAConfig;
FILE* fp_cmd_log;
FILE* fp_text_log;																								  
/////////////////////////////////////////////////////////
//int fp_cmd_log_1,fp_cmd_log_2,fp_cmd_log_3,fp_cmd_log_4;
/////////////////////////////////////////////////////////
char WrDataPath[300]={"./"};
int CRC_Array[16]={1};
int ALLFEE;
int HitAndArray[64] ={0};
unsigned char CRC_Result[2];
//unsigned char CommandWord[2];
char IPAddress[100] = {"USB0::0x0699::0x0345::C022722::INSTR"};
unsigned int     writerChannel = 0; 
int duty_i = 0;
int Get_Mask_Ch_ID[64];
int Statue_Mask_Ch[64];
void CRC_Check ( unsigned char data ) ;

int main (int argc, char *argv[])											  //done
{
  if (InitCVIRTE (0, argv, 0) == 0)
    return -1;	/* out of memory */
  if ((mainHandle = LoadPanel (0, "Main.uir", Main)) < 0)
    return -1;
  DisplayPanel (mainHandle);
  RunUserInterface ();
  DiscardPanel (mainHandle);																																					
  return 0;
}

int CVICALLBACK Manual_USB_Cmd (int panel, int control, int event,			  //done
				void *callbackData, int eventData1, int eventData2)
{   
  unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData;
  int Add_Data;
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:
      /*	if (instr==0 || USB_Status==0) 
		{
		MessagePopup ("错误", "未打开USB设备！");
		return -1;
		}					 */
      //	GetPanelHandleFromTabPage(mainHandle,Main_Cmd,0,&usbHandle);
      GetCtrlVal(mainHandle,Main_Manual_USB,&ConfigData);
      GetCtrlVal(mainHandle,Main_Manual_USB_Add,&Add_Data);    
      ConfigData = ConfigData +  Add_Data;
      SetCtrlVal(mainHandle,Main_Manual_USB,ConfigData);
      TempBuffer[1]=ConfigData&(0xFF);
      TempBuffer[0]=ConfigData>>8;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  					 
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      break;
    }
  return 0;
}

int CVICALLBACK Open_ConfigFile (int panel, int control, int event,			 //done
				 void *callbackData, int eventData1, int eventData2)
{
  FILE *fp_read;
  char ConfigFile[300]="ConfigFile";
  unsigned char TempBuffer[12];
  int flag=0;
  switch (event)
    {
    case EVENT_COMMIT:
      if(COM_Status==0)
	{
	  MessagePopup("错误","未打开COM设备！");
	  return(-1);
	}
      //FileSelectPopup ("", "", "dat", "选择TA配置文件的路径", VAL_OK_BUTTON, 0, 0, 1, 0, ConfigFile);  
      SetCtrlVal(mainHandle,Main_TA_ConfigFile,ConfigFile);
      if((fp_read=fopen(ConfigFile,"r"))==NULL)
	{
	  MessagePopup("错误","打开文件失败！");
	  return(-1);
	}
      if(Monitor_Status==1)
	{
	  flag=1;
	  Monitor_Status=0;
	  if (threadID4){
	    //std::thread
	    //CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,threadID4 , OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
	  }
	  SetCtrlVal(mainHandle, Main_TEXTBOX, "关闭遥测线程..\n");
	  SetCtrlVal(mainHandle,Main_Monitor_Status,0);
	}
      while(!feof(fp_read))
	{
	  fread(TempBuffer,1,12,fp_read);
	  if((TempBuffer[2]!=0xeb)||(TempBuffer[3]!=0x90)||(TempBuffer[10]!=0x5a)||(TempBuffer[11]!=0xa5))
	    {
	      MessagePopup("错误","配置文件有误！");
	      return(-1);
	    }
	  SendComData(1, TempBuffer);
	  sleep(15);
	}
      SetCtrlVal(mainHandle,Main_TEXTBOX,"TA命令配置成功...\n");
      if(flag==1)
	{
	  if(COM_Status==0)
	    {
	      MessagePopup("错误","未连接COM设备！");
	      Monitor_Status=0;
	      return(-1);
	    }
	  //std::thread
	  //CmtScheduleThreadPoolFunction(DEFAULT_THREAD_POOL_HANDLE, MonitorFunction, &panel, &threadID4);
	  SetCtrlVal(mainHandle,Main_TEXTBOX,"开启遥测线程...\n");
	  SetCtrlVal(mainHandle,Main_Monitor_Status,1);
	  Monitor_Status=1;
	}
      break;
    }
  return 0;
}

int CVICALLBACK Open_SaveFile (int panel, int control, int event,			 //done
			       void *callbackData, int eventData1, int eventData2)
{
  char FILE_CMD_LOG[300];
  char FILE_TEXT_BOX[300];

	
  switch (event)
    {
    case EVENT_COMMIT:
      
      //DirSelectPopup ("E:\\Work_File\\CEPC\\Data", "选择文件保存路径", 1, 1, WrDataPath);   //E:\Work_File\CEPC\Data
      datasaveflag=1;
      SetCtrlVal(mainHandle,Main_Save_Path,WrDataPath);
      sprintf(FILE_CMD_LOG, "%s//CMD_LOG.txt", WrDataPath);
      sprintf(FILE_TEXT_BOX,"%s\\TEXT_LOG.txt",WrDataPath);
      fp_cmd_log = fopen(FILE_CMD_LOG, "a");
      
      break;
    }
  return 0;
}

int CVICALLBACK Set_Delay (int panel, int control, int event,			//done
			   void *callbackData, int eventData1, int eventData2)
{
  int i;
  char TempMessage[100];
  int TempID1,TempID2;
  int flag=0;
  switch (event)
    {
    case EVENT_COMMIT:
      if(COM_Status==0)
	{
	  MessagePopup("错误","未打开COM设备！");
	  return(-1);
	}
      if(Monitor_Status==1)
	{
	  flag=1;
	  Monitor_Status=0;
	  if (threadID4){
	    //std::thread
	    //CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,threadID4 , OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
	  }
	  //SetCtrlVal(mainHandle, Main_TEXTBOX, "关闭遥测线程..\n");
				
	  //GetCurrentDateTime(&CurTime);
	  //FormatDateTimeString(CurTime, "%Y-%m-%d-%H-%M-%S", timemsg, 20);
	  Time_Length = strlen ( timemsg ) ;
	  //WriteFile(fp_text_log, timemsg, Time_Length ) ;
	  fwrite(timemsg, Time_Length, 1, fp_text_log);
	  sprintf(text_log,"关闭遥测线程..\n");
	  SetCtrlVal(mainHandle,Main_TEXTBOX,text_log);
	  Log_Length = strlen(text_log);
	  //WriteFile(fp_text_log, text_log, Log_Length );
	  fwrite(text_log, Log_Length, 1, fp_text_log);
			
	  SetCtrlVal(mainHandle,Main_Monitor_Status,0);
	}
      GetCtrlVal(mainHandle,Main_Delay_Value,&TrigDelay);
      for(i=2;i<16;i++)
	{
	  if(FEE[i]==0)
	    continue;
	  if((SetDelay(1,FEEID[i],TrigDelay))==-1)
	    {
	      MessagePopup("错误","发送COM配置命令失败！");
	      return(-1);
	    }
	  sprintf(TempMessage,"发送触发延迟值0x%02x到FEE %s...\n",TrigDelay,FEENAME[i]);
	  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	}
      if(flag==1)
	{
	  if(COM_Status==0)
	    {
	      MessagePopup("错误","未连接COM设备！");
	      Monitor_Status=0;
	      return(-1);
	    }
	  //std::thread
	  //CmtScheduleThreadPoolFunction(DEFAULT_THREAD_POOL_HANDLE, MonitorFunction, &panel, &threadID4);
	  SetCtrlVal(mainHandle,Main_TEXTBOX,"开启遥测线程...\n");
	  SetCtrlVal(mainHandle,Main_Monitor_Status,1);
	  Monitor_Status=1;
	}
      break;
    }
  return 0;
}

int CVICALLBACK Clear_Textbox (int panel, int control, int event,		 //done
			       void *callbackData, int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_COMMIT:
      ResetTextBox(mainHandle,Main_TEXTBOX,"");
      break;
    }
  return 0;
}

int CVICALLBACK Stop_Monitor (int panel, int control, int event,		  //done
			      void *callbackData, int eventData1, int eventData2)
{
  int normalHandle;
  switch (event)
    {
    case EVENT_COMMIT:
      if(Monitor_Status==1)
	{
	  if (threadID4)
	    {
	      Monitor_Status=0;
	      //std::thread
	      //CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,threadID4 , OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
	    }
	  SetCtrlVal(mainHandle, Main_TEXTBOX, "关闭遥测线程..\n");
	  SetCtrlVal(mainHandle,Main_Monitor_Status,0);
	  Monitor_Status=0;
	}
      GetPanelHandleFromTabPage(mainHandle,Main_WorkMode,0,&normalHandle);
      /* SetCtrlAttribute(normalHandle,Normal_Stop_Norm_Acq,ATTR_DIMMED,0);  */
      /* SetCtrlAttribute(mainHandle,Main_Stop_Monitor,ATTR_DIMMED,1); */
      /* SetCtrlAttribute(mainHandle,Main_Start_Monitor,ATTR_DIMMED,0); */
      break;
    }
  return 0;
}

int CVICALLBACK Start_Monitor (int panel, int control, int event,		   //done
			       void *callbackData, int eventData1, int eventData2)
{
  int normalHandle;
  switch (event)
    {
    case EVENT_COMMIT:
      if(datasaveflag==0)
	{
	  MessagePopup ("错误", "请选择文件保存路径！");
	  return -1;
	}
      if(FEE[0]+FEE[1]+FEE[2]+FEE[3]+FEE[4]+FEE[5]+FEE[6]+FEE[7]+FEE[8]+FEE[9]+FEE[10]+FEE[11]+FEE[12]+FEE[13]+FEE[14]+FEE[15]==0)
	{
	  MessagePopup("错误","没有找到挂载的FEE！");
	  return(-1);
	}
      if(COM_Status==0)
	{
	  MessagePopup("错误","未连接COM设备！");
	  Monitor_Status=0;
	  return(-1);
	}
      Monitor_Status=1;
      //std::thread
      //CmtScheduleThreadPoolFunction(DEFAULT_THREAD_POOL_HANDLE, MonitorFunction, &panel, &threadID4);
      SetCtrlVal(mainHandle,Main_TEXTBOX,"开启遥测线程...\n");
      SetCtrlVal(mainHandle,Main_Monitor_Status,1);
      GetPanelHandleFromTabPage(mainHandle,Main_WorkMode,0,&normalHandle);
      /* SetCtrlAttribute(normalHandle,Normal_Stop_Norm_Acq,ATTR_DIMMED,1);  */
      /* SetCtrlAttribute(mainHandle,Main_Stop_Monitor,ATTR_DIMMED,0); */
      /* SetCtrlAttribute(mainHandle,Main_Start_Monitor,ATTR_DIMMED,1); */
      break;
    }
  return 0;
}

int CVICALLBACK Connect_Com (int panel, int control, int event,			  //done
			     void *callbackData, int eventData1, int eventData2)
{
  int i;
  int  normalHandle;
  char TempMessage[100];
  switch (event)
    {
    case EVENT_COMMIT:
      if(FEE[0]+FEE[1]+FEE[2]+FEE[3]+FEE[4]+FEE[5]+FEE[6]+FEE[7]+FEE[8]+FEE[9]+FEE[10]+FEE[11]+FEE[12]+FEE[13]+FEE[14]+FEE[15]==0)
	{
	  MessagePopup("错误","没有找到挂载的FEE！");
	  return(-1);
	}
      if(OpenComConfig (1, "COM5", 115200, 1, 8, 2, 512, 512)<0)
	{
	  MessagePopup("错误","打开COM设备失败！");
	  SetCtrlVal(mainHandle,Main_COM_Status,0);
	  COM_Status=0;
	  return(-1);
	}
      SetCtrlVal(mainHandle,Main_COM_Status,1);
      COM_Status=1;
      SetCtrlVal(mainHandle,Main_TEXTBOX,"COM设备已连接...\n");
      FlushOutQ(1);
      //std::thread
      //CmtScheduleThreadPoolFunction(DEFAULT_THREAD_POOL_HANDLE, COMStatus, &panel, &threadID3);
      for(i=0;i<16;i++)
	{	
	  if(FEE[i]==0)
	    continue;
	  LoopbackTest(1,FEEID[i]);
	  LoopbackTest(1,FEEID[i]);
	  LoopbackTest(1,FEEID[i]);
	  if ( (LoopbackTest(1,FEEID[i]))==-1)
	    {
	      MessagePopup("错误","发送COM配置命令失败！");
	      return -1;
	    }
	  else
	    {
	      sprintf(TempMessage,"%s<%s%s%s","FEE",FEENAME[i],"回环测试...\n");
	      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	    }
	  /*	if ( (ResetFPGA(1,FEEID[i]))==-1)
		{
		MessagePopup("错误","发送COM配置命令失败！");
		return -1;
		}
		else
		{
		Fmt(TempMessage,"%s<%s%s%s","FEE",FEENAME[i],"FPGA逻辑复位...\n");
		SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		}		  */
	  if ( (ClearTrig(1,FEEID[i]))==-1)
	    {
	      MessagePopup("错误","发送COM配置命令失败！");
	      return -1;
	    }
	  else
	    {
	      sprintf(TempMessage,"%s<%s%s%s","FEE",FEENAME[i],"触发计数清零...\n");
	      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	    }
	  if ( (ConfigReg(1,FEEID[i],0,0,0,0,0,0))==-1)
	    {
	      MessagePopup("错误","发送COM配置命令失败！");
	      return -1;
	    }
	  else
	    {
	      sprintf(TempMessage,"%s<%s%s%s","FEE",FEENAME[i],"触发接收关闭...\n");
	      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	    }  
	}
      Set_Current_Thr();
      GetPanelHandleFromTabPage(mainHandle,Main_WorkMode,0,&normalHandle);
      /* SetCtrlAttribute(normalHandle,Normal_Start_Norm_Acq,ATTR_DIMMED,0);  */
      break;
    }
  return 0;
}

int CVICALLBACK Connect_USB (int panel, int control, int event,			   //done
			     void *callbackData, int eventData1, int eventData2)
{
  int n;
  int i;
  char TempMessage[100];
  unsigned char TmpData[512];
  char desc[256];
  unsigned char CommandWord[2];
  switch (event)
    {
    case EVENT_COMMIT:
      /*	if(FEE[0]+FEE[1]+FEE[2]+FEE[3]+FEE[4]+FEE[5]+FEE[6]+FEE[7]+FEE[8]+FEE[9]+FEE[10]+FEE[11]+FEE[12]+FEE[13]+FEE[14]+FEE[15]==0)
		{
		MessagePopup("错误","没有找到挂载的FEE！");
		return(-1);
		}			   */
      status = viOpenDefaultRM(&defaultRM);		
      if (status < VI_SUCCESS)
	{
	  MessagePopup ("错误", "打开USB设备失败！");
	  SetCtrlVal(mainHandle,Main_USB_Status,0);
	  USB_Status=0;
	  return -1;
	}
      status = viFindRsrc(defaultRM, "USB[0-9]::0x04B4::0x1004::NI-VISA-[0-9]000[0-9]::RAW", VI_NULL, VI_NULL, desc);		//搜索设备，仅匹配VID和PID
      if (status < VI_SUCCESS)
	{
	  status = viFindRsrc(defaultRM, "USB[0-9]::0x04B4::0x1004::NI-VISA-[0-9]::RAW", VI_NULL, VI_NULL, desc);
	  if(status<VI_SUCCESS)
	    {
	      MessagePopup ("错误", "USB设备不存在！");
	      SetCtrlVal(mainHandle,Main_USB_Status,0);
	      USB_Status=0;
	      return -1;
	    }
	}
      status = viOpen(defaultRM, desc, VI_NULL, VI_NULL, &instr);
      if (status < VI_SUCCESS)
	{
	  MessagePopup ("错误", "未能打开指定USB设备！");
	  SetCtrlVal(mainHandle,Main_USB_Status,0);
	  USB_Status=0;
	  return -1;					
	}
      SetCtrlVal(mainHandle,Main_USB_Status,1);
			
      SetCtrlVal(mainHandle,Main_TEXTBOX,"USB设备已连接...\n"); 
      sprintf(text_log,"USB设备已连接...\n");
      break;
    }
  return 0;
}

int CVICALLBACK Sys_Reset (int panel, int control, int event,			   //done
			   void *callbackData, int eventData1, int eventData2)
{
  int i;
  int flag=0;
  char TempMessage[100];
  unsigned char CommandWord[2];
  switch (event)
    {
    case EVENT_COMMIT:
      MessagePopup ("抱歉", "暂时不支持软件复位，请按复位键U72！");
      break;
    }
  return 0;
}

int CVICALLBACK Show_Monitor (int panel, int control, int event,			  //halfdone
			      void *callbackData, int eventData1, int eventData2)
{
  int FEENum;
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(mainHandle,Main_Show_FEE,&FEENum);
      if(FEENum>0)
	{
	  switch(FEENum)
	    {
	    case 1:dutyHandle=LoadPanel(0,"Duty_Mode.uir",Duty_Mode);DisplayPanel(dutyHandle);break;
	    case 2:PX_Handle =LoadPanel(0,"PX.uir",PX);DisplayPanel(PX_Handle);break;  
	      //	default :countHandle = LoadPanel(0,"CountHit.uir",CountHit);DisplayPanel(countHandle);break;
	    }
	  //			Show_FEE_Status[FEENum-1]=1;
	}
      else
	countHandle = LoadPanel(0,"CountHit.uir",CountHit);DisplayPanel(countHandle);;
      break;
    }
  return 0;
}

int CVICALLBACK Quit (int panel, int control, int event,					  //done
		      void *callbackData, int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_COMMIT:
      QuitUserInterface (0);
      break;
    }
  return 0;
}

int CVICALLBACK Data_Mode (int panel, int control, int event,				 //done
			   void *callbackData, int eventData1, int eventData2)
{
  int normalHandle;
  switch (event)
    {
    case EVENT_COMMIT:
      GetPanelHandleFromTabPage(mainHandle,Main_WorkMode,0,&normalHandle);
      GetCtrlVal(normalHandle,Normal_Data_Mode,&Data_Mode_Status);
      if(Data_Mode_Status==1)
	{
	  //SetCtrlAttribute(normalHandle,Normal_Threshold_ChnID,ATTR_DIMMED,1);
	  //SetCtrlAttribute(normalHandle,Normal_High_Threshold,ATTR_DIMMED,1);
	  //SetCtrlAttribute(normalHandle,Normal_Low_Threshold,ATTR_DIMMED,1);
	}
      else
	{
	  //SetCtrlAttribute(normalHandle,Normal_Threshold_ChnID,ATTR_DIMMED,0);
	  //SetCtrlAttribute(normalHandle,Normal_High_Threshold,ATTR_DIMMED,0);
	  //SetCtrlAttribute(normalHandle,Normal_Low_Threshold,ATTR_DIMMED,0);
	}
      break;
    }
  return 0;
}

int CVICALLBACK Cali_Mode (int panel, int control, int event,				 //done
			   void *callbackData, int eventData1, int eventData2)
{
  switch (event)
    {
      int caliHandle;
    case EVENT_COMMIT:
      GetPanelHandleFromTabPage(mainHandle,Main_WorkMode,1,&caliHandle);
      GetCtrlVal(caliHandle,Calitest_Cali_Mode,&Cali_Mode_Status);
      if(Cali_Mode_Status==0)
	{
	  //SetCtrlAttribute(caliHandle,Calitest_Volt,ATTR_DIMMED,0);
	  //SetCtrlAttribute(caliHandle,Calitest_S_Volt,ATTR_DIMMED,1);
	  //SetCtrlAttribute(caliHandle,Calitest_E_Volt,ATTR_DIMMED,1);
	  //SetCtrlAttribute(caliHandle,Calitest_Step,ATTR_DIMMED,1);
	}
      else
	{
	  //SetCtrlAttribute(caliHandle,Calitest_Volt,ATTR_DIMMED,1);
	  //SetCtrlAttribute(caliHandle,Calitest_S_Volt,ATTR_DIMMED,0);
	  //SetCtrlAttribute(caliHandle,Calitest_E_Volt,ATTR_DIMMED,0);
	  //SetCtrlAttribute(caliHandle,Calitest_Step,ATTR_DIMMED,0);
	}
      break;
    }
  return 0;
}

int CVICALLBACK MainFunc (int panel, int event, void *callbackData,			  //done
			  int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_GOT_FOCUS:

      break;
    case EVENT_LOST_FOCUS:

      break;
    case EVENT_CLOSE:
      GetData_Status=0;
      if (threadID1){
	//std::thread
	//CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,threadID1 , OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
      }
      USB_Status=0;
      if (threadID2){
	//std::thread
	//CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,threadID2 , OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
      }
      COM_Status=0;
      if (threadID3){
	//std::thread
	//CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,threadID3 , OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
      }
      Monitor_Status=0;
      if (threadID4){
	//std::thread
	//CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,threadID4 , OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
      }
      status = viClose(instr);
      status = viClose(defaultRM);
      Error=CloseCom(1);
      break;
    }
  return 0;
}

int CVICALLBACK FEE1_Close (int panel, int event, void *callbackData,		    //done   
			    int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_GOT_FOCUS:

      break;
    case EVENT_LOST_FOCUS:

      break;
    case EVENT_CLOSE:
      DiscardPanel(Status1Handle);
      Show_FEE_Status[0]=0;
      break;
    }
  return 0;
}

int CVICALLBACK FEE2_Close (int panel, int event, void *callbackData,		    //done   
			    int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_GOT_FOCUS:

      break;
    case EVENT_LOST_FOCUS:

      break;
    case EVENT_CLOSE:
      DiscardPanel(Status2Handle);
      Show_FEE_Status[1]=0;

      break;
    }
  return 0;
}

int CVICALLBACK FEE3_Close (int panel, int event, void *callbackData,		   //done   
			    int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_GOT_FOCUS:

      break;
    case EVENT_LOST_FOCUS:

      break;
    case EVENT_CLOSE:
      DiscardPanel(Status3Handle);
      Show_FEE_Status[2]=0;

      break;
    }
  return 0;
}

int CVICALLBACK FEE4_Close (int panel, int event, void *callbackData,		    //done   
			    int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_GOT_FOCUS:

      break;
    case EVENT_LOST_FOCUS:

      break;
    case EVENT_CLOSE:
      DiscardPanel(Status4Handle);
      Show_FEE_Status[3]=0;

      break;
    }
  return 0;
}

int CVICALLBACK FEE5_Close (int panel, int event, void *callbackData,		    //done   
			    int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_GOT_FOCUS:

      break;
    case EVENT_LOST_FOCUS:

      break;
    case EVENT_CLOSE:
      DiscardPanel(Status5Handle);
      Show_FEE_Status[4]=0;

      break;
    }
  return 0;
}

int CVICALLBACK FEE6_Close (int panel, int event, void *callbackData,			  //done   
			    int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_GOT_FOCUS:

      break;
    case EVENT_LOST_FOCUS:

      break;
    case EVENT_CLOSE:
      DiscardPanel(Status6Handle);
      Show_FEE_Status[5]=0;

      break;
    }
  return 0;
}

int CVICALLBACK FEE7_Close (int panel, int event, void *callbackData,			  //done   
			    int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_GOT_FOCUS:

      break;
    case EVENT_LOST_FOCUS:

      break;
    case EVENT_CLOSE:
      DiscardPanel(Status7Handle);
      Show_FEE_Status[6]=0;

      break;
    }
  return 0;
}

int CVICALLBACK FEE8_Close (int panel, int event, void *callbackData,			 //done   
			    int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_GOT_FOCUS:

      break;
    case EVENT_LOST_FOCUS:

      break;
    case EVENT_CLOSE:
      DiscardPanel(Status8Handle);
      Show_FEE_Status[7]=0;

      break;
    }
  return 0;
}

int CVICALLBACK Start_Norm_Acq (int panel, int control, int event,	  //done
				void *callbackData, int eventData1, int eventData2)
{
  char WrDataFile[300];
  char TempMessage[100];
  unsigned char TempBuffer[512];
  unsigned char CommandBuffer[2];
  char timemsg[20]="0000-00-00-00-00-00";
  double CurTime;
  int Mode,DataMode,WorkMode,Trig;
  int InternalTime;
  int i,j;
  unsigned int n;
  int mode1;
  int Chn_ID;
  int normalHandle;
  int High_Threshold,Low_Threshold;
  int flag=0;
  int four_Cmd,HitEnable,TrigChoose,ClassicsHitMode,HitChnPara;
  int HitChoose;
  int CMD_Sel;   
  switch (event)
    {
    case EVENT_COMMIT:
	
      if(datasaveflag==0)
	{
	  MessagePopup ("错误", "请选择文件保存路径！");
	  return -1;
	}

      /* GetCurrentDateTime(&CurTime); */
      /* FormatDateTimeString(CurTime, "%Y-%m-%d-%H-%M-%S", timemsg, 20); */
      sprintf(WrDataFile,"%s\\%s.dat",WrDataPath,timemsg);
      fp_write=fopen(WrDataFile, "wb");
      if(fp_write==NULL)
	{
	  MessagePopup ("错误", "新建数据保存文件失败！");
	  return -1;
	}
			
      GetCtrlVal(mainHandle,Main_DAQTrigMode,&DAQTrig_Mode_Status);
      switch(DAQTrig_Mode_Status)
	{
	case 1:
	  CommandBuffer[1] = 0xaa;
          CommandBuffer[0] = 0x01;
	  if(SendUsbData(instr, CommandBuffer) ==-1)
	    {
	      MessagePopup ("错误", "发送USB配置命令失败！！");
	      return -1;
	    }
	  else
	    {

	      //printf("Command[1] = %02x, Command[0] = %02x\n",CommandBuffer[1],CommandBuffer[0]);
	      SetCtrlVal(mainHandle,Main_TEXTBOX,"设置为内触发模式\n");
	    }
	  GetCtrlVal(mainHandle,Main_CaliInternal,&InternalTime);
          CommandBuffer[1] = 0xab;
          CommandBuffer[0] = InternalTime;
	  if(SendUsbData(instr, CommandBuffer) ==-1)
	    {
	      MessagePopup ("错误", "发送USB配置命令失败！！");
	      return -1;
	    }
	  else
	    {

	      //printf("Command[1] = %02x, Command[0] = %02x\n",CommandBuffer[1],CommandBuffer[0]);
	      sprintf(TempMessage,"设置内触发间隔为 %d ms\n",InternalTime);
	      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	    }


          
	  break;
	case 2:
	  CommandBuffer[1] = 0xaa;
          CommandBuffer[0] = 0x02;
	  if(SendUsbData(instr, CommandBuffer) ==-1)
	    {
	      MessagePopup ("错误", "发送USB配置命令失败！！");
	      return -1;
	    }
	  else
	    {

	      //printf("Command[1] = %02x, Command[0] = %02x\n",CommandBuffer[1],CommandBuffer[0]);
	      SetCtrlVal(mainHandle,Main_TEXTBOX,"设置为外触发模式，触发信号为500ns宽，高电平有效的信号\n");
	    }
	  break;
	case 3:
          CommandBuffer[1] = 0xaa;
          CommandBuffer[0] = 0x03;
	  if(SendUsbData(instr, CommandBuffer) ==-1)
	    {
	      MessagePopup ("错误", "发送USB配置命令失败！！");
	      return -1;
	    }
	  else
	    {

	      //printf("Command[1] = %02x, Command[0] = %02x\n",CommandBuffer[1],CommandBuffer[0]);
	      SetCtrlVal(mainHandle,Main_TEXTBOX,"设为自触发“或”模式，4个TA相或\n");
	    }
	  break;
	case 4:
          CommandBuffer[1] = 0xaa;
          CommandBuffer[0] = 0x04;
	  if(SendUsbData(instr, CommandBuffer) ==-1)
	    {
	      MessagePopup ("错误", "发送USB配置命令失败！！");
	      return -1;
	    }
	  else
	    {

	      //printf("Command[1] = %02x, Command[0] = %02x\n",CommandBuffer[1],CommandBuffer[0]);
	      SetCtrlVal(mainHandle,Main_TEXTBOX,"设为自触发“与”模式\n");
	    }
	  break;
					
	default:
	  MessagePopup("错误","不明的触发模式！");
	  return(-1);
			
		     
	  break;
	}

      CommandBuffer[1] = 0x00;
      CommandBuffer[0] = 0xa1;
      if(SendUsbData(instr, CommandBuffer) ==-1)
	{
	  MessagePopup ("错误", "发送USB配置命令失败！！");
	  return -1;
	}
      else
	{

	  //printf("Command[1] = %02x, Command[0] = %02x\n",CommandBuffer[1],CommandBuffer[0]);
	  SetCtrlVal(mainHandle,Main_TEXTBOX,"开启触发\n");
	}
      status=viRead(instr,TempBuffer,512,&n);
      sleep(15);
      //	status=viRead(instr,TempBuffer,512,&n);
      //	sleep(15);
			
		
		
			
      GetData_Status=1; 
      SetCtrlVal(mainHandle,Main_TEXTBOX,"开始数据采集...\n");
      SetCtrlVal(mainHandle,Main_Acq_Status,1);
      //std::thread
      //CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, Acqing, &mainHandle, &threadID4);
      SetCtrlVal(mainHandle,Main_TEXTBOX,"开启监测...\n");
      //std::thread
      //CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, DataAcq, &mainHandle, &threadID1);
      SetCtrlVal(mainHandle,Main_TEXTBOX,"开启数据采集线程...\n");

      break;
    }
  return 0;
}

int CVICALLBACK Stop_Norm_Acq (int panel, int control, int event,	  //done
			       void *callbackData, int eventData1, int eventData2)
{
  char TempMessage[100];
  unsigned char CommandBuffer[2];
  int normalHandle;
  int i;
  unsigned char CommandWord[2];
  switch (event)
    {
    case EVENT_COMMIT:
      CommandBuffer[1] = 0x00;
      CommandBuffer[0] = 0xa0;
      if(SendUsbData(instr, CommandBuffer) ==-1)
	{
	  MessagePopup ("错误", "发送USB配置命令失败！！");
	  return -1;
	}
      else
	{

	  //printf("Command[1] = %02x, Command[0] = %02x\n",CommandBuffer[1],CommandBuffer[0]);
	  SetCtrlVal(mainHandle,Main_TEXTBOX,"关闭触发\n");
	}
      GetData_Status=0;
      sleep(100); 
      SetCtrlVal(mainHandle,Main_Acq_Status,0);
      SetCtrlVal(mainHandle,Main_TEXTBOX,"结束采集命令发出...\n");
	
      fclose(fp_write);
	
      //Beep();																	
      GetPanelHandleFromTabPage(mainHandle,Main_WorkMode,0,&normalHandle);
	
      break;
    }
  return 0;
}

int CVICALLBACK Start_Cali_Acq (int panel, int control, int event,	  //done
				void *callbackData, int eventData1, int eventData2)
{
  int i;
  int InternalTime;
  char TempMessage[100];
  int Mode;
  int flag=0;
  switch (event)
    {
    case EVENT_COMMIT:
      //std::thread
      //CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, CaliAcq, &mainHandle, &threadID5);
      Cali_Status=1;
      break;
    }
  return 0;
}

int CVICALLBACK Stop_Cali_Acq (int panel, int control, int event,	   //done
			       void *callbackData, int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_COMMIT:
      Cali_Status=0;
      break;
    }
  return 0;
}

int CVICALLBACK LoadFEE (int panel, int control, int event,		   //done
			 void *callbackData, int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_COMMIT:
      LoadFEE_Status=1;
      GetCtrlVal(mainHandle,Main_FEE1,&FEE[0]);
      GetCtrlVal(mainHandle,Main_FEE2,&FEE[1]);
      GetCtrlVal(mainHandle,Main_FEE3,&FEE[2]);
      GetCtrlVal(mainHandle,Main_FEE4,&FEE[3]);
      GetCtrlVal(mainHandle,Main_FEE5,&FEE[4]);
      GetCtrlVal(mainHandle,Main_FEE6,&FEE[5]);
      GetCtrlVal(mainHandle,Main_FEE7,&FEE[6]);
      GetCtrlVal(mainHandle,Main_FEE8,&FEE[7]);
      GetCtrlVal(mainHandle,Main_FEE9,&FEE[8]);
      GetCtrlVal(mainHandle,Main_FEE10,&FEE[9]);
      GetCtrlVal(mainHandle,Main_FEE11,&FEE[10]);
      GetCtrlVal(mainHandle,Main_FEE12,&FEE[11]);
      GetCtrlVal(mainHandle,Main_FEE13,&FEE[12]);
      GetCtrlVal(mainHandle,Main_FEE14,&FEE[13]);
      GetCtrlVal(mainHandle,Main_FEE15,&FEE[14]);
      GetCtrlVal(mainHandle,Main_FEE16,&FEE[15]);
      break;
    }
  return 0;
}

int CVICALLBACK Manual_COM_Cmd (int panel, int control, int event,			//done
				void *callbackData, int eventData1, int eventData2)
{
  int TempData;
  int Plus;
  int CheckTemp=0;
  int FeeID,flag =0;
  int i;
  int comHandle;
  unsigned char TempBuffer[12];
  unsigned char CheckBuffer[2];
  char TempMessage[100];
  switch (event)
    {
    case EVENT_COMMIT:
      if(COM_Status==0)
	{
	  MessagePopup("错误","未打开COM设备！");
	  return(-1);
	}
      FlushInQ(1);
      if(Monitor_Status==1)
	{
	  Monitor_Status=0;
	  if (threadID4){
	    //std::thread
	    //CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,threadID4 , OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
	  }
	  SetCtrlVal(mainHandle, Main_TEXTBOX, "关闭遥测线程...\n");
	  SetCtrlVal(mainHandle,Main_Monitor_Status,0);
	  MessagePopup("注意","遥测线程已关闭！");
	}
      TempBuffer[0]=0x55;
      TempBuffer[1]=0xaa;
      TempBuffer[2]=0xeb;
      TempBuffer[3]=0x90;
      GetPanelHandleFromTabPage(mainHandle,Main_Cmd,0,&comHandle);
      GetCtrlVal(comHandle,COM_Cmd_Input_COM_FEE,&FeeID);
      if(FeeID == 0)   //by siyuan
	{
	  FeeID = 1;
	  flag = 1;
	}
      TempBuffer[4]=FEEID[FeeID-1];
      CheckTemp=FEEID[FeeID-1];
      GetCtrlVal(comHandle,COM_Cmd_Manual_COM_Type,&TempData);
      TempBuffer[5]=TempData;
      CheckTemp=CheckTemp+TempData;
      GetCtrlVal(comHandle,COM_Cmd_Manual_COM_High,&TempData);
      TempBuffer[6]=TempData;
      CheckTemp=CheckTemp+TempData;
      GetCtrlVal(comHandle,COM_Cmd_Manual_COM_Low,&TempData);
      TempBuffer[7]=TempData;
      CheckTemp=CheckTemp+TempData;
      Check(CheckTemp,CheckBuffer);
      TempBuffer[8]=CheckBuffer[0];
      //			TempBuffer[8]=0x00;
      TempBuffer[9]=CheckBuffer[1];
      //			TempBuffer[9]=0x00;
      TempBuffer[10]=0x5a;
      TempBuffer[11]=0xa5;
      if(flag == 1)			   //by siyuan
	{
	  for(i = 0;i<16; i ++)
	    {
	      sleep(100);
	      TempBuffer[4]=FEEID[i];
	      CheckTemp=FEEID[i];
	      GetCtrlVal(comHandle,COM_Cmd_Manual_COM_Type,&TempData);
	      TempBuffer[5]=TempData;
	      CheckTemp=CheckTemp+TempData;
	      GetCtrlVal(comHandle,COM_Cmd_Manual_COM_High,&TempData);
	      TempBuffer[6]=TempData;
	      CheckTemp=CheckTemp+TempData;
	      GetCtrlVal(comHandle,COM_Cmd_Manual_COM_Low,&TempData);
	      TempBuffer[7]=TempData;
	      CheckTemp=CheckTemp+TempData;
	      Check(CheckTemp,CheckBuffer);
	      TempBuffer[8]=CheckBuffer[0];
	      TempBuffer[9]=CheckBuffer[1];
	      if(SendComData(1,TempBuffer)==-1)
		{
		  MessagePopup("错误","发送COM配置命令失败！");
		  return(-1);
		}
	      sleep(100);
	      if(ReadComCmdResp(1)<0)
		return(-1);
	      sprintf(TempMessage,"Write 0x%02x%02x 0x%02x%02x Command via COM\n",TempBuffer[4],TempBuffer[5],TempBuffer[6],TempBuffer[7]);
	      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage); 
	      GetCtrlVal(comHandle,COM_Cmd_Manual_COM_High,&TempData);
	      GetCtrlVal(comHandle,COM_Cmd_Manual_COM_High_Plus,&Plus);
	      SetCtrlVal(comHandle,COM_Cmd_Manual_COM_High,TempData+Plus);
													
	    }
	  break;
	}
      if(SendComData(1,TempBuffer)==-1)
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      sleep(100);
      if(ReadComCmdResp(1)<0)
	return(-1);
      sprintf(TempMessage,"Write 0x%02x%02x 0x%02x%02x Command via COM\n",TempBuffer[4],TempBuffer[5],TempBuffer[6],TempBuffer[7]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage); 
      GetCtrlVal(comHandle,COM_Cmd_Manual_COM_High,&TempData);
      GetCtrlVal(comHandle,COM_Cmd_Manual_COM_High_Plus,&Plus);
      SetCtrlVal(comHandle,COM_Cmd_Manual_COM_High,TempData+Plus);
      GetCtrlVal(comHandle,COM_Cmd_Manual_COM_Low,&TempData);
      GetCtrlVal(comHandle,COM_Cmd_Manual_COM_Low_Plus,&Plus);
      SetCtrlVal(comHandle,COM_Cmd_Manual_COM_Low,TempData+Plus);
      break;
    }
  return 0;
}

int CVICALLBACK DAQ_Trig_Mode (int panel, int control, int event,			//done
			       void *callbackData, int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(mainHandle,Main_DAQTrigMode,&DAQTrig_Mode_Status);
      if(DAQTrig_Mode_Status==1){
	//SetCtrlAttribute(mainHandle,Main_CaliInternal,ATTR_DIMMED,0);
      }
      //SetCtrlAttribute(mainHandle,Main_HitEnable,ATTR_DIMMED,1);
      //SetCtrlAttribute(mainHandle,Main_HitChoose,ATTR_DIMMED,1);
      //SetCtrlAttribute(mainHandle,Main_ClassicsHitMode,ATTR_DIMMED,1);
      SetCtrlVal(mainHandle,Main_HitEnable,1);
      //SetCtrlAttribute(mainHandle,Main_More_Hit_Mode,ATTR_DIMMED,1);
      if(DAQTrig_Mode_Status==2){
	//SetCtrlAttribute(mainHandle,Main_CaliInternal,ATTR_DIMMED,1);
      }
      //SetCtrlAttribute(mainHandle,Main_HitEnable,ATTR_DIMMED,1);
      //SetCtrlAttribute(mainHandle,Main_HitChoose,ATTR_DIMMED,1);
      //SetCtrlAttribute(mainHandle,Main_ClassicsHitMode,ATTR_DIMMED,1);
      SetCtrlVal(mainHandle,Main_HitEnable,1);
      //SetCtrlAttribute(mainHandle,Main_More_Hit_Mode,ATTR_DIMMED,1);
      if(DAQTrig_Mode_Status==3)
	{
	  //SetCtrlAttribute(mainHandle,Main_HitEnable,ATTR_DIMMED,0);
	  //SetCtrlAttribute(mainHandle,Main_HitChoose,ATTR_DIMMED,0);
	  //SetCtrlAttribute(mainHandle,Main_ClassicsHitMode,ATTR_DIMMED,0);
	  //SetCtrlAttribute(mainHandle,Main_CaliInternal,ATTR_DIMMED,1);
	  SetCtrlVal(mainHandle,Main_HitEnable,0); 
	  //SetCtrlAttribute(mainHandle,Main_More_Hit_Mode,ATTR_DIMMED,0);//0:ON  1:OFF
	  
	}
      if(DAQTrig_Mode_Status==4)
	{
	  //SetCtrlAttribute(mainHandle,Main_HitEnable,ATTR_DIMMED,0);
	  //SetCtrlAttribute(mainHandle,Main_HitChoose,ATTR_DIMMED,0);
	  //SetCtrlAttribute(mainHandle,Main_ClassicsHitMode,ATTR_DIMMED,0);
	  //SetCtrlAttribute(mainHandle,Main_CaliInternal,ATTR_DIMMED,1);
	  SetCtrlVal(mainHandle,Main_HitEnable,0); 
	  //SetCtrlAttribute(mainHandle,Main_More_Hit_Mode,ATTR_DIMMED,0);//0:ON  1:OFF
	}
			
      break;
    }
  return 0;
}

int CVICALLBACK Close_COM_Cmd (int panel, int event, void *callbackData,    //done   
			       int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_GOT_FOCUS:

      break;
    case EVENT_LOST_FOCUS:

      break;
    case EVENT_CLOSE:
      DiscardPanel(comcmdHandle);
      COM_Cmd_Status=0;
      break;
    }
  return 0;
}

int CVICALLBACK SignalSweepStart (int panel, int control, int event,		 //done   
				  void *callbackData, int eventData1, int eventData2)
{
  int i;
  int InternalTime;
  char TempMessage[100];
  int Mode;
  switch (event)
    {
    case EVENT_COMMIT:
      if (instr==0 || USB_Status==0) 
	{
	  MessagePopup ("错误", "未打开USB设备！");
	  return -1;
	}
      if(datasaveflag==0)
	{
	  MessagePopup ("错误", "请选择文件保存路径！");
	  return -1;
	}
      if (COM_Status==0)
	{
	  MessagePopup ("错误", "未打开COM设备！");
	  return -1;
	}
      if(Cmd2DAQ_ChooseTrigMode(instr,3)==-1)
	{
	  MessagePopup("错误","发送USB配置命令失败！");
	  return(-1);
	}
      if(Cmd2DAQ_ChooseTrigMode(instr,4)==-1)
	{
	  MessagePopup("错误","发送USB配置命令失败！");
	  return(-1);
	}
      SetCtrlVal(mainHandle,Main_TEXTBOX,"设置DAQ为外触发模式...\n");
      SetCtrlVal(mainHandle,Main_Delay_Value,0x1b);
      //			TrigDelay = 0x1b;		modified by siyuan 20140807
      if(TrigDelay==0)
	{
	  MessagePopup("错误","未设置触发延迟值！");
	  return(-1);
	}
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
      //std::thread
      //CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, SignalSweepAcq, &mainHandle, &threadID6);
      SignalSweep_Status=1;
      break;
    }
  return 0;
}

int CVICALLBACK Close_SignalSweep (int panel, int event, void *callbackData,	   //done   
				   int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_GOT_FOCUS:

      break;
    case EVENT_LOST_FOCUS:

      break;
    case EVENT_CLOSE:
      DiscardPanel(signalsweepHandle);
      break;
    }
  return 0;
}

int CVICALLBACK CloseMutiUSB (int panel, int event, void *callbackData,		    //done   
			      int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_GOT_FOCUS:

      break;
    case EVENT_LOST_FOCUS:

      break;
    case EVENT_CLOSE:
      DiscardPanel(mutiusbHandle);
      break;
    }
  return 0;
}

int CVICALLBACK wtUSB1 (int panel, int control, int event,				    //done   
			void *callbackData, int eventData1, int eventData2)
{
  unsigned int n;
  int ConfigData;
  char TempMessage[100];
  unsigned char TempBuffer[2];
  switch (event)
    {
    case EVENT_COMMIT:
      if (instr==0 || USB_Status==0) 
	{
	  MessagePopup ("错误", "未打开USB设备！");
	  return -1;
	}
      GetCtrlVal(mutiusbHandle,MutiUSB_USB_Cmd1,&ConfigData);
      TempBuffer[0]=ConfigData&(0xFF);
      TempBuffer[1]=ConfigData>>8;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      break;
    }
  return 0;
}

int CVICALLBACK wtUSB2 (int panel, int control, int event,					   //done   
			void *callbackData, int eventData1, int eventData2)
{
  unsigned int n;
  int ConfigData;
  char TempMessage[100];
  unsigned char TempBuffer[2];
  switch (event)
    {
    case EVENT_COMMIT:
      if (instr==0 || USB_Status==0) 
	{
	  MessagePopup ("错误", "未打开USB设备！");
	  return -1;
	}
      GetCtrlVal(mutiusbHandle,MutiUSB_USB_Cmd2,&ConfigData);
      TempBuffer[0]=ConfigData&(0xFF);
      TempBuffer[1]=ConfigData>>8;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      break;
    }
  return 0;
}

int CVICALLBACK wtUSB3 (int panel, int control, int event,					  //done   
			void *callbackData, int eventData1, int eventData2)
{
  unsigned int n;
  int ConfigData;
  char TempMessage[100];
  unsigned char TempBuffer[2];
  switch (event)
    {
    case EVENT_COMMIT:
      if (instr==0 || USB_Status==0) 
	{
	  MessagePopup ("错误", "未打开USB设备！");
	  return -1;
	}
      GetCtrlVal(mutiusbHandle,MutiUSB_USB_Cmd3,&ConfigData);
      TempBuffer[0]=ConfigData&(0xFF);
      TempBuffer[1]=ConfigData>>8;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[1],TempBuffer[0]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      break;
    }
  return 0;
}

int CVICALLBACK wtUSB4 (int panel, int control, int event,					 //done   
			void *callbackData, int eventData1, int eventData2)
{
  unsigned int n;
  int ConfigData;
  char TempMessage[100];
  unsigned char TempBuffer[2];
  switch (event)
    {
    case EVENT_COMMIT:
      if (instr==0 || USB_Status==0) 
	{
	  MessagePopup ("错误", "未打开USB设备！");
	  return -1;
	}
      GetCtrlVal(mutiusbHandle,MutiUSB_USB_Cmd4,&ConfigData);
      TempBuffer[0]=ConfigData&(0xFF);
      TempBuffer[1]=ConfigData>>8;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      break;
    }
  return 0;
}

int CVICALLBACK wtUSB5 (int panel, int control, int event,					   //done   
			void *callbackData, int eventData1, int eventData2)
{
  unsigned int n;
  int ConfigData;
  char TempMessage[100];
  unsigned char TempBuffer[2];
  switch (event)
    {
    case EVENT_COMMIT:
      if (instr==0 || USB_Status==0) 
	{
	  MessagePopup ("错误", "未打开USB设备！");
	  return -1;
	}
      GetCtrlVal(mutiusbHandle,MutiUSB_USB_Cmd5,&ConfigData);
      TempBuffer[0]=ConfigData&(0xFF);
      TempBuffer[1]=ConfigData>>8;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      break;
    }
  return 0;
}

int CVICALLBACK wtUSB6 (int panel, int control, int event,					   //done   
			void *callbackData, int eventData1, int eventData2)
{
  unsigned int n;
  int ConfigData;
  char TempMessage[100];
  unsigned char TempBuffer[2];
  switch (event)
    {
    case EVENT_COMMIT:
      if (instr==0 || USB_Status==0) 
	{
	  MessagePopup ("错误", "未打开USB设备！");
	  return -1;
	}
      GetCtrlVal(mutiusbHandle,MutiUSB_USB_Cmd6,&ConfigData);
      TempBuffer[0]=ConfigData&(0xFF);
      TempBuffer[1]=ConfigData>>8;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      break;
    }
  return 0;
}

int CVICALLBACK wtUSB8 (int panel, int control, int event,					   //done   
			void *callbackData, int eventData1, int eventData2)
{
  unsigned int n;
  int ConfigData;
  char TempMessage[100];
  unsigned char TempBuffer[2];
  switch (event)
    {
    case EVENT_COMMIT:
      if (instr==0 || USB_Status==0) 
	{
	  MessagePopup ("错误", "未打开USB设备！");
	  return -1;
	}
      GetCtrlVal(mutiusbHandle,MutiUSB_USB_Cmd8,&ConfigData);
      TempBuffer[0]=ConfigData&(0xFF);
      TempBuffer[1]=ConfigData>>8;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      break;
    }
  return 0;
}

int CVICALLBACK wtUSB9 (int panel, int control, int event,						 //done   
			void *callbackData, int eventData1, int eventData2)
{
  unsigned int n;
  int ConfigData;
  char TempMessage[100];
  unsigned char TempBuffer[2];
  switch (event)
    {
    case EVENT_COMMIT:
      if (instr==0 || USB_Status==0) 
	{
	  MessagePopup ("错误", "未打开USB设备！");
	  return -1;
	}
      GetCtrlVal(mutiusbHandle,MutiUSB_USB_Cmd9,&ConfigData);
      TempBuffer[0]=ConfigData&(0xFF);
      TempBuffer[1]=ConfigData>>8;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[1],TempBuffer[0]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{							   
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      break;
    }
  return 0;
}

int CVICALLBACK wtUSB10 (int panel, int control, int event,					    //done   
			 void *callbackData, int eventData1, int eventData2)
{
  unsigned int n;
  int ConfigData;
  char TempMessage[100];
  unsigned char TempBuffer[2];
  switch (event)
    {
    case EVENT_COMMIT:
      if (instr==0 || USB_Status==0) 
	{
	  MessagePopup ("错误", "未打开USB设备！");
	  return -1;
	}
      GetCtrlVal(mutiusbHandle,MutiUSB_USB_Cmd10,&ConfigData);
      TempBuffer[0]=ConfigData&(0xFF);
      TempBuffer[1]=ConfigData>>8;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[1],TempBuffer[0]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      break;
    }
  return 0;
}

int CVICALLBACK wtUSB7 (int panel, int control, int event,					    //done   
			void *callbackData, int eventData1, int eventData2)
{
  unsigned int n;
  int ConfigData;
  char TempMessage[100];
  unsigned char TempBuffer[2];
  switch (event)
    {
    case EVENT_COMMIT:
      if (instr==0 || USB_Status==0) 
	{
	  MessagePopup ("错误", "未打开USB设备！");
	  return -1;
	}
      GetCtrlVal(mutiusbHandle,MutiUSB_USB_Cmd7,&ConfigData);
      TempBuffer[0]=ConfigData&(0xFF);
      TempBuffer[1]=ConfigData>>8;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[1],TempBuffer[0]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      break;
    }
  return 0;
}

int CVICALLBACK More_USB_Cmd (int panel, int control, int event,		  //done
			      void *callbackData, int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_COMMIT:
      mutiusbHandle=LoadPanel(0,"MutiUSB.uir",MutiUSB);
      DisplayPanel(mutiusbHandle);
      break;
    }
  return 0;
}


int CVICALLBACK SingalSweep (int panel, int control, int event,		   //done
			     void *callbackData, int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_COMMIT:
      signalsweepHandle=LoadPanel(0,"SignalSweep.uir",PANEL_Sig);
      DisplayPanel(signalsweepHandle);
      break;
    }
  return 0;
}
int CVICALLBACK SetTA (int panel, int control, int event,			 //done
		       void *callbackData, int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_COMMIT:
      TAconfigHandle=LoadPanel(0,"TAConfig.uir",TAConfig);
      DisplayPanel(TAconfigHandle);
      break;
    }
  return 0;
}

int CVICALLBACK SetTAConfig (int panel, int control, int event,		  //done
			     void *callbackData, int eventData1, int eventData2)
{
  struct Config
  {
    uint64_t data; //64bit
    int p0;
  };
  struct Config ConfigArray[36];
  int Parameter;
  int ArrayID;
  int p;
  int i;
  int j;
  int n;
  int bit;
  int Address;
  unsigned char CommandBuffer[2];
  char FileName[300];
  char timemsg[20]="0000-00-00-00-00-00";
  char TempMesg[300];
  double CurTime;
  int TempData;
  uint64_t TempData_1;
  int flag=0;
  switch (event)
    {
    case EVENT_COMMIT:


      CommandBuffer[1]  = 0xac;
      GetCtrlVal(TAconfigHandle,TAConfig_TA_ChipMode,&TempData);
      CommandBuffer[0]  = 0xFF&TempData;
      if(SendUsbData(instr, CommandBuffer) ==-1)
	{
	  MessagePopup ("错误", "发送USB配置命令失败！！");
	  return -1;
	}
      else
	{
	  sprintf(TempMesg,"设置刻度模式为0x%02x \n",CommandBuffer[0]);
	  //	printf("Command[1] = %02x, Command[0] = %02x\n",CommandBuffer[1],CommandBuffer[0]);
	  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMesg);
	}
      sleep(300);
      CommandBuffer[1]  = 0x00;
      CommandBuffer[0]  = 0xb1;
      if(SendUsbData(instr, CommandBuffer) ==-1)
	{
	  MessagePopup ("错误", "发送USB配置命令失败！！");
	  return -1;
	}
      else
	{

	  printf("Command[1] = %02x, Command[0] = %02x\n",CommandBuffer[1],CommandBuffer[0]); 
	  SetCtrlVal(mainHandle,Main_TEXTBOX,"完成TA配置\n");
	}

      

			

    }
  return 0;
}

int CVICALLBACK CloseTAConfig (int panel, int event, void *callbackData,	//done
			       int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_GOT_FOCUS:

      break;
    case EVENT_LOST_FOCUS:

      break;
    case EVENT_CLOSE:
      DiscardPanel(TAconfigHandle);
      break;
    }
  return 0;
}

int CVICALLBACK SignalSweepStop (int panel, int control, int event,		   //done
				 void *callbackData, int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_COMMIT:
      SignalSweep_Status=0;
      break;
    }
  return 0;
}

void CRC_Check ( unsigned char data )										//done
{
  int New_CRC_Array[16];
  int i ;
  int j ;
  int Data_Bit[8] ;
  int temp ;
  for ( i = 7 ; i >= 0 ; i-- )
    {
      Data_Bit[i] = data >> i ;
      data = data - ( Data_Bit[i] << i );
    }
  New_CRC_Array[0] = ( ( unsigned char )Data_Bit[4] + ( unsigned char )Data_Bit[0] + CRC_Array[8] + CRC_Array[12] ) % 2 ;
  New_CRC_Array[1] = ( ( unsigned char )Data_Bit[5] + ( unsigned char )Data_Bit[1] + CRC_Array[9] + CRC_Array[13] ) % 2 ;
  New_CRC_Array[2] = ( ( unsigned char )Data_Bit[6] + ( unsigned char )Data_Bit[2] + CRC_Array[10] + CRC_Array[14] ) % 2 ;
  New_CRC_Array[3] = ( ( unsigned char )Data_Bit[7] + ( unsigned char )Data_Bit[3] + CRC_Array[11] + CRC_Array[15] ) % 2 ;
  New_CRC_Array[4] = ( ( unsigned char )Data_Bit[4] + CRC_Array[12] ) % 2 ;
  New_CRC_Array[5] = ( ( unsigned char )Data_Bit[5] + ( unsigned char )Data_Bit[4] + ( unsigned char )Data_Bit[0] + CRC_Array[8] + CRC_Array[12] + CRC_Array[13] ) % 2 ;
  New_CRC_Array[6] = ( ( unsigned char )Data_Bit[6] + ( unsigned char )Data_Bit[5] + ( unsigned char )Data_Bit[1] + CRC_Array[9] + CRC_Array[13] + CRC_Array[14] ) % 2 ;
  New_CRC_Array[7] = ( ( unsigned char )Data_Bit[7] + ( unsigned char )Data_Bit[6] + ( unsigned char )Data_Bit[2] + CRC_Array[10] + CRC_Array[14] + CRC_Array[15] ) % 2 ;
  New_CRC_Array[8] = ( ( unsigned char )Data_Bit[7] + ( unsigned char )Data_Bit[3] + CRC_Array[0] + CRC_Array[11] + CRC_Array[15] ) % 2;
  New_CRC_Array[9] = ( ( unsigned char )Data_Bit[4] + CRC_Array[1] + CRC_Array[12] ) % 2 ;
  New_CRC_Array[10] = ( ( unsigned char )Data_Bit[5] + CRC_Array[2] + CRC_Array[13] ) % 2 ;
  New_CRC_Array[11] = ( ( unsigned char )Data_Bit[6] + CRC_Array[3] + CRC_Array[14] ) % 2 ;
  New_CRC_Array[12] = ( ( unsigned char )Data_Bit[7] + ( unsigned char )Data_Bit[4] + ( unsigned char )Data_Bit[0] + CRC_Array[4] + CRC_Array[8] + CRC_Array[12] + CRC_Array[15] ) % 2 ;
  New_CRC_Array[13] = ( ( unsigned char )Data_Bit[5] + ( unsigned char )Data_Bit[1] + CRC_Array[5] + CRC_Array[9] + CRC_Array[13] ) % 2 ;
  New_CRC_Array[14] = ( ( unsigned char )Data_Bit[6] + ( unsigned char )Data_Bit[2] + CRC_Array[6] + CRC_Array[10] + CRC_Array[14] ) % 2 ;
  New_CRC_Array[15] = ( ( unsigned char )Data_Bit[7] + ( unsigned char )Data_Bit[3] + CRC_Array[7] + CRC_Array[11] + CRC_Array[15] ) % 2 ;
  for ( j = 0 ; j < 16 ; j++ )
    CRC_Array[j] = New_CRC_Array[j] ;
  CRC_Result[0] = 0 ;
  CRC_Result[1] = 0 ;
  for ( i = 0 ; i <= 7 ; i++ )
    {
      temp = CRC_Array[i]<<i ;
      CRC_Result[0] = CRC_Result[0] + temp ;
    }
  for ( i = 8 ; i <= 15 ; i++ )
    {
      temp = CRC_Array[i]<< ( i-8 ) ;
      CRC_Result[1] = CRC_Result[1] + temp ;
    }
  i = 0 ;
}

int CVICALLBACK Set_HighThr (int panel, int control, int event,				//Modified by siyuan 20150310
			     void *callbackData, int eventData1, int eventData2)
{
  int Address ;
  int i ;
  int High_Thread;
  unsigned char High_Thr ;
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(TAconfigHandle, TAConfig_High_Thr, &High_Thread );
      //		printf("%d\n",High_Thread);   
      High_Thr =(0x4200 - ( High_Thread +0x400 )) / 8;
      //		printf("%2x\n",High_Thr);
      for ( i = 0 ; i < 16 ; i++ )
	CRC_Array[i] = 1 ;
      for ( Address = 0 ; Address < 144 ; )
	{
	  for ( i = 0 ; i < 16 ; i++ )
	    {
	      if ( FEE[i] == 0 )
		continue ;
	      SetMaxValue(1, FEEID[i], Address, (int)High_Thr );
	    }
	  Address++;
	  CRC_Check ( High_Thr );
	}
      for ( i = 0 ; i < 16 ; i++ )
	{
	  if ( FEE[i] == 0 )
	    continue ;
	  SetMaxValue(1, FEEID[i], Address, (int)CRC_Result[1] );
	}
      Address++ ;
      for ( i = 0 ; i < 16 ; i++ )
	{
	  if ( FEE[i] == 0 )
	    continue ;
	  SetMaxValue(1, FEEID[i], Address, (int)CRC_Result[0] );
	}	   
      break;
    }
  //Beep();
  return 0;
}

int CVICALLBACK Set_LowThr (int panel, int control, int event,				//done
			    void *callbackData, int eventData1, int eventData2)
{
  int Address ;
  int i,Low_Thread ;
  unsigned char Low_Thr ;
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(TAconfigHandle, TAConfig_Low_Thr, &Low_Thread );
      Low_Thr = (Low_Thread + 0x400)/8  ;
      //		printf("%2x\n",Low_Thr);
      for ( i = 0 ; i < 16 ; i++ )
	CRC_Array[i] = 1 ;
      for ( Address = 0 ; Address < 144 ; )
	{
	  for ( i = 0 ; i < 16 ; i++ )
	    {
	      if ( FEE[i] == 0 )
		continue ;
	      SetMinValue(1, FEEID[i], Address, (int)Low_Thr );
	    }
	  Address++;
	  CRC_Check ( Low_Thr );
	}
      for ( i = 0 ; i < 16 ; i++ )
	{
	  if ( FEE[i] == 0 )
	    continue ;
	  SetMinValue(1, FEEID[i], Address, (int)CRC_Result[1] );
	}
      Address++ ;
      for ( i = 0 ; i < 16 ; i++ )
	{
	  if ( FEE[i] == 0 )
	    continue ;
	  SetMinValue(1, FEEID[i], Address, (int)CRC_Result[0] );
	}
      break;
    }
  return 0;
}

int CVICALLBACK TA_Test_Func (int panel, int control, int event,
			      void *callbackData, int eventData1, int eventData2)
{
  char WrDataFile[300];
  char timemsg[20]="0000-00-00-00-00-00";
  char TempMessage[100];
  double CurTime;
  FILE* ta_fp;
  int Channel ;
  int Hit_Num1 ;
  int Hit_Num2 ;
  void Trigger_Gen(ViSession devise);
  int i ;
  int j ;
  int thr ;
  int Length;
  switch (event)
    {
    case EVENT_COMMIT:
      status = viOpenDefaultRM(&rm);		
      if (status < VI_SUCCESS)
	{
	  MessagePopup ("错误", "连接信号源失败");
	  return -1;
	}
      status = viOpen(rm, IPAddress, VI_NULL, VI_NULL, &SignalSource );
      if (status < VI_SUCCESS)
	{
	  MessagePopup ("错误", "未能打开信号源！");
	  return -1;					
	}
      if (instr==0 || USB_Status==0) 
	{
	  MessagePopup ("错误", "未打开USB设备！");
	  return -1;
	}
      if(datasaveflag==0)
	{
	  MessagePopup ("错误", "请选择文件保存路径！");
	  return -1;
	}
      if (COM_Status==0)
	{
	  MessagePopup ("错误", "未打开COM设备！");
	  return -1;
	}
      //GetCurrentDateTime(&CurTime);
      //FormatDateTimeString(CurTime, "%Y-%m-%d-%H-%M-%S", timemsg, 20);
      sprintf(WrDataFile,"%s\\%sTA Test.csv",WrDataPath,timemsg);
      ta_fp = fopen(WrDataFile, "wb");
      if(ta_fp == NULL)
	{
	  MessagePopup ("error", "fail to create file");
	  return -1;
	}
      sprintf(TempMessage,"Channel,HitCount1,HitCount2\n") ;
      Length = strlen (TempMessage);
      //WriteFile(ta_fp,TempMessage,Length);
      fwrite(TempMessage, Length, 1, ta_fp);
      Cmd2DAQ_ChooseTrigMode(instr, 4);
      Cmd2DAQ_StartAcq(instr);
      Cmd2DAQ_EnableTrig(instr);


      for ( Channel = 0; Channel <=191; Channel++ )
	{
	  if (  ( Channel >= 0 ) && ( Channel <= 63 ) )
	    continue ;
	  for ( j = 0; j < 16 ; j++ )
	    {
	      if(FEE[j] == 0 || ( ( j % 4 ) == 1 ) || ( ( j % 4 ) == 2 ) )
		continue;
	      if((ResetFPGA(1,FEEID[j]))==-1)
		{
		  MessagePopup("错误","发送COM配置命令失败！");
		  return(-1);
		}
	      else
		{
		  sprintf(TempMessage,"FEE寄存器复位\n");
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		}
	      for ( i = 0; i <4 ; i++ )
		TA_Thr(1, FEEID[j], i, 0x900);
				  
	      TA_DAC(1,FEEID[j],0x40) ;
				  	
	      if((TA_Self_Test(1,FEEID[j],Channel))==-1)
		{
		  MessagePopup("错误","发送COM配置命令失败！");
		  return(-1);
		}
	      else
		{
		  sprintf(TempMessage,"Choose FEE 0x%2x Channel %d\n",FEEID[j], Channel);
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		}
	      sleep(10) ;
	      if((CheckFPGALogic(1,FEEID[j]))==-1)
		{
		  MessagePopup("错误","发送COM配置命令失败！");
		  return(-1);
		}
				  
				  
	      if ( ( Channel >=64 )&&( Channel <= 95 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num1 = Response[26] ;
		  else
		    Hit_Num1 = Response[27] ;
		  sprintf(TempMessage,"FEE %2x,Top5,Channel %d,",FEEID[j], Channel-64);
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		  Length = strlen(TempMessage);
		  //WriteFile(ta_fp, TempMessage, Length);
		  fwrite(TempMessage, Length, 1, ta_fp);
		}
				  
				  
	      if ( ( Channel >= 96 )&&( Channel <= 127 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num1 = Response[29] ;
		  else
		    Hit_Num1 = Response[30] ;
		  sprintf(TempMessage,"FEE %2x,Btm5,Channel %d,",FEEID[j], Channel-96);
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		  Length = strlen(TempMessage);
		  //WriteFile(ta_fp, TempMessage, Length);
		  fwrite(TempMessage, Length, 1, ta_fp);
		}
				  
				  
	      if ( ( Channel >= 128 )&&( Channel <= 159 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num1 = Response[32] ;
		  else
		    Hit_Num1 = Response[33] ;
		  sprintf(TempMessage,"FEE %2x,Top8,Channel %d,",FEEID[j], Channel-128);
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		  Length = strlen(TempMessage);
		  //WriteFile(ta_fp, TempMessage, Length);
		  fwrite(TempMessage, Length, 1, ta_fp);
		}
				  
				  
	      if ( ( Channel >= 160 )&&( Channel <= 191 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num1 = Response[35] ;
		  else
		    Hit_Num1 = Response[36] ;
		  sprintf(TempMessage,"FEE %2x,Btm8,Channel %d,",FEEID[j], Channel-160);
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		  Length = strlen(TempMessage);
		  //WriteFile(ta_fp, TempMessage, Length);
		  fwrite(TempMessage, Length, 1, ta_fp);
		}
	      Trigger_Gen(SignalSource) ;
	      sleep(1800);
	      if((CheckFPGALogic(1,FEEID[j]))==-1)
		{
		  MessagePopup("错误","发送COM配置命令失败！");
		  return(-1);
		}
				  
				  
	      if ( ( Channel >=64 )&&( Channel <= 95 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num2 = Response[26] ;
		  else
		    Hit_Num2 = Response[27] ;
		  sprintf(TempMessage,"%d,", Hit_Num2-Hit_Num1);
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		  Length = strlen(TempMessage);
		  //WriteFile(ta_fp, TempMessage, Length);
		  fwrite(TempMessage, Length, 1, ta_fp);
		}
				  
				  
	      if ( ( Channel >= 96 )&&( Channel <= 127 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num2 = Response[29] ;
		  else
		    Hit_Num2 = Response[30] ;
		  sprintf(TempMessage,"%d,",Hit_Num2-Hit_Num1);
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		  Length = strlen(TempMessage);
		  //WriteFile(ta_fp, TempMessage, Length);
		  fwrite(TempMessage, Length, 1, ta_fp);
		}
				  
				  
	      if ( ( Channel >= 128 )&&( Channel <= 159 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num2 = Response[32] ;
		  else
		    Hit_Num2 = Response[33] ;
		  sprintf(TempMessage,"%d,", Hit_Num2-Hit_Num1);
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		  Length = strlen(TempMessage);
		  //WriteFile(ta_fp, TempMessage, Length);
		  fwrite(TempMessage, Length, 1, ta_fp);
		}
				  
				  
	      if ( ( Channel >= 160 )&&( Channel <= 191 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num2 = Response[35] ;
		  else
		    Hit_Num2 = Response[36] ;
		  sprintf(TempMessage,"%d,", Hit_Num2-Hit_Num1);
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		  Length = strlen(TempMessage);
		  //WriteFile(ta_fp, TempMessage, Length);
		  fwrite(TempMessage, Length, 1, ta_fp);
		}
	      /////////////////////////////////////////////////    modified by siyuan 2015 1 13
	      if((CheckFPGALogic(1,FEEID[j]))==-1)
		{
		  MessagePopup("错误","发送COM配置命令失败！");
		  return(-1);
		}
				  
				  
	      if ( ( Channel >=64 )&&( Channel <= 95 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num1 = Response[26] ;
		  else
		    Hit_Num1 = Response[27] ;
		}
				  
				  
	      if ( ( Channel >= 96 )&&( Channel <= 127 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num1 = Response[29] ;
		  else
		    Hit_Num1 = Response[30] ;
		}
				  
				  
	      if ( ( Channel >= 128 )&&( Channel <= 159 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num1 = Response[32] ;
		  else
		    Hit_Num1 = Response[33] ;
		}
				  
				  
	      if ( ( Channel >= 160 )&&( Channel <= 191 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num1 = Response[35] ;
		  else
		    Hit_Num1 = Response[36] ;
		}
	      Trigger_Gen(SignalSource) ;
	      sleep(1800);
	      if((CheckFPGALogic(1,FEEID[j]))==-1)
		{
		  MessagePopup("错误","发送COM配置命令失败！");
		  return(-1);
		}
				  
				  
	      if ( ( Channel >=64 )&&( Channel <= 95 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num2 = Response[26] ;
		  else
		    Hit_Num2 = Response[27] ;
		  sprintf(TempMessage,"%d,", Hit_Num2-Hit_Num1);
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		  Length = strlen(TempMessage);
		  //WriteFile(ta_fp, TempMessage, Length);
		  fwrite(TempMessage, Length, 1, ta_fp);
		}
				  
				  
	      if ( ( Channel >= 96 )&&( Channel <= 127 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num2 = Response[29] ;
		  else
		    Hit_Num2 = Response[30] ;
		  sprintf(TempMessage,"%d,",Hit_Num2-Hit_Num1);
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		  Length = strlen(TempMessage);
		  //WriteFile(ta_fp, TempMessage, Length);
		  fwrite(TempMessage, Length, 1, ta_fp);
		}
				  
				  
	      if ( ( Channel >= 128 )&&( Channel <= 159 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num2 = Response[32] ;
		  else
		    Hit_Num2 = Response[33] ;
		  sprintf(TempMessage,"%d,", Hit_Num2-Hit_Num1);
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		  Length = strlen(TempMessage);
		  //WriteFile(ta_fp, TempMessage, Length);
		  fwrite(TempMessage, Length, 1, ta_fp);
		}
				  
				  
	      if ( ( Channel >= 160 )&&( Channel <= 191 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num2 = Response[35] ;
		  else
		    Hit_Num2 = Response[36] ;
		  sprintf(TempMessage,"%d,", Hit_Num2-Hit_Num1);
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		  Length = strlen(TempMessage);
		  //WriteFile(ta_fp, TempMessage, Length);
		  fwrite(TempMessage, Length, 1, ta_fp);
		}
	      /////////////////////////////////////////////////
				  
	      /*				  if((ResetFPGA(1,FEEID[j]))==-1)
						  {
						  MessagePopup("错误","发送COM配置命令失败！");
						  return(-1);
						  }		
						  sleep(3000);	   */
	      if((TA_Self_Test(1,FEEID[j],Channel))==-1)
		{
		  MessagePopup("错误","发送COM配置命令失败！");
		  return(-1);
		}
	      sleep(10) ;
				  
	      for ( i = 0; i <4 ; i++ )
		TA_Thr(1, FEEID[j], i, 0xb00);
	      TA_DAC(1,FEEID[j],0x40) ;   
				  
	      if((CheckFPGALogic(1,FEEID[j]))==-1)
		{
		  MessagePopup("错误","发送COM配置命令失败！");
		  return(-1);
		}
				  
				  
	      if ( ( Channel >=64 )&&( Channel <= 95 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num1 = Response[26] ;
		  else
		    Hit_Num1 = Response[27] ;
		}
				  
				  
	      if ( ( Channel >= 96 )&&( Channel <= 127 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num1 = Response[29] ;
		  else
		    Hit_Num1 = Response[30] ;
		}
				  
				  
	      if ( ( Channel >= 128 )&&( Channel <= 159 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num1 = Response[32] ;
		  else
		    Hit_Num1 = Response[33] ;
		}
				  
				  
	      if ( ( Channel >= 160 )&&( Channel <= 191 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num1 = Response[35] ;
		  else
		    Hit_Num1 = Response[36] ;
		}
	      Trigger_Gen(SignalSource) ;
	      sleep(1800);
	      if((CheckFPGALogic(1,FEEID[j]))==-1)
		{
		  MessagePopup("错误","发送COM配置命令失败！");
		  return(-1);
		}
		  
	      if ( ( Channel >=64 )&&( Channel <= 95 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num2 = Response[26] ;
		  else
		    Hit_Num2 = Response[27] ;
		  sprintf(TempMessage,"%d,", Hit_Num2-Hit_Num1);
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		  Length = strlen(TempMessage);
		  //WriteFile(ta_fp, TempMessage, Length);
		  fwrite(TempMessage, Length, 1, ta_fp);
		}
				  
				  
	      if ( ( Channel >= 96 )&&( Channel <= 127 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num2 = Response[29] ;
		  else
		    Hit_Num2 = Response[30] ;
		  sprintf(TempMessage,"%d,",Hit_Num2-Hit_Num1);
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		  Length = strlen(TempMessage);
		  //WriteFile(ta_fp, TempMessage, Length);
		  fwrite(TempMessage, Length, 1, ta_fp);
		}
				  
				  
	      if ( ( Channel >= 128 )&&( Channel <= 159 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num2 = Response[32] ;
		  else
		    Hit_Num2 = Response[33] ;
		  sprintf(TempMessage,"%d,", Hit_Num2-Hit_Num1);
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		  Length = strlen(TempMessage);
		  //WriteFile(ta_fp, TempMessage, Length);
		  fwrite(TempMessage, Length, 1, ta_fp);
		}
				  
				  
	      if ( ( Channel >= 160 )&&( Channel <= 191 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num2 = Response[35] ;
		  else
		    Hit_Num2 = Response[36] ;
		  sprintf(TempMessage,"%d,", Hit_Num2-Hit_Num1);
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		  Length = strlen(TempMessage);
		  fwrite(TempMessage, Length, 1, ta_fp);
		  //WriteFile(ta_fp, TempMessage, Length);
		}
	      //////////////////////////////////////////////////   modified by siyuan
	      if((TA_Self_Test(1,FEEID[j],Channel))==-1)
		{
		  MessagePopup("错误","发送COM配置命令失败！");
		  return(-1);
		}
	      sleep(10) ;
				  
	      for ( i = 0; i <4 ; i++ )
		TA_Thr(1, FEEID[j], i, 0xb00);
	      TA_DAC(1,FEEID[j],0x40) ;   
				  
	      if((CheckFPGALogic(1,FEEID[j]))==-1)
		{
		  MessagePopup("错误","发送COM配置命令失败！");
		  return(-1);
		}
				  
				  
	      if ( ( Channel >=64 )&&( Channel <= 95 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num1 = Response[26] ;
		  else
		    Hit_Num1 = Response[27] ;
		}
				  
				  
	      if ( ( Channel >= 96 )&&( Channel <= 127 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num1 = Response[29] ;
		  else
		    Hit_Num1 = Response[30] ;
		}
				  
				  
	      if ( ( Channel >= 128 )&&( Channel <= 159 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num1 = Response[32] ;
		  else
		    Hit_Num1 = Response[33] ;
		}
				  
				  
	      if ( ( Channel >= 160 )&&( Channel <= 191 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num1 = Response[35] ;
		  else
		    Hit_Num1 = Response[36] ;
		}
	      Trigger_Gen(SignalSource) ;
	      sleep(1800);
	      if((CheckFPGALogic(1,FEEID[j]))==-1)
		{
		  MessagePopup("错误","发送COM配置命令失败！");
		  return(-1);
		}
		  
	      if ( ( Channel >=64 )&&( Channel <= 95 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num2 = Response[26] ;
		  else
		    Hit_Num2 = Response[27] ;
		  sprintf(TempMessage,"%d\n", Hit_Num2-Hit_Num1);
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		  Length = strlen(TempMessage);
		  //WriteFile(ta_fp, TempMessage, Length);
		  fwrite(TempMessage, Length, 1, ta_fp);
		}
				  
				  
	      if ( ( Channel >= 96 )&&( Channel <= 127 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num2 = Response[29] ;
		  else
		    Hit_Num2 = Response[30] ;
		  sprintf(TempMessage,"%d\n",Hit_Num2-Hit_Num1);
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		  Length = strlen(TempMessage);
		  fwrite(TempMessage, Length, 1, ta_fp);
		  //WriteFile(ta_fp, TempMessage, Length);
		}
				  
				  
	      if ( ( Channel >= 128 )&&( Channel <= 159 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num2 = Response[32] ;
		  else
		    Hit_Num2 = Response[33] ;
		  sprintf(TempMessage,"%d\n", Hit_Num2-Hit_Num1);
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		  Length = strlen(TempMessage);
		  fwrite(TempMessage, Length, 1, ta_fp);
		  //WriteFile(ta_fp, TempMessage, Length);
		}
				  
				  
	      if ( ( Channel >= 160 )&&( Channel <= 191 ) )
		{
		  if ( Response[0] == 0x55 )
		    Hit_Num2 = Response[35] ;
		  else
		    Hit_Num2 = Response[36] ;
		  sprintf(TempMessage,"%d\n", Hit_Num2-Hit_Num1);
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		  Length = strlen(TempMessage);
		  fwrite(TempMessage, Length, 1, ta_fp);
		  
		}
	      //////////////////////////////////////////////////
	    }
	}
      fclose(ta_fp);
      MessagePopup("","finish TA test");
      break;
    }
  return 0;
}


int CVICALLBACK Hit_Test_Func (int panel, int control, int event,	  //done
			       void *callbackData, int eventData1, int eventData2)
{
  struct Config
  {
    uint64_t data;
    int p0;
  };
  char TempMessage[100];
  int Hit_Num1  ;
  int Hit_Num2 ;
  int Hit_Num3 ;
  int Hit_Num4 ;
  int Length;
  struct Config ConfigArray[36];
  int Parameter;
  int ArrayID;
  int p;
  int i;
  int j;
  int n;
  int bit;
  int Address;
  int TA_NUM;
  char FileName[300];
  char timemsg[20]="0000-00-00-00-00-00";
  double CurTime;
  uint64_t TempData;
  char WrDataFile[300];
  int Chn;
  int ChnNum=0;
  FILE* hit_fp;
  void Trigger_Gen(ViSession devise);
  switch (event)
    {
    case EVENT_COMMIT:
      status = viOpenDefaultRM(&rm);		
      if (status < VI_SUCCESS)
	{
	  MessagePopup ("错误", "连接信号源失败");
	  return -1;
	}
      status = viOpen(rm, IPAddress, VI_NULL, VI_NULL, &SignalSource );
      if (status < VI_SUCCESS)
	{
	  MessagePopup ("错误", "未能打开信号源！");
	  return -1;					
	}
      if (instr==0 || USB_Status==0) 
	{
	  MessagePopup ("错误", "未打开USB设备！");
	  return -1;
	}
      if(datasaveflag==0)
	{
	  MessagePopup ("错误", "请选择文件保存路径！");
	  return -1;
	}
      if (COM_Status==0)
	{
	  MessagePopup ("错误", "未打开COM设备！");
	  return -1;
	}
      //GetCurrentDateTime(&CurTime);
      //FormatDateTimeString(CurTime, "%Y-%m-%d-%H-%M-%S", timemsg, 20);
      sprintf(WrDataFile,"%s\\%sHit Test.csv",WrDataPath,timemsg);
      hit_fp = fopen (WrDataFile, "wb");
      if(hit_fp == NULL)
	{
	  MessagePopup ("error", "fail to create file");
	  return -1;
	}
      sprintf(TempMessage,"Channel,HitCount_Top,HitCount_Bom\n") ;
      Length = strlen (TempMessage);
      //WriteFile(hit_fp,TempMessage,Length);
      fwrite(TempMessage, Length, 1, hit_fp);
      ConfigArray[0].data=0;
      ConfigArray[0].p0=32;
			
      for(i=1;i<=32;i++)
	{
	  ConfigArray[i].data=0;
	  ConfigArray[i].p0=3;
	}
			
			
      ConfigArray[34].data=24; //'2b11000
      ConfigArray[34].p0=5;

      ConfigArray[35].data = 0 ;
      ConfigArray[35].p0 = 3 ;
			
      //			GetCurrentDateTime(&CurTime);
      //			FormatDateTimeString(CurTime, "%Y-%m-%d-%H-%M-%S", timemsg, 20);
      //			sprintf(FileName,"%s\\%s_TAConfig.dat",WrDataPath,timemsg);
      //			fp_TAConfig=OpenFile(FileName,VAL_READ_WRITE,VAL_TRUNCATE,VAL_BINARY);
			
      GetCtrlVal(mainHandle,Main_TA_Num,&TA_NUM);
      for ( Chn = 0; Chn < 32; Chn++)
	{
	  if ( ( Chn % 4 ) == 0 )
	    continue;
	  ChnNum++;
	  TempData = 1<<Chn;
	  ConfigArray[33].data = ~TempData;
	  ConfigArray[33].p0=32;
	  ArrayID=0;
	  p=ConfigArray[ArrayID].p0;
	  TempData=ConfigArray[ArrayID].data;
	  n=0;
	  Parameter=0;
	  Address=0;
	  for ( i = 0 ; i <= 15 ; i++ )
	    CRC_Array[i] = 1 ;
	  for ( j = 0 ; j < 4 ; j++ )
	    {
	      ArrayID = 0 ;
	      p = ConfigArray[ArrayID].p0;
	      TempData = ConfigArray[ArrayID].data;
	      n = 0 ;
	      Parameter = 0 ;
	      while(ArrayID<36)
		{
		  bit=TempData/(int)pow(2,p-1);
		  Parameter=bit+2*Parameter;
		  TempData=TempData-bit*pow(2,p-1);
		  n++;
		  p--;
		  if(p==0&&(ArrayID<36))
		    {
		      ArrayID++;
		      if(ArrayID<36)
			{
			  p=ConfigArray[ArrayID].p0;
			  TempData=ConfigArray[ArrayID].data;
			}
		    }
		  if(n==8)
		    {
		      for(i=0;i<16;i++)
			{
			  if(FEE[i]==0)
			    continue;
			  SetTA_Config(1,FEEID[i],Address, Parameter);
			}
		      CRC_Check ( (unsigned char)Parameter );
		      Address++;
		      n=0;
		    }
		}
	    }
	  for ( i= 0 ; i < 16 ; i++ )
	    {
	      if ( FEE[i] == 0 )
		continue ;
	      SetTA_Config( 1, FEEID[i], Address, (int)CRC_Result[1] ) ;
	    }
	  Address ++ ;
	  for ( i = 0 ; i < 16 ; i++ )
	    {
	      if ( FEE[i] == 0 )
		continue ;
	      SetTA_Config ( 1, FEEID[i], Address, (int)CRC_Result[0] ) ;
	    }
	  for ( i = 0; i < 16; i++ )
	    {
	      if ( FEE[i] == 0 )
		continue ;
	      TA_load(1, FEEID[i] ) ;
	      sleep(1000);
	    }
        
	  if((ResetFPGA(1,FEEID[0]))==-1)
	    {
	      MessagePopup("错误","发送COM配置命令失败！");
	      return(-1);
	    }
	  else
	    {
	      sprintf(TempMessage,"FEE寄存器复位\n");
	      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	    }
				
	  sleep(3000);
		
	  for( i = 0; i < 4; i++)
	    {
	      if (( TA_Thr(1, FEEID[0], i, 0x900)) == -1)
		{
		  MessagePopup("错误","发送COM配置命令失败！");
		  return(-1);
		}
	      else
		{
		  sprintf(TempMessage,"设置阈值0x900\n");
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		}
	    }
	  if((CheckFPGALogic(1,FEEID[0]))==-1)
	    {
	      MessagePopup("错误","发送COM配置命令失败！");
	      return(-1);
	    }
        

	  if ( TA_NUM == 1 )
	    {
	      if ( Response[0] == 0x55 )
		{
		  Hit_Num1 = Response[26] ;
		  Hit_Num3 = Response[29] ;
		}
	      else
		{
		  Hit_Num1 = Response[27] ;
		  Hit_Num3 = Response[30] ;
		}
	      sprintf(TempMessage,"Dy5 Channel %d,", ChnNum);
	      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	      Length = strlen(TempMessage);
	      //WriteFile(hit_fp, TempMessage, Length);
	      fwrite(TempMessage, Length, 1, hit_fp);
	    }
				
				
	  if ( TA_NUM == 2 )
	    {
	      if ( Response[0] == 0x55 )
		{
		  Hit_Num1 = Response[32] ;
		  Hit_Num3 = Response[35] ;
		}
	      else
		{
		  Hit_Num1 = Response[33] ;
		  Hit_Num3 = Response[36] ;
		}
	      sprintf(TempMessage,"Dy8 Channel %d,", ChnNum);
	      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	      Length = strlen(TempMessage);
	      //WriteFile(hit_fp, TempMessage, Length);
	      fwrite(TempMessage, Length, 1, hit_fp);
	    }

	  Trigger_Gen(SignalSource) ;
	  sleep(1800);
	  if((CheckFPGALogic(1,FEEID[0]))==-1)
	    {
	      MessagePopup("错误","发送COM配置命令失败！");
	      return(-1);
	    }
				
				
	  if ( TA_NUM == 1 )
	    {
	      if ( Response[0] == 0x55 )
		{
		  Hit_Num2 = Response[26] ;
		  Hit_Num4 = Response[29] ;
		}
	      else
		{
		  Hit_Num2 = Response[27] ;
		  Hit_Num4 = Response[30] ;
		}
	      sprintf(TempMessage,"%d,%d ,", Hit_Num2-Hit_Num1,Hit_Num4-Hit_Num3);
	      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	      Length = strlen(TempMessage);
	      //WriteFile(hit_fp, TempMessage, Length);
	    }
				
				
	  if ( TA_NUM == 2 )
	    {
	      if ( Response[0] == 0x55 )
		{
		  Hit_Num2 = Response[32] ;
		  Hit_Num4 = Response[35] ;
		}
	      else
		{
		  Hit_Num2 = Response[33] ;
		  Hit_Num4 = Response[36] ;
		}
	      sprintf(TempMessage,"%d,%d  ", Hit_Num2-Hit_Num1,Hit_Num4-Hit_Num3);
	      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	      Length = strlen(TempMessage);
	      //WriteFile(hit_fp, TempMessage, Length);
	    }
				
	  //////////////////////////////////////////////////////
	  if((CheckFPGALogic(1,FEEID[0]))==-1)
	    {
	      MessagePopup("错误","发送COM配置命令失败！");
	      return(-1);
	    }
        

	  if ( TA_NUM == 1 )
	    {
	      if ( Response[0] == 0x55 )
		{
		  Hit_Num1 = Response[26] ;
		  Hit_Num3 = Response[29] ;
		}
	      else
		{
		  Hit_Num1 = Response[27] ;
		  Hit_Num3 = Response[30] ;
		}
	      sprintf(TempMessage,"Dy5 Channel %d,", ChnNum);
	      //		SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	      Length = strlen(TempMessage);
	      //		WriteFile(hit_fp, TempMessage, Length);
	    }
				
				
	  if ( TA_NUM == 2 )
	    {
	      if ( Response[0] == 0x55 )
		{
		  Hit_Num1 = Response[32] ;
		  Hit_Num3 = Response[35] ;
		}
	      else
		{
		  Hit_Num1 = Response[33] ;
		  Hit_Num3 = Response[36] ;
		}
	      sprintf(TempMessage,"Dy8 Channel %d,", ChnNum);
	      //		SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	      Length = strlen(TempMessage);
	      //		WriteFile(hit_fp, TempMessage, Length);
	    }

	  Trigger_Gen(SignalSource) ;
	  sleep(1800);
	  if((CheckFPGALogic(1,FEEID[0]))==-1)
	    {
	      MessagePopup("错误","发送COM配置命令失败！");
	      return(-1);
	    }
				
				
	  if ( TA_NUM == 1 )
	    {
	      if ( Response[0] == 0x55 )
		{
		  Hit_Num2 = Response[26] ;
		  Hit_Num4 = Response[29] ;
		}
	      else
		{
		  Hit_Num2 = Response[27] ;
		  Hit_Num4 = Response[30] ;
		}
	      sprintf(TempMessage,"%d,%d\n", Hit_Num2-Hit_Num1,Hit_Num4-Hit_Num3);
	      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	      Length = strlen(TempMessage);
	      //WriteFile(hit_fp, TempMessage, Length);
	    }
				
				
	  if ( TA_NUM == 2 )
	    {
	      if ( Response[0] == 0x55 )
		{
		  Hit_Num2 = Response[32] ;
		  Hit_Num4 = Response[35] ;
		}
	      else
		{
		  Hit_Num2 = Response[33] ;
		  Hit_Num4 = Response[36] ;
		}
	      sprintf(TempMessage,"%d,%d\n", Hit_Num2-Hit_Num1,Hit_Num4-Hit_Num3);
	      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	      Length = strlen(TempMessage);
	      //WriteFile(hit_fp, TempMessage, Length);
	    }
				
	  /////////////////////////////////////////////////////
	}
      fclose(hit_fp); 
      break;
    }
  return 0;
}

void Trigger_Gen(ViSession devise)									  //done
{
  unsigned int Cnt;
  char TempMessage[100];
  sprintf(TempMessage,"*TRG");   
  status = viWrite(devise, (ViBuf)TempMessage, 5, &Cnt);
  if (status < VI_SUCCESS)
    {	
      MessagePopup ("错误", "未能打开信号源！");
		
      return;
    }
  return;
}

int CVICALLBACK LoadFEEALL (int panel, int control, int event,
			    void *callbackData, int eventData1, int eventData2)
{
  int i;
  switch (event)
    {
    case EVENT_COMMIT:
      // GetCtrlVal(mainHandle,Main_ALLFEE ,&ALLFEE);
      if(ALLFEE == 1)
	{
	  SetCtrlVal(mainHandle,Main_FEE1,1);	 
	  SetCtrlVal(mainHandle,Main_FEE2,1);
	  SetCtrlVal(mainHandle,Main_FEE3,1);
	  SetCtrlVal(mainHandle,Main_FEE4,1);
	  SetCtrlVal(mainHandle,Main_FEE5,1);
	  SetCtrlVal(mainHandle,Main_FEE6,1);
	  SetCtrlVal(mainHandle,Main_FEE7,1);
	  SetCtrlVal(mainHandle,Main_FEE8,1);
	  SetCtrlVal(mainHandle,Main_FEE9,1);
	  SetCtrlVal(mainHandle,Main_FEE10,1);
	  SetCtrlVal(mainHandle,Main_FEE11,1);
	  SetCtrlVal(mainHandle,Main_FEE12,1);
	  SetCtrlVal(mainHandle,Main_FEE13,1);
	  SetCtrlVal(mainHandle,Main_FEE14,1);
	  SetCtrlVal(mainHandle,Main_FEE15,1);
	  SetCtrlVal(mainHandle,Main_FEE16,1);
	  for(i=0 ; i<16 ; i++)
	    FEE[i] = 1;
	}
      else
	{
	  SetCtrlVal(mainHandle,Main_FEE1,0);	 
	  SetCtrlVal(mainHandle,Main_FEE2,0);
	  SetCtrlVal(mainHandle,Main_FEE3,0);
	  SetCtrlVal(mainHandle,Main_FEE4,0);
	  SetCtrlVal(mainHandle,Main_FEE5,0);
	  SetCtrlVal(mainHandle,Main_FEE6,0);
	  SetCtrlVal(mainHandle,Main_FEE7,0);
	  SetCtrlVal(mainHandle,Main_FEE8,0);
	  SetCtrlVal(mainHandle,Main_FEE9,0);
	  SetCtrlVal(mainHandle,Main_FEE10,0);
	  SetCtrlVal(mainHandle,Main_FEE11,0);
	  SetCtrlVal(mainHandle,Main_FEE12,0);
	  SetCtrlVal(mainHandle,Main_FEE13,0);
	  SetCtrlVal(mainHandle,Main_FEE14,0);
	  SetCtrlVal(mainHandle,Main_FEE15,0);
	  SetCtrlVal(mainHandle,Main_FEE16,0);
	  for(i=0 ; i<16 ; i++)
	    FEE[i] = 0;
	}
      break;
    }
  return 0;
}

int CVICALLBACK Chn_Mode(int panel, int control, int event,			//done
			 void *callbackData, int eventData1, int eventData2)
{
  int caliHandle;
  int Chn_Mode;
  char TempMessage[100];
  switch (event)
    {
    case EVENT_COMMIT:
      GetPanelHandleFromTabPage(mainHandle,Main_WorkMode,1,&caliHandle);
      GetCtrlVal(caliHandle,Calitest_Chn_Mode,&Chn_Mode);
      /*sprintf(TempMessage,"Chn_Mode = %d\n",Chn_Mode);
	SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
      */
      if(Chn_Mode==1)
	{
	  /* SetCtrlAttribute(caliHandle,Calitest_ChnID_End ,ATTR_DIMMED,1); */
	  /* SetCtrlAttribute(caliHandle,Calitest_Calitest_ChnID,ATTR_DIMMED,1); */
	}
      else if(Chn_Mode==2)
	{
	  /* SetCtrlAttribute(caliHandle,Calitest_ChnID_End ,ATTR_DIMMED,0); */
	  /* SetCtrlAttribute(caliHandle,Calitest_Calitest_ChnID,ATTR_DIMMED,0); */
	}
      else
	{
	  /* SetCtrlAttribute(caliHandle,Calitest_ChnID_End ,ATTR_DIMMED,1); */
	  /* SetCtrlAttribute(caliHandle,Calitest_Calitest_ChnID,ATTR_DIMMED,0); */
	}
      break;
    }
  return 0;
}

int CVICALLBACK MoreHitMode (int panel, int control, int event,
			     void *callbackData, int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_COMMIT:
      mutihitHandle = LoadPanel(0,"MutiHit.uir",MutiHit);
      DisplayPanel(mutihitHandle);
      break;
    }
  return 0;
}

int CVICALLBACK LoadHit (int panel, int control, int event,
			 void *callbackData, int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(mutihitHandle,MutiHit_PX1_T5,&HitAndArray[0]);
      GetCtrlVal(mutihitHandle,MutiHit_PX1_B5,&HitAndArray[1]);
      GetCtrlVal(mutihitHandle,MutiHit_PX1_T8,&HitAndArray[2]);
      GetCtrlVal(mutihitHandle,MutiHit_PX1_B8,&HitAndArray[3]);
      GetCtrlVal(mutihitHandle,MutiHit_PX4_T5,&HitAndArray[4]);
      GetCtrlVal(mutihitHandle,MutiHit_PX4_B5,&HitAndArray[5]);
      GetCtrlVal(mutihitHandle,MutiHit_PX4_T8,&HitAndArray[6]);
      GetCtrlVal(mutihitHandle,MutiHit_PX4_B8,&HitAndArray[7]);
      GetCtrlVal(mutihitHandle,MutiHit_NX1_T5,&HitAndArray[8]);
      GetCtrlVal(mutihitHandle,MutiHit_NX1_B5,&HitAndArray[9]);
      GetCtrlVal(mutihitHandle,MutiHit_NX1_T8,&HitAndArray[10]);
      GetCtrlVal(mutihitHandle,MutiHit_NX1_B8,&HitAndArray[11]);
      GetCtrlVal(mutihitHandle,MutiHit_NX4_T5,&HitAndArray[12]);
      GetCtrlVal(mutihitHandle,MutiHit_NX4_B5,&HitAndArray[13]);
      GetCtrlVal(mutihitHandle,MutiHit_NX4_T8,&HitAndArray[14]);
      GetCtrlVal(mutihitHandle,MutiHit_NX4_B8,&HitAndArray[15]);
			
      GetCtrlVal(mutihitHandle,MutiHit_PY1_T5,&HitAndArray[16]);
      GetCtrlVal(mutihitHandle,MutiHit_PY1_B5,&HitAndArray[17]);
      GetCtrlVal(mutihitHandle,MutiHit_PY1_T8,&HitAndArray[18]);
      GetCtrlVal(mutihitHandle,MutiHit_PY1_B8,&HitAndArray[19]);
      GetCtrlVal(mutihitHandle,MutiHit_PY4_T5,&HitAndArray[20]);
      GetCtrlVal(mutihitHandle,MutiHit_PY4_B5,&HitAndArray[21]);
      GetCtrlVal(mutihitHandle,MutiHit_PY4_T8,&HitAndArray[22]);
      GetCtrlVal(mutihitHandle,MutiHit_PY4_B8,&HitAndArray[23]);
      GetCtrlVal(mutihitHandle,MutiHit_NY1_T5,&HitAndArray[24]);
      GetCtrlVal(mutihitHandle,MutiHit_NY1_B5,&HitAndArray[25]);
      GetCtrlVal(mutihitHandle,MutiHit_NY1_T8,&HitAndArray[26]);
      GetCtrlVal(mutihitHandle,MutiHit_NY1_B8,&HitAndArray[27]);
      GetCtrlVal(mutihitHandle,MutiHit_NY4_T5,&HitAndArray[28]);
      GetCtrlVal(mutihitHandle,MutiHit_NY4_B5,&HitAndArray[29]);
      GetCtrlVal(mutihitHandle,MutiHit_NY4_T8,&HitAndArray[30]);
      GetCtrlVal(mutihitHandle,MutiHit_NY4_B8,&HitAndArray[31]);
      HitChnParaM[0] = 0x10 + (HitAndArray[0]<<3) +  (HitAndArray[1]<<2) + (HitAndArray[2]<<1) + HitAndArray[3];
      HitChnParaM[1] = 0x20 + (HitAndArray[4]<<3) +  (HitAndArray[5]<<2) + (HitAndArray[6]<<1) + HitAndArray[7];
      HitChnParaM[2] = 0x30 + (HitAndArray[8]<<3) +  (HitAndArray[9]<<2) + (HitAndArray[10]<<1) + HitAndArray[11];
      HitChnParaM[3] = 0x40 + (HitAndArray[12]<<3) +  (HitAndArray[13]<<2) + (HitAndArray[14]<<1) + HitAndArray[15];
      HitChnParaM[4] = 0x50 + (HitAndArray[16]<<3) +  (HitAndArray[17]<<2) + (HitAndArray[18]<<1) + HitAndArray[19];
      HitChnParaM[5] = 0x60 + (HitAndArray[20]<<3) +  (HitAndArray[21]<<2) + (HitAndArray[22]<<1) + HitAndArray[23];
      HitChnParaM[6] = 0x70 + (HitAndArray[24]<<3) +  (HitAndArray[25]<<2) + (HitAndArray[26]<<1) + HitAndArray[27];
      HitChnParaM[7] = 0x80 + (HitAndArray[28]<<3) +  (HitAndArray[29]<<2) + (HitAndArray[30]<<1) + HitAndArray[31];
			
      break;
    }
  return 0;
}

int CVICALLBACK Quit_MutiHit (int panel, int control, int event,
			      void *callbackData, int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_COMMIT:
      DiscardPanel(mutihitHandle);
      break;
    }
  return 0;
}

int CVICALLBACK SetTA_THR (int panel, int control, int event,
			   void *callbackData, int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_COMMIT:
      mutiTHRHandle = LoadPanel(0,"MutiTHR.uir",MutiTHR);
      DisplayPanel(mutiTHRHandle);
      break;
    }
  return 0;
}

int CVICALLBACK Set_T5 (int panel, int control, int event,
			void *callbackData, int eventData1, int eventData2)
{
  double T5;
  int i;
  int  *DACcode_P;
  int  DACcode=0;
  int  TempData;
  DACcode_P = &DACcode;
  char TempMesg[300];
  unsigned char CommandBuffer[2];
  unsigned char type = 0x20;
  char tempmessage[100];
  unsigned char DACvalue[2];
  unsigned char Test;
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(TAconfigHandle,TAConfig_TA_ChipMode,&TempData);
      GetCtrlVal(mutiTHRHandle,MutiTHR_T5,&T5);
      printf("mitiTHRHandle = %d TAconfigHandle = %d  TempData = %d\n", mutiTHRHandle,TAconfigHandle,TempData);
      //	 printf("T5 = %3f, DACcode = %03x \n",T5,(DACcode));    
      if(TempData == 3)
	Translation (T5, DACcode_P,  Relationship_TA1_11);
      else if(TempData == 2)
	Translation (T5, DACcode_P,  Relationship_TA1_10); 
      else
	break;
      //  printf("T5 = %3f, *DACcode = %03x \n",T5,(DACcode)); 
      CommandBuffer[1]  = 0x20  + (((0xF00)&(DACcode))>>8);
      CommandBuffer[0]  = (0xFF)&(DACcode);
      Test =  (unsigned char)(DACcode); 
      printf("Command[1] = %02x, Command[0] = %02x Test = %02x DACcode = %03x \n",CommandBuffer[1],CommandBuffer[0],Test,(DACcode));

      if(SendUsbData(instr,CommandBuffer) ==-1)
	{
	  MessagePopup ("错误", "发送USB配置命令失败！！");
	  return -1;
	}
      else
	{
	  sprintf(TempMesg,"设置TA1的阈值为%3d fC \n",(int)T5);
          
	  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMesg);
	}
      break;
    }
  return 0;
}

int CVICALLBACK Set_B5 (int panel, int control, int event,
			void *callbackData, int eventData1, int eventData2)
{
  double B5;
  int i;
  int  *DACcode_P;
  int  DACcode=0;
  int  TempData;
  DACcode_P = &DACcode;
  char TempMesg[300];
  unsigned char CommandBuffer[2];
  unsigned char type = 0x10;
  char* tempmessage[100];
  unsigned char DACvalue[2];
  unsigned char Test;
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(TAconfigHandle,TAConfig_TA_ChipMode,&TempData);
      GetCtrlVal(mutiTHRHandle,MutiTHR_B5,&B5);
      printf("mitiTHRHandle = %d TAconfigHandle = %d  TempData = %d\n", mutiTHRHandle,TAconfigHandle,TempData);
      //	 printf("T5 = %3f, DACcode = %03x \n",T5,(DACcode));    
      if(TempData == 3)
	Translation (B5, DACcode_P,  Relationship_TA2_11);
      else if(TempData == 2)
	Translation (B5, DACcode_P,  Relationship_TA2_10); 
      else
	break;
      //  printf("T5 = %3f, *DACcode = %03x \n",T5,(DACcode)); 
      CommandBuffer[1]  = 0x10  + (((0xF00)&(DACcode))>>8);
      CommandBuffer[0]  = (0xFF)&(DACcode);
      Test =  (unsigned char)(DACcode); 
      printf("Command[1] = %02x, Command[0] = %02x Test = %02x DACcode = %03x \n",CommandBuffer[1],CommandBuffer[0],Test,(DACcode));

      if(SendUsbData(instr,CommandBuffer) ==-1)
	{
	  MessagePopup ("错误", "发送USB配置命令失败！！");
	  return -1;
	}
      else
	{
	  sprintf(TempMesg,"设置TA2的阈值为%3d fC \n",(int)B5);
          
	  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMesg);
	}
      break;
    }
  return 0;
}

int CVICALLBACK Set_T8 (int panel, int control, int event,
			void *callbackData, int eventData1, int eventData2)
{
  double T8;
  int i;
  int  *DACcode_P;
  int  DACcode=0;
  int  TempData;
  DACcode_P = &DACcode;
  char TempMesg[300];
  unsigned char CommandBuffer[2];
  unsigned char type = 0x20;
  char* tempmessage[100];
  unsigned char DACvalue[2];
  unsigned char Test,Test1;
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(TAconfigHandle,TAConfig_TA_ChipMode,&TempData);
      GetCtrlVal(mutiTHRHandle,MutiTHR_T8,&T8);
      printf("mitiTHRHandle = %d TAconfigHandle = %d  TempData = %d\n", mutiTHRHandle,TAconfigHandle,TempData);
      //	 printf("T5 = %3f, DACcode = %03x \n",T5,(DACcode));    
      if(TempData == 3)
	Translation (T8, DACcode_P,  Relationship_TA3_11);
      else if(TempData == 2)
	Translation (T8, DACcode_P,  Relationship_TA3_10); 
      else
	break;
      //  printf("T5 = %3f, *DACcode = %03x \n",T5,(DACcode)); 
      CommandBuffer[1]  = 0x40  + (((0xF00)&(DACcode))>>8);
      CommandBuffer[0]  = (0xFF)&(DACcode);
      Test =  (unsigned char)(DACcode);
	  
      printf("Command[1] = %02x, Command[0] = %02x Test = %02x DACcode = %03x \n",CommandBuffer[1],CommandBuffer[0],Test,(DACcode));

      if(SendUsbData(instr,CommandBuffer) ==-1)
	{
	  MessagePopup ("错误", "发送USB配置命令失败！！");
	  return -1;
	}
      else
	{
	  sprintf(TempMesg,"设置TA3的阈值为%3d fC \n",(int)T8);
          
	  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMesg);
	}
      break;
    }
  return 0;
}

int CVICALLBACK Set_B8 (int panel, int control, int event,
			void *callbackData, int eventData1, int eventData2)
{
  double B8;
  int DACcode,i;
  unsigned char type = 0x26;
  unsigned char DACvalue[2];
  char tempmessage[100];
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(mutiTHRHandle,MutiTHR_B8,&B8);
      DACcode = (int)((B8 + 2474)/1.2) ;
      DACvalue[0] = 0xC0+DACcode/256;
      DACvalue[1] =DACcode;   
      //	DACvalue[0] =0x40;
      //    DACvalue[1] = 0x00;
      //	sprintf(DACvalue,"%d",DACcode);
      for(i = 0;i<16;i++)
	{
	  if(FEE[i] == 0)
	    continue;
	  else
	    {
	      SetTHR(1,FEEID[i],DACvalue,type);
	      sprintf(tempmessage,"将%s的B8阈值设为:%.0fmV\n",FEENAME[i],B8);
	      SetCtrlVal(mainHandle,Main_TEXTBOX,tempmessage);
	    }
	}
      break;
    }
  return 0;
}

int CVICALLBACK Quit_THR (int panel, int control, int event,
			  void *callbackData, int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_COMMIT:
      DiscardPanel(mutiTHRHandle);
      break;
    }
  return 0;
}

int CVICALLBACK Help (int panel, int control, int event,
		      void *callbackData, int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_COMMIT:
      HelpHandle = LoadPanel(0,"Help.uir",Help_Panel);
      DisplayPanel(HelpHandle);
      break;
    }
  return 0;
}

int CVICALLBACK Quit_Help (int panel, int control, int event,
			   void *callbackData, int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_COMMIT:
      DiscardPanel(HelpHandle);
      break;
    }
  return 0;
}

int CVICALLBACK QuitCount (int panel, int control, int event,
			   void *callbackData, int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_COMMIT:
      DiscardPanel(countHandle);
      break;
    }
  return 0;
}

int CVICALLBACK Test_Text (int panel, int control, int event,
			   void *callbackData, int eventData1, int eventData2)
{
  char test = 0x33,test2 = 0xf;
  char message[100];
  int test1 = test*10;
  test = test2&test;
  switch (event)
    {
    case EVENT_COMMIT:
      break;
      sprintf(message,"%02x %d \n",test,test1);
      SetCtrlVal(mainHandle,Main_TEXTBOX,message);
      break;
    }
  return 0;
}

int CVICALLBACK LoadHitStatus (int panel, int control, int event,
			       void *callbackData, int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(countHandle,CountHit_Hit_1,&Hit[0]);
      GetCtrlVal(countHandle,CountHit_Hit_2,&Hit[1]);
      GetCtrlVal(countHandle,CountHit_Hit_3,&Hit[2]);
      GetCtrlVal(countHandle,CountHit_Hit_4,&Hit[3]);
      GetCtrlVal(countHandle,CountHit_Hit_5,&Hit[4]);
      GetCtrlVal(countHandle,CountHit_Hit_6,&Hit[5]);
      GetCtrlVal(countHandle,CountHit_Hit_7,&Hit[6]);
      GetCtrlVal(countHandle,CountHit_Hit_8,&Hit[7]);
      GetCtrlVal(countHandle,CountHit_Hit_9,&Hit[8]);
      GetCtrlVal(countHandle,CountHit_Hit_10,&Hit[9]);
      GetCtrlVal(countHandle,CountHit_Hit_11,&Hit[10]);
      GetCtrlVal(countHandle,CountHit_Hit_12,&Hit[11]);
      GetCtrlVal(countHandle,CountHit_Hit_13,&Hit[12]);
      GetCtrlVal(countHandle,CountHit_Hit_14,&Hit[13]);
      GetCtrlVal(countHandle,CountHit_Hit_15,&Hit[14]);
      GetCtrlVal(countHandle,CountHit_Hit_16,&Hit[15]);
      GetCtrlVal(countHandle,CountHit_Hit_17,&Hit[16]);
      GetCtrlVal(countHandle,CountHit_Hit_18,&Hit[17]);
      GetCtrlVal(countHandle,CountHit_Hit_19,&Hit[18]);
      GetCtrlVal(countHandle,CountHit_Hit_20,&Hit[19]);
      GetCtrlVal(countHandle,CountHit_Hit_21,&Hit[20]);
      GetCtrlVal(countHandle,CountHit_Hit_22,&Hit[21]);
      GetCtrlVal(countHandle,CountHit_Hit_23,&Hit[22]);
      GetCtrlVal(countHandle,CountHit_Hit_24,&Hit[23]);
      GetCtrlVal(countHandle,CountHit_Hit_25,&Hit[24]);
      GetCtrlVal(countHandle,CountHit_Hit_26,&Hit[25]);
      GetCtrlVal(countHandle,CountHit_Hit_27,&Hit[26]);
      GetCtrlVal(countHandle,CountHit_Hit_28,&Hit[27]);
      GetCtrlVal(countHandle,CountHit_Hit_29,&Hit[28]);
      GetCtrlVal(countHandle,CountHit_Hit_30,&Hit[29]);
      GetCtrlVal(countHandle,CountHit_Hit_31,&Hit[30]);
      GetCtrlVal(countHandle,CountHit_Hit_32,&Hit[31]);
      break;
    }
  return 0;
}

int CVICALLBACK Write_NET (int panel, int control, int event,
			   void *callbackData, int eventData1, int eventData2)
{
  unsigned char data[2];
  data[0] = 0x0A;
  data[1] = 0x0D;
  char destAddr[21]="192.168.0.101";
  switch (event)
    {
    case EVENT_COMMIT:
      CreateUDPChannel(UDP_ANY_LOCAL_PORT, &writerChannel);
      
      //SendNetData(writerChannel,destAddr,data); // comment out: Yi

      //	CreateUDPChannelConfig(4000, UDP_ANY_ADDRESS, 0, UDPCallback, NULL, &readerChannel);
      break;
    }
  return 0;
}

int CVICALLBACK CMD_Sel_A_B (int panel, int control, int event,
			     void *callbackData, int eventData1, int eventData2)
{
  int CMD_Sel;
  char TempMessage[200];
  switch (event)
    {
    case EVENT_COMMIT:
      GetPanelHandleFromTabPage (mainHandle,Main_Cmd, 0, &comcmdHandle);
      GetCtrlVal(comcmdHandle,COM_Cmd_CMD_Sel,&CMD_Sel);
      if(Cmd2DAQ_CMD_Sel(instr,CMD_Sel)==-1)
	{
	  MessagePopup("错误","设置CMD命令通道主备失败！");
	  return(-1);
	}
      else
	{
	  if(CMD_Sel== 0)
	    {
	      sprintf(TempMessage,"设置CMD命令通过主通道发送...\n");
	      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	    }
	  else
	    {
	      sprintf(TempMessage,"设置CMD命令通过备通道发送...\n");
	      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	    }
						
	}
			
      break;
    }
  return 0;
}


int CVICALLBACK Remind_Timer (int panel, int control, int event,
			      void *callbackData, int eventData1, int eventData2)
{
  char Music_Path[300]="./";
  switch (event)
    {
    case EVENT_TIMER_TICK:
      /* GetDir(Music_Path); */
      sprintf(Music_Path,"%s\\Activ-Doar Cu Tine.wav",Music_Path);
      //PlaySound(TEXT(Music_Path), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); 
      MessagePopup("提醒","2小时已到,请记录电流等关键数据!");	
      //Beep();
      //PlaySound(NULL, NULL, SND_FILENAME);
      break;
    }
  return 0;
}

int CVICALLBACK Close_Duty_Mode (int panel, int event, void *callbackData,
				 int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_GOT_FOCUS:

      break;
    case EVENT_LOST_FOCUS:

      break;
    case EVENT_CLOSE:
      DiscardPanel(dutyHandle);
      break;
    }
  return 0;
}

int CVICALLBACK PX_Close (int panel, int event, void *callbackData,
			  int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_GOT_FOCUS:

      break;
    case EVENT_LOST_FOCUS:

      break;
    case EVENT_CLOSE:
      DiscardPanel (PX_Handle);
      break;
    }
  return 0;
}

int Translation (double Charge, int *DAC, double Relationship[12][2])
{
  int i;
  for (i = 0; i<12 ; i++)
    {
      if((Relationship[i][0] <= Charge)&&(Relationship[i+1][0]>Charge))
        {
          *DAC = (Relationship[i+1][1]-Relationship[i][1])/(Relationship[i+1][0]-Relationship[i][0])*((Charge)-Relationship[i][0]) + Relationship[i][1];
	  printf("Relationship[2][1]= %03x \n",(int)Relationship[2][1]);
	  return (0);
        }
      else
	continue;
    }
  return 0;
}

int CVICALLBACK CloseMutiTHR (int panel, int event, void *callbackData,
			      int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_GOT_FOCUS:

      break;
    case EVENT_LOST_FOCUS:

      break;
    case EVENT_CLOSE:
      DiscardPanel (mutiTHRHandle);
      break;
    }
  return 0;
}

int CVICALLBACK Config_SC_para (int panel, int control, int event,
				void *callbackData, int eventData1, int eventData2)
{
  unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData;
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:
			
      TempBuffer[1]=0xb1;
      TempBuffer[0]=0x00;
      //	TempBuffer[1]=0xe3;
      //	TempBuffer[0]=0xff;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[1],TempBuffer[0]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      break;
    }
  return 0;
}

int CVICALLBACK Switch_SC_Prob (int panel, int control, int event,
				void *callbackData, int eventData1, int eventData2)
{
  unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData = 0;
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(mainHandle,Main_Switch_SC_Prob,&ConfigData);
      if(ConfigData == 1)
	{
	  TempBuffer[1]=0x01;
	  TempBuffer[0]=0xbb;
	}
      else
	{
	  TempBuffer[1]=0x00;
	  TempBuffer[0]=0xbb;
	}
			

      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      break;
    }
  return 0;
}

int CVICALLBACK Set_DAC (int panel, int control, int event,
			 void *callbackData, int eventData1, int eventData2)
{	   	unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData;
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(mainHandle,Main_DAC_Value,&ConfigData); 
      TempBuffer[1]=0xFF&ConfigData;
      TempBuffer[0]= 0x50  + (((0xF00)&(ConfigData))>>8); 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
		
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      sleep(100); 
			
      TempBuffer[1]=0xe3;
      TempBuffer[0]=0xff;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      sleep(100); 
      status=viWrite(instr,TempBuffer,2,&n);	  
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      break;
    }
  return 0;
}

int CVICALLBACK Set_Thre_DAC (int panel, int control, int event,
			      void *callbackData, int eventData1, int eventData2)
{			  	unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData=0;
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(mainHandle,Main_DAC_Value_Threshold,&ConfigData); 
      TempBuffer[1]=0xFF&ConfigData;
      TempBuffer[0]= 0x40  + (((0xF00)&(ConfigData))>>8); 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      sleep(100);
			  
			  
			  
      TempBuffer[1]=0xFF&ConfigData;
      TempBuffer[0]= 0x60  + (((0xF00)&(ConfigData))>>8); 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      sleep(100);
			  
			  
      TempBuffer[1]=0x01;
      TempBuffer[0]=0xbb; 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
			
      sleep(100);  
			
      TempBuffer[1]=0xb1;
      TempBuffer[0]=0x00; 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
			
      sleep(100);
			 
      GetCtrlVal(mainHandle,Main_Switch_SC_Prob,&ConfigData);
      if(ConfigData == 1)
	{
	  TempBuffer[1]=0x01;
	  TempBuffer[0]=0xbb;
	}
      else
	{
	  TempBuffer[1]=0x00;
	  TempBuffer[0]=0xbb;
	}
			

      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
			 
			  
      break;
    }
  return 0;
}


int CVICALLBACK Start_Acq_Test (int panel, int control, int event,
				void *callbackData, int eventData1, int eventData2)
{	 	unsigned char TempBuffer[2];
  unsigned char TempBuffer1[512];
  char TempMessage[100];
  double CurTime; 
  char timemsg[20]="0000-00-00-00-00-00";
  char WrDataFile[300];
  int ConfigData = 0;
	
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(mainHandle,Main_Start_Acq_Test,&ConfigData);
      if(ConfigData == 1)
	{
				
				
				
	  if(datasaveflag==0)
	    {
	      MessagePopup ("错误", "请选择文件保存路径！");
	      return -1;
	    }

	  //GetCurrentDateTime(&CurTime);
	  //FormatDateTimeString(CurTime, "%Y-%m-%d-%H-%M-%S", timemsg, 20);
	  sprintf(WrDataFile,"%s\\%s.dat",WrDataPath,timemsg);
	  fp_write=fopen (WrDataFile, "wb");
	  if(fp_write==NULL)
	    {
	      MessagePopup ("错误", "新建数据保存文件失败！");
	      return -1;
	    }
				
	  sleep(100);
	  TempBuffer[1]=0xe3;
	  TempBuffer[0]=0xff;
	  sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
	  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
	  status=viWrite(instr,TempBuffer,2,&n);
	  if(status==VI_SUCCESS)
	    InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
	  else
	    {
	      MessagePopup("错误","发送COM配置命令失败！");
	      return(-1);
	    }
	  sleep(100);
	  TempBuffer[1]=0xf1;
	  TempBuffer[0]=0xf0;
	  sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
	  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
	  status=viWrite(instr,TempBuffer,2,&n);
	  if(status==VI_SUCCESS)
	    InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
	  else
	    {
	      MessagePopup("错误","发送COM配置命令失败！");
	      return(-1);
	    }
	  status=viRead(instr,TempBuffer1,512,&n);
	  sleep(15);
	  //	status=viRead(instr,TempBuffer,512,&n);
	  //	sleep(15);
			
		
	  GetData_Status=1; 
				
	  SetCtrlVal(mainHandle,Main_TEXTBOX,"开始数据采集...\n");
	  SetCtrlVal(mainHandle,Main_Acq_Status,1);
	  //std::thread
	  //CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, Acqing, &mainHandle, &threadID4);
	  SetCtrlVal(mainHandle,Main_TEXTBOX,"开启监测...\n");
	  //std::thread
	  //CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, DataAcq, &mainHandle, &threadID1);
	  SetCtrlVal(mainHandle,Main_TEXTBOX,"开启数据采集线程...\n");		
				
				
	}
      else
	{
	  sleep(100);
	  TempBuffer[1]=0xf0;
	  TempBuffer[0]=0xf0;
	  sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
	  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
	  status=viWrite(instr,TempBuffer,2,&n);
	  if(status==VI_SUCCESS)
	    InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
	  else
	    {
	      MessagePopup("错误","发送COM配置命令失败！");
	      return(-1);
	    }
				
	  GetData_Status=0;
	  sleep(100); 
	  SetCtrlVal(mainHandle,Main_Acq_Status,0);
	  SetCtrlVal(mainHandle,Main_TEXTBOX,"结束采集命令发出...\n");
	
	  fclose(fp_write);
	}
			
			
		
      break;
    }
  return 0;
}

int CVICALLBACK Set_Thre_Fsb (int panel, int control, int event,
			      void *callbackData, int eventData1, int eventData2)
{
  unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData = 0;
  int Add_Data = 0;
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:
			
      TempBuffer[1]=0x00;
      TempBuffer[0]=0x30;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      sleep(100); 
			
			
      GetCtrlVal(mainHandle,Main_Manual_USB_Add,&Add_Data);
      GetCtrlVal(mainHandle,Main_Thre_Fsb_Cmd,&ConfigData);      
      TempBuffer[1]=ConfigData+Add_Data;
      TempBuffer[0]=0x31;
      SetCtrlVal(mainHandle,Main_Thre_Fsb_Cmd,TempBuffer[1]);
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      sleep(100); 
					
      TempBuffer[1]=0x00;
      TempBuffer[0]=0xbb; 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
					
				
			
      sleep(100);  
			
      TempBuffer[1]=0xb1;
      TempBuffer[0]=0x00; 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
			
			
			
      sleep(100);
			 
      GetCtrlVal(mainHandle,Main_Switch_SC_Prob,&ConfigData);
      if(ConfigData == 1)
	{
	  TempBuffer[1]=0x01;
	  TempBuffer[0]=0xbb;
	}
      else
	{
	  TempBuffer[1]=0x00;
	  TempBuffer[0]=0xbb;
	}
			

      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
			 
			  
			
      break;
    }
  return 0;
}

int CVICALLBACK Set_SS1 (int panel, int control, int event,
			 void *callbackData, int eventData1, int eventData2)
{
  unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData = 0;
  int Add_Data = 0; 
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:
			
			
      TempBuffer[1]=0x00;
      TempBuffer[0]=0x31;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      sleep(100); 
			
      GetCtrlVal(mainHandle,Main_Manual_USB_Add,&Add_Data);    
      GetCtrlVal(mainHandle,Main_SS1_SS10_PA,&ConfigData);      
      TempBuffer[1]=ConfigData+Add_Data;
      TempBuffer[0]=0x30;
      SetCtrlVal(mainHandle,Main_SS1_SS10_PA,TempBuffer[1]); 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      sleep(100); 
					
      TempBuffer[1]=0x00;
      TempBuffer[0]=0xbb; 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}	
				
			
      sleep(100);  
			
      TempBuffer[1]=0xb1;
      TempBuffer[0]=0x00; 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
			
			
      sleep(100);
			 
      GetCtrlVal(mainHandle,Main_Switch_SC_Prob,&ConfigData);
      if(ConfigData == 1)
	{
	  TempBuffer[1]=0x01;
	  TempBuffer[0]=0xbb;
	}
      else
	{
	  TempBuffer[1]=0x00;
	  TempBuffer[0]=0xbb;
	}
			

      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
			 
			  
			
      break;
    }
  return 0;
}

int CVICALLBACK Thre_Fsb_Cmd_Callback (int panel, int control, int event,
				       void *callbackData, int eventData1, int eventData2)
{
  unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData = 0;
  int Add_Data = 0;
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:
			
      TempBuffer[1]=0x00;
      TempBuffer[0]=0x30;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      sleep(100); 
			
			
      GetCtrlVal(mainHandle,Main_Manual_USB_Add,&Add_Data);
      GetCtrlVal(mainHandle,Main_Thre_Fsb_Cmd,&ConfigData);      
      TempBuffer[1]=ConfigData+Add_Data;
      TempBuffer[0]=0x31;
      SetCtrlVal(mainHandle,Main_Thre_Fsb_Cmd,TempBuffer[1]);
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      sleep(100); 
					
      TempBuffer[1]=0x00;
      TempBuffer[0]=0xbb; 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
					
				
			
      sleep(100);  
			
      TempBuffer[1]=0xb1;
      TempBuffer[0]=0x00; 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
			
			
			
      sleep(100);
			 
      GetCtrlVal(mainHandle,Main_Switch_SC_Prob,&ConfigData);
      if(ConfigData == 1)
	{
	  TempBuffer[1]=0x01;
	  TempBuffer[0]=0xbb;
	}
      else
	{
	  TempBuffer[1]=0x00;
	  TempBuffer[0]=0xbb;
	}
			

      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
			 
		
			  
			
      break;
    }
  return 0;
}

int CVICALLBACK SS1_SS10_PA_Callback (int panel, int control, int event,
				      void *callbackData, int eventData1, int eventData2)
{
  unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData = 0;
  int Add_Data = 0; 
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:
			
			
      TempBuffer[1]=0x00;
      TempBuffer[0]=0x31;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      sleep(100); 
			
      GetCtrlVal(mainHandle,Main_Manual_USB_Add,&Add_Data);    
      GetCtrlVal(mainHandle,Main_SS1_SS10_PA,&ConfigData);      
      TempBuffer[1]=ConfigData+Add_Data;
      TempBuffer[0]=0x30;
      SetCtrlVal(mainHandle,Main_SS1_SS10_PA,TempBuffer[1]); 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      sleep(100); 
					
      TempBuffer[1]=0x00;
      TempBuffer[0]=0xbb; 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}	
				
			
      sleep(100);  
			
      TempBuffer[1]=0xb1;
      TempBuffer[0]=0x00; 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
			
			
      sleep(100);
			 
      GetCtrlVal(mainHandle,Main_Switch_SC_Prob,&ConfigData);
      if(ConfigData == 1)
	{
	  TempBuffer[1]=0x01;
	  TempBuffer[0]=0xbb;
	}
      else
	{
	  TempBuffer[1]=0x00;
	  TempBuffer[0]=0xbb;
	}
			

      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
			 
			  
			
      break;
    }
  return 0;
}

int CVICALLBACK Select_SCAorBackup (int panel, int control, int event,
				    void *callbackData, int eventData1, int eventData2)
{
  unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData = 0;
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(mainHandle,Main_Select_SCAorBackup,&ConfigData);   
			 
      if(ConfigData == 1)
	{
	  TempBuffer[1]=0x03;
	  TempBuffer[0]=0xfd;
	}
      else
	{
	  TempBuffer[1]=0x04;
	  TempBuffer[0]=0xfd;
	}
			

      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
			 
      sleep(100);
      TempBuffer[1]=0xb1;
      TempBuffer[0]=0x00;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
			 
      break;
    }
  return 0;
}

int CVICALLBACK Set_Analog_out_Backup (int panel, int control, int event,
				       void *callbackData, int eventData1, int eventData2)
{
  unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData = 0;
  int Add_Data = 0;
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:

      GetCtrlVal(mainHandle,Main_Manual_USB_Add,&Add_Data);
      GetCtrlVal(mainHandle,Main_Set_Analog_out_Backup,&ConfigData);      
      TempBuffer[1]=ConfigData+Add_Data;
      TempBuffer[0]=0x34;
      SetCtrlVal(mainHandle,Main_Set_Analog_out_Backup,TempBuffer[1]);
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      sleep(100); 
			
      TempBuffer[1]=0xb2;
      TempBuffer[0]=0x00; 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
			
      break;
    }
  return 0;
}

int CVICALLBACK Send_Trigger (int panel, int control, int event,
			      void *callbackData, int eventData1, int eventData2)
{
  unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData;
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:
			
      TempBuffer[1]=0xe0;
      TempBuffer[0]=0xff;
      //	TempBuffer[1]=0xe3;
      //	TempBuffer[0]=0xff;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      break;
    }
  return 0;
}

int CVICALLBACK Select_Cf (int panel, int control, int event,
			   void *callbackData, int eventData1, int eventData2)
{
  unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData = 0;
  int Add_Data = 0; 
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:

      GetCtrlVal(mainHandle,Main_Select_Cf,&ConfigData);      
      TempBuffer[1]=0xA0 + ((0xF)&(ConfigData));
      TempBuffer[0]=0xAC;
      //	SetCtrlVal(mainHandle,Main_SS1_SS10_PA,TempBuffer[1]); 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
				
      sleep(100);  
			
      TempBuffer[1]=0xb1;
      TempBuffer[0]=0x00; 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
				
      break;
    }
  return 0;
}

int CVICALLBACK Switch_Mode (int panel, int control, int event,
			     void *callbackData, int eventData1, int eventData2)
{
  unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData = 0;
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(mainHandle,Main_Switch_Mode,&ConfigData);
      if(ConfigData == 1)
	{
	  TempBuffer[1]=0x40;
	  TempBuffer[0]=0xfd;
	}
      else
	{
	  TempBuffer[1]=0x41;
	  TempBuffer[0]=0xfd;
	}
			

      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
			
      sleep(100); 
			
      TempBuffer[1]=0xb1;
      TempBuffer[0]=0x00;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      sleep(100); 
      status=viWrite(instr,TempBuffer,2,&n);	  
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
			
      break;
    }
  return 0;
}

int CVICALLBACK Select_ADC_Ext (int panel, int control, int event,
				void *callbackData, int eventData1, int eventData2)
{
  unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData = 0;
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(mainHandle,Main_Select_ADC_Ext,&ConfigData);   
      if(ConfigData == 1)
	{
	  TempBuffer[1]=0x00;
	  TempBuffer[0]=0xf3;
	}
      else
	{
	  TempBuffer[1]=0x01;
	  TempBuffer[0]=0xf3;
	}
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
				
      sleep(100); 
			
      TempBuffer[1]=0xb1;
      TempBuffer[0]=0x00;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      sleep(100); 
      status=viWrite(instr,TempBuffer,2,&n);	  
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
			
      break;
    }
  return 0;
}

int CVICALLBACK Switch_Raz (int panel, int control, int event,
			    void *callbackData, int eventData1, int eventData2)
{
  unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData = 0;
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(mainHandle,Main_Switch_Raz,&ConfigData);   
      if(ConfigData == 1)
	{
	  TempBuffer[1]=0xa1;
	  TempBuffer[0]=0xff;
	}
      else
	{
	  TempBuffer[1]=0xa0;
	  TempBuffer[0]=0xff;
	}
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      break;
    }
  return 0;
}

int CVICALLBACK Select_TDC_On (int panel, int control, int event,
			       void *callbackData, int eventData1, int eventData2)
{
  unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData = 0;
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(mainHandle,Main_Select_TDC_On,&ConfigData);   
      if(ConfigData == 1)
	{
	  TempBuffer[1]=0x01;
	  TempBuffer[0]=0xf9;
	}
      else
	{
	  TempBuffer[1]=0x00;
	  TempBuffer[0]=0xf9;
	}
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
				
      sleep(100); 
			
      TempBuffer[1]=0xb1;
      TempBuffer[0]=0x00;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      sleep(100); 
      status=viWrite(instr,TempBuffer,2,&n);	  
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
				
      break;
    }
  return 0;
}

int CVICALLBACK Auto_TA_Test (int panel, int control, int event,
			      void *callbackData, int eventData1, int eventData2)
{
  unsigned char TempBuffer[2];
  unsigned char TempBuffer1[512];
  char TempMessage[100];
  double CurTime; 
  char timemsg[20]="0000-00-00-00-00-00";
  char WrDataFile[300];
  int ConfigData = 0;
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(mainHandle,Main_Auto_TA_Test,&ConfigData);  
      if(ConfigData == 1)
	{
				
				
				
	  if(datasaveflag==0)
	    {
	      MessagePopup ("错误", "请选择文件保存路径！");
	      return -1;
	    }

	  //GetCurrentDateTime(&CurTime);
	  //FormatDateTimeString(CurTime, "%Y-%m-%d-%H-%M-%S", timemsg, 20);
	  sprintf(WrDataFile,"%s\\%s.dat",WrDataPath,timemsg);
	  fp_write=fopen (WrDataFile, "wb");
	  if(fp_write==NULL)
	    {
	      MessagePopup ("错误", "新建数据保存文件失败！");
	      return -1;
	    }
			
				
	  sleep(100);
	  TempBuffer[1]=0xd0;
	  TempBuffer[0]=0xff;
	  sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
	  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
	  status=viWrite(instr,TempBuffer,2,&n);
	  if(status==VI_SUCCESS)
	    InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
	  else
	    {
	      MessagePopup("错误","发送COM配置命令失败！");
	      return(-1);
	    }
	  sleep(100);
				
	
	  GetData_Status=1; 
	  SetCtrlVal(mainHandle,Main_TEXTBOX,"开始数据采集...\n");
	  SetCtrlVal(mainHandle,Main_Acq_Status,1);
	  //std::thread
	  //CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, DataAcq, &mainHandle, &threadID1);
	  //std::thread
	  //CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, Acqing, &mainHandle, &threadID4);
	  SetCtrlVal(mainHandle,Main_TEXTBOX,"开启监测...\n");  
	  SetCtrlVal(mainHandle,Main_TEXTBOX,"开启数据采集线程...\n");		
				
				
				
	}
      else
	{
		
	  sleep(100);
	  GetData_Status=0;
	  sleep(100); 
	  SetCtrlVal(mainHandle,Main_Acq_Status,0);
	  SetCtrlVal(mainHandle,Main_TEXTBOX,"结束采集命令发出...\n");
	
	  fclose(fp_write);
	}
			
			
      break;
    }
  return 0;
}

int CVICALLBACK Set_Delay_OR64 (int panel, int control, int event,
				void *callbackData, int eventData1, int eventData2)
{
	
  unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData = 0;
  int Add_Data = 0; 
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:

      GetCtrlVal(mainHandle,Main_Set_Delay_OR64,&ConfigData);      
      TempBuffer[1]=ConfigData;
      TempBuffer[0]=0xF4;
      //	SetCtrlVal(mainHandle,Main_SS1_SS10_PA,TempBuffer[1]); 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
				
      sleep(100);  
			
      TempBuffer[1]=0xb1;
      TempBuffer[0]=0x00; 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
				
      break;
    }
  return 0;
}

int CVICALLBACK Switch_Val (int panel, int control, int event,
			    void *callbackData, int eventData1, int eventData2)
{  	
  unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData = 0;
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(mainHandle,Main_Switch_Val,&ConfigData);   
      if(ConfigData == 1)
	{
	  TempBuffer[1]=0xf1;
	  TempBuffer[0]=0xf2;
	}
      else
	{
	  TempBuffer[1]=0xf0;
	  TempBuffer[0]=0xf2;
	}
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      break;
    }
  return 0;
}

int CVICALLBACK Only_ExTrig (int panel, int control, int event,
			     void *callbackData, int eventData1, int eventData2)
{
  unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData = 0;
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(mainHandle,Main_Only_ExTrig,&ConfigData);   
      if(ConfigData == 1)
	{
	  TempBuffer[1]=0xf1;
	  TempBuffer[0]=0xf5;
	}
      else
	{
	  TempBuffer[1]=0xf0;
	  TempBuffer[0]=0xf5;
	}
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      sleep(100);       
      TempBuffer[1]=0xb1;
      TempBuffer[0]=0x00; 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
				
      break;
    }
  return 0;
}

int CVICALLBACK Close_Mask_Panel (int panel, int event, void *callbackData,
				  int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_GOT_FOCUS:

      break;
    case EVENT_LOST_FOCUS:

      break;
    case EVENT_CLOSE:
      DiscardPanel(Mask_Handle);     
      break;
    }
  return 0;
}


int CVICALLBACK Mask_Channel (int panel, int control, int event,
			      void *callbackData, int eventData1, int eventData2)
{
  unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData = 0;
  int Chn[64] = {0},Maks_All_Sig;
  int test;
  int i;
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(Mask_Handle,Panel_Mask_Mask_All,&Maks_All_Sig);
      if(control == Panel_Mask_Mask_All )	
	{
	  for(i = 0; i <64 ; i++)
	    {
	      SetCtrlVal(Mask_Handle,Get_Mask_Ch_ID[i],Maks_All_Sig);		
	      Statue_Mask_Ch[i] = Maks_All_Sig;
					   
	    }
	  if(Maks_All_Sig)
	    {
	      TempBuffer[1]=0xdc;
	      TempBuffer[0]=0xff;
	    }
	  else
	    {
	      TempBuffer[1]=0xdf;
	      TempBuffer[0]=0xff;
	    }
	  sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
	  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
	  status=viWrite(instr,TempBuffer,2,&n);	
	  if(status==VI_SUCCESS)
	    InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
	  else
	    {
	      MessagePopup("错误","发送COM配置命令失败！");
	      return(-1);
	    }
	  sleep(100);
	  TempBuffer[1]=0xb1;
	  TempBuffer[0]=0x00;
	  sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
	  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	  status=viWrite(instr,TempBuffer,2,&n);
	  if(status==VI_SUCCESS)
	    InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");
	  else
	    {
	      MessagePopup("错误","发送COM配置命令失败！");
	      return(-1);
	    }	
	}
      else
	{
	  for(i = 0;i < 64; i++)
	    {
	      GetCtrlVal(Mask_Handle,Get_Mask_Ch_ID[i],&Chn[i]);
	      if(Chn[i] ==Statue_Mask_Ch[i])
		continue;
	      else
		{
		  if(Chn[i])
		    {
		      TempBuffer[1]=i + 1;
		      TempBuffer[0]=0xfc;
		      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
		      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		      status=viWrite(instr,TempBuffer,2,&n);
		      if(status==VI_SUCCESS)
			InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");
		      else
			{
			  MessagePopup("错误","发送COM配置命令失败！");
			  return(-1);
			}
		      sleep(30);
		    }
		  else
		    {
		      TempBuffer[1]=i + 1;
		      TempBuffer[0]=0xfb;
		      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
		      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		      status=viWrite(instr,TempBuffer,2,&n);
		      if(status==VI_SUCCESS)
			InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");
		      else
			{
			  MessagePopup("错误","发送COM配置命令失败！");
			  return(-1);
			}
		      sleep(30);
		    }
		  Statue_Mask_Ch[i] = Chn[i];
		  TempBuffer[1]=0xb1;
		  TempBuffer[0]=0x00;
		  sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
		  SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		  status=viWrite(instr,TempBuffer,2,&n);
		  if(status==VI_SUCCESS)
		    InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");
		  else
		    {
		      MessagePopup("错误","发送COM配置命令失败！");
		      return(-1);
		    }	
		}
					   
	    }
	}
	
    }
  return 0;
}

int CVICALLBACK Display_Mask (int panel, int control, int event,
			      void *callbackData, int eventData1, int eventData2)
{
  int i;
  switch (event)
    {
    case EVENT_COMMIT:
      Mask_Handle = LoadPanel(0,"Mask_Channels.uir",Panel_Mask);
      DisplayPanel(Mask_Handle);
      Get_Mask_Ch_ID[0] =  Panel_Mask_Ch1;
      Get_Mask_Ch_ID[1] =  Panel_Mask_Ch2;
      Get_Mask_Ch_ID[2] =  Panel_Mask_Ch3;
      Get_Mask_Ch_ID[3] =  Panel_Mask_Ch4;
      Get_Mask_Ch_ID[4] =  Panel_Mask_Ch5;
      Get_Mask_Ch_ID[5] =  Panel_Mask_Ch6;
      Get_Mask_Ch_ID[6] =  Panel_Mask_Ch7;
      Get_Mask_Ch_ID[7] =  Panel_Mask_Ch8;
      Get_Mask_Ch_ID[8] =  Panel_Mask_Ch9;
      Get_Mask_Ch_ID[9] =  Panel_Mask_Ch10;
      Get_Mask_Ch_ID[10] =  Panel_Mask_Ch11;
      Get_Mask_Ch_ID[11] =  Panel_Mask_Ch12;
      Get_Mask_Ch_ID[12] =  Panel_Mask_Ch13;
      Get_Mask_Ch_ID[13] =  Panel_Mask_Ch14;
      Get_Mask_Ch_ID[14] =  Panel_Mask_Ch15;
      Get_Mask_Ch_ID[15] =  Panel_Mask_Ch16;
      Get_Mask_Ch_ID[16] =  Panel_Mask_Ch17;
      Get_Mask_Ch_ID[17] =  Panel_Mask_Ch18;
      Get_Mask_Ch_ID[18] =  Panel_Mask_Ch19;
      Get_Mask_Ch_ID[19] =  Panel_Mask_Ch20;
      Get_Mask_Ch_ID[20] =  Panel_Mask_Ch21;
      Get_Mask_Ch_ID[21] =  Panel_Mask_Ch22;
      Get_Mask_Ch_ID[22] =  Panel_Mask_Ch23;
      Get_Mask_Ch_ID[23] =  Panel_Mask_Ch24;
      Get_Mask_Ch_ID[24] =  Panel_Mask_Ch25;
      Get_Mask_Ch_ID[25] =  Panel_Mask_Ch26;
      Get_Mask_Ch_ID[26] =  Panel_Mask_Ch27;
      Get_Mask_Ch_ID[27] =  Panel_Mask_Ch28;
      Get_Mask_Ch_ID[28] =  Panel_Mask_Ch29;
      Get_Mask_Ch_ID[29] =  Panel_Mask_Ch30;
      Get_Mask_Ch_ID[30] =  Panel_Mask_Ch31;
      Get_Mask_Ch_ID[31] =  Panel_Mask_Ch32;
      Get_Mask_Ch_ID[32] =  Panel_Mask_Ch33;
      Get_Mask_Ch_ID[33] =  Panel_Mask_Ch34;
      Get_Mask_Ch_ID[34] =  Panel_Mask_Ch35;
      Get_Mask_Ch_ID[35] =  Panel_Mask_Ch36;
      Get_Mask_Ch_ID[36] =  Panel_Mask_Ch37;
      Get_Mask_Ch_ID[37] =  Panel_Mask_Ch38;
      Get_Mask_Ch_ID[38] =  Panel_Mask_Ch39;
      Get_Mask_Ch_ID[39] =  Panel_Mask_Ch40;
      Get_Mask_Ch_ID[40] =  Panel_Mask_Ch41;
      Get_Mask_Ch_ID[41] =  Panel_Mask_Ch42;
      Get_Mask_Ch_ID[42] =  Panel_Mask_Ch43;
      Get_Mask_Ch_ID[43] =  Panel_Mask_Ch44;
      Get_Mask_Ch_ID[44] =  Panel_Mask_Ch45;
      Get_Mask_Ch_ID[45] =  Panel_Mask_Ch46;
      Get_Mask_Ch_ID[46] =  Panel_Mask_Ch47;
      Get_Mask_Ch_ID[47] =  Panel_Mask_Ch48;
      Get_Mask_Ch_ID[48] =  Panel_Mask_Ch49;
      Get_Mask_Ch_ID[49] =  Panel_Mask_Ch50;
      Get_Mask_Ch_ID[50] =  Panel_Mask_Ch51;
      Get_Mask_Ch_ID[51] =  Panel_Mask_Ch52;
      Get_Mask_Ch_ID[52] =  Panel_Mask_Ch53;
      Get_Mask_Ch_ID[53] =  Panel_Mask_Ch54;
      Get_Mask_Ch_ID[54] =  Panel_Mask_Ch55;
      Get_Mask_Ch_ID[55] =  Panel_Mask_Ch56;
      Get_Mask_Ch_ID[56] =  Panel_Mask_Ch57;
      Get_Mask_Ch_ID[57] =  Panel_Mask_Ch58;
      Get_Mask_Ch_ID[58] =  Panel_Mask_Ch59;
      Get_Mask_Ch_ID[59] =  Panel_Mask_Ch60;
      Get_Mask_Ch_ID[60] =  Panel_Mask_Ch61;
      Get_Mask_Ch_ID[61] =  Panel_Mask_Ch62;
      Get_Mask_Ch_ID[62] =  Panel_Mask_Ch63;
      Get_Mask_Ch_ID[63] =  Panel_Mask_Ch64;
      for(i = 0;i<64;i++)
	{
	  GetCtrlVal(Mask_Handle,Get_Mask_Ch_ID[i],&Statue_Mask_Ch[i]);
	
	}
      break;
    }
  return 0;
}

int CVICALLBACK Close_Hv_Uir (int panel, int event, void *callbackData,
			      int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_GOT_FOCUS:

      break;
    case EVENT_LOST_FOCUS:

      break;
    case EVENT_CLOSE:
      DiscardPanel(Hv_Uir_Handle);     

      break;
    }
  return 0;
}

int CVICALLBACK Set_Hv_Value (int panel, int control, int event,
			      void *callbackData, int eventData1, int eventData2)
{   
  unsigned char TempBuffer[2];

  char TempMessage[100];
  int ConfigData=0;
  unsigned int n;
  int Hv_Value;
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(Hv_Uir_Handle,Hv_Uir_Set_Hv_Value,&Hv_Value);   
			
      ConfigData = 0xF&Hv_Value;
      TempBuffer[1]=ConfigData;
      TempBuffer[0]= 0xd1; 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      sleep(100);
			 
      ConfigData = (0xF0&Hv_Value)>>4;
      TempBuffer[1]=0x10 + ConfigData;
      TempBuffer[0]= 0xd1; 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      sleep(100);  
			
      ConfigData = (0xF00&Hv_Value)>>8;
      TempBuffer[1]=0x20 + ConfigData;
      TempBuffer[0]= 0xd1; 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      sleep(100);   
			  
      ConfigData = (0xF000&Hv_Value)>>12;
      TempBuffer[1]=0x30 + ConfigData;
      TempBuffer[0]= 0xd1; 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      sleep(100);    
			  
      TempBuffer[1]=0x41;
      TempBuffer[0]= 0xd1; 
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      sleep(100);     
      break;
    }
  return 0;
}

int CVICALLBACK On_Off_Hv (int panel, int control, int event,
			   void *callbackData, int eventData1, int eventData2)
{	unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData = 0;
  unsigned int n;
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(Hv_Uir_Handle,Hv_Uir_On_Off_Hv,&ConfigData);   
      if(ConfigData == 1)
	{
	  TempBuffer[1]=0xd1;
	  TempBuffer[0]=0xd0;
	}
      else
	{
	  TempBuffer[1]=0xd0;
	  TempBuffer[0]=0xd0;
	}
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      break;
    }
  return 0;
}

int CVICALLBACK Display_Hv (int panel, int control, int event,
			    void *callbackData, int eventData1, int eventData2)
{
  switch (event)
    {
    case EVENT_COMMIT:
      Hv_Uir_Handle = LoadPanel(0,"Hv_Uir.uir",Hv_Uir);     
      DisplayPanel(Hv_Uir_Handle);

      break;
    }
  return 0;
}

int CVICALLBACK Switch_Leakage (int panel, int control, int event,
				void *callbackData, int eventData1, int eventData2)
{	unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData = 0;
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:
      GetCtrlVal(mainHandle,Main_Switch_Leakage,&ConfigData);
      if(ConfigData == 1)
	{
	  TempBuffer[1]=0x51;
	  TempBuffer[0]=0xd1;
	}
      else
	{
	  TempBuffer[1]=0x50;
	  TempBuffer[0]=0xd1;
	}
			

      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
			
      sleep(100); 
			
      TempBuffer[1]=0xb1;
      TempBuffer[0]=0x00;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);	
      sleep(100); 
      status=viWrite(instr,TempBuffer,2,&n);	  
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      break;
    }
  return 0;
}

int CVICALLBACK Reset_b (int panel, int control, int event,
			 void *callbackData, int eventData1, int eventData2)
{
  unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData;
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:
      TempBuffer[1]=0xc0;
      TempBuffer[0]=0xff;
      //	TempBuffer[1]=0xe3;
      //	TempBuffer[0]=0xff;
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      break;
    }
  return 0;
}

int CVICALLBACK Cosmic_Mode (int panel, int control, int event,
			     void *callbackData, int eventData1, int eventData2)
{
  unsigned char TempBuffer[2];
  char TempMessage[100];
  int ConfigData = 0;
  unsigned int n;
  int usbHandle;
  switch (event)
    {
    case EVENT_COMMIT:

      GetCtrlVal(mainHandle,Main_Cosmic_Mode,&ConfigData);
      if(ConfigData == 1)
	{
	  TempBuffer[1]=0x61;
	  TempBuffer[0]=0xd1;
	}
      else
	{
	  TempBuffer[1]=0x60;
	  TempBuffer[0]=0xd1;
	}
      sprintf(TempMessage,"Write 0x%02x%02x Command via USB\n",TempBuffer[0],TempBuffer[1]);  
      SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);  
      status=viWrite(instr,TempBuffer,2,&n);
      if(status==VI_SUCCESS)
	InsertTextBoxLine(mainHandle,Main_TEXTBOX,-1,"配置成功！");  
      else
	{
	  MessagePopup("错误","发送COM配置命令失败！");
	  return(-1);
	}
      break;
    }
  return 0;
}
