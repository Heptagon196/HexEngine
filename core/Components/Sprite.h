#pragma once
#include "BaseComponent.h"
#include "Renderer.h"
#include "Camera.h"

class Sprite : BaseComponent {
    private:
        Renderer* render;
        Camera* camera;
        SDL_Texture* texture;
    public:
        static void refl();
        std::string sprite;
        float width;
        float height;
        bool flipX;
        bool flipY;
        float scale = 1;
        Vector4f SpriteRect();
        void SetSprite(const std::string& newFile);
        void Start();
        void Draw();
        void OnDestroy();
        void RenderTo(Renderer& renderer);
        void TargetCamera(Camera& camera);
};
