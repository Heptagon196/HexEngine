#include "Debug.h"

void Debug::refl() {
    auto& mgr = ReflMgr::Instance();
    mgr.AddClass<Debug>();
    mgr.AddMethod(&Debug::print, "print");
}

void Debug::print(ReflMgr::Any val) {
    std::cout << val << std::endl;
}
