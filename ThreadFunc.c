#include <windows.h>
#include <rs232.h>
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <visa.h>
#include <userint.h>
#include <stdio.h>
#include "All.h"
int AcqPackCnt;
int fp_TAConfig;
int BeepFlag=0;
FILE *fp_temperature;
FILE *fp_current;
unsigned char CommandWord[2]; 

unsigned char High_Cmd[5] = {0x51,0x52,0x52,0x53,0x53,0x54};
unsigned char Low_Cmd[5]  = {0x98,0x21,0xa9,0x31,0xb7,0x40};	
int CVICALLBACK DataAcq(void *functionData)
{
//	unsigned char dat[512], TempMessage[100];
	unsigned char dat[2048], TempMessage[100];  
	int i, j, n;
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
			WriteFile (fp_write, dat,n);
			if(AcqPackCnt%1000==0)
			{
			    sprintf(TempMessage,"�ɼ��� %d �����ݰ�(ÿ�����ݰ�512Byte)...\n",AcqPackCnt);        
				SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
			}
		}
		else
		{   
			/*if(USB_Status==0)
			{
				SetCtrlVal(mainHandle,Main_TEXTBOX,"USB�豸�Ͽ����ɼ��жϣ�\n");
				break;
			}
			else
			{		  */
				if(GetData_Status==1)
				{
					SetCtrlVal(mainHandle,Main_TEXTBOX,"�ȴ����ݡ���\n");
					Beep();
				//	Sleep(100);
					WaitCnt++;
				}
			
		}
		//���������Ӻ������вɼ��������ݵĲ���
	}
	sprintf(TempMessage,"�ɼ�ֹͣ�����ɼ��� %d �����ݰ�...\n",AcqPackCnt);
	SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	Cnt_AcqPackage		=		  AcqPackCnt / 7.56;
	sprintf(TempMessage,"���ɼ��� %d ����������...\n",Cnt_AcqPackage);
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
		Sleep(200);
		Cnt_AcqPack_Delay = Cnt_AcqPack;
	}
	SetCtrlVal(mainHandle,Main_Acq_ing,0); 
	return(0);
}
int CVICALLBACK USBStatus (void* functionData)						  //��ѯUSB�˿�״̬�߳�
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
				
						Beep();
						Sleep(500);
					
				}
				
			
			}
		}
		Sleep(1000);
	}
	return 0;
}

int CVICALLBACK COMStatus (void* functionData)
{
	while(COM_Status==1)
	{
		Error=ReturnRS232Err ();
		if (Error<0)
		{
			COM_Status=0;
			GetData_Status=0;
			AcqPackCnt=0;
			Monitor_Status=0;
			SetCtrlVal(mainHandle,Main_COM_Status,0);
			SetCtrlVal(mainHandle,Main_Acq_Status,0);
			SetCtrlVal(mainHandle,Main_Monitor_Status,0);
			MessagePopup("����","COM�豸�Ͽ���");
			return -1;
		}
		Sleep(1000);
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
			MessagePopup("����","û�гɹ�����TemperatureFile��");
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
					MessagePopup("����","����ң�����");
					return(-1);
				}
			} 
			SetCtrlVal(mainHandle,Main_TEXTBOX,"\n�������FEE�ĵ���ң��......\n");
		}
		
		if(n%100==0)
		{
			for(i=0;i<16;i++)
			{
				if(FEE[i]==0)
					continue;
				if(CheckTemperature(1,FEEID[i],i,fp_temperature)==-1)
				{
					MessagePopup("����","�¶�ң�����");
					return(-1);
				}
			}
			SetCtrlVal(mainHandle,Main_TEXTBOX,"�������FEE���¶�ң��......\n");
		}
		
		if(n%100==0)
		{
			for(i=0;i <16;i++)
			{
				if(FEE[i] == 0)
					continue;
				if(CheckFPGALogic(1, FEEID[i]) == -1)
				{
					MessagePopup("����","FPGA״̬����ң�����");
					return(-1);
				}
			}
			SetCtrlVal(mainHandle,Main_TEXTBOX,"�������FEE��FPGA״̬����ң��......\n");
			
		}
		
		if(n%100==0)
		{
			for(i=0;i<32;i++)
			{
				if(Hit[i]==0)
					continue;
				if(CheckHit( 1 , i)==-1)
				{
					MessagePopup("����","Sub-DAQ״̬�Ĵ���ң�����");
					return(-1);
				}
			}
		 	SetCtrlVal(mainHandle,Main_TEXTBOX,"���Sub-DAQ״̬�Ĵ�����Trig��Hit������ң��......\n\n");
			n=1;
		}
		
		Sleep(100);
		n++;
	}
	fclose(fp_temperature);
	fclose(fp_current);
	return 0;
}

