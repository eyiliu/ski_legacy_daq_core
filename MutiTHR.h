/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2016. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  MutiTHR                          1       /* callback function: CloseMutiTHR */
#define  MutiTHR_Set_B8                   2       /* control type: command, callback function: Set_B8 */
#define  MutiTHR_Set_T8                   3       /* control type: command, callback function: Set_T8 */
#define  MutiTHR_Set_B5                   4       /* control type: command, callback function: Set_B5 */
#define  MutiTHR_Set_T5                   5       /* control type: command, callback function: Set_T5 */
#define  MutiTHR_Quit_THR                 6       /* control type: command, callback function: Quit_THR */
#define  MutiTHR_DECORATION               7       /* control type: deco, callback function: (none) */
#define  MutiTHR_B8                       8       /* control type: numeric, callback function: (none) */
#define  MutiTHR_T8                       9       /* control type: numeric, callback function: (none) */
#define  MutiTHR_B5                       10      /* control type: numeric, callback function: (none) */
#define  MutiTHR_T5                       11      /* control type: numeric, callback function: (none) */
#define  MutiTHR_TEXTMSG                  12      /* control type: textMsg, callback function: (none) */
#define  MutiTHR_TEXTMSG_3                13      /* control type: textMsg, callback function: (none) */
#define  MutiTHR_TEXTMSG_2                14      /* control type: textMsg, callback function: (none) */
#define  MutiTHR_TEXTMSG_4                15      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK CloseMutiTHR(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Quit_THR(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Set_B5(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Set_B8(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Set_T5(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Set_T8(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
