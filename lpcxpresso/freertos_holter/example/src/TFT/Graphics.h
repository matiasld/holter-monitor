#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

#include "TFT.h"

extern void Line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned short color);
extern void Rectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int radius, unsigned short color);
extern void RectangleFill(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned short color);
extern void CircleFill(unsigned int x, unsigned int y, unsigned int radius, unsigned char fill, unsigned short color);


#define DrawPixel(x,y)				  LCD_PutPixel(x, y,  color)
#define DrawRect(x1,y1,x2,y2)		LCD_Rect(x1, y1, x2, y2, color)
#define Circle(x, y, radius, color)		CircleFill(x, y, radius, 0, color)

#define BORDER_NONE		0x00
#define BORDER_RECT		0x04
#define BORDER_FILL		0x08
#define BORDER_BEVEL	0xF0	// bevel radius 0x00(rectangle), 0x10-0xF0(radius size)
#define BORDER_MASK		(BORDER_FILL | BORDER_RECT)
#define bevel(x)		(x<<4)

#endif