int CVICALLBACK CaliAcq(void *functionData)
{
	unsigned char CommandBuffer[2];
	unsigned char TempMessage[100];
	int i;
 	int Volt = 0;
	int FileLength;
 
	char timemsg[20];
	double CurTime;

	char WrDataFile[300];
	char File[100];

  
	  CommandBuffer[1]  = 0x00;
    CommandBuffer[0]  = 0x11;
	if(SendUsbData(instr, CommandBuffer) ==-1)
		{
		   	MessagePopup ("����", "����USB��������ʧ�ܣ���");
			return -1;
		}
			else
			{
			    SetCtrlVal(mainHandle,Main_TEXTBOX,"����̶�ģʽ\n");
		  }
 	GetCurrentDateTime(&CurTime);

	FormatDateTimeString(CurTime, "%Y-%m-%d-%H-%M-%S", timemsg, 20);

	sprintf(WrDataFile,"%s\\%s.dat",WrDataPath,timemsg);		 
	FileLength=strlen(WrDataFile);
	WrDataFile[FileLength-4]='\0';
  
	for( i = 0 ; i <8 ; i++)
	{	  
    Sleep(100);
    /*-------Creat file------------------------------*/
      Volt  =  i*100;
			sprintf(File,"%s_%d.dat",WrDataFile,Volt);
			fp_write=OpenFile (File, VAL_WRITE_ONLY, VAL_TRUNCATE, VAL_BINARY);
			if(fp_write==-1)
			{
				MessagePopup ("����", "�½����ݱ����ļ�ʧ�ܣ�");
				return -1;
			}
      /*--------------Set Cali Voltage-------------*/
	    CommandBuffer[1]  = Cali_DAC_Code_High[i];
      CommandBuffer[0]  = Cali_DAC_Code_Low[i];
	    if(SendUsbData(instr, CommandBuffer) ==-1)
			  	{
			  	   	MessagePopup ("����", "����USB��������ʧ�ܣ���");
			  		return -1;
			  	}
			  	else
          {
		  		    SetCtrlVal(mainHandle,Main_TEXTBOX,"���ÿ̶ȵ�ѹ\n");
		  	  }
     /*------------Start Acq----------------*/
			GetData_Status=1;
			CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, DataAcq, &mainHandle, &threadID1);       
			SetCtrlVal(mainHandle,Main_Acq_Status,1);
      SetCtrlVal(mainHandle,Main_TEXTBOX,"��ʼ����\n");
	    CommandBuffer[1]  = 0x00;
      CommandBuffer[0]  = 0xa1;
    	if(SendUsbData(instr, CommandBuffer) ==-1)
	    	{
		      	MessagePopup ("����", "����USB��������ʧ�ܣ���");
			    return -1;
	    	}
			else
			{
			    SetCtrlVal(mainHandle,Main_TEXTBOX,"�򿪴���\n");
		  }
      
      while( AcqPackCnt  < 300)
      {
        Sleep(300);
      }				
      
        /*-----Stop Acq-----*/
      	CommandBuffer[1]  = 0x00;
       CommandBuffer[0]  = 0xa0;
    	if(SendUsbData(instr, CommandBuffer) ==-1)
    		{
    		   	MessagePopup ("����", "����USB��������ʧ�ܣ���");
      			return -1;
    		}
			else
			{
			    SetCtrlVal(mainHandle,Main_TEXTBOX,"�رմ���\n");
		  }
      
      GetData_Status=0;  
			CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,threadID1,OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
      SetCtrlVal(mainHandle,Main_TEXTBOX,"ֹͣ����\n");
      AcqPackCnt = 0;
			SetCtrlVal(mainHandle,Main_Acq_Status,0);
  }    
	
  SetCtrlVal(mainHandle,Main_TEXTBOX,"��ɼȶ��̶Ȳ���\n");
	  CommandBuffer[1]  = 0x00;
    CommandBuffer[0]  = 0x10;
	  if(SendUsbData(instr, CommandBuffer) ==-1)
		{
		   	MessagePopup ("����", "����USB��������ʧ�ܣ���");
			return -1;
		}
			else
			{
			    SetCtrlVal(mainHandle,Main_TEXTBOX,"������������ģʽ\n");
		  }
  
  return (0);
	
}	  

