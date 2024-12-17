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

#define  Duty_Mode                        1       /* callback function: Close_Duty_Mode */
#define  Duty_Mode_TEXTMSG                2       /* control type: textMsg, callback function: (none) */
#define  Duty_Mode_Remind_Timer           3       /* control type: timer, callback function: Remind_Timer */
#define  Duty_Mode_TEXTMSG_2              4       /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Close_Duty_Mode(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Remind_Timer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
