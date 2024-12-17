/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  Main                             1       /* callback function: MainFunc */
#define  Main_Receive                     2       /* control type: string, callback function: (none) */
#define  Main_Manual_USB_Add              3       /* control type: numeric, callback function: (none) */
#define  Main_Manual_USB                  4       /* control type: numeric, callback function: (none) */
#define  Main_Send                        5       /* control type: string, callback function: (none) */
#define  Main_USB_Status                  6       /* control type: LED, callback function: (none) */
#define  Main_More_Hit_Mode               7       /* control type: command, callback function: MoreHitMode */
#define  Main_ConNET                      8       /* control type: command, callback function: More_USB_Cmd */
#define  Main_Send_Trigger                9       /* control type: command, callback function: Send_Trigger */
#define  Main_Config_SC_para              10      /* control type: command, callback function: Config_SC_para */
#define  Main_Manual_USB_Cmd              11      /* control type: command, callback function: Manual_USB_Cmd */
#define  Main_Open_ConfigFile             12      /* control type: command, callback function: Open_ConfigFile */
#define  Main_Open_SaveFile               13      /* control type: command, callback function: Open_SaveFile */
#define  Main_Set_Thre_DAC                14      /* control type: command, callback function: Set_Thre_DAC */
#define  Main_Set_SS1                     15      /* control type: command, callback function: Set_SS1 */
#define  Main_Set_Thre_Fsb                16      /* control type: command, callback function: Set_Thre_Fsb */
#define  Main_Set_DAC                     17      /* control type: command, callback function: Set_DAC */
#define  Main_Set_Delay                   18      /* control type: command, callback function: Set_Delay */
#define  Main_CaliInternal                19      /* control type: numeric, callback function: (none) */
#define  Main_COM_Status                  20      /* control type: LED, callback function: (none) */
#define  Main_Acq_ing                     21      /* control type: LED, callback function: (none) */
#define  Main_Acq_Status                  22      /* control type: LED, callback function: (none) */
#define  Main_TA_Num                      23      /* control type: numeric, callback function: (none) */
#define  Main_Monitor_Status              24      /* control type: LED, callback function: (none) */
#define  Main_TEXTBOX                     25      /* control type: textBox, callback function: (none) */
#define  Main_SignalSweep                 26      /* control type: command, callback function: SingalSweep */
#define  Main_Display_Hv                  27      /* control type: command, callback function: Display_Hv */
#define  Main_Display_Mask                28      /* control type: command, callback function: Display_Mask */
#define  Main_Help                        29      /* control type: command, callback function: Help */
#define  Main_Write_NET                   30      /* control type: command, callback function: Write_NET */
#define  Main_Clear_Textbox               31      /* control type: command, callback function: Clear_Textbox */
#define  Main_ClassicsHitMode             32      /* control type: ring, callback function: (none) */
#define  Main_HitChoose                   33      /* control type: ring, callback function: (none) */
#define  Main_DAQTrigMode                 34      /* control type: ring, callback function: DAQ_Trig_Mode */
#define  Main_Show_FEE                    35      /* control type: ring, callback function: (none) */
#define  Main_Sys_Reset                   36      /* control type: command, callback function: Sys_Reset */
#define  Main_Stop_Monitor                37      /* control type: command, callback function: Stop_Monitor */
#define  Main_Start_Monitor               38      /* control type: command, callback function: Start_Monitor */
#define  Main_Hit_Test                    39      /* control type: command, callback function: Hit_Test_Func */
#define  Main_TA_Test                     40      /* control type: command, callback function: TA_Test_Func */
#define  Main_SetTA_thr                   41      /* control type: command, callback function: SetTA_THR */
#define  Main_SetTA                       42      /* control type: command, callback function: SetTA */
#define  Main_Connect_Com                 43      /* control type: command, callback function: Connect_Com */
#define  Main_Quit                        44      /* control type: command, callback function: Quit */
#define  Main_Show_Monitor                45      /* control type: command, callback function: Show_Monitor */
#define  Main_DECORATION_7                46      /* control type: deco, callback function: (none) */
#define  Main_DECORATION                  47      /* control type: deco, callback function: (none) */
#define  Main_Reset_b                     48      /* control type: command, callback function: Reset_b */
#define  Main_Connect_USB                 49      /* control type: command, callback function: Connect_USB */
#define  Main_DECORATION_8                50      /* control type: deco, callback function: (none) */
#define  Main_DECORATION_2                51      /* control type: deco, callback function: (none) */
#define  Main_WorkMode                    52      /* control type: tab, callback function: (none) */
#define  Main_DAC_Value_Threshold         53      /* control type: numeric, callback function: (none) */
#define  Main_Set_Delay_OR64              54      /* control type: numeric, callback function: Set_Delay_OR64 */
#define  Main_Select_Cf                   55      /* control type: numeric, callback function: Select_Cf */
#define  Main_SS1_SS10_PA                 56      /* control type: numeric, callback function: SS1_SS10_PA_Callback */
#define  Main_Set_Analog_out_Backup       57      /* control type: numeric, callback function: Set_Analog_out_Backup */
#define  Main_Thre_Fsb_Cmd                58      /* control type: numeric, callback function: Thre_Fsb_Cmd_Callback */
#define  Main_DAC_Value                   59      /* control type: numeric, callback function: (none) */
#define  Main_Delay_Value                 60      /* control type: numeric, callback function: (none) */
#define  Main_TA_ConfigFile               61      /* control type: string, callback function: (none) */
#define  Main_Save_Path                   62      /* control type: string, callback function: (none) */
#define  Main_FEE16                       63      /* control type: radioButton, callback function: LoadFEE */
#define  Main_FEE15                       64      /* control type: radioButton, callback function: LoadFEE */
#define  Main_FEE14                       65      /* control type: radioButton, callback function: LoadFEE */
#define  Main_FEE13                       66      /* control type: radioButton, callback function: LoadFEE */
#define  Main_FEE12                       67      /* control type: radioButton, callback function: LoadFEE */
#define  Main_FEE11                       68      /* control type: radioButton, callback function: LoadFEE */
#define  Main_FEE10                       69      /* control type: radioButton, callback function: LoadFEE */
#define  Main_FEE9                        70      /* control type: radioButton, callback function: LoadFEE */
#define  Main_FEE8                        71      /* control type: radioButton, callback function: LoadFEE */
#define  Main_FEE7                        72      /* control type: radioButton, callback function: LoadFEE */
#define  Main_FEE6                        73      /* control type: radioButton, callback function: LoadFEE */
#define  Main_FEE5                        74      /* control type: radioButton, callback function: LoadFEE */
#define  Main_FEE4                        75      /* control type: radioButton, callback function: LoadFEE */
#define  Main_FEE3                        76      /* control type: radioButton, callback function: LoadFEE */
#define  Main_FEE2                        77      /* control type: radioButton, callback function: LoadFEE */
#define  Main_FEE1                        78      /* control type: radioButton, callback function: LoadFEE */
#define  Main_TrigChoose                  79      /* control type: binary, callback function: (none) */
#define  Main_Cmd                         80      /* control type: tab, callback function: (none) */
#define  Main_HitEnable                   81      /* control type: binary, callback function: (none) */
#define  Main_TEXTMSG_3                   82      /* control type: textMsg, callback function: (none) */
#define  Main_TEXTMSG_2                   83      /* control type: textMsg, callback function: (none) */
#define  Main_DECORATION_13               84      /* control type: deco, callback function: (none) */
#define  Main_DECORATION_14               85      /* control type: deco, callback function: (none) */
#define  Main_DECORATION_12               86      /* control type: deco, callback function: (none) */
#define  Main_DECORATION_4                87      /* control type: deco, callback function: (none) */
#define  Main_DECORATION_9                88      /* control type: deco, callback function: (none) */
#define  Main_DECORATION_10               89      /* control type: deco, callback function: (none) */
#define  Main_Cosmic_Mode                 90      /* control type: binary, callback function: Cosmic_Mode */
#define  Main_Select_ADC_Ext              91      /* control type: binary, callback function: Select_ADC_Ext */
#define  Main_Select_SCAorBackup          92      /* control type: binary, callback function: Select_SCAorBackup */
#define  Main_Auto_TA_Test                93      /* control type: binary, callback function: Auto_TA_Test */
#define  Main_Start_Acq_Test              94      /* control type: binary, callback function: Start_Acq_Test */
#define  Main_Select_TDC_On               95      /* control type: binary, callback function: Select_TDC_On */
#define  Main_Only_ExTrig                 96      /* control type: binary, callback function: Only_ExTrig */
#define  Main_Switch_Val                  97      /* control type: binary, callback function: Switch_Val */
#define  Main_Switch_Raz                  98      /* control type: binary, callback function: Switch_Raz */
#define  Main_Switch_Leakage              99      /* control type: binary, callback function: Switch_Leakage */
#define  Main_Switch_Mode                 100     /* control type: binary, callback function: Switch_Mode */
#define  Main_Switch_SC_Prob              101     /* control type: binary, callback function: Switch_SC_Prob */
#define  Main_TEXTMSG                     102     /* control type: textMsg, callback function: (none) */
#define  Main_TEXTMSG_4                   103     /* control type: textMsg, callback function: (none) */
#define  Main_TEXTMSG_5                   104     /* control type: textMsg, callback function: (none) */
#define  Main_DAC_DATA                    105     /* control type: textMsg, callback function: (none) */
#define  Main_DECORATION_11               106     /* control type: deco, callback function: (none) */
#define  Main_DECORATION_3                107     /* control type: deco, callback function: (none) */

     /* tab page panel controls */
