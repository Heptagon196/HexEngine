#include <list>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include "HexEngine.h"
#include "Debug.h"
#include "GameConfig.h"
#include "GameObject.h"
#include "Gos/Reflection/ReflMgrInit.h"
#include "Gos/Reflection/JSON.h"
#include "Components/BaseComponentsRefl.h"
#include "Utils/StringUtils.h"
#include <chrono>
#include <thread>

using HexEngine::GameConfig;

HexEngine::HexEngine *HexEngine::HexEngine::HexEngine::currentEngine;

struct unknown {};

ReflMgr::Any Array::operator [] (int idx) {
    return cont[idx].ToObjectPtr();
}

void Array::insert(int idx, ReflMgr::Any val) {
    cont[idx] = ReflMgr::Instance().New(val.GetType());
    cont[idx].assign(val);
}

void HexEngine::HexEngine::AddModules(void (*func)()) {
    this->func = func;
}

void HexEngine::HexEngine::InitGame() {
    std::cout << "Start engine" << std::endl;
    SetAsDefault();
    std::cout << "Init SDL" << std::endl;
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
    TTF_Init();
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP | IMG_INIT_AVIF | IMG_INIT_JXL);
    Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID | MIX_INIT_OPUS);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, MIX_CHANNELS, 4096);
    std::cout << "Init reflection library" << std::endl;
    // Init reflection library
    auto& mgr = ReflMgr::Instance();
    ReflMgrTool::Init();
    JSON::Init();
    mgr.AddClass<ReflMgr::Any>();
    mgr.AddClass<std::string>();
    mgr.AddClass<unknown>();
    ReflMgrTool::AutoRegister<Array>();
    mgr.AddClass<Array>();
    mgr.AddMethod<Array>(std::function([](Array* self, int size) -> decltype(auto) {
        self->cont.resize(size);
    }), "__ctor");;
    mgr.AddMethod<Array>(std::function([](Array* self, int size) -> decltype(auto) {
        self->cont.resize(size);
    }), "resize");;
    mgr.AddMethod(&Array::insert, "insert");
    mgr.AddMethod<Array>(std::function([](Array* self) -> int { return self->cont.size(); }), "size");
    mgr.AddMethod<SharedObject>(std::function([&mgr](SharedObject* self, ReflMgr::Any val) -> decltype(auto) {
        (*self) = mgr.New(val.GetType());
        return self->assign(val);
    }), "__assign");
    mgr.AddAliasClass("any", TypeID::get<ReflMgr::Any>().getName());
    mgr.AddAliasClass("array", "Array");
    mgr.AddAliasClass("object", "SharedObject");
    mgr.AddAliasClass("string", TypeID::get<std::string>().getName());
    mgr.AddMethod(std::function([](std::vector<SharedObject>* self, int size) { self->resize(size); }), "resize");
    mgr.AddMethod(std::function([](bool* self, int val) { *self = val; }), "__ctor");

    std::cout << "Reflect engine library" << std::endl;
    // Reflect engine library
    GameConfig::refl();
    GameObject::refl();
    HexPrefab::refl();
    Debug::refl();
    ReflectBaseComponents();
    // Reflect modules
    if (func != nullptr) {
        std::cout << "Reflect modules" << std::endl;
        func();
    }

    std::cout << "Load project files" << std::endl;
    // Load project files. Load scripts
    std::cout << "Path " << std::filesystem::current_path().string() << std::endl;
    FileSystem::AddDirectory(".");
    if (std::filesystem::exists("data.pck")) {
        std::cout << "Load data.pck" << std::endl;
        FileSystem::AddPack("data.pck");
    }
    if (std::filesystem::exists("gameConfig.json")) {
        std::cout << "Load gameConfig.json" << std::endl;
        cfg.ReadConfig("gameConfig.json");
    }
    std::cout << "Load scripts" << std::endl;
    prj.ScanDirectory(".");

    std::cout << "Load prefabs" << std::endl;
    // Load prefabs
    FileSystem::Iterate(".", std::function([this](Path p) {
        if (p.string().ends_with(".prefab")) {
            std::cout << "Prefab: " << p.string() << std::endl;
            prefabs[p.string()] = HexPrefab(std::move(FileSystem::Read(p)));
        }
    }));

    std::cout << "Load scenes" << std::endl;
    // Load scenes
    FileSystem::Iterate(".", std::function([this](Path p) {
        if (p.string().ends_with(".scene")) {
            std::cout << "Scene: " << p.string() << std::endl;
            scenes[p.string()] = HexPrefab(std::move(FileSystem::Read(p)));
        }
    }));

    std::cout << "Start game" << std::endl;
    std::cout << "Load scene " << StringUtils::Path(cfg.GetString("startScene", "")) << std::endl;
    if (root != nullptr) {
        DestroyGameObjectInstant(root);
    }
    dontDestroy = new GameObject("DontDestroyOnLoad");
    dontDestroy->transform.parent = nullptr;
    root = scenes[StringUtils::Path(cfg.GetString("startScene", ""))].Instantiate();
}

