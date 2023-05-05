#pragma once
#include <vector>
#include "BaseComponent.h"
#include "HexVector.h"

struct Transform : BaseComponent {
    static void refl();
    Transform* parent = nullptr;
    std::vector<Transform*> childs;
    Vector2f position = {0, 0};
    Vector2f worldPos();
    float worldRot();
    float angle = 0;
    Vector2f size = {0, 0};
    Vector2f pivot = {0.5f, 0.5f};
    Transform& Parent();
    int ChildCount();
    Transform& GetChild(int idx);
};
