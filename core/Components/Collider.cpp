#include "Collider.h"

void Collider::refl() {
    REG_COMP(Collider);
    REG_METHOD(GetShape);
}

b2Shape& Collider::GetShape() {
    return *shape;
}
