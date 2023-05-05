#include "Transform.h"

Vector2f Transform::worldPos() {
    Vector2f ret = { 0, 0 };
    for (Transform* p = this; p != nullptr; p = p->parent) {
        ret.x += p->position.x;
        ret.y += p->position.y;
    }
    return ret;
}

float Transform::worldRot() {
    float ret = 0;
    for (Transform* p = this; p != nullptr; p = p->parent) {
        ret += p->angle;
    }
    return ret;
}

Transform& Transform::Parent() {
    return *parent;
}

int Transform::ChildCount() {
    return childs.size();
}

Transform& Transform::GetChild(int idx) {
    return *childs[idx];
}