#define  Calitest_Chn_Mode                2       /* control type: ring, callback function: Chn_Mode */
#define  Calitest_ChnID_End               3       /* control type: ring, callback function: (none) */
#define  Calitest_Calitest_ChnID          4       /* control type: ring, callback function: (none) */
#define  Calitest_Cali_Mode               5       /* control type: binary, callback function: Cali_Mode */
#define  Calitest_S_Volt                  6       /* control type: numeric, callback function: (none) */
#define  Calitest_E_Volt                  7       /* control type: numeric, callback function: (none) */
#define  Calitest_DECORATION              8       /* control type: deco, callback function: (none) */
#define  Calitest_Stop_Cali_Acq           9       /* control type: command, callback function: Stop_Cali_Acq */
#define  Calitest_Start_Cali_Acq          10      /* control type: command, callback function: Start_Cali_Acq */
#define  Calitest_Step                    11      /* control type: numeric, callback function: (none) */
#define  Calitest_Volt                    12      /* control type: numeric, callback function: (none) */

     /* tab page panel controls */
#define  COM_Cmd_DECORATION               2       /* control type: deco, callback function: (none) */
#define  COM_Cmd_Manual_Com_Cmd           3       /* control type: command, callback function: Manual_COM_Cmd */
#define  COM_Cmd_Input_COM_FEE            4       /* control type: ring, callback function: (none) */
#define  COM_Cmd_Manual_COM_Type          5       /* control type: numeric, callback function: (none) */
#define  COM_Cmd_Manual_COM_Low_Plus      6       /* control type: numeric, callback function: (none) */
#define  COM_Cmd_Manual_COM_High_Plus     7       /* control type: numeric, callback function: (none) */
#define  COM_Cmd_Manual_COM_High          8       /* control type: numeric, callback function: (none) */
#define  COM_Cmd_Manual_COM_Low           9       /* control type: numeric, callback function: (none) */
#define  COM_Cmd_CMD_Sel                  10      /* control type: binary, callback function: CMD_Sel_A_B */

     /* tab page panel controls */
