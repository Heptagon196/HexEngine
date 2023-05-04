#include "FileSystem.h"
#include "Gos/Reflection/ReflMgr.h"
#include "Gos/GosVM.h"
#include "HexPrefab.h"
#include "Utils/StringUtils.h"

void HexPrefab::refl() {
    auto& mgr = ReflMgr::Instance();
    mgr.AddClass<HexPrefab>();
    mgr.AddMethod(&HexPrefab::__ctor, "__ctor");
    mgr.AddMethod(&HexPrefab::Instantiate, "Instantiate");
}

HexPrefab::HexPrefab() {}

HexPrefab::HexPrefab(std::istream&& reader) {
    reader >> cont;
}

void HexPrefab::__ctor(const std::string& prefabPath) {
    HexEngine::FileSystem::Read(HexEngine::Path(prefabPath)) >> cont;
}

/* Prefab example
[
    {
        name: "Test",
        transform: {
            position: [0, 0],
        },
        components: [
            {
                name: "Test",
                output: "Hello World!",
            }
        ],
    }
]
*/
GameObject* HexPrefab::Instantiate() {
    GameObject* root = new GameObject();
    cont.Foreach(std::function([&root](int idx, JSON& cont) {
        // Basics
        GameObject* newObj = new GameObject();
        newObj->name = cont["name"].ToString();
        std::cout << "Instantiate gameObject " << newObj->name << std::endl;
        newObj->transform.parent = &root->transform;
        root->transform.childs.push_back(&newObj->transform);

        // Transform
        JSON& transformData = cont["transform"];
        newObj->transform.position.x = HexEngine::StringUtils::To<float>(transformData["position"][0]);
        newObj->transform.position.y = HexEngine::StringUtils::To<float>(transformData["position"][1]);
        std::cout << "Transform: " << newObj->transform.position.x << ' ' << newObj->transform.position.y << std::endl;
        if (transformData.HasKey("size")) {
            newObj->transform.size.x = HexEngine::StringUtils::To<float>(transformData["size"][0]);
            newObj->transform.size.y = HexEngine::StringUtils::To<float>(transformData["size"][1]);
        }
        if (transformData.HasKey("pivot")) {
            newObj->transform.pivot.x = HexEngine::StringUtils::To<float>(transformData["pivot"][0]);
            newObj->transform.pivot.y = HexEngine::StringUtils::To<float>(transformData["pivot"][1]);
        }

        // Components
        JSON comps = cont["components"];
        auto& mgr = ReflMgr::Instance();
        comps.Foreach(std::function([&mgr, &newObj](int idx, JSON& cont) {
            SharedObject comp = mgr.New(HexEngine::StringUtils::ToString(cont["name"]));
            std::cout << "Add comp " << HexEngine::StringUtils::ToString(cont["name"]) << std::endl;
            cont.Foreach(std::function([&comp](std::string_view key, JSON& val) {
                ObjectPtr field = comp.GetField(key);
                std::cout << "Comp field " << key << std::endl;
                bool isNum = field.GetType() == TypeID::get<int>() || field.GetType() == TypeID::get<float>();
                if (isNum && val.content().GetType() == TypeID::get<int>()) {
                    if (field.GetType() == TypeID::get<int>()) {
                        field.As<int>() = val.content().As<int>();
                    } else {
                        field.As<float>() = val.content().As<int>();
                    }
                } else if (isNum && val.content().GetType() == TypeID::get<float>()) {
                    if (field.GetType() == TypeID::get<int>()) {
                        field.As<int>() = val.content().As<float>();
                    } else {
                        field.As<float>() = val.content().As<float>();
                    }
                } else if (field.GetType() == TypeID::get<std::string>()) {
                    field.As<std::string>() = HexEngine::StringUtils::ToString(val);
                }
            }));
            if (mgr.GetClassTag(comp.GetType()).contains("GosInstance")) {
                comp.As<GosVM::GosInstance>().field["gameObject"] = ObjectPtr(TypeID::get<GameObject>(), newObj);
            } else {
                comp.As<BaseComponent>().gameObject = newObj;
            }
            newObj->components.push_back(comp);
        }));
    }));
    if (root->transform.ChildCount() == 1) {
        root->transform.GetChild(0).parent = nullptr;
        GameObject* ret = root->transform.GetChild(0).gameObject;
        root->transform.childs.clear();
        delete root;
        return ret;
    }
    return root;
}
