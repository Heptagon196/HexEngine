#pragma once
#include <string>
#include "../Gos/Reflection/ReflMgr.h"
#include "../Gos/Reflection/ReflMgrInit.h"

template<typename T> concept compHas__ctor = requires(T instance) { instance.__ctor(); };
template<typename T> concept compHasAwake = requires(T instance) { instance.Awake(); };
template<typename T> concept compHasStart = requires(T instance) { instance.Start(); };
template<typename T> concept compHasPhysicsUpdate = requires(T instance) { instance.PhysicsUpdate(); };
template<typename T> concept compHasEarlyUpdate = requires(T instance) { instance.EarlyUpdate(); };
template<typename T> concept compHasUpdate = requires(T instance) { instance.Update(); };
template<typename T> concept compHasLateUpdate = requires(T instance) { instance.LateUpdate(); };
template<typename T> concept compHasDraw = requires(T instance) { instance.Draw(); };
template<typename T> concept compHasAfterDraw = requires(T instance) { instance.AfterDraw(); };
template<typename T> concept compHasOnDestroy = requires(T instance) { instance.OnDestroy(); };

#define REG_METHOD(method) mgr.AddMethod(&REG_CLASS::method, #method)

#define REG_STATIC_METHOD(method) mgr.AddStaticMethod(TypeID::get<REG_CLASS>(), &REG_CLASS::method, #method)

#define REG_FIELD(field) mgr.AddField(&REG_CLASS::field, #field)

#define REG_STATIC_FIELD(field) mgr.AddStaticField(TypeID::get<REG_CLASS>(), &REG_CLASS::field, #field)

#define REG_COMP_LIFECYCLE_METHOD(method)           \
if constexpr (compHas ## method<REG_CLASS>) {       \
    REG_METHOD(method);                             \
}

template<typename T>
static inline void REG_COMP_LIFECYCLE_METHODS() {
    using REG_CLASS = T;
    auto& mgr = ReflMgr::Instance();
    REG_COMP_LIFECYCLE_METHOD(__ctor);
    REG_COMP_LIFECYCLE_METHOD(Awake);
    REG_COMP_LIFECYCLE_METHOD(Start);
    REG_COMP_LIFECYCLE_METHOD(EarlyUpdate);
    REG_COMP_LIFECYCLE_METHOD(PhysicsUpdate);
    REG_COMP_LIFECYCLE_METHOD(Update);
    REG_COMP_LIFECYCLE_METHOD(LateUpdate);
    REG_COMP_LIFECYCLE_METHOD(Draw);
    REG_COMP_LIFECYCLE_METHOD(AfterDraw);
    REG_COMP_LIFECYCLE_METHOD(OnDestroy);
}

#define REG_COMP_BASE(class, base)                  \
    using REG_CLASS = class;                        \
    auto& mgr = ReflMgr::Instance();                \
    ReflMgrTool::AutoRegister<REG_CLASS>();         \
    mgr.SetInheritance<REG_CLASS, base>();          \
    REG_COMP_LIFECYCLE_METHODS<REG_CLASS>();        \

#define REG_COMP(class) REG_COMP_BASE(class, BaseComponent)

struct GameObject;

struct BaseComponent {
    static void refl();
    bool enabled;
    std::string name;
    GameObject* gameObject;
};
