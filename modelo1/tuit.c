#include "tuit.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct nodo{
    id_usuario_t elem;
    struct nodo *sig; 
}nodo_t;

struct lista{
    nodo_t *prim;  
};
typedef struct lista lista_t; 

lista_t * lista_crear(void){
    lista_t *lista; 
    lista = malloc(sizeof(lista_t)); 
    lista->prim = NULL;
    return lista; 
}
void lista_destruir(lista_t *l){
    nodo_t *aux = l->prim;
    nodo_t *sig;
    while(aux != NULL){
        sig = aux->sig;
        free (aux); 
        aux = sig; 
    }
    if(l != NULL)
        free (l); 
}

size_t lista_longitud(lista_t *l){
    size_t n = 0; 
    nodo_t *aux = l->prim; 
    while(aux != NULL){
        n++; 
        aux = aux->sig;
    }
    return n; 
}

int cmp_ids(id_usuario_t a,id_usuario_t b){
    if(a < b)
        return -1; 
    if(a > b)
        return 1; 
    else 
        return 0; 
}

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

bool lista_busqueda_binaria(nodo_t *n,id_usuario_t buscado,size_t start, size_t end, size_t mid_prev){
    if(n == NULL)
        return false; 
    
    size_t mid; 
    mid = (start + end)/2; 
    nodo_t *aux; 
    aux = nodo_medio(n,mid);
    if(aux == NULL || mid == mid_prev)
        return false;              
    
    int comparacion = cmp_ids(aux->elem,buscado); 
   
    if(comparacion == 0){
        return true;
    }
    if(comparacion > 0){
        end = mid-1;  
        return lista_busqueda_binaria(n,buscado,start,end,mid);
    }
    else{
        start = mid+1; 
        return lista_busqueda_binaria(n,buscado,start,end,mid);
    }
    return false; 
}

bool lista_buscar(lista_t *l,id_usuario_t buscado){ //funcion que da verdadero si el buscado esta en la lista
    size_t start, end,mid;
    start = 0; 
    end = lista_longitud(l); 
    mid = end+1; 
    if (l->prim == NULL || l->prim->elem > buscado)
        return false;
    return(lista_busqueda_binaria(l->prim,buscado,start,end,mid));
}

nodo_t* nodo_crear(id_usuario_t id){
    nodo_t *n = malloc(sizeof(nodo_t)); 
    if(n == NULL)
        return NULL; 
    n->elem = id; 
    n->sig = NULL;
    return n; 
}

bool lista_agregar_comienzo(lista_t *l,id_usuario_t id){
    nodo_t *nuevo = nodo_crear(id);
    if (nuevo == NULL)
        return false; 
    nodo_t *aux = l->prim; 
    l->prim = nuevo; 
    nuevo->sig = aux; 
    return true; 
}


