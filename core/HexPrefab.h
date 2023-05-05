#pragma once
#include "GameObject.h"
#include "Gos/Reflection/JSON.h"
#include <iostream>

class HexPrefab {
    private:
        JSON cont;
    public:
        static void refl();
        HexPrefab();
        HexPrefab(std::istream&& reader);
        void __ctor(const std::string& prefabPath);
        GameObject* Instantiate(bool alwaysPreserveRoot = true, bool callAwakeAndStart = false);
};
