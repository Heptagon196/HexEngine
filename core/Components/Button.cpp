#include "Button.h"
#include "../GameObject.h"
#include "Events.h"
#include "Math.h"

Button* Button::buttonToPress = nullptr;

void Button::refl() {
    REG_COMP(Button);
    REG_FIELD(upImage);
    REG_FIELD(downImage);
    REG_METHOD(ClickButton);
    REG_METHOD(AddListener);
    REG_METHOD(RemoveListener);
    REG_METHOD(RemoveAllListeners);
}

void Button::ClickButton() {
    for (auto listener : listeners) {
        listener(ObjectPtr::Null);
    }
}

int Button::AddListener(HexAction method) {
    listeners.push_back(method);
    return listeners.size() - 1;
}

void Button::RemoveListener(int id) {
    listeners.erase(listeners.begin() + id);
}

void Button::RemoveAllListeners() {
    listeners.clear();
}

void Button::Awake() {
    if (!gameObject->HasComponent("Image")) {
        std::cout << "Error: component Button needs component Image" << std::endl;
        enabled = false;
        return;
    }
    img = &gameObject->GetComponent("Image").As<Image>();
    img->image = upImage;
    showDown = false;
}

void Button::Update() {
    if (Math::PointInRecti(Events::mousePos, img->ImageRect())) {
        buttonToPress = this;
    }
    if (showDown && Events::LeftMouseUp()) {
        img->SetImage(upImage);
        showDown = false;
    }
}

void Button::LateUpdate() {
    if (buttonToPress != nullptr) {
        if (Events::LeftMouseDown()) {
            buttonToPress->img->SetImage(downImage);
            buttonToPress->showDown = true;
        } else if (Events::LeftMouseUp()) {
            buttonToPress->ClickButton();
        }
        buttonToPress = nullptr;
    }
}
