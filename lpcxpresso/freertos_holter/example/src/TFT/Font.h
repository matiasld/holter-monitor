#ifndef _FONT_H_
#define _FONT_H_

#define ROTACION0         0
#define ROTACION90        1
#define ROTACION180       2
#define ROTACION270       3
 
#define MODONORMAL        0
#define MODOTRANSPARENTE  1

#define ALINE_LEFT		0
#define ALINE_CENTER	1
#define ALINE_RIGHT		2
#define ALINE_MARK		(ALINE_LEFT | ALINE_CENTER | ALINE_RIGHT)


void MostrarString(unsigned int x, unsigned int y, unsigned char * src, unsigned short color, unsigned char rotacion);
void DibujarLetra(unsigned int *x, unsigned int *y, unsigned char nn, unsigned short color, unsigned short rotacion);

#endif
