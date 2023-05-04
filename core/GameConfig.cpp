#include "GameConfig.h"
#include "Utils/StringUtils.h"
#include "Gos/Reflection/ReflMgr.h"
#include "Gos/Reflection/JSON.h"

void HexEngine::GameConfig::refl() {
    auto& mgr = ReflMgr::Instance();
    mgr.AddClass<GameConfig>();
    mgr.AddField(&GameConfig::gameTitle, "gameTitle");
    mgr.AddField(&GameConfig::startScene, "startScene");
    mgr.AddField(&GameConfig::screenWidth, "screenWidth");
    mgr.AddField(&GameConfig::screenHeight, "screenHeight");
    mgr.AddField(&GameConfig::frame, "frame");
}

HexEngine::GameConfig::GameConfig() {
    self = ObjectPtr(TypeID::get<GameConfig>(), (void*)this);
}

void HexEngine::GameConfig::ReadConfig(Path cfg) {
    auto reader = FileSystem::Read(cfg);
    JSON cont;
    reader >> cont;
    cont.Foreach(std::function([this](std::string_view key, JSON& item) {
        self.GetField(key).As<std::string>() = StringUtils::ToString(item);
    }));
    startScene = StringUtils::Path(startScene);
    std::cout << "Title: " << gameTitle << std::endl;
    std::cout << "Width: " << screenWidth << std::endl;
    std::cout << "Height: " << screenHeight << std::endl;
    std::cout << "StartScene: " << startScene << std::endl;
    std::cout << "Frame: " << frame << std::endl;
}
