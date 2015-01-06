/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GUI_TOUCH_X.C
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/

#include "..\APP\includes.h"
//#include "..\GUIinc\GUI.h"
//#include "..\GUIinc\GUI_X.h"
//#include "..\TFT\ili932x.h"
//#include "bsp.h"


unsigned short int X,Y;

// Correction parameters
MATRIX  matrix=
{0x0000F900, 0xFFFFFDC0, 0xFFE714B0, 0x00000280, 0x00017700, 0xFFC219C0, 0x0002887A };

void GUI_TOUCH_X_ActivateX(void) {
}

void GUI_TOUCH_X_ActivateY(void) {
}


//int  GUI_TOUCH_X_MeasureX(void) 
//{
//	unsigned short X=0;	
// 	
//	X=Read_XY(CHY);
//	return(X);  
//}
//
//int  GUI_TOUCH_X_MeasureY(void) {
//  	unsigned short Y=0;	
// 
//    Y=Read_XY(CHX);
//	return(Y); 
//}

int  GUI_TOUCH_X_MeasureX(void) 
{
	unsigned short X=0;	
 	
	X=Read_XY(CHX);
	return(X);  
}

int  GUI_TOUCH_X_MeasureY(void) {
  	unsigned short Y=0;	
 
    Y=Read_XY(CHY);
	return(Y); 
}

//int  GUI_TOUCH_X_MeasureX(void) 
//{
// unsigned short X=0;
// POINT   TouchSample;
// POINT   display;
// uint16 xt,yt;
// uint8 flag;
//	   
//	   flag=TP_GetAdXY2(&xt, &yt, 0);
//	   if(flag)
//	   {
//	        TouchSample.x=xt/4; TouchSample.y=yt/4;
//          getDisplayPoint( &display, &TouchSample, &matrix );
//	   }
// 	   //X=display.x;
//	   X=320-display.y;
//	   //X=Read_XY(CHY);
//	   return(X);  
//}
//
//int  GUI_TOUCH_X_MeasureY(void) 
//{
// unsigned short Y=0;
// POINT   TouchSample;
// POINT   display;
// uint16 xt,yt;
// uint8 flag;
//	   
//	   flag=TP_GetAdXY2(&xt, &yt, 0);
//	   if(flag)
//	   {
//	        TouchSample.x=xt/4; TouchSample.y=yt/4;
//          getDisplayPoint( &display, &TouchSample, &matrix );
//	   }
// 	   //Y=display.y;
//	   Y=display.x;	
//     //Y=Read_XY(CHX);
//	   return(Y); 
//}

//end