int CVICALLBACK SignalSweepAcq(void *functionData)
{
	int WaitCnt=0;
	int ChnID;
	int i;
	int n;
	int flag=0;
	int FileLength;
	int Volt;
	int S_Volt, E_Volt, Step;
	int WatiCount;
	unsigned char TempMessage[100];
	char WrDataFile[300];
	char timemsg[20];
	double CurTime;
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
	GetCurrentDateTime(&CurTime);
	GetCtrlVal(mainHandle,Main_HitEnable,&HitEnable);
	GetCtrlVal(mainHandle,Main_TrigChoose,&TrigChoose);
	FormatDateTimeString(CurTime, "%Y-%m-%d-%H-%M-%S", timemsg, 20);
	sprintf(TempMessage,"%s\\VA%d",WrDataPath,ChnID);
	MakeDir(TempMessage);
	sprintf(WrDataFile,"%s\\VA%d\\%s.dat",WrDataPath,ChnID,timemsg);
	IPLength=strlen(IPAddress);
	sprintf(IP,"%s",IPAddress);
	IP[IPLength+14]='\0';
	status = viOpenDefaultRM(&rm);
	if (status < VI_SUCCESS) 
	{
		MessagePopup ("����", "�ź�Դ��ʧ�ܣ�");
		return -1;
	}
	status = viOpen(rm,IP,VI_NULL,VI_NULL,&SignalSource);
	if (status < VI_SUCCESS) 
	{
		MessagePopup ("����", "�����ź�Դ�����ڣ�");
		return -1;
	}
//	CloseSignal(SignalSource);
	SetCtrlVal(mainHandle,Main_TEXTBOX,"��ʼ�����ź�Դɨ�����...\n");
	if(S_Volt>=E_Volt)
	{
		MessagePopup("����","�̶ȵ�ѹ����ʼֵӦС����ֵֹ��");
		return(-1);
	}
	if(Step<=0)
	{
		MessagePopup("����","����ӦΪ��ֵ��");
		return(-1);
	}
	FileLength=strlen(WrDataFile);
	WrDataFile[FileLength-4]='\0';
	if(Cmd2DAQ_Reset(instr)==-1)
	{
		MessagePopup("����","����USB��������ʧ�ܣ�");
		return(-1);
	}
	Sleep(10);
	for(i=0;i<16;i++)
	{
		if(FEE[i]==0)
			continue;
		if(Cmd2DAQ_FEESet(instr,i,FEEID[i])==-1)
		{
			MessagePopup("����","����USB��������ʧ�ܣ�");
			return(-1);
		}
		else
		{
			sprintf(TempMessage,"FEE%d����ΪFEE %s\n",i+1,FEENAME[i]);
			SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		}
	}
			CommandWord[0] = 0x03;
			CommandWord[1] = 0x3f;
			if(SendUsbData(instr, CommandWord) ==-1)
			{
				MessagePopup ("����", "����USB��������ʧ�ܣ���");
				return -1;
			}
			else
			    SetCtrlVal(mainHandle,Main_TEXTBOX,"����ͨ��ѡ��Ϊ����...\n");
			CommandWord[0] = 0x02;
			CommandWord[1] = 0x2f;
			if(SendUsbData(instr, CommandWord)==-1)								 
			{
			   	MessagePopup ("����", "����USB��������ʧ�ܣ�");
				return -1;
			}
			else
			    SetCtrlVal(mainHandle,Main_TEXTBOX,"�ر�����ͨ��...\n");
        	CommandWord[0] = 0x0a;
			CommandWord[1] = 0xaf;
			if(SendUsbData(instr, CommandWord)==-1)
			{
				MessagePopup("����","����USB��������ʧ�ܣ�");
				return(-1);
			}
			else
				SetCtrlVal(mainHandle,Main_TEXTBOX,"�ر����������ͨ��...\n");
	for(i=0;i<16;i++)
	{
		if(FEE[i]==0)
			continue;
		if((Cmd2DAQ_EnableFEECmdChn(instr,i))==-1)
		{
			MessagePopup("����","����USB��������ʧ�ܣ�");
			return(-1);
		}
	else
	{
			sprintf(TempMessage,"FEE %s�����ͨ����...\n",FEENAME[i]);
			SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
		}
	}	  
	Cmd2DAQ_ChooseTrigMode(instr,4);									 
	SetCtrlVal(mainHandle,Main_TEXTBOX,"Sub_DAQѡ��Ϊ�ⴥ��...\n");
	CloseSignal(SignalSource);
	Cmd2DAQ_ClrFifo(instr);				//1013
//	SetCtrlVal(mainHandle,Main_TEXTBOX,"clrfifoִ������\n");  
	status=viRead(instr,dat,512,&n);		
	  
	Sleep(10);
/*	while(n!=0)
	{
		status=viRead(instr,dat,512,&n);
		Sleep(10);
	}   			   */

	Cmd2DAQ_SetFifo(instr);			//	  1013
	for(i=0;i<16;i++)
	{
		if(FEE[i]==0)
			continue;
		if(Cmd2DAQ_EnableFEEChn(instr,i)==-1)
		{
			MessagePopup("����","����USB��������ʧ�ܣ�");
			return(-1);
		}
		sprintf(TempMessage,"FEE %sͨ����...\n",FEENAME[i]);
		SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	}
	if(Cmd2DAQ_StartAcq(instr)==-1)
	{
		MessagePopup("����","����USB��������ʧ�ܣ�");
		return(-1);
	}
	if(Cmd2DAQ_EnableTrig(instr)==-1)
	{
		MessagePopup ("����", "����USB��������ʧ�ܣ�");
		return -1;
	}
	SetSignal(SignalSource,0);
	for(Volt=S_Volt;Volt<=E_Volt;Volt=Volt+Step)
	{
				AcqPackCnt=0;
				/*	if(Cmd2DAQ_Reset(instr)==-1)
				{
					MessagePopup("����","����USB��������ʧ�ܣ�");
					return(-1);
				}
				Sleep(10);
				if(Cmd2DAQ_Reset(instr)==-1)
				{
					MessagePopup("����","����USB��������ʧ�ܣ�");
					return(-1);
				}
				else
					SetCtrlVal(mainHandle,Main_TEXTBOX,"Sub_DAQ FPGA��λ...\n");			   */
				/*for(i=0;i<8;i++)
				{
					if(FEE[i]==0)
						continue;
					if(Cmd2DAQ_FEESet(instr,i,FEEID[i])==-1)
					{
						MessagePopup("����","����USB��������ʧ�ܣ�");
						return(-1);
					}
					else
					{
						sprintf(TempMessage,"FEE%d����ΪFEE %s\n",i+1,FEENAME[i]);
						SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
					}
				}
				if(Cmd2DAQ_ChooseMasterFEEChn(instr,0xF)==-1)
				{
				   	MessagePopup ("����", "����USB��������ʧ�ܣ���");
					return -1;
				}
				else
				    SetCtrlVal(mainHandle,Main_TEXTBOX,"����ͨ��ѡ��Ϊ����...\n");
				if(Cmd2DAQ_DisableFEEChn(instr,0xF)==-1)								 
				{
				   	MessagePopup ("����", "����USB��������ʧ�ܣ�");
					return -1;
				}
				else
				    SetCtrlVal(mainHandle,Main_TEXTBOX,"�ر�����ͨ��...\n");
				if(Cmd2DAQ_DisableFEECmdChn(instr,0xF)==-1)
				{
					MessagePopup("����","����USB��������ʧ�ܣ�");
					return(-1);
				}
				else
					SetCtrlVal(mainHandle,Main_TEXTBOX,"�ر����������ͨ��...\n");
				for(i=0;i<8;i++)
				{
					if(FEE[i]==0)
						continue;
					if((Cmd2DAQ_EnableFEECmdChn(instr,i))==-1)
					{
						MessagePopup("����","����USB��������ʧ�ܣ�");
						return(-1);
					}
					else
					{
						sprintf(TempMessage,"FEE %s�����ͨ����...\n",FEENAME[i]);
						SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
					}
				}	  */
				if(COM_Status==1)
				{
					if(Monitor_Status==1)
					{
						flag=1;
						Monitor_Status=0;
						if (threadID4)
							CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,threadID4 , OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
						SetCtrlVal(mainHandle, Main_TEXTBOX, "�ر�ң���߳�..\n");
						SetCtrlVal(mainHandle,Main_Monitor_Status,0);
						Monitor_Status=0;
					}
					for(i=0;i<16;i++)
					{
						if(FEE[i]==0)
							continue;
						if((ResetFPGA(1,FEEID[i]))==-1)
						{
							MessagePopup("����","����COM��������ʧ�ܣ�");
							return(-1);
						}
						Sleep(10);
						if((ResetFPGA(1,FEEID[i]))==-1)
						{
							MessagePopup("����","����COM��������ʧ�ܣ�");
							return(-1);
						}
						else
						{
							sprintf(TempMessage,"FEE%s FPGA��λ\n",FEENAME[i]);
							SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
						}
					}
				}		 
			/*	if(TrigDelay==0)
				{
					MessagePopup("����","δ���ô����ӳ�ֵ��");
					return(-1);
				}			   */
				TrigDelay = 0x1D;
				for(i=0;i<16;i++)
				{
					if(FEE[i]==0)
						continue;
					if((SetDelay(1,FEEID[i],TrigDelay))==-1)
					{
						MessagePopup("����","����COM��������ʧ�ܣ�");
						return(-1);
					}
					sprintf(TempMessage,"����FEE %s�Ĵ����ӳ�ֵΪ%d...\n",FEENAME[i],TrigDelay);
					SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
				}
				for(i=0;i<16;i++)
				{
					if(FEE[i]==0)
						continue;
					if ( (ConfigReg(1,FEEID[i],0,0,0,0,1,0))==-1)
					{
						MessagePopup("����","����COM��������ʧ�ܣ�");
						return -1;
					}
					else
					{
						Fmt(TempMessage,"%s<%s%d%s","FEE",FEE[i],"�������չر�...\n");
						SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
					}
				}
			//	status=viRead(instr,dat,512,&n);	
			//	Sleep(100);
			//	while(n!=0)
			//	{
			//		status=viRead(instr,dat,512,&n);
			//		Sleep(100);
			//	}
				sprintf(File,"%s_%dmV.dat",WrDataFile,Volt);
				fp_write=OpenFile (File, VAL_WRITE_ONLY, VAL_TRUNCATE, VAL_BINARY);
				if(fp_write==-1)
				{
					MessagePopup ("����", "�½����ݱ����ļ�ʧ�ܣ�");
					return -1;
				}
				SetSignal(SignalSource,Volt);
				sprintf(TempMessage,"�����źŷ����������Ĵ�����ѹΪ%dmV...\n",Volt);
				SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
				for(i=0;i<16;i++)
				{
					if(FEE[i]==0)
					continue;
					if ( (ClearTrig(1,FEEID[i]))==-1)
					{
						MessagePopup("����","����COM��������ʧ�ܣ�");
						return -1;
					}
					else
					{
						sprintf(TempMessage,"FEE %s������������...\n",FEENAME[i]);
						SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
					}
					SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
				}
		
				OpenSignal(SignalSource);
				Sleep(100);
				for(i=0;i<16;i++)
				{
					if(FEE[i]==0)
						continue;
					if(ConfigReg(1,FEEID[i],HitEnable,0,TrigChoose,0,0,1)==-1)
					{
						MessagePopup("����","����COM��������ʧ�ܣ�");
						return(-1);
					}
					sprintf(TempMessage,"��ɶ�FEE %s������...\n",FEENAME[i]);
					SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
				}
				sprintf(TempMessage,"��ʼ%dmV�ź�Դɨ�����...\n",Volt);
				SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
				SetCtrlVal(mainHandle,Main_Acq_Status,1);
				SetCtrlAttribute(mainHandle,Main_Open_SaveFile,ATTR_DIMMED,1);
		/*		for(AcqPackCnt=0;AcqPackCnt<200;)
				{
					status=viRead(instr,dat,512,&n);
					if (n!=0)
					{
						WaitCount=0;
						AcqPackCnt++;
						WriteFile (fp_write, dat,n);
					}
					else
					{   
						if(USB_Status==0)
						{
							SetCtrlVal(mainHandle,Main_TEXTBOX,"USB�豸�Ͽ����ɼ��жϣ�\n");
							break;
						}
						else
						{
							SetCtrlVal(mainHandle,Main_TEXTBOX,"�ȴ����ݡ���\n");
							Sleep(100);
				//			WaitCount++;
						}
					}
				}	*/ 
				GetData_Status=1;
				CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, DataAcq, &mainHandle, &threadID1);
				SetCtrlVal(mainHandle,Main_TEXTBOX,"��ʼ�ɼ�\n"); 
				while(AcqPackCnt<200)
				{
					if(SignalSweep_Status==0)
					{   
							GetData_Status=0;
							CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,threadID1,OPT_TP_PROCESS_EVENTS_WHILE_WAITING);

							if(Cmd2DAQ_DisableTrig(instr)==-1)
							{
								MessagePopup("����","����USB��������ʧ�ܣ�");
								return(-1);
							}
							else
								SetCtrlVal(mainHandle,Main_TEXTBOX,"Sub_DAQ����ʹ�ܹر�...\n");
							for(i=0;i<16;i++)
							{
								if(Cmd2DAQ_DisableTrig(instr)==-1)
								{
									MessagePopup ("����", "����USB��������ʧ�ܣ�");
									return -1;
								}
								if(FEE[i]==0)
									continue;
								if ( (ConfigReg(1,FEEID[i],0,0,0,0,0,0))==-1)
								{
									MessagePopup("����","����COM��������ʧ�ܣ�");
									return -1;
								}
								else
								{
									Fmt(TempMessage,"%s<%s%d%s","FEE",FEE[i],"�������չر�...\n");
									SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
								}
							}
				            CommandWord[0] = 0x02;
							CommandWord[1] = 0x2f;
							if(SendUsbData(instr, CommandWord)==-1)									 
							{
								  	MessagePopup ("����", "����USB��������ʧ�ܣ�");
								return -1;
							}
							else
							    SetCtrlVal(mainHandle,Main_TEXTBOX,"�ر�����ͨ��...\n");
								if(Cmd2DAQ_StopAcq(instr)==-1)
								{
								   	MessagePopup ("����", "����USB��������ʧ�ܣ�");
									return -1;
								}
								else
								    SetCtrlVal(mainHandle,Main_TEXTBOX,"��ȡʣ������...\n");  
								Sleep(500); 
								GetData_Status=0;
								Sleep(100); 
								SetCtrlVal(mainHandle,Main_Acq_Status,0);
								SetCtrlVal(mainHandle,Main_TEXTBOX,"�����ɼ������...\n");
								CloseFile(fp_write);
								SetCtrlAttribute(mainHandle,Main_Open_SaveFile,ATTR_DIMMED,0);
								SetCtrlVal(mainHandle,Main_Acq_Status,0);
								CloseFile(fp_write);
								return(-1);
					}
					else
						Sleep(1000);
				
				}
				for(i=0;i<16;i++)
				{
					if(FEE[i]==0)
						continue;
					if ( (ConfigReg(1,FEEID[i],0,0,0,0,1,0))==-1)
					{
						MessagePopup("����","����COM��������ʧ�ܣ�");
						return -1;
					}
					else
					{
						Fmt(TempMessage,"%s<%s%d%s","FEE",FEE[i],"�������չر�...\n");
						SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
					}
				}
				CloseSignal(SignalSource);
				if(Cmd2DAQ_ClrFifo(instr)==-1)
				{
					MessagePopup("����","����USB��������ʧ�ܣ�");
					return(-1);
				}
				Sleep(500);
				GetData_Status=0;
				CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,threadID1,OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
				sprintf(TempMessage,"���%dmV�ź�Դ����...\n",Volt);
				SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
				SetCtrlVal(mainHandle,Main_Acq_Status,0);
				CloseFile(fp_write);
				if(Cmd2DAQ_SetFifo(instr)==-1)
				{
					MessagePopup("����","����USB��������ʧ�ܣ�");
					return(-1);
				}
				SetCtrlVal(mainHandle,Main_Acq_Status,0);
				CloseFile(fp_write);
				SetCtrlAttribute(mainHandle,Main_Save_Path,ATTR_DIMMED,0);
	}
	if(Cmd2DAQ_DisableTrig(instr)==-1)
	{
		MessagePopup ("����", "����USB��������ʧ�ܣ�");
		return -1;
	}
	if(Cmd2DAQ_StopAcq(instr)==-1)
	{
		MessagePopup ("����", "����USB��������ʧ�ܣ�");
		return -1;
	}
            CommandWord[0] = 0x02;
			CommandWord[1] = 0x2f;
			if(SendUsbData(instr, CommandWord)==-1)									 
			{
				  	MessagePopup ("����", "����USB��������ʧ�ܣ�");
				return -1;
			}
			else
			    SetCtrlVal(mainHandle,Main_TEXTBOX,"�ر�����ͨ��...\n");
	SetCtrlVal(mainHandle,Main_TEXTBOX,"����ź�Դɨ�����...\n");
	SignalSweep_Status=0;
	SetCtrlAttribute(mainHandle,Main_Open_SaveFile,ATTR_DIMMED,0);
	SetCtrlVal(signalsweepHandle, PANEL_Sig_ChnID,(ChnID+1));
	BeepFlag=1;
	CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE,BeepFunction, &mainHandle, &threadID7);
	MessagePopup("���","���ֶ��л�����һ��оƬ������");
	BeepFlag=0;
	return(0);
}

