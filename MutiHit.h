/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2014. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  MutiHit                          1
#define  MutiHit_NY4_B8                   2       /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_NY4_T8                   3       /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_NY4_B5                   4       /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_NY4_T5                   5       /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_NY1_B8                   6       /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_NY1_T8                   7       /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_NY1_B5                   8       /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_NY1_T5                   9       /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_PY4_B8                   10      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_PY4_T8                   11      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_PY4_B5                   12      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_PY4_T5                   13      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_PY1_B8                   14      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_PY1_T8                   15      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_PY1_B5                   16      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_PY1_T5                   17      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_NX4_B8                   18      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_NX4_T8                   19      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_NX4_B5                   20      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_NX4_T5                   21      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_NX1_B8                   22      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_NX1_T8                   23      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_NX1_T5                   24      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_NX1_B5                   25      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_PX4_T5                   26      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_PX4_B5                   27      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_PX4_T8                   28      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_PX4_B8                   29      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_PX1_T5                   30      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_PX1_T8                   31      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_PX1_B8                   32      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_PX1_B5                   33      /* control type: radioButton, callback function: LoadHit */
#define  MutiHit_TEXTMSG                  34      /* control type: textMsg, callback function: (none) */
#define  MutiHit_TEXTMSG_2                35      /* control type: textMsg, callback function: (none) */
#define  MutiHit_TEXTMSG_3                36      /* control type: textMsg, callback function: (none) */
#define  MutiHit_TEXTMSG_4                37      /* control type: textMsg, callback function: (none) */
#define  MutiHit_TEXTMSG_5                38      /* control type: textMsg, callback function: (none) */
#define  MutiHit_TEXTMSG_6                39      /* control type: textMsg, callback function: (none) */
#define  MutiHit_TEXTMSG_7                40      /* control type: textMsg, callback function: (none) */
#define  MutiHit_TEXTMSG_8                41      /* control type: textMsg, callback function: (none) */
#define  MutiHit_TEXTMSG_9                42      /* control type: textMsg, callback function: (none) */
#define  MutiHit_Quit_MutiHit             43      /* control type: command, callback function: Quit_MutiHit */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK LoadHit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Quit_MutiHit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
