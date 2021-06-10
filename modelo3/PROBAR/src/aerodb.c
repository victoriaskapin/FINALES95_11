#include "include/aerodb.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//ESTRUCTURA AEROPUERTO 
/*struct aero {
    char designacion [3];
    char nombre [63];
    char ciudad [63];
    char pais [63];
    double latitud; 
    double longitud; 
};*/
//typedef struct aero aerop_t; 

//ESTRUCTURA NODOS
struct nodo
{
	aerop_t * elem;
	struct nodo *sig; 
};
typedef struct nodo nodo_t; 

//ESTRUCTURA BASE DE DATOS 
struct aerodb {
    nodo_t* prim; 
};


aerodb_t *aerodb_crear(){
    aerodb_t *a = malloc(sizeof(aerodb_t)); 
    if(a == NULL)
        return NULL; 
    a->prim = NULL; 
    return a; 
}

aerop_t* aerop_crear(const char *designacion, const char *nombre, const char *ciudad, const char *pais, double lat, double lon){
    aerop_t *n; 
    n = malloc(sizeof(aerop_t)); 
    if(n == NULL)
        return n; 
    strcpy(n->designacion,designacion); 
    strcpy(n->nombre,nombre);
    strcpy(n->ciudad,ciudad); 
    strcpy(n->pais,pais); 
    n->latitud = lat; 
    n->longitud = lon; 
    return n; 
}

nodo_t* nodo_crear(aerop_t *dato){
    nodo_t* nuevo = malloc(sizeof(nodo_t)); 
    if(nuevo == NULL)
        return NULL;

    nuevo->elem = dato; 
    nuevo->sig = NULL; 
    return nuevo;
}

bool es_aerodb_vacia(aerodb_t *a){
    return(a->prim == NULL);
}

int comparar_aeropuertos(aerop_t *dato1, aerop_t *dato2){
    int i = strncmp(dato1->designacion,dato2->designacion,sizeof(char)*3);
    if(i == 0)
        return 0;
    if(i > 0)
        return 1; 
    return -1;
}//devuelve 1 si dato1>dato2  // 0 si son iguales // -1 si dato1<dato2


// si la lista esta ordenada de  menor a mayor flag == 1, mayor a menor flag == -1
bool aero_insertar_ordenado (aerodb_t *l, aerop_t *dato,int(*cmp)(aerop_t *dato1, aerop_t *dato2), int flag){
	if (dato == NULL || l == NULL){
		return false;} //lista vacia

    nodo_t *n = nodo_crear(dato);
    
    if (n == NULL){
        return false;} 
    
	if (es_aerodb_vacia(l)){
		l->prim = n; 
		return true; 
	}

	nodo_t *ant, *actual;
	actual = l->prim;
	ant = actual; 
	if (cmp(actual->elem,n->elem) == flag){ //primer elemento
		l->prim = n; 
		n->sig = actual; 
		return true; 
	}

    while(actual != NULL){
		if(cmp(actual->elem,n->elem) == flag ){
			ant->sig = n; 
			n->sig = actual;
			return true;
		} 	// si quiero que sea de menor a mayor flag == 1, menor a mayor flag == -1
		ant = actual; 
		actual = actual->sig; 
	}
	ant->sig = n; 
	n->sig = NULL; 
return true; 
}


bool aerodb_agregar_al_comienzo(aerodb_t *a,aerop_t *dato){
    if(a == NULL)
        return false; 
    nodo_t *aux = nodo_crear(dato);
    if(aux == NULL)
        return false;  
    aux->sig = a->prim;
    a->prim = aux; 
    return true; 
}

bool aerodb_agregar(aerodb_t *a, const char *designacion, const char *nombre, const char *ciudad, const char *pais, double lat, double lon){

    aerop_t *aeronuevo = aerop_crear(designacion,nombre,ciudad,pais,lat,lon); 
    if(aeronuevo == NULL)
        return false; 
    if (a == NULL)
        return false; 

    return(aero_insertar_ordenado(a,aeronuevo,comparar_aeropuertos,1));  
}

