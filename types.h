#ifndef __TYPES_H
#define __TYPES_H

    // We define types so if we were to use a different compiler that had different byte amounts
    // We would only have to change this file.
    typedef char int8_t;
    typedef unsigned char uint8_t;

    typedef short int16_t;
    typedef unsigned short uint16_t;

    typedef int int32_t;
    typedef unsigned int uint32_t;

    typedef long long int int64_t;
    typedef unsigned long long int uint64_t;

#endif