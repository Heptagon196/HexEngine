#include <list>
#include "FileSystem.h"
#include "Gos/Reflection/ReflMgr.h"
#include "Gos/GosVM.h"
#include "HexPrefab.h"
#include "Utils/StringUtils.h"
#include "HexEngine.h"
#include "Components/Enums.h"

void HexPrefab::refl() {
    auto& mgr = ReflMgr::Instance();
    mgr.AddClass<HexPrefab>();
    mgr.AddMethod(&HexPrefab::__ctor, "__ctor");
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
        childs: [
            {
                name: "Test",
            }
        ],
    },
    {
        name: "Another",
        prefab: "./prefab.prefab",
    }
]
*/

static inline GameObject* instantiateByJSON(GameObject* root, JSON& cont) {
    cont.Foreach(std::function([&root](int idx, JSON& cont) {
        GameObject* newObj;
        // Prefab
        bool needInit = true;
        if (cont.HasKey("prefab")) {
            std::string prefabPath = HexEngine::StringUtils::ToString(cont["prefab"]);
            prefabPath = HexEngine::StringUtils::Path(prefabPath);
            newObj = HexEngine::HexEngine::Instance().GetPrefab(prefabPath).Instantiate(false, false);
            needInit = false;
        } else {
            newObj = new GameObject();
        }

        // ID
        if (cont.HasKey("id")) {
            newObj->id = HexEngine::StringUtils::To<int>(cont["id"]);
            HexEngine::HexEngine::Instance().objIDMap[newObj->id] = newObj;
        }

        // Basics
        if (cont.HasKey("name")) {
            newObj->name = cont["name"].ToString();
        }
        newObj->transform.parent = &root->transform;
        root->transform.childs.push_back(&newObj->transform);

        // Transform
        newObj->transform.name = newObj->name;
        if (needInit) {
            newObj->transform.enabled = true;
            newObj->transform.position = {0, 0};
            newObj->transform.pivot = {0, 0};
            newObj->transform.size = {0, 0};
        }
        if (cont.HasKey("transform")) {
            JSON& transformData = cont["transform"];
            if (transformData.HasKey("position")) {
                newObj->transform.position.x = HexEngine::StringUtils::To<float>(transformData["position"][0]);
                newObj->transform.position.y = HexEngine::StringUtils::To<float>(transformData["position"][1]);
            }
            if (transformData.HasKey("size")) {
                newObj->transform.size.x = HexEngine::StringUtils::To<float>(transformData["size"][0]);
                newObj->transform.size.y = HexEngine::StringUtils::To<float>(transformData["size"][1]);
            }
            if (transformData.HasKey("pivot")) {
                newObj->transform.pivot.x = HexEngine::StringUtils::To<float>(transformData["pivot"][0]);
                newObj->transform.pivot.y = HexEngine::StringUtils::To<float>(transformData["pivot"][1]);
            }
            if (transformData.HasKey("angle")) {
                newObj->transform.angle = HexEngine::StringUtils::To<float>(transformData["angle"]);
            }
        } else {
            newObj->transform.position.x = 0;
            newObj->transform.position.y = 0;
        }

        if (cont.HasKey("enabled")) {
            auto enable = cont["enabled"].ToString();
            if (enable == "true") {
                newObj->enabled = true;
            } else {
                newObj->enabled = false;
            }
        } else {
            newObj->enabled = true;
        }

        // Components
        if (cont.HasKey("components")) {
            JSON comps = cont["components"];
            auto& mgr = ReflMgr::Instance();
            comps.Foreach(std::function([&mgr, &newObj](int idx, JSON& cont) {
                std::string& compName = cont["name"].content().As<std::string>();
                SharedObject comp;
                int pos = -1;
                for (int i = 0; i < newObj->components.size(); i++) {
                    if (newObj->components[i].GetType().getName() == compName) {
                        pos = i;
                        break;
                    }
                }
                if (pos == -1) {
                    comp = HexEngine::HexEngine::NewObj(compName);
                    newObj->components.push_back(comp);
                } else {
                    comp = newObj->components[pos];
                }
                if (!cont.HasKey("enabled") && pos == -1) {
                    comp.GetField("enabled").As<bool>() = true;
                }
                cont.Foreach(std::function([&comp](std::string_view key, JSON& val) {
                    ObjectPtr field = comp.GetField(key);
                    bool isNum = field.GetType() == TypeID::get<int>() || field.GetType() == TypeID::get<float>();
#define VAL(type, idx) idx >= val.VecSize() ? 0 : HexEngine::StringUtils::To<type>(val[idx])
                    if (field.GetType() == TypeID::get<bool>()) {
                        field.As<bool>() = HexEngine::StringUtils::ToString(val) == "true";
                    } else if (field.GetType() == TypeID::get<Vector2i>()) {
                        Vector2i& vec = field.As<Vector2i>();
                        vec.x = VAL(int, 0);
                        vec.y = VAL(int, 1);
                    } else if (field.GetType() == TypeID::get<Vector2f>()) {
                        Vector2f& vec = field.As<Vector2f>();
                        vec.x = VAL(float, 0);
                        vec.y = VAL(float, 1);
                    } else if (field.GetType() == TypeID::get<Vector3i>()) {
                        Vector3i& vec = field.As<Vector3i>();
                        vec.x = VAL(int, 0);
                        vec.y = VAL(int, 1);
                        vec.z = VAL(int, 2);
                    } else if (field.GetType() == TypeID::get<Vector3f>()) {
                        Vector3f& vec = field.As<Vector3f>();
                        vec.x = VAL(float, 0);
                        vec.y = VAL(float, 1);
                        vec.z = VAL(float, 2);
                    } else if (field.GetType() == TypeID::get<Vector4i>()) {
                        Vector4i& vec = field.As<Vector4i>();
                        vec.x = VAL(int, 0);
                        vec.y = VAL(int, 1);
                        vec.z = VAL(int, 2);
                        vec.w = VAL(int, 3);
                    } else if (field.GetType() == TypeID::get<Vector4f>()) {
                        Vector4f& vec = field.As<Vector4f>();
                        vec.x = VAL(float, 0);
                        vec.y = VAL(float, 1);
                        vec.z = VAL(float, 2);
                        vec.w = VAL(float, 3);
                    } else if (isNum && val.content().GetType() == TypeID::get<int>()) {
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
                    } else if (field.GetType().getHash() == TypeID::get<int>().getHash()
                            && val.content().GetType().getHash() == TypeID::get<std::string>().getHash()) {
                        std::string num = HexEngine::StringUtils::ToString(val.content());
                        if (num.length() > 0 && (num[0] == '-' || num[0] == '+' || isdigit(num[0]))) {
                            field.As<int>() = HexEngine::StringUtils::To<int>(num);
                        } else {
                            field.As<int>() = ObjectPtr{ TypeID::get<Enums>(), nullptr }.GetField(num).As<int>();
                        }
                    } else if (field.GetType().getHash() == TypeID::get<Array>().getHash()) {
                        Array& arr = field.As<Array>();
                        val.Foreach(std::function([&arr](int idx, JSON& cont) {
                            arr.cont.push_back(cont.content());
                        }));
                    }
                }));
#undef VAL
                if (mgr.GetClassTag(comp.GetType()).contains("GosInstance")) {
                    comp.As<GosVM::GosInstance>().field["gameObject"] = ObjectPtr(TypeID::get<GameObject>(), newObj);
                } else {
                    comp.As<BaseComponent>().gameObject = newObj;
                }
            }));
        }

        // childs
        if (cont.HasKey("childs")) {
            JSON childs = cont["childs"];
            instantiateByJSON(newObj, childs);
        }
    }));
    return root;
}

GameObject* HexPrefab::Instantiate(bool alwaysPreserveRoot, bool callAwakeAndStart) {
    GameObject* root = new GameObject();
    root->name = "root";
    root->enabled = true;
    root->transform.enabled = true;
    root->transform.name = "root";
    root->transform.parent = nullptr;
    instantiateByJSON(root, cont);
    if (!alwaysPreserveRoot && root->transform.ChildCount() == 1) {
        root->transform.GetChild(0).parent = nullptr;
        GameObject* ret = root->transform.GetChild(0).gameObject;
        root->transform.childs.clear();
        delete root;
        if (callAwakeAndStart) {
            ret->Broadcast("Awake", true);
            ret->Broadcast("Start", true);
        }
        return ret;
    }
    if (callAwakeAndStart) {
        root->Broadcast("Awake", true);
        root->Broadcast("Start", true);
    }
    return root;
}
