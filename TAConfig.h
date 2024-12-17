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

#define  TAConfig                         1       /* callback function: CloseTAConfig */
#define  TAConfig_TA_Disable              2       /* control type: numeric, callback function: (none) */
#define  TAConfig_TA_DAC                  3       /* control type: numeric, callback function: (none) */
#define  TAConfig_Low_Thr                 4       /* control type: numeric, callback function: (none) */
#define  TAConfig_High_Thr                5       /* control type: numeric, callback function: (none) */
#define  TAConfig_TA_TestMast             6       /* control type: numeric, callback function: (none) */
#define  TAConfig_SetLowThr               7       /* control type: command, callback function: Set_LowThr */
#define  TAConfig_TA_ChipMode             8       /* control type: numeric, callback function: (none) */
#define  TAConfig_SetHighThr              9       /* control type: command, callback function: Set_HighThr */
#define  TAConfig_SetTAConfig             10      /* control type: command, callback function: SetTAConfig */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK CloseTAConfig(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Set_HighThr(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Set_LowThr(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetTAConfig(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
