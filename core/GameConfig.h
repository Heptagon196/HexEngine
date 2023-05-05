#pragma once
#include "Gos/Reflection/ReflMgr.h"
#include "FileSystem.h"
#include "Utils/StringUtils.h"

namespace HexEngine {
    struct GameConfig {
        static void refl();
        static GameConfig* instance;
        GameConfig();
        static GameConfig& content();
        std::unordered_map<std::string, std::string> config;
        void ReadConfig(Path cfg);
        std::string GetString(const std::string& key, const std::string& defaultVal);
        bool GetBool(const std::string& key, bool defaultVal);
        template<typename T>
        T Get(const std::string& key, T defaultValue) {
            if (config.find(key) == config.end()) {
                return defaultValue;
            }
            return StringUtils::To<T>(config[key]);
        }
    };
}
