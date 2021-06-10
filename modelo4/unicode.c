#include "unicode.h"
#include <stdio.h>

// EJERCICIO 1
size_t unicode_longitud(const uchar_t *s){
    size_t i=0;  
    while(s[i] != 0)
        i++; 
    return i; 
}

int unicode_comparar(const uchar_t *s1, const uchar_t *s2){ //1 si S1>S2 0 si son iguales -1 si S1<S2
    int sz1,sz2;
    int i = 0;  
    sz1 = unicode_longitud(s1); 
    sz2 = unicode_longitud(s2); 
    if(sz1 == sz2){
        while(s2[i] != 0){
            if(s1[i] > s2[i])
                return 1;
            if(s1[i] < s2[i])
                return -1; 
            i++; 
        }
        return 0;
    }
    else{
         while(s1[i] != 0){
            if(s1[i] > s2[i])
                return 1;
            if(s1[i] < s2[i])
                return -1; 
            i++; 
        }
    }
    if(sz1 > sz2)
        return 1; 
    if(sz1 < sz2)
        return -1; 
    
    return 0; 
}

//EJERCICIO 2 en ambos asimo que dst tiene suficiente espacio
void unicode_concatenar(uchar_t *dst, const uchar_t *src){
    size_t aux = unicode_longitud(dst); 
    int i = 0; 
    while(src[i] != 0){
        dst[aux+i] = src[i]; 
        i++;
    }
}
void unicode_insertar(uchar_t *dst, int desde, const uchar_t *src){
    int lonsrc = unicode_longitud(src);  
    int i = 0; 
    while(src[i] != 0){
        dst[lonsrc+i]=dst[desde+i];
        dst[desde+i] = src[i];
        i++;
    }
}

//EJERCICIO 3

void unicode_decodificar_utf8(uchar_t *dst, const uint8_t *src){
    int i = 0; 
    int j = 0; 
    while(src[i] != 0 ){
        if((src[i] >> 7) == 0){
            dst[j] = src[i];
           // j++;
        } 
        else if((src[i]>> 5) == 6){
            uint8_t b1,b2;
            b1 = 0x1F & src[i]; 
            b2 = 0x3F & src[i+1];
            dst[j] = b1; 
            dst[j] = (dst[j] << 6) | b2; 
            //j++;
            i++; 
        }
        else if((src[i] >> 4) == 0xE){
            uint8_t b1,b2,b3; 
            b1 = 0x0F & src[i];
            b2 = 0x3F & src[i+1];
            b3 = 0x3F & src[i+2];
            dst[j] = b1; 
            dst[j] = (dst[j] << 6) | b2; 
            dst[j] = (dst[j] << 6) | b3;
            i+=2; 
           // j++;
        }
        else{
            uint8_t b1,b2,b3,b4;
            b1 = 0x07 & src[i];
            b2 = 0x3F & src[i+1];
            b3 = 0x3F & src[i+2];
            b4 = 0x3F & src[i+3];
            dst[j] = b1; 
            dst[j] = (dst[j] << 6) | b2; 
            dst[j] = (dst[j] << 6) | b3;
            dst[j] = (dst[j] << 6) | b4;
            i+=3; 
        }
        j++;
        i++;
    }
}


void unicode_codificar_utf8(const uchar_t *src, uint8_t *dst){
    int i = 0;
    int j = 0; 
    while (src[i] != 0){
        if(src[i] <= (int)0x7F){
            dst[j] = src[i];
            j++; 
        }
        else if(src[i] <= (int)0x7FF){
            uint8_t b1 = 0xC0 | (0x1F & (src[i] >> 6));
            dst[j] = b1; 
            uint8_t b2 = 0x80 | (0x3F & src[i]); 
            j++;
            dst[j] = b2; 
            j++;      
        }
        else if( src[i] <= (int)0xFFFF){
            uint8_t b1,b2,b3; 
            b3 = 0x80 | (0x3F & src[i]); 
            b2 = 0x80 | (0x3F & (src[i] >> 6));
            b1 = 0xE0 | (0x0F & (src[i] >> 12));
            dst[j] = b1; 
            j++; 
            dst[j] = b2; 
            j++; 
            dst[j] = b3; 
            j++;
        }
        else {
            uint8_t b1,b2,b3,b4; 
            b4 = 0x80 | (0x3F & src[i]); 
            b3 = 0x80 | (0x3F & (src[i] >> 6));
            b2 = 0x80 | (0x3F & (src[i] >> 12));
            b1 = 0xF0 | (0x08 & (src[i] >> 18));
            dst[j] = b1; 
            j++; 
            dst[j] = b2; 
            j++; 
            dst[j] = b3; 
            j++; 
            dst[j] =b4;
            j++; 
        }
        i++;
    }
}

uchar_t *unicode_leer_archivo_utf8(const char *nombre_archivo){
    
    FILE *fp = fopen(nombre_archivo,"rb"); 
    if(fp == NULL)
        return NULL;
    //Leo el archivo y lo guardo en formato utf 
    uint8_t *utf;
    utf = malloc(sizeof(uint8_t)); 
    if(utf == NULL)
        return NULL;
    //Leo el primero
    fread(utf,sizeof(uint8_t),1,fp); 
    size_t n = 2;
    utf = realloc(utf,sizeof(uint8_t)*n); 
    if (utf == NULL)
        return NULL;
   //Leo los demas
    while(fread(&utf[n-1],sizeof(uint8_t),1,fp)){
        n++; 
        utf = realloc(utf,sizeof(uint8_t)*n);
        if(utf == NULL)
            return NULL;
    }
    //Lo decodifico 
    uchar_t *unicode; 
    unicode = malloc(sizeof(uchar_t)*n); 
    if(unicode == NULL)
        return NULL;

    unicode_decodificar_utf8(unicode,utf);
    free(utf);
    fclose(fp); 
    return unicode; 

}
bool unicode_escribir_archivo_utf8(const uchar_t *s, const char *nombre_archivo){
    FILE *fp = fopen(nombre_archivo,"wb"); 
    if(fp == NULL)
        return false;
    int i = 0; 
    uint8_t *aux = malloc(sizeof(uint8_t)*(unicode_longitud(s)*4));
    if(aux == NULL)
        return false;
    unicode_codificar_utf8(s,aux);
    while(aux[i] != 0){
        fwrite(aux,sizeof(uint8_t),1,fp);
        i++;
    }
    
    free(aux);
    fclose(fp); 
    return true;
}