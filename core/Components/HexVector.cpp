#include "HexVector.h"

Vector2i::Vector2i() {}
Vector2f::Vector2f() {}
Vector3i::Vector3i() {}
Vector3f::Vector3f() {}
Vector4i::Vector4i() {}
Vector4f::Vector4f() {}

Vector2i::Vector2i(int x, int y) : x(x), y(y) {}
Vector2f::Vector2f(float x, float y) : x(x), y(y) {}
Vector3i::Vector3i(int x, int y, int z) : x(x), y(y), z(z) {}
Vector3f::Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}
Vector4i::Vector4i(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}
Vector4f::Vector4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

SDL_Color Vector4i::color() {
    return { (Uint8)x, (Uint8)y, (Uint8)z, (Uint8)w };
}
