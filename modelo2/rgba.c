#include "rgba.h"
#define TAM_COMPONENTE 8 
rgba_t rgba (componente_t r,componente_t g,componente_t b,componente_t a){
    rgba_t color = 0x00000000; 
    color = color | r; 
    color = color << TAM_COMPONENTE;
    color = color | g; 
    color = color << TAM_COMPONENTE; 
    color = color | b; 
    color = color << TAM_COMPONENTE; 
    color = color | a; 
    return color; 
}

componente_t rgba_r (rgba_t color){
    componente_t r = 0x00; 
    rgba_t aux = color >> (TAM_COMPONENTE*3); 
    r = aux | r; 
    return r; 
}
componente_t rgba_g (rgba_t color){
    componente_t g = 0x00; 
    rgba_t aux = color >> (TAM_COMPONENTE*2); 
    g = aux | g; 
    return g; 
}
componente_t rgba_b (rgba_t color){
    componente_t b = 0x00; 
    rgba_t aux = color >> TAM_COMPONENTE; 
    b = aux | b; 
    return b; 
}
componente_t rgba_a (rgba_t color){
    componente_t a = 0x00; 
    a = a | color;  
    return a; 
}


