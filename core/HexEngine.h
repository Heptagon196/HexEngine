#pragma once
#include "GameConfig.h"
#include "Gos/Gos.h"
#include "HexPrefab.h"
#include "Components/HexLib.h"

struct Array {
    std::vector<SharedObject> cont;
    ReflMgr::Any operator [] (int idx);
    void insert(int idx, ReflMgr::Any val);
};

namespace HexEngine {
    class HexEngine {
        private:
            static HexEngine* currentEngine;
            Gos::GosProject prj;
            GameConfig cfg;
            std::unordered_map<std::string, HexPrefab> prefabs;
            std::unordered_map<std::string, HexPrefab> scenes;
            GameObject* root = nullptr;
            GameObject* dontDestroy = nullptr;
            void CallLifeCycleFunctions(const std::string& funcName, bool forceCall);
            bool terminated = false;
            std::set<GameObject*> toBeDestroyed;
            void DestroyGameObjectInstant(GameObject* obj);
            std::string switchSceneTo;
            std::queue<std::pair<GameObject*, GameObject*>> setParentOps;
            void PrintGameObjectStructure(GameObject* root, int indent);
            void ProcessAfterLateUpdate();
            int currentFrame;
            void (*func)() = nullptr;
        public:
            std::unordered_map<int, GameObject*> objIDMap;
            static HexEngine& Instance();
            static SharedObject NewObj(const std::string& name);
            int Frame();
            GameConfig& config();
            GameObject& GameRoot();
            GameObject& DontDestroyRoot();
            GameObject& CreateGameObject(GameObject& parent, const std::string& name, const std::string& prefab);
            HexPrefab& GetPrefab(const std::string& prefab);
            HexPrefab& GetScene(const std::string& prefab);
            void LoadScene(const std::string& scene);
            GameObject& LoadSceneAsGameObject(GameObject& parent, const std::string& scene);
            void DontDestroyOnLoad(GameObject& obj);
            void DestroyGameObject(GameObject* obj, bool instant = false);
            void SetParent(GameObject& obj, GameObject& newParent);
            void SetAsDefault();
            void AddModules(void (*func)());
            void InitGame();
            void StartGame();
            void QuitGame();
            ~HexEngine();
    };
}
