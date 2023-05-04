#include "GameObject.h"

void GameObject::refl() {
    auto& mgr = ReflMgr::Instance();
    mgr.AddClass<GameObject>();
    mgr.AddField(&GameObject::name, "name");
    mgr.AddField(&GameObject::transform, "transform");
    mgr.AddMethod(&GameObject::AddComponent, "AddComponent");
    mgr.AddMethod(&GameObject::GetComponent, "GetComponent");
    mgr.AddMethod(&GameObject::RemoveComponent, "RemoveComponent");
}

GameObject::GameObject() {
    transform.gameObject = this;
}

ReflMgr::Any GameObject::GetComponent(const std::string& name) {
    for (auto obj : components) {
        if (obj.GetType().getName() == name) {
            return obj.ToObjectPtr();
        }
    }
    return ObjectPtr::Null;
}

ReflMgr::Any GameObject::AddComponent(const std::string& name) {
    components.push_back(ReflMgr::Instance().New(name));
    return components.back();
}

bool GameObject::HasComponent(const std::string& name) {
    for (auto obj : components) {
        if (obj.GetType().getName() == name) {
            return true;
        }
    }
    return false;
}

void GameObject::RemoveComponent(const std::string& name) {
    for (int i = 0; i < components.size(); i++) {
        if (components[i].GetType().getName() == name) {
            components.erase(components.begin() + i);
        }
    }
}