void aerodb_destruir(aerodb_t*a){
    if(a == NULL)
        return;
    if(!es_aerodb_vacia(a)){
        nodo_t *aux,*sig; 
        aux = a->prim; 
        while(aux != NULL){
            sig = aux->sig; 
            free(aux->elem); 
            free(aux); 
            aux = sig; 
        }
    }
    free(a); 
}

size_t aerodb_cantidad(const aerodb_t *a){
    if(a == NULL)
        return 0; 
    nodo_t *aux; 
    size_t n=0; 
    aux = a->prim;
    while(aux != NULL){
        n++;
        aux = aux->sig;
    }
    return n;  
}

aerop_t* buscar_aeropuerto(const aerodb_t *a, const char *designacion){
    if(a == NULL)
        return NULL;
    if (designacion == NULL)
        return NULL; 

    nodo_t *aux; 
    aux = a->prim;
    while (aux != NULL){
        if(!strncmp(aux->elem->designacion,designacion,sizeof(char)*3)){
            return aux->elem;
        }  
        aux = aux->sig; 
    }
    return NULL; 
}

const char *aerodb_nombre(const aerodb_t *a, const char *designacion){
    aerop_t *aerop = buscar_aeropuerto(a,designacion); 
    if(aerop == NULL)
        return NULL;
    const char * nom = aerop->nombre;
    return nom; 
}
const char *aerodb_ciudad(const aerodb_t *a, const char *designacion){
    aerop_t *aerop = buscar_aeropuerto(a,designacion); 
    if(aerop == NULL)
        return "NULL"; 
    return aerop->ciudad;
}
const char *aerodb_pais(const aerodb_t *a, const char *designacion){
    aerop_t *aerop = buscar_aeropuerto(a,designacion); 
    if(aerop == NULL)
        return "NULL"; 
    return aerop->pais;
}
double aerodb_lat(const aerodb_t *a, const char *designacion){
    aerop_t *aerop = buscar_aeropuerto(a,designacion); 
    if(aerop == NULL)
        return 0; 
    double lat = aerop->latitud;
    return lat;
}
double aerodb_lon(const aerodb_t *a, const char *designacion){
    aerop_t *aerop = buscar_aeropuerto(a,designacion); 
    if(aerop == NULL)
        return 0; 
    return aerop->longitud;
}

//EJERCICIO 2 

bool aerodb_escribir(const aerodb_t *a, const char *ruta){
    if(a == NULL || ruta == NULL)
        return false; 
    
    FILE *fp = fopen(ruta,"wb");
    if(fp == NULL)
        return false; 
    
    nodo_t *aux = a->prim; 
    while(aux != NULL){  
        fwrite(aux->elem,sizeof(aerop_t),1,fp);
        aux = aux->sig;
    }
        fclose(fp);
        return true; 
}
aerodb_t *aerodb_leer(const char *ruta){
    aerodb_t *a = aerodb_crear();
    if(a == NULL || ruta == NULL)
        return NULL;
    
    FILE *fp = fopen(ruta,"rb");
    if(fp == NULL)
        return false;

    aerop_t aux; 
    //Leo el primero
    fread(&aux,sizeof(aerop_t),1,fp);
    if(aerodb_agregar(a,aux.designacion,aux.nombre,aux.ciudad,aux.pais,aux.latitud,aux.longitud));

    while(fread(&aux,sizeof(aerop_t),1,fp)){//!feof(fp)){
       // fread(&aux,sizeof(aerop_t),1,fp);
        if(aerodb_agregar(a,aux.designacion,aux.nombre,aux.ciudad,aux.pais,aux.latitud,aux.longitud)); 
    
    }
    fclose(fp);
    return a;  
}
// EJERCICIO 3 

