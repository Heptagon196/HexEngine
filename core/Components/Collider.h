#pragma once
#include "BaseComponent.h"
#include "box2d/box2d.h"

class Collider : public BaseComponent {
    protected:
        b2Shape* shape = nullptr;
    public:
        static void refl();
        b2Shape& GetShape();
};
