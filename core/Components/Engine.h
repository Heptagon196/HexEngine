#pragma once
#include "BaseComponent.h"

class Engine : BaseComponent {
    public:
        static void refl();
        static GameObject& GameRoot();
        static GameObject& Instantiate(GameObject& parent, const std::string& name, const std::string& prefab);
        static GameObject& FindObj(int id);
        static void Destroy(GameObject& obj);
        static void DontDestroyOnLoad(GameObject& obj);
        static void LoadScene(const std::string& scene);
        static void LoadSceneAsGameObject(GameObject& parent, const std::string& scene);
        static void QuitGame();
};
