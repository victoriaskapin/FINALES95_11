#ifndef AERODB_H
#define AERODB_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

struct aero {
    char designacion [3];
    char nombre [63];
    char ciudad [63];
    char pais [63];
    double latitud; 
    double longitud; 
};

typedef struct aero aerop_t;
typedef struct aerodb aerodb_t;

void imprimir_designacion(aerop_t *dato);
void aero_imprimir(aerodb_t*l,void (*imprimir_dato)(aerop_t *dato));


aerodb_t *aerodb_crear();
bool aerodb_agregar(aerodb_t *a, const char *designacion, const char *nombre, const char *ciudad, const char *pais, double lat, double lon);
size_t aerodb_cantidad(const aerodb_t *a);
const char *aerodb_nombre(const aerodb_t *a, const char *designacion);
const char *aerodb_ciudad(const aerodb_t *a, const char *designacion);
const char *aerodb_pais(const aerodb_t *a, const char *designacion);
double aerodb_lat(const aerodb_t *a, const char *designacion);
double aerodb_lon(const aerodb_t *a, const char *designacion);
void aerodb_destruir(aerodb_t *a);

bool aerodb_escribir(const aerodb_t *a, const char *ruta);
aerodb_t *aerodb_leer(const char *ruta);

bool aerodb_buscar(aerodb_t *a, const char *designacion, char **nombre, char **ciudad, char **pais, double *lat, double *lon);
/*
bool aerodb_mas_cercano(aerodb_t *a, double xlat, double xlon, char **nombre, char **ciudad, char **pais, double *lat, double *lon);
*/
void decodificar_angulo(uint32_t angulo_sexagesimal, bool *negativo, uint8_t *grados, uint8_t *minutos, uint8_t *segundos);

#endif
