#include "..\APP\includes.h"


/****************************************************************************
* Name: GUI_CmpColor ()
* Function: to determine the color values are the same.
* Entrance parameters: color1 a color value
* Color2 color value 2
* Export parameters: return 1 represent the same, not the same returns 0.
* Description:
****************************************************************************/
#define  GUI_CmpColor(color1, color2)	( (color1&0x01) == (color2&0x01) )


/****************************************************************************
* Name: u16 GUI_Color565 (u32 RGB)
* Function: RGB color conversion to 16-bit color.
* Entrance parameters: RGB color values
* Export Parameters: Returns 16 RGB color values.
* Description:
* Call the method: i = GUI_Color565 (0xafafaf);
****************************************************************************/
u16 GUI_Color565(u32 RGB)
{
  u8  r, g, b;

  b = ( RGB >> (0+3) ) & 0x1f;		// Take B-colored high five
  g = ( RGB >> (8+2) ) & 0x3f;		// Take G-color high 6
  r = ( RGB >> (16+3)) & 0x1f;		// Take R-colored high five
   
  return( (r<<11) + (g<<5) + (b<<0) );		
}

/****************************************************************************
* Name: void GUI_Text (u16 x, u16 y, u8 * str, u16 len, u16 Color, u16 bkColor)
* Function: displays the string at the specified coordinates
* Entrance parameters: x coordinate line
* Y coordinate column
* * Str string
* Len string length
* Color Character Color
* BkColor character background color
* Export parameters: None
* Description:
* Call the method: GUI_Text (0,0, "0123456789", 10,0x0000,0xffff);
****************************************************************************/
void GUI_Text(u16 x, u16 y, u8 *str, u16 len,u16 Color, u16 bkColor)
{
  u8 i;
  
  for (i=0;i<len;i++)
  {
    LCDWriteEN16((x+8*i),y,*str++,Color,bkColor);
  }
}

/****************************************************************************
* Name: void GUI_Line (u16 x0, u16 y0, u16 x1, u16 y1, u16 color)
* Function: draw a straight line at the specified coordinates
* Entrance parameters: x0 A point line coordinates
* Y0 A column coordinates of the point
* X1 B point line coordinates
* Y1 B point coordinates column
* Color line color
* Export parameters: None
* Description:
* Call the method: GUI_Line (0,0,240,320,0x0000);
****************************************************************************/
void GUI_Line(u16 x0, u16 y0, u16 x1, u16 y1,u16 color)
{
 	u16 x,y;
 	u16 dx;// = abs(x1 - x0);
 	u16 dy;// = abs(y1 - y0);

	if(y0==y1)
	{
		if(x0<=x1)
		{
			x=x0;
		}
		else
		{
			x=x1;
			x1=x0;
		}
  		while(x <= x1)
  		{
   			DrawPixel(x,y0,color);
   			x++;
  		}
  		return;
	}
	else if(y0>y1)
	{
		dy=y0-y1;
	}
	else
	{
		dy=y1-y0;
	}
 
 	if(x0==x1)
	{
		if(y0<=y1)
		{
			y=y0;
		}
		else
		{
			y=y1;
			y1=y0;
		}
  		while(y <= y1)
  		{
   			DrawPixel(x0,y,color);
   			y++;
  		}
  		return;
	}
	else if(x0 > x1)
 	{
		dx=x0-x1;
  		x = x1;
  		x1 = x0;
  		y = y1;
  		y1 = y0;
 	}
 	else
 	{
		dx=x1-x0;
  		x = x0;
  		y = y0;
 	}

 	if(dx == dy)
 	{
  		while(x <= x1)
  		{

   			x++;
			if(y>y1)
			{
				y--;
			}
			else
			{
   				y++;
			}
   			DrawPixel(x,y,color);
  		}
 	}
 	else
 	{
 		DrawPixel(x, y, color);
  		if(y < y1)
  		{
   			if(dx > dy)
   			{
    			s16 p = dy * 2 - dx;
    			s16 twoDy = 2 * dy;
    			s16 twoDyMinusDx = 2 * (dy - dx);
    			while(x < x1)
    			{
     				x++;
     				if(p < 0)
     				{
      					p += twoDy;
     				}
     				else
     				{
      					y++;
      					p += twoDyMinusDx;
     				}
     				DrawPixel(x, y,color);
    			}
   			}
   			else
   			{
    			s16 p = dx * 2 - dy;
    			s16 twoDx = 2 * dx;
    			s16 twoDxMinusDy = 2 * (dx - dy);
    			while(y < y1)
    			{
     				y++;
     				if(p < 0)
     				{
      					p += twoDx;
     				}
     				else
     				{
      					x++;
      					p+= twoDxMinusDy;
     				}
     				DrawPixel(x, y, color);
    			}
   			}
  		}
  		else
  		{
   			if(dx > dy)
   			{
    			s16 p = dy * 2 - dx;
    			s16 twoDy = 2 * dy;
	    		s16 twoDyMinusDx = 2 * (dy - dx);
    			while(x < x1)
    			{
     				x++;
     				if(p < 0)
	     			{
    	  				p += twoDy;
     				}
     				else
     				{
      					y--;
	      				p += twoDyMinusDx;
    	 			}
     				DrawPixel(x, y,color);
    			}
   			}
	   		else
   			{
    			s16 p = dx * 2 - dy;
    			s16 twoDx = 2 * dx;
	    		s16 twoDxMinusDy = 2 * (dx - dy);
    			while(y1 < y)
    			{
     				y--;
     				if(p < 0)
	     			{
    	  				p += twoDx;
     				}
     				else
     				{
      					x++;
	      				p+= twoDxMinusDy;
    	 			}
     				DrawPixel(x, y,color);
    			}
   			}
  		}
 	}
}

