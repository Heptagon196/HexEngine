#pragma once
#include "Components/Transform.h"
#include "Gos/Reflection/ReflMgr.h"

struct GameObject {
    static void refl();
    GameObject();
    GameObject(const std::string& name);
    bool null = false;
    bool enabled;
    int id = -1;
    std::string name;
    Transform transform;
    std::vector<SharedObject> components;
    ReflMgr::Any GetComponent(const std::string& name);
    ReflMgr::Any AddComponent(const std::string& name);
    bool HasComponent(const std::string& name);
    void RemoveComponent(const std::string& name);
    void SetParent(GameObject& newParent);
    void Broadcast(const std::string& method, bool forceCall);
    void SendMessage(const std::string& method, const std::vector<ObjectPtr>& params = {});
};
