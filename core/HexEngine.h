#pragma once
#include "GameConfig.h"
#include "Gos/Gos.h"
#include "HexPrefab.h"

namespace HexEngine {
    class HexEngine {
        private:
            static HexEngine* currentEngine;
            Gos::GosProject prj;
            GameConfig cfg;
            std::unordered_map<std::string, HexPrefab> prefabs;
            std::unordered_map<std::string, HexPrefab> scenes;
            GameObject* root = nullptr;
            void CallLifeCycleFunctions(const std::string& funcName);
        public:
            void DestroyGameObject(GameObject* obj);
            static HexEngine& Instance();
            void SetAsDefault();
            void InitGame();
            void StartGame();
            ~HexEngine();
    };
}