/****************************************************************************
* Name: void GUI_Circle (u16 cx, u16 cy, u16 r, u16 color, u8 fill)
* Function: Circle in the specified coordinates, can be filled
* Entrance parameters:
* Export parameters:
* Description:
* Call the method:
****************************************************************************/
void GUI_Circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill)
{
	u16 x,y;
	s16 delta,tmp;
	x=0;
	y=r;
	delta=3-(r<<1);

	while(y>x)
	{
		if(fill)
		{
			GUI_Line(cx+x,cy+y,cx-x,cy+y,color);
			GUI_Line(cx+x,cy-y,cx-x,cy-y,color);
			GUI_Line(cx+y,cy+x,cx-y,cy+x,color);
			GUI_Line(cx+y,cy-x,cx-y,cy-x,color);
		}
		else
		{
			DrawPixel(cx+x,cy+y,color);
			DrawPixel(cx-x,cy+y,color);
			DrawPixel(cx+x,cy-y,color);
			DrawPixel(cx-x,cy-y,color);
			DrawPixel(cx+y,cy+x,color);
			DrawPixel(cx-y,cy+x,color);
			DrawPixel(cx+y,cy-x,color);
			DrawPixel(cx-y,cy-x,color);
		}
		x++;
		if(delta>=0)
		{
			y--;
			tmp=(x<<2);
			tmp-=(y<<2);
			delta+=(tmp+10);
		}
		else
		{
			delta+=((x<<2)+6);
		}
	}
}

/****************************************************************************
* Name: void GUI_Rectangle (u16 x0, u16 y0, u16 x1, u16 y1, u16 color, u8 fill)
* Function: draw a rectangle in the specified area can be filled with color
* Entrance parameters:
* Export parameters:
* Description:
* Call the method:
****************************************************************************/
void GUI_Rectangle(u16 x0, u16 y0, u16 x1, u16 y1,u16 color,u8 fill)
{
	if(fill)
	{
		u16 i;
		if(x0>x1)
		{
			i=x1;
			x1=x0;
		}
		else
		{
			i=x0;
		}
		for(;i<=x1;i++)
		{
			GUI_Line(i,y0,i,y1,color);
		}
		return;
	}
	GUI_Line(x0,y0,x0,y1,color);
	GUI_Line(x0,y1,x1,y1,color);
	GUI_Line(x1,y1,x1,y0,color);
	GUI_Line(x1,y0,x0,y0,color);
}

/****************************************************************************
* Name: void GUI_Square (u16 x0, u16 y0, u16 with, u16 color, u8 fill)
* Function: draw a square in the specified area can be filled with color
* Entrance parameters:
* Export parameters:
* Description:
* Call the method:
****************************************************************************/
void  GUI_Square(u16 x0, u16 y0, u16 with, u16 color,u8 fill)
{
	GUI_Rectangle(x0, y0, x0+with, y0+with, color,fill);
}
