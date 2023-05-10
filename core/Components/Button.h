#pragma once
#include "BaseComponent.h"
#include "Image.h"
#include "HexAction.h"

class Button : BaseComponent {
    private:
        static Button* buttonToPress;
        std::vector<HexAction> listeners;
        Image* img;
        bool showDown;
    public:
        static void refl();
        std::string upImage;
        std::string downImage;
        void Awake();
        void Update();
        void LateUpdate();
        void ClickButton();
        int AddListener(HexAction method);
        void RemoveListener(int id);
        void RemoveAllListeners();
};
