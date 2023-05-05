#pragma once
#include "SDL2/SDL.h"

struct Vector2f {
    static void refl();
    Vector2f();
    Vector2f(float x, float y);
    float x = 0;
    float y = 0;
};

struct Vector2i {
    static void refl();
    Vector2i();
    Vector2i(int x, int y);
    int x = 0;
    int y = 0;
};

struct Vector3f {
    static void refl();
    Vector3f();
    Vector3f(float x, float y, float z);
    float x = 0;
    float y = 0;
    float z = 0;
};

struct Vector3i {
    static void refl();
    Vector3i();
    Vector3i(int x, int y, int z);
    int x = 0;
    int y = 0;
    int z = 0;
};
struct Vector4f {
    static void refl();
    Vector4f();
    Vector4f(float x, float y, float z, float w);
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 0;
};

struct Vector4i {
    static void refl();
    Vector4i();
    Vector4i(int x, int y, int z, int w);
    SDL_Color color();
    int x = 0;
    int y = 0;
    int z = 0;
    int w = 0;
};
