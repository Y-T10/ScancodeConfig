#pragma once

#ifdef _WIN32
    #ifndef STRICT
    #define STRICT
    #endif
    #ifndef NOMIMAX
    #define NOMIMAX
    #endif
    #include <windows.h>
#endif