int CVICALLBACK BeepFunction(void *functionData)
{
	while(BeepFlag==1)
	{
		Beep();
		Sleep(800);
	}
	return(0);
}

ViUInt32 retCnt = 0, itemCnt = 0;

void OpenSignal(ViSession devise)											    //���ź�Դ���
{
	unsigned char TempMessage[100];
	sprintf(TempMessage,"OUTPut2:STATe ON");
	status = viWrite(devise, (ViBuf)TempMessage, 17, &retCnt);
	if (status < VI_SUCCESS)
	{	
		MessagePopup ("����", "δ�ܴ��ź�Դ��");
		return;
	}
	sprintf(TempMessage,"OUTPut1:STATe ON");
	status = viWrite(devise, (ViBuf)TempMessage, 17, &retCnt);
	if (status < VI_SUCCESS)
	{	
		MessagePopup ("����", "δ�ܴ��ź�Դ��");
		return;
	}
	SetCtrlVal(mainHandle,Main_TEXTBOX,"���ź�Դ��\n");
	return;
}

void CloseSignal(ViSession devise)												//�ر��ź�Դ���
{
	unsigned char TempMessage[100];
	sprintf(TempMessage,"OUTPut1:STATe OFF");
	status = viWrite(devise, (ViBuf)TempMessage, 18, &retCnt);
	if (status < VI_SUCCESS)
	{	
		MessagePopup ("����", "δ�ܹر��ź�Դ��");
		return;
	}
	sprintf(TempMessage,"OUTPut2:STATe OFF");
	status = viWrite(devise, (ViBuf)TempMessage, 18, &retCnt);
	if (status < VI_SUCCESS)
	{	
		MessagePopup ("����", "δ�ܹر��ź�Դ��");
		return;
	}
	SetCtrlVal(mainHandle,Main_TEXTBOX,"�ر��ź�Դ��\n");
	return;
}

void SetSignal(ViSession devise, int voltage )									//�����ź�Դ�ķ��Ⱥ�ƫ��
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
		MessagePopup ("����", "��ѹ��������ʧ�ܣ�");
		return;
	}
	sprintf(Off,"%.3f",o);
	strcat(OffStr,Off);
	status = viWrite(devise, (ViBuf) OffStr, strlen(OffStr) + 1, &retCnt);
	if (status < VI_SUCCESS)
	{
		MessagePopup ("����", "ƫ�õ�ѹ����ʧ�ܣ�");
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
				SetCtrlVal(mainHandle,Main_TEXTBOX,"�����߳�\n"); 
			}
			n++;
			Sleep(100);
			
		//���������Ӻ������вɼ��������ݵĲ���
	}
	sprintf(TempMessage,"�����߳̽���...\n");
	SetCtrlVal(mainHandle,Main_TEXTBOX,TempMessage);
	return(0);
}


