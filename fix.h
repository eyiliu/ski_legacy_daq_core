#pragma once

void MessagePopup(const char*, const  char*);
void SetCtrlVal (int, int , const char* );
void SetCtrlVal (int, int , const unsigned char* );
void SetCtrlVal(int&, int, int);
void SetCtrlAttribute(int,int,int,int);
void GetCtrlVal(int, int, int*);
void GetCtrlVal(int, int, double*);



void InsertTextBoxLine(int , int , int , const char*);
void DisplayPanel(int);
void DiscardPanel(int);
int InitCVIRTE (int, char**, int);
int LoadPanel (int, const char*, int);
void RunUserInterface();
void QuitUserInterface(int);
void ResetTextBox(int,int,const char*);
void FlushInQ(int);
void FlushOutQ(int);
int GetPanelHandleFromTabPage(int,int,int,int*);

int OpenComConfig (int, const char*, int, int, int, int, int, int);
int CloseCom(int);
int CreateUDPChannel(int, unsigned int*);
#define UDP_ANY_LOCAL_PORT 12345


#define EVENT_GOT_FOCUS 1
#define EVENT_COMMIT  2
#define EVENT_LOST_FOCUS 3
#define EVENT_CLOSE 4
#define EVENT_TIMER_TICK 5


