#include "Graphics.h"
#include "font.h"

////////////////////////////////////////////////////////////////////
void MostrarString(unsigned int x, unsigned int y, unsigned char * src, unsigned short color, unsigned char rotacion)
{
	while(*src){
		DibujarLetra(&x, &y, *src, color, rotacion);
		src++;
	}
}

////////////////////////////////////////////////////////////////////
void DibujarLetra(unsigned int *x, unsigned int *y, unsigned char nn, unsigned short color, unsigned short rotacion)
{
	unsigned char *punteroFont;
    unsigned short aux1,aux2;
    unsigned short punterochartable;
    unsigned short alturafont,anchofont;
    unsigned short punterobitmapfont;
    unsigned short tamanioletra,punteropixel;
    unsigned int x1,y1;
    unsigned short mascara,byteleido,pixel,piccounter;

    //el primer char debe ser siempre 32..
    nn=(unsigned char)(nn-32);

    //la zona de memoria de cada fuente empieza en 0
    GetFontCalibri(&punteroFont);
    aux1 = *punteroFont;
    if (aux1!=0) return;
    
    punterochartable=(unsigned short)(nn*4+8);        
    
    aux1=*(punteroFont+punterochartable+2);
    
    aux2=*(punteroFont+punterochartable+1);
    
    punterobitmapfont=(unsigned short)(aux1*256+aux2);
    
    alturafont=*(punteroFont+6);
    
    anchofont=*(punteroFont+punterochartable);
    
    tamanioletra=(unsigned short)(alturafont*anchofont);
 
    x1=*x;
    
    y1=*y;
        
    piccounter=0;	
    
    mascara=0x01;
    
    for (punteropixel=0;punteropixel<tamanioletra;punteropixel++)
    {        
        if (mascara==0x01)
        {
             byteleido=*(punteroFont+punterobitmapfont);
             punterobitmapfont++;
        }
        
        pixel=(unsigned char)(byteleido & mascara);

		if (pixel){
			LCD_PutPixel(x1,y1,color);
		}
        
        if (mascara==0x80)
        	mascara=0x01;
        else
        	mascara=(unsigned char)(mascara*2);

        piccounter++;
        
        if (rotacion==ROTACION0){
                x1++;
                if (piccounter==anchofont){   
                    piccounter=0;
                    mascara=0x01;
                    x1=*x;
                    y1++;
                }
        }
        if (rotacion==ROTACION90){
                y1++;
                if (piccounter==anchofont){   
                    piccounter=0;
                    mascara=0x01;
                    y1=*y;
                    x1--;
                }
        }
        if (rotacion==ROTACION180){
                x1--;
                if (piccounter==anchofont){   
                    piccounter=0;
                    mascara=0x01;
                    x1=*x;
                    y1--;
                }
        }
        if (rotacion==ROTACION270){
                y1--;
                if (piccounter==anchofont){   
                    piccounter=0;
                    mascara=0x01;
                    y1=*y;
                    x1++;
                }
        }
    }

    
    if (rotacion==ROTACION0)   *x=(unsigned int)(*x+anchofont);
    if (rotacion==ROTACION90)  *y=(unsigned int)(*y+anchofont);
    if (rotacion==ROTACION180) *x=(unsigned int)(*x-anchofont);
    if (rotacion==ROTACION270) *y=(unsigned int)(*y-anchofont);
}
