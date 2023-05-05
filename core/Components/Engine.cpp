#include "Engine.h"
#include "../HexEngine.h"

void Engine::refl() {
    REG_COMP(Engine);
    REG_STATIC_METHOD(GameRoot);
    REG_STATIC_METHOD(Instantiate);
    REG_STATIC_METHOD(Destroy);
    REG_STATIC_METHOD(DontDestroyOnLoad);
    REG_STATIC_METHOD(LoadScene);
    REG_STATIC_METHOD(LoadSceneAsGameObject);
    REG_STATIC_METHOD(QuitGame);
    REG_STATIC_METHOD(FindObj);
}

GameObject& Engine::GameRoot() {
    return HexEngine::HexEngine::Instance().GameRoot();
}

GameObject& Engine::Instantiate(GameObject& parent, const std::string& name, const std::string& prefab) {
    return HexEngine::HexEngine::Instance().CreateGameObject(parent, name, prefab);
}

void Engine::Destroy(GameObject& obj) {
    HexEngine::HexEngine::Instance().DestroyGameObject(&obj);
}

void Engine::DontDestroyOnLoad(GameObject& obj) {
    HexEngine::HexEngine::Instance().DontDestroyOnLoad(obj);
}

void Engine::LoadScene(const std::string& scene) {
    HexEngine::HexEngine::Instance().LoadScene(scene);
}

void Engine::LoadSceneAsGameObject(GameObject& parent, const std::string& scene) {
    HexEngine::HexEngine::Instance().LoadSceneAsGameObject(parent, scene);
}

void Engine::QuitGame() {
    HexEngine::HexEngine::Instance().QuitGame();
}

GameObject& Engine::FindObj(int id) {
    static GameObject null;
    null.null = true;
    auto& objMap = HexEngine::HexEngine::Instance().objIDMap;
    if (!objMap.contains(id)) {
        return null;
    }
    return *objMap[id];
}
