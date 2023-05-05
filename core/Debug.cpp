#include "Debug.h"
#include "Components/BaseComponent.h"

void Debug::refl() {
    auto& mgr = ReflMgr::Instance();
    mgr.AddClass<Debug>();
    using REG_CLASS = Debug;
    REG_STATIC_METHOD(print);
}

void Debug::print(ReflMgr::Any val) {
    std::cout << val << std::endl;
}
