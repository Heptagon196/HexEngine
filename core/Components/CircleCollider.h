#pragma once
#include "Collider.h"

class CircleCollider : public Collider {
    public:
        static void refl();
        bool showCollider = false;
        float radius;
        void SetCollider(float radius);
        void Awake();
        void Draw();
        void OnDestroy();
};