void HexEngine::HexEngine::DestroyGameObjectInstant(GameObject* obj) {
    if (toBeDestroyed.contains(obj)) {
        toBeDestroyed.erase(obj);
    }
    for (auto* transform : obj->transform.childs) {
        DestroyGameObjectInstant(transform->gameObject);
    }
    for (auto& comp : obj->components) {
        comp.TryInvoke("OnDestroy");
        comp = ObjectPtr::Null;
    }
    if (obj != root && obj != dontDestroy) {
        for (int i = 0; i < obj->transform.ChildCount(); i++) {
            if (obj->transform.GetChild(i).gameObject == obj) {
                auto& vec = obj->transform.childs;
                vec.erase(vec.begin() + i);
            }
        }
    }
    delete obj;
}

void HexEngine::HexEngine::DestroyGameObject(GameObject* obj, bool instant) {
    if (instant) {
        DestroyGameObjectInstant(obj);
        return;
    }
    toBeDestroyed.insert(obj);
}

void HexEngine::HexEngine::SetAsDefault() {
    currentEngine = this;
}

HexEngine::HexEngine& HexEngine::HexEngine::Instance() {
    return *currentEngine;
}

HexEngine::HexEngine::~HexEngine() {
    if (root != nullptr) {
        DestroyGameObjectInstant(root);
        root = nullptr;
    }
    if (dontDestroy != nullptr) {
        DestroyGameObjectInstant(dontDestroy);
        dontDestroy = nullptr;
    }
}

GameConfig &HexEngine::HexEngine::config() {
    return cfg;
}

void HexEngine::HexEngine::CallLifeCycleFunctions(const std::string& funcName, bool forceCall) {
    if (dontDestroy != nullptr) {
        dontDestroy->Broadcast(funcName, forceCall);
    }
    if (root != nullptr) {
        root->Broadcast(funcName, forceCall);
    }
}

void HexEngine::HexEngine::ProcessAfterLateUpdate() {
    // Set parent
    while (!setParentOps.empty()) {
        auto& obj = *setParentOps.front().first;
        auto& newParent = *setParentOps.front().second;
        setParentOps.pop();
        auto& vec = obj.transform.parent->childs;
        for (int i = 0; i < vec.size(); i++) {
            if (vec[i]->gameObject == &obj) {
                vec.erase(vec.begin() + i);
                break;
            }
        }
        obj.transform.parent = &newParent.transform;
        newParent.transform.childs.push_back(&obj.transform);
    }

    // Destroy
    std::queue<GameObject*> destroyList;
    for (auto& obj : toBeDestroyed) {
        destroyList.push(obj);
    }
    while (!destroyList.empty()) {
        if (toBeDestroyed.contains(destroyList.front())) {
            DestroyGameObjectInstant(destroyList.front());
        }
        destroyList.pop();
    }
    toBeDestroyed.clear();

    // Switch Scene
    if (switchSceneTo != "") {
        DestroyGameObjectInstant(root);
        root = GetScene(switchSceneTo).Instantiate(true, true);
        switchSceneTo = "";
    }
}

int HexEngine::HexEngine::Frame() {
    return currentFrame;
}

