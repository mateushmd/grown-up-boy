#pragma once

#include <cstdio>
#include <cstdlib>

#define TODO(...) \
    do { \
        fprintf(stderr, "\n[TODO] Not yet implemented! %s\n", #__VA_ARGS__); \
        fprintf(stderr, "  -> Function: %s\n", __PRETTY_FUNCTION__); \
        fprintf(stderr, "  -> Location: %s:%d\n\n", __FILE__, __LINE__); \
        __builtin_trap(); \
    } while (0)
