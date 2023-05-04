#pragma once
#include "Gos/Reflection/ReflMgr.h"

struct Debug {
    static void refl();
    void print(ReflMgr::Any val);
};