nodo_t *nodo_medio(nodo_t*n,size_t mid){
    if (n == NULL)
        return NULL; 
    nodo_t* aux = n; 
    size_t i = 1;
    while(i < mid && aux != NULL){
        aux = aux->sig; 
        i++;
    }
    return aux;  
}

aerop_t* buscar_aeropuerto_recursivo(nodo_t *a, const char *designacion,size_t start, size_t end){
    if(a == NULL)
        return NULL;

    if (designacion == NULL)
        return NULL; 

    size_t mid; 
    mid = (start + end)/2; 
    nodo_t *aux; 
    aux = nodo_medio(a,mid);
    
    int comparacion = strncmp(aux->elem->designacion,designacion,sizeof(char)*3); 
   
    if(!comparacion){
        return aux->elem;
    }
    if(comparacion > 0){
        end = mid-1;  
        return buscar_aeropuerto_recursivo(a,designacion,start,end);
    }
    else{
        start = mid+1; 
        return buscar_aeropuerto_recursivo(a,designacion,start,end);
    }
    return NULL; 
}

aerop_t* buscar_aeropuerto_binario(const aerodb_t *a, const char *designacion){
    if(a == NULL)
        return NULL;
    if (designacion == NULL)
        return NULL; 
    size_t start, end;
    start =0; 
    end = aerodb_cantidad(a); 
    aerop_t * aero = buscar_aeropuerto_recursivo(a->prim,designacion,start,end); 
    if (aero == NULL)    
     return NULL; 
    
    return aero; 
}

bool aerodb_buscar(aerodb_t *a, const char *designacion, char **nombre, char **ciudad, char **pais, double *lat, double *lon){
    aerop_t *aero = buscar_aeropuerto_binario(a,designacion);
    if(aero == NULL ) 
        return false; 
    *nombre = aero->nombre; 
    *ciudad = aero->ciudad; 
    *pais = aero->pais; //strcpy(*pais,aero->pais); 
    *lat = aero->latitud; 
    *lon = aero->longitud; 
    return true;
}

void imprimir_designacion(aerop_t *dato){
    printf("%c%c%c",dato->designacion[0],dato->designacion[1],dato->designacion[2]);
}
void aero_imprimir(aerodb_t*l,void (*imprimir_dato)(aerop_t *dato)){
	nodo_t* aux = l->prim;
	printf("{"); 
	while (aux!=NULL){
		imprimir_dato(aux->elem);
		printf(" --> "); 
		aux = aux->sig; 
	}
	printf("NULL }\n");
}

//EJERCICIO 5
#define MASCARASEG 0x3F
#define MASCARAGRAD 0xFF
#define MASCARANEG 0x01
bool es_negativo (uint32_t angulo){
   uint8_t aux; 
   aux =  angulo >> 20; 
    aux = (aux & MASCARANEG);
    return(aux != 0x00); 
}

uint8_t obtener_grados(uint32_t angulo){
    uint8_t grados; 
    uint32_t aux = angulo >> 12;
    grados = (aux & MASCARAGRAD); 
    return grados; 
}

uint8_t obtener_minutos(uint32_t angulo){
    uint8_t minutos; 
   uint32_t aux =  angulo >> 6;
    minutos = (aux & MASCARASEG); 
    return minutos; 
}

uint8_t obtener_segundos(uint32_t angulo){
    uint8_t segundos; 
    segundos = (angulo & MASCARASEG); 
    return segundos; 
}

void decodificar_angulo(uint32_t angulo_sexagesimal, bool *negativo, uint8_t *grados, uint8_t *minutos, uint8_t *segundos){
    *negativo = es_negativo(angulo_sexagesimal);
    *grados = obtener_grados(angulo_sexagesimal); 
    *minutos = obtener_minutos(angulo_sexagesimal); 
    *segundos = obtener_segundos(angulo_sexagesimal); 
}
