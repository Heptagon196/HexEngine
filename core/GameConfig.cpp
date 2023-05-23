#include "GameConfig.h"
#include "Utils/StringUtils.h"
#include "Gos/Reflection/ReflMgr.h"
#include "Gos/Reflection/JSON.h"
#include "Components/BaseComponent.h"

HexEngine::GameConfig* HexEngine::GameConfig::instance = nullptr;

HexEngine::GameConfig& HexEngine::GameConfig::content() {
    return *instance;
}

HexEngine::GameConfig::GameConfig() {
    if (instance == nullptr) {
        instance = this;
    }
}

void HexEngine::GameConfig::refl() {
    auto& mgr = ReflMgr::Instance();
    mgr.AddClass<GameConfig>();
    mgr.AddAliasClass(TypeID::get<GameConfig>().getName(), "GameConfig");
    mgr.AddStaticMethod(TypeID::get<GameConfig>(), &GameConfig::content, "content");
    mgr.AddMethod(&GameConfig::GetString, "Get");
    mgr.AddMethod(&GameConfig::GetBool, "GetBool");
    mgr.AddMethod(&GameConfig::Get<int>, "GetInt");
    mgr.AddMethod(&GameConfig::Get<float>, "GetFloat");
}

void HexEngine::GameConfig::ReadConfig(Path cfg) {
    while (true) {
        auto reader = FileSystem::Read(cfg);
        JSON cont;
        reader >> cont;
        cont.Foreach(std::function([this](std::string_view key, JSON& item) {
            if (!config.contains((std::string)key)) {
                config[(std::string)key] = StringUtils::ToString(item);
            }
        }));
        if (config.contains("base")) {
            cfg = Path(config["base"]);
            config.erase("base");
            std::cout << "Load sub config " << cfg.string() << std::endl;
        } else {
            break;
        }
    }
}

std::string HexEngine::GameConfig::GetString(const std::string& key, const std::string& defaultVal) {
    if (config.find(key) == config.end()) {
        return defaultVal;
    }
    return config[key];
}

bool HexEngine::GameConfig::GetBool(const std::string& key, bool defaultVal) {
    if (config.find(key) == config.end()) {
        return defaultVal;
    }
    return config[key] == "true";
}