//flag  = 1 inserta menor a mayor, flag = -1  mayor a menor
bool lista_insertar_ordenado(lista_t *l,id_usuario_t id,int flag){
    nodo_t *n = nodo_crear(id);
    if (n == NULL || l == NULL)
        return false; 
    if(l->prim == NULL){ // caso lista vacia 
        l->prim = n; 
        return true;
    }
    nodo_t *ant, *actual;
	actual = l->prim;
	ant = actual; 
    
    if (cmp_ids(actual->elem,n->elem) == flag){ //primer elemento
		l->prim = n; 
		n->sig = actual; 
		return true; 
	}
    while(actual != NULL){
		if(cmp_ids(actual->elem,n->elem) == flag ){
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


// ************************* TDA TUIT ********************************** //

struct tuit{
    id_tuit_t id_tuit;
    id_usuario_t id_usuario; 
    char msj[TUIT_MAX_LARGO];
    lista_t *likes;
    id_tuit_t respuesta;
    id_tuit_t respuestas[TUIT_MAX_RESPUESTAS]; 
};

tuit_t *tuit_crear(id_tuit_t id_tuit, id_usuario_t id_autor, const char *mensaje){
    tuit_t *tnuevo = malloc(sizeof(tuit_t)); 
    if (tnuevo == NULL)
        return NULL; 
    tnuevo->id_tuit = id_tuit; 
    tnuevo->id_usuario = id_autor;
    strcpy(tnuevo->msj,mensaje);
    tnuevo->respuesta = -1; 
    tnuevo->likes = lista_crear(); 
    for(int i = 0; i< TUIT_MAX_RESPUESTAS; i++){
        tnuevo->respuestas[i] = 0;
    } 
    
    return tnuevo; 
}
void tuit_destruir(tuit_t *tuit){
    lista_destruir(tuit->likes); 
    free(tuit); 
}
id_tuit_t tuit_id(const tuit_t *tuit){
    return tuit->id_tuit;
}
id_usuario_t tuit_id_autor(const tuit_t *tuit){
    return tuit->id_usuario;
}
const char *tuit_mensaje(const tuit_t *tuit){
    return (tuit->msj); 
}
size_t tuit_cantidad_likes(const tuit_t *tuit){
    return lista_longitud(tuit->likes);
}
bool tuit_dar_like(tuit_t *tuit, id_usuario_t id_usuario){
    if(tuit->id_usuario == id_usuario)
        return false; 
    if(lista_buscar(tuit->likes,id_usuario))
        return false; 
    return (lista_insertar_ordenado(tuit->likes,id_usuario,1));  
}
bool tuit_fue_likeado_por(const tuit_t *tuit, id_usuario_t id_usuario){
    return (lista_buscar(tuit->likes,id_usuario)); 
}


//EJERCICIO 2

// tuit_escribir escribe el tuit en el archivo `f` (abierto previamente en modo binario).
//
// Devuelve false en caso de error.
bool tuit_escribir(const tuit_t *tuit, FILE *f){
    if(f == NULL)
        return false; 

    fwrite(tuit,sizeof(tuit_t),1,f);

    nodo_t *aux = tuit->likes->prim;
    while (aux != NULL){
        fwrite(&aux->elem,sizeof(id_usuario_t),1,f);
        aux = aux->sig;
    }
    return true; 
}

// tuit_leer lee del archivo `f` (abierto previamente en modo binario) un tuit previamente
// escrito mediante tuit_escribir.
//
// Devuelve un nuevo tuit con la información leida, o NULL en caso de error, que puede ser por:
// * no se pudo reservar memoria
// * error al leer el archivo
tuit_t* tuit_leer(FILE *f){
    if (f == NULL)
        return NULL; 
    
    tuit_t *t = malloc(sizeof(tuit_t));
    if(t == NULL)
        return NULL;  
    
    fread(t,sizeof(tuit_t),1,f); 
    
    t->likes = lista_crear(); 
    id_usuario_t usuario = 0;
    while(fread(&usuario,sizeof(id_usuario_t),1,f)){
        if(tuit_dar_like(t,usuario));      
    }
    return t;  
}

//EJERCICIO 3 

void tuits_ordenar_por_cantidad_de_likes(tuit_t *tuits[], size_t n){

    for(int i=0; i<n-1; i++)//itera sobre el vector 
	{
		int min = i;
		for(int j=i+1; j< n; j++) //itera sobre todo el array buscando minimo
		{
			if( tuit_cantidad_likes(tuits[j]) > tuit_cantidad_likes(tuits[min]) )
			{
				min = j;//si encuentra un nuevo minimo lo actualiza
			}
		}
		tuit_t *temp = tuits[min]; // lo ordena
		tuits[min] = tuits[i];
		tuits[i] = temp;
	}
}

//EJERCICIO 5 
// tuit_responder crea un tuit en respuesta de otro tuit
// Devuelve el tuit creado (con id = id_tuit_respuesta), o NULL en caso de error.
tuit_t *tuit_responder(tuit_t *tuit, id_tuit_t id_tuit_respuesta, id_usuario_t id_autor, const char *mensaje){
    if(tuit->respuestas[TUIT_MAX_RESPUESTAS-1] != 0)
        return NULL;
    tuit_t *nuevo = tuit_crear(id_tuit_respuesta,id_autor,mensaje); 
    nuevo->respuesta = tuit->id_tuit; 
    for(int i = 0; i< TUIT_MAX_RESPUESTAS; i++){
        if(tuit->respuestas[i] == 0){
            tuit->respuestas[i]=id_tuit_respuesta;
            i = TUIT_MAX_RESPUESTAS;
        }
    }
    return nuevo; 
}

// tuit_en_respuesta_de devuelve el ID del tuit respondido, o -1 si no es una respuesta de ningun tuit
id_tuit_t tuit_en_respuesta_de(const tuit_t *tuit){
    return tuit->respuesta;
}

// tuit_respuestas guarda en id_respuestas los IDs de los tuits que son respuestas, y en
// cant_respuestas la cantidad de respuestas
void tuit_respuestas(const tuit_t *tuit, id_tuit_t id_respuestas[], size_t *cant_respuestas){
    size_t n = 0; 
    while(tuit->respuestas[n]!= 0 && n < TUIT_MAX_RESPUESTAS){
        id_respuestas[n] = tuit->respuestas[n];
        n++;
    }
    *cant_respuestas = n; 
}