void HexEngine::HexEngine::StartGame() {
    int frame = cfg.Get<int>("frame", 30);
    currentFrame = frame;
    int delayTime = 1000.0 / frame;
    Uint32 frameStart, frameTime;

    std::cout << "Hex: Awake" << std::endl;
    CallLifeCycleFunctions("Awake", true);

    std::cout << "Hex: Start" << std::endl;
    CallLifeCycleFunctions("Start", true);

    ProcessAfterLateUpdate();

    std::cout << "Hex: Game Loop" << std::endl;
    while (!terminated) {
        frameStart = SDL_GetTicks();

        // LifeCycle
        CallLifeCycleFunctions("EarlyUpdate", false);
        CallLifeCycleFunctions("PhysicsUpdate", false);
        CallLifeCycleFunctions("Update", false);
        CallLifeCycleFunctions("LateUpdate", false);
        CallLifeCycleFunctions("Draw", false);
        CallLifeCycleFunctions("AfterDraw", false);

        ProcessAfterLateUpdate();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < delayTime) {
            SDL_Delay(delayTime - frameTime);
        }
    }
    if (root != nullptr) {
        DestroyGameObjectInstant(root);
        root = nullptr;
    }
    if (dontDestroy != nullptr) {
        DestroyGameObjectInstant(dontDestroy);
        dontDestroy = nullptr;
    }
    Mix_CloseAudio();
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void HexEngine::HexEngine::QuitGame() {
    terminated = true;
}

GameObject& HexEngine::HexEngine::GameRoot() {
    return *root;
}

GameObject& HexEngine::HexEngine::DontDestroyRoot() {
    return *dontDestroy;
}

GameObject& HexEngine::HexEngine::CreateGameObject(GameObject& parent, const std::string& name, const std::string& prefab) {
    if (!prefabs.contains(prefab)) {
        std::cerr << "No such prefab: " << prefab << std::endl;
    }
    GameObject* obj = prefabs[prefab].Instantiate(false, true);
    obj->transform.parent = &parent.transform;
    parent.transform.childs.push_back(&obj->transform);
    obj->name = name;
    obj->transform.name = name;
    return *obj;
}

HexPrefab& HexEngine::HexEngine::GetPrefab(const std::string& prefab) {
    if (!prefabs.contains(prefab)) {
        std::cerr << "No such prefab: " << prefab << std::endl;
    }
    return prefabs[prefab];
}

HexPrefab& HexEngine::HexEngine::GetScene(const std::string& scene) {
    if (!scenes.contains(scene)) {
        std::cerr << "No such scene: " << scene << std::endl;
    }
    return scenes[scene];
}

void HexEngine::HexEngine::LoadScene(const std::string& scene) {
    switchSceneTo = StringUtils::Path(scene);
    if (!scenes.contains(switchSceneTo)) {
        std::cerr << "No such scene: " << scene << std::endl;
        switchSceneTo = "";
    }
}

GameObject& HexEngine::HexEngine::LoadSceneAsGameObject(GameObject& parent, const std::string& scene) {
    GameObject* obj = GetScene(scene).Instantiate(true, true);
    obj->transform.parent = &parent.transform;
    parent.transform.childs.push_back(&obj->transform);
    return *obj;
}

void HexEngine::HexEngine::DontDestroyOnLoad(GameObject& obj) {
    SetParent(obj, *dontDestroy);
}

void HexEngine::HexEngine::SetParent(GameObject& obj, GameObject& newParent) {
    if (&obj == root || &obj == dontDestroy) {
        return;
    }
    setParentOps.push({&obj, &newParent});
}

void HexEngine::HexEngine::PrintGameObjectStructure(GameObject* root, int indent) {
    if (root == nullptr) {
        return;
    }
    for (int i = 0; i < indent * 2; i++) {
        std::cout << ' ';
    }
    std::cout << root->name << " with components ";
    for (int i = 0; i < root->components.size(); i++) {
        std::cout << root->components[i].GetType().getName() << " ";
    }
    std::cout << std::endl;
    for (Transform* child : root->transform.childs) {
        PrintGameObjectStructure(child->gameObject, indent + 1);
    }
}

static inline std::list<std::string> names;
static inline std::unordered_map<std::string, std::list<std::string>::iterator> nameIdx;
SharedObject HexEngine::HexEngine::NewObj(const std::string& name) {
    if (nameIdx.find(name) == nameIdx.end()) {
        names.push_back(name);
        nameIdx[name] = std::prev(names.end());
        return ReflMgr::Instance().New(names.back());
    } else {
        return ReflMgr::Instance().New(*nameIdx[name]);
    }
}
