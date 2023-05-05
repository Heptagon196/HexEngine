#pragma once
#include "Collider.h"
#include "SDL2/SDL.h"

class BoxCollider : public Collider {
    private:
        SDL_Texture* colliderTex = nullptr;
        void GenerateColliderTex();
    public:
        static void refl();
        bool showCollider = false;
        float width;
        float height;
        void SetCollider(float width, float height);
        void ShowCollider(bool show);
        void Draw();
        void Awake();
        void OnDestroy();
};
