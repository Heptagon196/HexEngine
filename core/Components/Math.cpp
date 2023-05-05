#include <random>
#include "Math.h"

float Math::PI = acos(-1);

void Math::refl() {
    using REG_CLASS = Math;
    auto& mgr = ReflMgr::Instance();
    REG_STATIC_FIELD(PI);
    REG_STATIC_METHOD(PointInRecti);
    REG_STATIC_METHOD(PointInRectf);
    REG_STATIC_METHOD(DEG2RAD);
    REG_STATIC_METHOD(RAD2DEG);
    REG_STATIC_METHOD(GetSeed);
    REG_STATIC_METHOD(SetSeed);
    REG_STATIC_METHOD(RandInt);
    REG_STATIC_METHOD(RandRange);
    REG_STATIC_METHOD(RandFloat);
}

bool Math::PointInRecti(Vector2i point, Vector4i rect) {
    return point.x >= rect.x && point.x <= rect.x + rect.z && point.y >= rect.y && point.y <= rect.y + rect.w;
}

bool Math::PointInRectf(Vector2f point, Vector4f rect) {
    return point.x > rect.x && point.x < rect.x + rect.z && point.y > rect.y && point.y < rect.y + rect.w;
}

float Math::DEG2RAD(float deg) {
    return deg * PI / 180.0;
}

float Math::RAD2DEG(float rad) {
    return rad * 180.0 / PI;
}

static inline int seed = time(NULL);
static inline std::mt19937 gen(seed);

int Math::GetSeed() {
    return seed;
}

void Math::SetSeed(int seed) {
    gen.seed(seed);
}

int Math::RandInt(int num) {
    return std::uniform_int_distribution<int>(0, num - 1)(gen);
}

int Math::RandRange(int l, int r) {
    return std::uniform_int_distribution<int>(l, r)(gen);
}

float Math::RandFloat(float l, float r) {
    return std::uniform_real_distribution<float>(l, r)(gen);
}
