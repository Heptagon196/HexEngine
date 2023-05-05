#include "core/HexEngine.h"
#include "modules/Modules.h"
#if defined(_MSC_VER)
#include <Windows.h>
#endif

int main(int argc, char* argv[]) {
    if (argc > 2 && strcmp(argv[1], "pack") == 0) {
        Gos::FileSystem::AddDirectory(".");
        Gos::FileSystem::PackDirectory(argv[2], "data.pck");
        return 0;
    }
#if defined(_MSC_VER)
    if (!(argc > 1 && strcmp(argv[1], "debug") == 0)) {
        HWND windowHandle = GetConsoleWindow();
        ShowWindow(windowHandle, SW_HIDE);
    }
#endif
    HexEngine::HexEngine engine;
    engine.AddModules(Modules::ExportModules);
    engine.InitGame();
    engine.StartGame();
    return 0;
}
