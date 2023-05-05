#include "GameObject.h"
#include "HexEngine.h"
#include <list>

void GameObject::refl() {
    auto& mgr = ReflMgr::Instance();
    mgr.AddClass<GameObject>();
    mgr.AddField(&GameObject::enabled, "enabled");
    mgr.AddField(&GameObject::id, "id");
    mgr.AddField(&GameObject::name, "name");
    mgr.AddField(&GameObject::null, "null");
    mgr.AddField(&GameObject::transform, "transform");
    mgr.AddMethod(&GameObject::AddComponent, "AddComponent");
    mgr.AddMethod(&GameObject::HasComponent, "HasComponent");
    mgr.AddMethod(&GameObject::GetComponent, "GetComponent");
    mgr.AddMethod(&GameObject::RemoveComponent, "RemoveComponent");
    mgr.AddMethod(&GameObject::SetParent, "SetParent");
    mgr.AddMethod(&GameObject::Broadcast, "Broadcast");
}

GameObject::GameObject() {
    transform.gameObject = this;
    enabled = true;
    transform.enabled = true;
}

GameObject::GameObject(const std::string& objName) {
    transform.gameObject = this;
    enabled = true;
    transform.enabled = true;
    name = objName;
    transform.name = objName;
}

ReflMgr::Any GameObject::GetComponent(const std::string& name) {
    for (auto obj : components) {
        if (obj.GetType().getName() == name) {
            return obj.ToObjectPtr();
        }
    }
    for (auto obj : components) {
        if (ReflMgr::Instance().IsBaseClass(obj.GetType(), TypeID::getRaw(name))) {
            return obj.ToObjectPtr();
        }
    }
    return ObjectPtr::Null;
}

ReflMgr::Any GameObject::AddComponent(const std::string& name) {
    components.push_back(HexEngine::HexEngine::NewObj(name));
    auto& comp = components.back();
    comp.GetField("enabled").As<bool>() = true;
    comp.TryInvoke("Awake", {});
    comp.TryInvoke("Start", {});
    return comp;
}

bool GameObject::HasComponent(const std::string& name) {
    for (auto obj : components) {
        if (obj.GetType().getName() == name) {
            return true;
        }
    }
    for (auto obj : components) {
        if (ReflMgr::Instance().IsBaseClass(obj.GetType(), TypeID::getRaw(name))) {
            return true;
        }
    }
    return false;
}

void GameObject::RemoveComponent(const std::string& name) {
    for (int i = 0; i < components.size(); i++) {
        if (components[i].GetType().getName() == name || ReflMgr::Instance().IsBaseClass(components[i].GetType(), TypeID::getRaw(name))) {
            components.erase(components.begin() + i);
        }
    }
}

void GameObject::SetParent(GameObject& newParent) {
    HexEngine::HexEngine::Instance().SetParent(*this, newParent);
}

void GameObject::Broadcast(const std::string& method, bool forceCall) {
    std::stack<Transform*> st;
    st.push(&transform);
    while (!st.empty()) {
        std::string name = st.top()->gameObject->name;
        if (!st.top()->gameObject->enabled && !forceCall) {
            st.pop();
            continue;
        }
        auto& childs = st.top()->childs;
        auto& comps = st.top()->gameObject->components;
        st.pop();
        std::queue<SharedObject> toInvoke;
        for (const auto& comp : comps) {
            if (forceCall || comp.GetField("enabled").As<bool>()) {
                toInvoke.push(comp);
            }
        }
        while (!toInvoke.empty()) {
            toInvoke.front().TryInvoke(method);
            toInvoke.pop();
        }
        for (auto iter = childs.rbegin(); iter != childs.rend(); iter++) {
            st.push(*iter);
        }
    }
}

void GameObject::SendMessage(const std::string& method, const std::vector<ObjectPtr>& params) {
    std::queue<SharedObject> toInvoke;
    for (const auto& comp : components) {
        if (comp.GetType() != TypeID::get<void>() && comp.GetField("enabled").As<bool>()) {
            toInvoke.push(comp);
        }
    }
    while (!toInvoke.empty()) {
        toInvoke.front().TryInvoke(method, params);
        toInvoke.pop();
    }
}
