#pragma once
#include <string>

struct GameObject;

struct BaseComponent {
    static void refl();
    std::string name;
    GameObject* gameObject;
};
