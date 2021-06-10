#include "actordb.h"
#include <string.h>
#include <stdio.h>

struct actor{
    char nombre[NOMBRE_MAX];
    fecha_t fnac; 
};

// ejercicio 1
actor_t * actor_crear(const char *nombre, fecha_t nacimiento){
    actor_t *actor = malloc(sizeof(actor_t)); 
    if(actor == NULL)
        return NULL; 
    strcpy(actor->nombre,nombre);
    actor->fnac = nacimiento; 
    return actor; 
}
void actor_destruir(actor_t *actor){
    if(actor != NULL)
        free(actor); 
}
const char *actor_nombre(const actor_t *actor){
    return actor->nombre;
}
fecha_t actor_nacimiento(const actor_t *actor){
    return actor->fnac; 
}

// ejercicio 2
bool actor_escribir(const actor_t *actor, const char *nombre_archivo){
    if(actor == NULL || nombre_archivo == NULL)
        return false; 
    FILE *fp = fopen(nombre_archivo,"wb"); 
    if( fp == NULL)
        return false;
    fwrite(actor,sizeof(actor_t),1,fp);
    fclose(fp); 
    return true;
}
actor_t *actor_leer(const char *nombre_archivo){
    actor_t *actor = malloc(sizeof(actor_t)); 
    if(actor == NULL || nombre_archivo == NULL)
        return NULL; 
    
    FILE *fp = fopen(nombre_archivo,"rb"); 
    if(fp == NULL)
        return NULL; 
    
    fread(actor,sizeof(actor_t),1,fp); 
    
    fclose(fp); 
    return actor; 
}

// ejercicio 3
int fecha_comparar(fecha_t a, fecha_t b){
    if(a.anio > b.anio)
        return 1; 
    if(a.anio < b.anio)
        return -1; 
    if(a.mes > b.mes)
        return 1; 
    if(a.mes < b.mes)
        return -1; 
    if(a.dia > b.dia)
        return 1; 
    if(a.dia < b.dia)
        return -1; 
    return 0; 
}
void actores_ordenar_por_fecha_nacimiento(actor_t *actores[], size_t n){
    for(int i=0; i<n-1; i++)//itera sobre el vector 
	{
		int min = i;
		for(int j=i+1; j< n; j++) //itera sobre todo el array buscando minimo
		{
			if( fecha_comparar(actores[j]->fnac,actores[min]->fnac) == -1 )
			{
				min = j;//si encuentra un nuevo minimo lo actualiza
			}
		}
		actor_t *temp = actores[min]; // lo ordena
		actores[min] = actores[i];
		actores[i] = temp;
	}
}

// ejercicio 4

actor_t* buscar_actor_recursivo(actor_t *a[], const char *nombre,size_t start, size_t end,size_t m_prev){
    if(a == NULL)
        return NULL;

    if (nombre == NULL)
        return NULL; 

    size_t mid; 
    mid = (start + end)/2; 
    actor_t *aux; 
    aux = a[mid];
    if(aux == NULL || mid == m_prev)
        return NULL;
    
    int comparacion = strcmp(aux->nombre,nombre); 
   
    if(!comparacion){
        return aux;
    }
    if(comparacion > 0){
        end = mid-1;  
        return buscar_actor_recursivo(a,nombre,start,end,mid);
    }
    else{
        start = mid+1; 
        return buscar_actor_recursivo(a,nombre,start,end,mid);
    }
    return NULL; 
}

actor_t *actores_buscar(actor_t *actores[], size_t n, const char *nombre){
    if(actores == NULL)
        return NULL;
    if (nombre == NULL)
        return NULL; 
    size_t start, end,mid;
    start = 0; 
    end = n; 
    mid = end+1;
    actor_t * actor= buscar_actor_recursivo(actores,nombre,start,end,mid); 
    return actor; 
}

// ejercicio 5
#define SHIFT 4


uint32_t fecha_a_bcd(fecha_t fecha){
    uint32_t f = 0;
    uint8_t aux = 0; 

//descompongo el año 
    
    int millares = fecha.anio/1000;
    int centenas = (fecha.anio-(millares*1000))/100;
    int decenas = (fecha.anio- (millares*1000 + centenas*100))/10;
    int unidades= fecha.anio-(millares*1000 + centenas*100 + decenas*10 );
    int descomposicion[4] = {millares,centenas,decenas,unidades};

    for(int i = 0; i < 4 ; i++){
        aux = descomposicion[i]; 
        f = f | aux; 
        f = f << SHIFT; 
    }
// descompongo el mes
    decenas = fecha.mes/10; 
    unidades = fecha.mes - decenas*10; 

    f = f | decenas; 
    f = f << SHIFT; 
    f = f | unidades; 
    f = f << SHIFT; 

// descompongo el dia
    decenas = fecha.dia/10; 
    unidades = fecha.dia - decenas*10; 

    f = f | decenas; 
    f = f << SHIFT; 
    f = f | unidades; 
    return f;
}

fecha_t bcd_a_fecha(uint32_t bcd){
    fecha_t fecha; 
//anio: 
    int millares = (bcd >> 28) *1000;
    int centenas = ((bcd >> 24) & 0x0F)*100; 
    int decenas = ((bcd >> 20) & 0x00F)*10; 
    int unidades = ((bcd >> 16) & 0x000F); 
    fecha.anio = millares + centenas + decenas + unidades; 

    decenas = ((bcd & 0x0000FF00) >> 12)*10; 
    unidades = ((bcd & 0x00000F00) >> 8);
    fecha.mes = decenas + unidades; 

    decenas = ((bcd & 0x000000FF) >> 4)*10; 
    unidades = (bcd & 0x0000000F);
    fecha.dia = decenas + unidades; 
    return fecha;  
}
