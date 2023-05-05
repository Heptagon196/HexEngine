#pragma once
#include "BaseComponent.h"
#include "HexVector.h"

class Math {
    public:
        static void refl();
        static float PI;
        static bool PointInRecti(Vector2i point, Vector4i rect);
        static bool PointInRectf(Vector2f point, Vector4f rect);
        static float DEG2RAD(float deg);
        static float RAD2DEG(float rad);
        static int GetSeed();
        static void SetSeed(int seed);
        static int RandInt(int num);
        static int RandRange(int l, int r);
        static float RandFloat(float l, float r);
};
