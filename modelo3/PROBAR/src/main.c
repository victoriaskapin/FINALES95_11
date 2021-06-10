#include "include/aerodb.h"
#include <stdio.h> 
#include <stdint.h>
#define ANGULO 0xC953A17A

int main (void){
//DECLARO AEROPUERTOS
    aerop_t a[10] = {
        {"GKA", "Goroka Airport", "Goroka", "Papua New Guinea", -6.081689834590001, 145.391998291},
        {"MAG", "Madang Airport", "Madang", "Papua New Guinea", -5.20707988739, 145.789001465},
        {"HGU", "Mount Hagen Kagamuga Airport", "Mount Hagen", "Papua New Guinea", -5.826789855957031, 144.29600524902344},{"LAE", "Nadzab Airport", "Nadzab", "Papua New Guinea", -6.569803, 146.725977},
        {"POM", "Port Moresby Jacksons International Airport", "Port Moresby", "Papua New Guinea", -9.443380355834961, 147.22000122070312},
        {"WWK", "Wewak International Airport", "Wewak", "Papua New Guinea", -3.58383011818, 143.669006348},
        {"GOH", "Godthaab / Nuuk Airport", "Godthaab", "Greenland", 64.19090271, -51.6781005859},
        {"SFJ", "Kangerlussuaq Airport", "Sondrestrom", "Greenland", 67.0122218992, -50.7116031647},
        {"THU", "Thule Air Base", "Thule", "Greenland", 76.5311965942, -68.7032012939},
        {"AEY", "Akureyri Airport", "Akureyri", "Iceland", 65.66000366210938, -18.07270050048828}
    };
  
    aerodb_t *bd = aerodb_crear(); 
    if(bd == NULL)
        return 1; 
    int i;
    for(i = 0; i<10; i++){
        if(aerodb_agregar(bd,a[i].designacion,a[i].nombre,a[i].ciudad,a[i].pais,a[i].latitud,a[i].longitud))
            printf("Se cargo el aeropuerto:  %i de nombre: %s\n",i+1,aerodb_nombre(bd,a[i].designacion));
    }
    if(aerodb_escribir(bd,"aeros.bin"))
        printf("\nse escribio el archivo\n");
    aerodb_t*a2;
    a2 = aerodb_leer("aeros.bin"); 
    printf("cantidad de aeros en bd: %lu\ncantidad de aeros en a2: %lu\n",aerodb_cantidad(bd),aerodb_cantidad(a2));
    aero_imprimir(bd,imprimir_designacion);
    aero_imprimir(a2,imprimir_designacion);
/* EJERCICIO 3   
    double lat,lon; 
    char **nom, **ciud, **pais;
    nom = malloc(sizeof(char)*63);
    ciud = malloc(sizeof(char)*63);
    pais = malloc(sizeof(char)*63);
    if( aerodb_buscar(bd,"GOH",nom,ciud,pais,&lat,&lon)){
        printf("lo encontro, nombre: %s\n",*nom);
    }
    free(nom);
    free(ciud);
    free(pais);
*/
//EJERCICIO 5 
    uint32_t angulo = 0xC9F3A175; 
    uint8_t grados, minutos, segundos; 
    bool neg; 
    decodificar_angulo(angulo,&neg, &grados, &minutos, &segundos);
    if(neg){
        printf("es negativo\n"); 
    }
    FILE*fp;
    fp = fopen("operadores.bin","wb"); 
    if(fp != NULL){
        fwrite(&angulo,sizeof(uint32_t),1,fp);
        fwrite(&grados,sizeof(uint8_t),1,fp);
        fwrite(&segundos,sizeof(uint8_t),1,fp);
        fwrite(&minutos,sizeof(uint8_t),1,fp);      
        fclose(fp); 
    }
    aerodb_destruir(a2); 
    aerodb_destruir(bd); 
    return 0;
}
