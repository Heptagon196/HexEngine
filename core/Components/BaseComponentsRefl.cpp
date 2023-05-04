#include "../Gos/Reflection/ReflMgr.h"
#include "Vector2.h"
#include "BaseComponent.h"
#include "Transform.h"
#include "BaseComponentsRefl.h"

void HexEngine::ReflectBaseComponents() {
    Vector2f::refl();
    Vector2i::refl();
    BaseComponent::refl();
    Transform::refl();
}

void Vector2f::refl() {
    auto& mgr = ReflMgr::Instance();
    mgr.AddClass<Vector2f>();
    mgr.AddField(&Vector2f::x, "x");
    mgr.AddField(&Vector2f::y, "y");
}

void Vector2i::refl() {
    auto& mgr = ReflMgr::Instance();
    mgr.AddClass<Vector2i>();
    mgr.AddField(&Vector2i::x, "x");
    mgr.AddField(&Vector2i::y, "y");
}

void BaseComponent::refl() {
    auto& mgr = ReflMgr::Instance();
    mgr.AddClass<BaseComponent>();
    mgr.AddAliasClass("Component", "BaseComponent");
    mgr.AddField(&BaseComponent::name, "name");
    mgr.RawAddField(TypeID::get<BaseComponent>(), TypeID::get<GameObject&>(), "gameObject", std::function([](ObjectPtr instance) -> decltype(auto) {
        return ObjectPtr(TypeID::get<GameObject>(), (void*)instance.As<BaseComponent>().gameObject);
    }));
}

void Transform::refl() {
    auto& mgr = ReflMgr::Instance();
    mgr.AddClass<Transform>();
    mgr.SetInheritance<Transform, BaseComponent>();
    mgr.AddField(&Transform::position, "position");
    mgr.AddField(&Transform::size, "size");
    mgr.AddField(&Transform::pivot, "pivot");
    mgr.AddMethod(&Transform::Parent, "Parent");
    mgr.AddMethod(&Transform::ChildCount, "ChildCount");
    mgr.AddMethod(&Transform::GetChild, "GetChild");
}
