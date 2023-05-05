#include "SDL2/SDL.h"
#include "../HexEngine.h"
#include "Events.h"

std::unordered_set<int> Events::downKeys;
std::unordered_set<int> Events::pressingKeys;
std::unordered_set<int> Events::upKeys;
int Events::mouseButtonClicks[3][2];
Vector2i Events::mousePos;

void Events::refl() {
    REG_COMP(Events);
    REG_STATIC_FIELD(mousePos);
    REG_STATIC_METHOD(KeyDown);
    REG_STATIC_METHOD(Key);
    REG_STATIC_METHOD(KeyUp);
    REG_STATIC_METHOD(LeftMouseDown);
    REG_STATIC_METHOD(LeftMouseUp);
    REG_STATIC_METHOD(MiddleMouseDown);
    REG_STATIC_METHOD(MiddleMouseUp);
    REG_STATIC_METHOD(RightMouseDown);
    REG_STATIC_METHOD(RightMouseUp);
}

void Events::EarlyUpdate() {
    downKeys.clear();
    upKeys.clear();
    memset(mouseButtonClicks, 0, sizeof(mouseButtonClicks));
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        mousePos.x = event.button.x;
        mousePos.y = event.button.y;
        switch (event.type) {
            case SDL_KEYDOWN:
                downKeys.insert(event.key.keysym.sym);
                pressingKeys.insert(event.key.keysym.sym);
                break;
            case SDL_KEYUP:
                upKeys.insert(event.key.keysym.sym);
                if (pressingKeys.contains(event.key.keysym.sym)) {
                    pressingKeys.erase(event.key.keysym.sym);
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mouseButtonClicks[0][0] = event.button.clicks;
                } else if (event.button.button == SDL_BUTTON_MIDDLE) {
                    mouseButtonClicks[1][0] = event.button.clicks;
                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                    mouseButtonClicks[2][0] = event.button.clicks;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mouseButtonClicks[0][1] = event.button.clicks;
                } else if (event.button.button == SDL_BUTTON_MIDDLE) {
                    mouseButtonClicks[1][1] = event.button.clicks;
                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                    mouseButtonClicks[2][1] = event.button.clicks;
                }
                break;
            case SDL_QUIT: HexEngine::HexEngine::Instance().QuitGame(); break;
        }
    }
}

bool Events::KeyDown(const int key) {
    return downKeys.contains(key);
}

bool Events::Key(const int key) {
    return pressingKeys.contains(key);
}

bool Events::KeyUp(const int key) {
    return upKeys.contains(key);
}

int Events::LeftMouseDown() {
    return mouseButtonClicks[0][0];
}

int Events::LeftMouseUp() {
    return mouseButtonClicks[0][1];
}

int Events::MiddleMouseDown() {
    return mouseButtonClicks[1][0];
}

int Events::MiddleMouseUp() {
    return mouseButtonClicks[1][1];
}

int Events::RightMouseDown() {
    return mouseButtonClicks[2][0];
}

int Events::RightMouseUp() {
    return mouseButtonClicks[2][1];
}
