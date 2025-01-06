/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2012. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

/* #include <userint.h> */

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  MutiUSB                          1       /* callback function: CloseMutiUSB */
#define  MutiUSB_USB_Cmd6                 2       /* control type: numeric, callback function: (none) */
#define  MutiUSB_USB_Cmd5                 3       /* control type: numeric, callback function: (none) */
#define  MutiUSB_wtUSB6                   4       /* control type: command, callback function: wtUSB6 */
#define  MutiUSB_USB_Cmd4                 5       /* control type: numeric, callback function: (none) */
#define  MutiUSB_wtUSB5                   6       /* control type: command, callback function: wtUSB5 */
#define  MutiUSB_USB_Cmd3                 7       /* control type: numeric, callback function: (none) */
#define  MutiUSB_wtUSB4                   8       /* control type: command, callback function: wtUSB4 */
#define  MutiUSB_USB_Cmd7                 9       /* control type: numeric, callback function: (none) */
#define  MutiUSB_wtUSB3                   10      /* control type: command, callback function: wtUSB3 */
#define  MutiUSB_USB_Cmd8                 11      /* control type: numeric, callback function: (none) */
#define  MutiUSB_wtUSB7                   12      /* control type: command, callback function: wtUSB7 */
#define  MutiUSB_USB_Cmd9                 13      /* control type: numeric, callback function: (none) */
#define  MutiUSB_wtUSB8                   14      /* control type: command, callback function: wtUSB8 */
#define  MutiUSB_USB_Cmd10                15      /* control type: numeric, callback function: (none) */
#define  MutiUSB_wtUSB9                   16      /* control type: command, callback function: wtUSB9 */
#define  MutiUSB_USB_Cmd2                 17      /* control type: numeric, callback function: (none) */
#define  MutiUSB_wtUSB10                  18      /* control type: command, callback function: wtUSB10 */
#define  MutiUSB_USB_Cmd1                 19      /* control type: numeric, callback function: (none) */
#define  MutiUSB_wtUSB2                   20      /* control type: command, callback function: wtUSB2 */
#define  MutiUSB_wtUSB1                   21      /* control type: command, callback function: wtUSB1 */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK CloseMutiUSB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK wtUSB1(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK wtUSB10(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK wtUSB2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK wtUSB3(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK wtUSB4(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK wtUSB5(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK wtUSB6(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK wtUSB7(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK wtUSB8(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK wtUSB9(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