#define  Normal_Threshold_ChnID           2       /* control type: ring, callback function: (none) */
#define  Normal_Data_Mode                 3       /* control type: ring, callback function: Data_Mode */
#define  Normal_Low_Threshold             4       /* control type: numeric, callback function: (none) */
#define  Normal_High_Threshold            5       /* control type: numeric, callback function: (none) */
#define  Normal_DECORATION                6       /* control type: deco, callback function: (none) */
#define  Normal_Stop_Norm_Acq             7       /* control type: command, callback function: Stop_Norm_Acq */
#define  Normal_Start_Norm_Acq            8       /* control type: command, callback function: Start_Norm_Acq */
#define  Normal_DataChnChoose             9       /* control type: binary, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Auto_TA_Test(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Cali_Mode(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Chn_Mode(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Clear_Textbox(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CMD_Sel_A_B(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Config_SC_para(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Connect_Com(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Connect_USB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Cosmic_Mode(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK DAQ_Trig_Mode(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Data_Mode(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Display_Hv(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Display_Mask(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Help(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Hit_Test_Func(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK LoadFEE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MainFunc(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Manual_COM_Cmd(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Manual_USB_Cmd(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK More_USB_Cmd(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MoreHitMode(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Only_ExTrig(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Open_ConfigFile(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Open_SaveFile(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Reset_b(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Select_ADC_Ext(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Select_Cf(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Select_SCAorBackup(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Select_TDC_On(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Send_Trigger(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Set_Analog_out_Backup(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Set_DAC(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Set_Delay(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Set_Delay_OR64(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Set_SS1(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Set_Thre_DAC(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Set_Thre_Fsb(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetTA(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetTA_THR(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Show_Monitor(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SingalSweep(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SS1_SS10_PA_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Start_Acq_Test(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Start_Cali_Acq(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Start_Monitor(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Start_Norm_Acq(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Stop_Cali_Acq(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Stop_Monitor(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Stop_Norm_Acq(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Switch_Leakage(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Switch_Mode(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Switch_Raz(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Switch_SC_Prob(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Switch_Val(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Sys_Reset(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TA_Test_Func(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Thre_Fsb_Cmd_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Write_NET(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
