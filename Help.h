/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2016. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

/* #include <userint.h> */

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  Help_Panel                       1
#define  Help_Panel_Help_Text3            2       /* control type: textBox, callback function: (none) */
#define  Help_Panel_Help_Text2            3       /* control type: textBox, callback function: (none) */
#define  Help_Panel_Help_Text1            4       /* control type: textBox, callback function: (none) */
#define  Help_Panel_Quit                  5       /* control type: command, callback function: Quit_Help */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Quit_Help(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
