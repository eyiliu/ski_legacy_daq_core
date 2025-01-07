//==============================================================================
//
// Title:       All.h
// Purpose:     A short description of the interface.
//
// Created on:  2012/5/16 at 15:44:12 by xxtt.
// Copyright:   xxtt. All Rights Reserved.
//
//==============================================================================

#ifndef __All_H__
#define __All_H__


//==============================================================================

// Include files
#include <ni-visa/visa.h>
#include <stdio.h>
/* #include "cvidef.h" */
#include "Main.h"
/* #include "COM_Cmd_Status.h" */
#include "SignalSweep.h"

//==============================================================================
// Constants
		/*
#define PX_1 17
#define PX_2 17
#define PX_3 16
#define PX_4 16
#define NX_1 16
#define NX_2 16
#define NX_3 16
#define NX_4 16
#define PY_1 16
#define PY_2 16
#define PY_3 16
#define PY_4 16
#define NY_1 16
#define NY_2 16
#define NY_3 16
#define NY_4 16
*/
#define PORT 1024   			//Define the port      
#define PX_1 0x10
#define PX_2 0x11
#define PX_3 0x12
#define PX_4 0x13
#define NX_1 0x14
#define NX_2 0x15
#define NX_3 0x16
#define NX_4 0x17
#define PY_1 0x18
#define PY_2 0x19
#define PY_3 0x1a
#define PY_4 0x1b
#define NY_1 0x1c
#define NY_2 0x1d
#define NY_3 0x1e
#define NY_4 0x1f
#define SUB_DAQ 0x30		
																												   


		
//==============================================================================
// Types

//==============================================================================
// External variables
extern int USB_Status, GetData_Status, datasaveflag,COM_Status, Monitor_Status,COM_Cmd_Status,Cali_Status,Cali_Mode_Status,SignalSweep_Status,DAQTrig_Mode_Status,Hit_Status,TestA_Status;
extern ViStatus status;
extern ViSession defaultRM, instr;
/* extern Error; */
extern int threadID1, threadID2, threadID3,threadID4,threadID5,threadID6,threadID7,threadID8,threadID9,threadID10,threadID11,threadID12,threadID13,threadID14,threadID15,threadID16;
extern int Show_FEE_Status[16];
extern int FEE[16],Hit[32];
extern int FEEID[17];
extern char FEENAME[16][6];
extern int mainHandle, Status1Handle, Status2Handle, Status3Handle, Status4Handle, Status5Handle, Status6Handle,Status7Handle,Status8Handle,comcmdHandle,signalsweepHandle,countHandle;
extern char WrDataPath[300];
extern FILE* fp_write;
extern FILE* fp_TAConfig;
extern int TrigDelay,Hit_Count;
extern FILE* fp_cmd_log;
extern FILE* fp_cmd_log_1, fp_cmd_log_2, fp_cmd_log_3, fp_cmd_log_4;
extern int Current_Remote[32],Temp_Remote[64];
extern unsigned int Trig_Count;
extern unsigned char Response[256]; // 20140320 Changqing Feng - 1V11   
extern char IPAddress[100] ;


extern unsigned char 	Cali_DAC_Code_High[8];
extern unsigned char 	Cali_DAC_Code_Low[8]; 

//==============================================================================
// Global functions

//----------------------------CmdViaCom-----------------------------------------		
void Check(int InitialData, unsigned char *ConBuf);
int SendComData(int PortNumber, unsigned char Command[2]);
int ReadComCmdResp(int PortNumber);
int LoopbackTest(int PortNumber, int FeeID);
int ResetFPGA(int PortNumber, int FeeID);
int ClearTrig(int PortNumber, int FeeID);
int ConfigReg(int PortNumber, int FeeID, int HitEnalbe, int HitChoose, int TrigChoose, int DataMode, int WorkMode, int Trig);
int CheckCurrent(int PortNumber, int FeeID, int Fee, FILE *fp);
int CheckTemperature(int PortNumber, int FeeID, int Fee, FILE *fp);
int CheckFPGALogic(int PortNumber, int FeeID);
int SetDelay(int PortNumber, int FeeID, int DelayValue);
int SetDACCode(int PortNumber, int FeeID, int Value);
int SetCaliTestChn(int PortNumber, int FeeID, int Chn);
int SetMaxValue(int PortNumber, int FeeID, int Chn, int MaxValue);
int SetMinValue(int PortNumber, int FeeID, int Chn, int MinValue);
int SetTA_Config(int PortNumber, int FeeID, int Address, int Parameter);
int TA_Self_Test(int PortNumber, int FeeID, int Channel);
int TA_Thr(int PortNumber, int FeeID, int Ch, int Thr);
int TA_DAC(int PortNumber, int FeeID, int DAC);
int TA_load(int PortNumber, int FeeID );
int SetTHR(int PortNumber, int FeeID, unsigned char THR_value[2] ,unsigned char type);
int CheckHit(int PortNumber, int HitID);
int Set_Current_Thr(void);
int Check_Current(unsigned char *Response);
int Check_Temp(unsigned char*Response);

//-----------------------------CmdViaUSB----------------------------------------
int SendUsbData(ViSession instr, unsigned char CommandWord[2]);
int Cmd2DAQ_StartAcq(ViSession instr);
int Cmd2DAQ_ClrFifo(ViSession instr);     
int Cmd2DAQ_SetFifo(ViSession instr);     
int Cmd2DAQ_StopAcq(ViSession instr);
int Cmd2DAQ_Reset(ViSession instr);
int Cmd2DAQ_EnableFEEChn(ViSession instr,int FEEID);
int Cmd2DAQ_DisableFEEChn(ViSession instr, int FEEID);
int Cmd2DAQ_EnableFEECmdChn(ViSession instr, int FEEID);
int Cmd2DAQ_DisableFEECmdChn(ViSession instr, int FEEID);
int Cmd2DAQ_ChooseMasterFEEChn(ViSession instr, int FEEID);
int Cmd2DAQ_ChooseBackupFEEChn(ViSession instr, int FEEID);
int Cmd2DAQ_ChooseTrigMode(ViSession instr, int Mode);
int Cmd2DAQ_CaliInternal(ViSession instr, int Time);
int Cmd2DAQ_FEESet(ViSession instr, int M, int N);
int Cmd2DAQ_EnableTrig(ViSession instr);
int Cmd2DAQ_DisableTrig(ViSession instr);
int Cmd2DAQ_HitChnChoose(ViSession instr, int HitChnPara);
int Cmd2DAQ_Trig_Sel(ViSession instr,int TrigChoos);
int Cmd2DAQ_CMD_Sel(ViSession instr,int CMD_Sel);
int Cmd2DAQ_Hit_Sel(ViSession instr,int HitChoose);

//-----------------------------ThreadFunc---------------------------------------
int CVICALLBACK DataAcq(void* functionData);
int CVICALLBACK USBStatus(void* functionData);
int CVICALLBACK COMStatus(void* functionData);
int CVICALLBACK MonitorFunction(void* functionData);
int CVICALLBACK CaliAcq(void* functionData);
int CVICALLBACK SignalSweepAcq(void *functionData);
int CVICALLBACK BeepFunction(void *functionData);
int CVICALLBACK TestA(void *functionData);
int CVICALLBACK Cali_For_DAQ(void *functionData);     
int CVICALLBACK Acqing(void *functionData) ;


int Translation (double Charge, int *DAC, double Relationship[12][2]);


#endif  /* ndef __All_H__ */
