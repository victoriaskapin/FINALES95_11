#include "img.h"
#include "tipografia8x8.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

struct imagen{
    rgba_t **px; 
    size_t alto; 
    size_t ancho; 
};

img_t* img_crear(size_t ancho,size_t alto){
    img_t *m = malloc (sizeof(img_t)); 
    if(m == NULL)
        return NULL;
    m->alto = alto; 
    m->ancho = ancho; 
    m->px = malloc(sizeof(rgba_t*)*alto);
    for(int i = 0; i < alto;i++) {
        m->px[i] = malloc(sizeof(rgba_t)*ancho); 
    }
    return m;
}

void img_destruir(img_t *m){
    if(m == NULL)
        return;
    if (m->px != NULL){
        for(int i = 0; i < m->alto ; i++){
            if(m->px[i]!= NULL)
                free(m->px[i]); 
        }
        free(m->px);
        }
    free (m); 
}
size_t img_ancho(img_t *m){
    return m->ancho;
}
size_t img_alto(img_t *m){
    return m->alto; 
} 

void img_set_pixel(img_t *m,int x,int y,rgba_t color){
    if((m == NULL) || (m->px == NULL)){
        return; 
    }
    if(x >= m->ancho)
        return; 
    if(y >= m->alto)
        return; 
    (m->px[y][x]) = color; 
}
rgba_t img_get_pixel(img_t *m,int x,int y){
    if(m == NULL || x >= m->ancho || y >= m->alto || m->px == NULL)
        return 0x000000FF; 
    rgba_t a = m->px[y][x]; 
    return a; 
}
img_t* img_clonar(img_t *m){
    img_t *clon = img_crear(m->ancho,m->alto);
    if (clon == NULL || m == NULL)
        return NULL; 
    for(int i = 0; i < m->alto; i++){
        for(int j = 0; j < m->ancho; j++ ){
            clon->px[i][j] = m->px[i][j];
        }
    }
    return clon; 
} 

// EJERCICIO 2 
bool img_escribir(img_t *m,FILE* fp){
    if(fp == NULL || m == NULL)
        return false; 
    if(fwrite(m,sizeof(img_t),1,fp) == 0)
        return false;
    return true; 
}

img_t* img_leer(FILE* fp){
    if(fp == NULL)
        return NULL; 
    img_t aux; 
    fread(&aux,sizeof(img_t),1,fp);   
    img_t *m = img_clonar(&aux);
    return m; 
}

// EJERCICIO 3 
size_t img_tamanio(img_t *m){
    return (m->ancho)*(m->alto); 
}

void img_ordenar_por_tamagno(img_t* m[],size_t n){
    size_t min,i,j; 
    for( i=0 ; i < n-1 ; i++ ){
        min = i; 
        for( j = i + 1 ; j < n ; j++ ){
            if(img_tamanio(m[j]) < img_tamanio(m[min]))
                min = j; 
        }
        img_t *temp = m[min]; // lo ordena
		m[min] = m[i];
		m[i] = temp;
    }
}

//EJERCIO 4 
void img_pintar(img_t *m,rgba_t color){
    int i,j; 
    for (i = 0; i < m->alto ; i++){
        for(j = 0; j < m->ancho; j++){
            m->px[i][j]= color; 
        }
    }
}

void img_sellar(img_t *fondo,img_t *sello, int x,int y){
    int i = 0; int j; 
        
    for(i = 0; ((i+y) < (int) fondo->alto) && (i < sello->alto); i++ ){

        for(j = 0; ((x+j) < (int) fondo->ancho) && (j < sello->ancho); j++){
            if((x+j)>=0 && (i+y)>=0 ){
                if(rgba_a(sello->px[i][j])!= 0x00){
                 fondo->px[y+i][x+j]=sello->px[i][j]; 
                }
            }
        }
    }
}

//EJERCICIO 5 
/*
mg_crear_texto : Recibe una cadena, un color de fondo y un color de texto. Devuelve una
imagen nueva con el texto ”pintado”. Se asume que la cadena no contiene caracteres \n .
Cada caracter de la cadena ocupará 8x8 pixels; es decir que la imagen tendrá (8 * cantidad
de caracteres) pixels de ancho y 8 pixels de alto.
La información para pintar cada caracter está en tipografia8x8 . Es un diccionario en el
que la clave es un caracter ASCII y el valor son los 64 bits (agrupados en filas de 8) que
representan al caracter ”pintado”.
Cada uno de los pixels tomará el color de fondo o de texto según si el bit correspondiente
es 0 o 1, respectivamente. Cada número de 8 bits representa una fila (el primer número
es la fila de arriba), y el bit menos significativo es el pixel de la izquierda.*/

bool es_neg(componente_t color,int pos){//funcion que devuelve true si el bit de la 
                                    //poscicion indicada es 0
    componente_t aux = color; 
    aux = aux <<(7-pos); 
    aux = aux >> 7; 
    return ((aux & 0xFF) == 1);
}
img_t* img_crear_texto(char *c,rgba_t fondo,rgba_t texto){
    img_t *m = img_crear ((strlen(c)*8),8);
    if(m == NULL)
        return NULL; 
    img_pintar(m,fondo);

    int i,n,aux; 
    aux = 0; 

    for (n = 0; n < strlen(c) ; n++){
        for (i = 0; i<8 ; i++){
            for (int j = 0; j < 8; j++){
                if(es_neg(tipografia8x8 [((int)c[n])][i],j))
                    m->px[i][j+aux] = texto;
            }             
        }
        aux +=8; 
    }
    return m; 
}