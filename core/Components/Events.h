#pragma once
#include "HexVector.h"
#include "unordered_set"
#include "BaseComponent.h"

class Events : BaseComponent {
    private:
        static std::unordered_set<int> downKeys;
        static std::unordered_set<int> pressingKeys;
        static std::unordered_set<int> upKeys;
        static int mouseButtonClicks[3][2]; // left, middle, right; down, up
    public:
        static Vector2i mousePos;
        static void refl();
        void EarlyUpdate();
        static bool KeyDown(const int key);
        static bool Key(const int key);
        static bool KeyUp(const int key);
        static int LeftMouseDown();
        static int LeftMouseUp();
        static int MiddleMouseDown();
        static int MiddleMouseUp();
        static int RightMouseDown();
        static int RightMouseUp();
};
