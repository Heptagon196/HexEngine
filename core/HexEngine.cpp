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

HexEngine::HexEngine* HexEngine::HexEngine::HexEngine::currentEngine;

struct unknown {};
struct Array {
    std::vector<SharedObject> cont;
    ReflMgr::Any operator [] (int idx) {
        return cont[idx].ToObjectPtr();
    }
    void insert(int idx, ReflMgr::Any val) {
        std::cout << "Insert " << val << " to " << idx << std::endl;
        cont[idx] = val;
    }
};

void HexEngine::HexEngine::InitGame() {
    std::cout << "Start engine" << std::endl;
    SetAsDefault();
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
    mgr.AddMethod(&Array::insert, "insert");
    mgr.AddMethod<SharedObject>(std::function([&mgr](SharedObject* self, ReflMgr::Any val) -> decltype(auto) {
        (*self) = mgr.New(val.GetType());
        return self->assign(val);
    }), "__assign");
    mgr.AddAliasClass("any", TypeID::get<ReflMgr::Any>().getName());
    mgr.AddAliasClass("array", "Array");
    mgr.AddAliasClass("string", TypeID::get<std::string>().getName());
    mgr.AddMethod(std::function([](std::vector<SharedObject>* self, int size) { self->resize(size); }), "resize");

    std::cout << "Reflect engine library" << std::endl;
    // Reflect engine library
    GameConfig::refl();
    GameObject::refl();
    HexPrefab::refl();
    Debug::refl();
    ReflectBaseComponents();

    std::cout << "Load project files" << std::endl;
    // Load project files. Load scripts
    std::cout << "Path " << std::filesystem::current_path().string() << std::endl;
    FileSystem::AddDirectory(".");
    if (std::filesystem::exists("data.pck")) {
        std::cout << "Load data.pck" << std::endl;
        FileSystem::AddPack("data.pack");
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
    std::cout << "Load scene " << cfg.startScene << std::endl;
    if (root != nullptr) {
        DestroyGameObject(root);
    }
    root = scenes[cfg.startScene].Instantiate();
}

void HexEngine::HexEngine::DestroyGameObject(GameObject* obj) {
    for (auto* transform : obj->transform.childs) {
        DestroyGameObject(transform->gameObject);
    }
    auto& parent = obj->transform.parent->childs;
    for (int i = 0; i < parent.size(); i++) {
        if (parent[i] == &obj->transform) {
            parent.erase(parent.begin() + i);
            break;
        }
    }
    delete obj;
}

void HexEngine::HexEngine::SetAsDefault() {
    currentEngine = this;
}

HexEngine::HexEngine& HexEngine::HexEngine::Instance() {
    return *currentEngine;
}

HexEngine::HexEngine::~HexEngine() {
    if (root != nullptr) {
        DestroyGameObject(root);
    }
}

void HexEngine::HexEngine::CallLifeCycleFunctions(const std::string& funcName) {
    std::stack<Transform*> st;
    st.push(&root->transform);
    while (!st.empty()) {
        auto& childs = st.top()->childs;
        auto& comps = st.top()->gameObject->components;
        st.pop();
        for (auto& comp : comps) {
            comp.TryInvoke(funcName);
        }
        for (auto* child : childs) {
            st.push(child);
        }
    }
}

void HexEngine::HexEngine::StartGame() {
    int frame = StringUtils::To<int>(cfg.frame);
    std::chrono::duration<double> d(1.0 / frame);
    CallLifeCycleFunctions("Start");
    while (true) {
        std::chrono::steady_clock::time_point loopStart = std::chrono::steady_clock::now();
        CallLifeCycleFunctions("Update");
        std::chrono::duration<double> used = std::chrono::steady_clock::now() - loopStart;
        if (used > d) {
            continue;
        }
        std::this_thread::sleep_for(d - used);
    }
}
