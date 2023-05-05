#pragma once
#include "BaseComponent.h"

struct HexAction : std::function<void(ReflMgr::Any)> {
    HexAction();
    HexAction(std::function<void(ReflMgr::Any)> func);
    static void refl();
    template<typename T>
    static HexAction New(T func) {
        return std::function([&func](ReflMgr::Any arg){
            func(arg);
        });
    }
    static HexAction Get(ReflMgr::Any);
};
