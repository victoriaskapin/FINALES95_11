#ifndef RGBA_H
#define RGBA_H

#include <stdint.h>

typedef uint8_t componente_t; 
typedef uint32_t rgba_t; 

rgba_t rgba (componente_t r,componente_t g,componente_t b,componente_t a);
componente_t rgba_r (rgba_t color); 
componente_t rgba_g (rgba_t color);
componente_t rgba_b (rgba_t color);
componente_t rgba_a (rgba_t color);

#endif
