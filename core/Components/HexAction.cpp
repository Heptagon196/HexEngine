#include "../Gos/GosVM.h"
#include "HexAction.h"

void HexAction::refl() {
    auto& mgr = ReflMgr::Instance();
    mgr.AddClass<HexAction>();
    mgr.AddMethod(&HexAction::operator(), "__call");
    mgr.AddMethod(std::function([](HexAction* self, ReflMgr::Any cont) {
        *self = Get(cont);
    }), "__ctor");
}

HexAction::HexAction() {}

HexAction::HexAction(std::function<void(ReflMgr::Any)> func) : std::function<void(ReflMgr::Any)>(func) {}

HexAction HexAction::Get(ReflMgr::Any method) {
    if (method.GetType().getHash() != TypeID::get<HexAction>().getHash()) {
        auto& lambdaInstance = method.As<GosVM::GosInstance>();
        auto obj = SharedObject::New<GosVM::GosInstance>();
        auto& objInstance = obj.As<GosVM::GosInstance>();
        objInstance.info = lambdaInstance.info;
        for (auto field : lambdaInstance.field) {
            objInstance.field[field.first] = field.second;
        }
        obj.ForceTypeTo(method.GetType());
        return std::function([obj](ReflMgr::Any arg) {
            obj({ arg });
        });
    } else {
        return method.As<HexAction>();
    }
}
