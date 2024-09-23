//
// Created by Bruno on 6/15/2024.
//

#ifndef VERTICES_SDK_BASE2048_H
#define VERTICES_SDK_BASE2048_H
#include <stdint.h>

typedef struct {
    uint16_t* data;
    size_t size;
    size_t capacity;
} Uint16Vector;

typedef struct {
    unsigned char* data;
    size_t size;
    size_t capacity;
} Bytes;

Uint16Vector b2048_encode(const Bytes* in);
Bytes b2048_decode(const Uint16Vector* in);

#endif //VERTICES_SDK_BASE2048_H
