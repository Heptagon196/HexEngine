#pragma once
#include <vector>
#include "BaseComponent.h"
#include "Vector2.h"

struct Transform : BaseComponent {
    static void refl();
    Transform* parent;
    std::vector<Transform*> childs;
    Vector2f position;
    Vector2f size;
    Vector2f pivot;
    Transform& Parent();
    int ChildCount();
    Transform& GetChild(int idx);
};
