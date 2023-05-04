#pragma once
#include "Gos/Reflection/ReflMgr.h"
#include "FileSystem.h"

namespace HexEngine {
    struct GameConfig {
        ObjectPtr self;
        static void refl();
        GameConfig();
        std::string gameTitle;
        std::string startScene;
        std::string screenWidth;
        std::string screenHeight;
        std::string frame;
        void ReadConfig(Path cfg);
    };
}
