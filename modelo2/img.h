#ifndef IMG_H
#define IMG_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "rgba.h"

typedef struct imagen img_t; 

img_t* img_crear(size_t ancho,size_t alto); 
void img_destruir(img_t *m);
size_t img_ancho(img_t *m);
size_t img_alto(img_t *m); 
void img_set_pixel(img_t *m,int x,int y,rgba_t color); 
rgba_t img_get_pixel(img_t *m,int x,int y); 
img_t* img_clonar(img_t *m); 

//EJERCICIO 2 
bool img_escribir(img_t *m,FILE* fp);
img_t* img_leer(FILE* fp);

//EJERCICIO 3 
void img_ordenar_por_tamagno(img_t* m[],size_t n);

//EJERCICIO 4
void img_pintar(img_t *m,rgba_t color);
void img_sellar(img_t *fondo,img_t *sello, int x,int y);

//EJERCICIO 5 

img_t* img_crear_texto(char *c,rgba_t fondo,rgba_t texto); 
#endif
