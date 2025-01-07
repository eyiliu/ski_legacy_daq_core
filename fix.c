
#include <unistd.h>
#include <cstdint>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void MessagePopup(const char*, const  char*){};
void SetCtrlVal (int, int , const char* ){};
void SetCtrlVal (int, int , const unsigned char* ){};
void SetCtrlVal(int&, int, int){};
void SetCtrlAttribute(int,int,int,int){};
void GetCtrlVal(int, int, int*){};
void GetCtrlVal(int, int, double*){};


void InsertTextBoxLine(int , int , int , const char*){};
void DisplayPanel(int){};
void DiscardPanel(int){};
int InitCVIRTE (int, char**, int){return 0;};
int LoadPanel (int, const char*, int){return 0;};
void RunUserInterface(){};
void QuitUserInterface(int){};
void ResetTextBox(int,int,const char*){};
void FlushInQ(int){};
void FlushOutQ(int){};
int GetPanelHandleFromTabPage(int,int,int,int*){return 0;};

int OpenComConfig (int, const char*, int, int, int, int, int, int){return 0;};
int CloseCom(int){return 0;};
int CreateUDPChannel(int, unsigned int*){return 0;};


//cvi cmdviacom
int SendComData(int PortNumber, unsigned char *Command){return 0;};
int SetDelay(int PortNumber, int FeeID, int DelayValue){return 0;};
int LoopbackTest(int PortNumber, int FeeID){return 0;};
int ClearTrig(int PortNumber, int FeeID){return 0;};
int ConfigReg(int PortNumber, int FeeID, int HitEnable, int HitChoose, int TrigChoose, int DataMode, int WorkMode, int Trig){return 0;};
int Set_Current_Thr(){return 0;};
void Check(int InitialData, unsigned char *ConBuf){};
int ReadComCmdResp(int PortNumber){return 0;};
int SetMaxValue(int PortNumber, int FeeID, int Chn, int MaxValue){return 0;};
int CheckFPGALogic(int PortNumber, int FeeID){return 0;};
int SetMinValue(int PortNumber, int FeeID, int Chn, int MinValue){return 0;};
int ResetFPGA(int PortNumber, int FeeID){return 0;};
int TA_Thr(int PortNumber, int FeeID, int Ch, int Thr){return 0;};
int TA_DAC(int PortNumber, int FeeID, int DAC){return 0;};
int TA_Self_Test(int PortNumber, int FeeID, int Channel){return 0;};
int SetTA_Config(int PortNumber, int FeeID, int Address, int Parameter){return 0;};
int TA_load(int PortNumber, int FeeID){return 0;};
int SetTHR(int PortNumber, int FeeID, unsigned char THR_value[2], unsigned char type){return 0;};
int CheckCurrent(int PortNumber, int FeeID, int Fee, FILE *fp){return 0;};
int CheckTemperature(int PortNumber, int FeeID, int Fee, FILE *fp){return 0;};
int CheckHit(int PortNumber, int HitID){return 0;};
//cvi dmdvisausb
//int SendUsbData(unsigned long instr, unsigned char CommandWord[2]){return 0;};



