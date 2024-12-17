/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2015. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL_Sig                        1       /* callback function: Close_SignalSweep */
#define  PANEL_Sig_InitVolt               2       /* control type: numeric, callback function: (none) */
#define  PANEL_Sig_FinalVolt              3       /* control type: numeric, callback function: (none) */
#define  PANEL_Sig_Step                   4       /* control type: numeric, callback function: (none) */
#define  PANEL_Sig_SignalSweepStop        5       /* control type: command, callback function: SignalSweepStop */
#define  PANEL_Sig_SignalSweepStart       6       /* control type: command, callback function: SignalSweepStart */
#define  PANEL_Sig_ChnID                  7       /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Close_SignalSweep(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SignalSweepStart(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SignalSweepStop(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
