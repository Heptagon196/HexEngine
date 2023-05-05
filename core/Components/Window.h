#pragma once
#include "SDL2/SDL.h"
#include "BaseComponent.h"

class Renderer;

class Window : BaseComponent {
    friend class Renderer;
    private:
        SDL_Window* window;
        static Window* mainWindow;
    public:
        static void refl();
        static Window& main();
        bool useConfig = true;
        std::string title;
        int width;
        int height;
        int centered;
        int windowType;
        // 0: Normal
        // 1: Maximized
        // 2: Fullscreen
        // 3: Fullscreen desktop
        void Awake();
        void OnDestroy();
};
