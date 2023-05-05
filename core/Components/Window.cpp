#include "../HexEngine.h"
#include "../Utils/StringUtils.h"
#include "Window.h"
#include "Engine.h"

Window* Window::mainWindow = nullptr;

void Window::refl() {
    REG_COMP(Window);
    REG_FIELD(useConfig);
    REG_FIELD(title);
    REG_FIELD(width);
    REG_FIELD(height);
    REG_FIELD(centered);
    REG_FIELD(windowType);
    REG_STATIC_METHOD(main);
}

Window& Window::main() {
    return *mainWindow;
}

void Window::Awake() {
    Engine::DontDestroyOnLoad(*gameObject);
    if (mainWindow == nullptr) {
        mainWindow = this;
    }
    if (useConfig) {
        HexEngine::GameConfig& cfg = HexEngine::HexEngine::Instance().config();
        title = cfg.GetString("gameTitle", "");
        width = cfg.Get<int>("screenWidth", 1080);
        height = cfg.Get<int>("screenHeight", 720);
        centered = cfg.GetBool("centered", false);
        std::string type = cfg.GetString("windowType", "Normal");
        if (type == "Normal") {
            windowType = 0;
        } else if (type == "Maximized") {
            windowType = 1;
        } else if (type == "FullScreen") {
            windowType = 2;
        } else if (type == "FullScreenDesktop") {
            windowType = 3;
        }
    }
    Uint32 flags = SDL_WINDOW_OPENGL;
    switch (windowType) {
        case 0: flags |= 0; break;
        case 1: flags |= SDL_WINDOW_MAXIMIZED; break;
        case 2: flags |= SDL_WINDOW_FULLSCREEN; break;
        case 3: flags |= SDL_WINDOW_FULLSCREEN_DESKTOP; break;
    }
    int x = SDL_WINDOWPOS_UNDEFINED;
    int y = SDL_WINDOWPOS_UNDEFINED;
    if (centered) {
        x = SDL_WINDOWPOS_CENTERED;
        y = SDL_WINDOWPOS_CENTERED;
    }
    window = SDL_CreateWindow(title.c_str(), x, y, width, height, flags);
    if (!gameObject->HasComponent("Events")) {
        gameObject->AddComponent("Events");
    }
    if (!gameObject->HasComponent("Renderer")) {
        gameObject->AddComponent("Renderer").Invoke("RenderTo", { ObjectPtr(TypeID::get<Window>(), this) });
    } else {
        gameObject->GetComponent("Renderer").Invoke("RenderTo", { ObjectPtr(TypeID::get<Window>(), this) });
    }
}

void Window::OnDestroy() {
    SDL_DestroyWindow(window);
}
