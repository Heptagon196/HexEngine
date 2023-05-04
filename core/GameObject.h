#pragma once
#include "Components/Transform.h"
#include "Gos/Reflection/ReflMgr.h"

struct GameObject {
    static void refl();
    GameObject();
    std::string name;
    Transform transform;
    std::vector<SharedObject> components;
    ReflMgr::Any GetComponent(const std::string& name);
    ReflMgr::Any AddComponent(const std::string& name);
    bool HasComponent(const std::string& name);
    void RemoveComponent(const std::string& name);
};
