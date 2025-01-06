/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

/* #include <userint.h> */

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  Hv_Uir                           1       /* callback function: Close_Hv_Uir */
#define  Hv_Uir_Set_Hv_Value              2       /* control type: numeric, callback function: Set_Hv_Value */
#define  Hv_Uir_On_Off_Hv                 3       /* control type: binary, callback function: On_Off_Hv */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Close_Hv_Uir(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Off_Hv(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Set_Hv_Value(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
