#pragma once
#include "BaseComponent.h"
#include "Renderer.h"

class Image : BaseComponent {
    private:
        Renderer* render;
        SDL_Texture* texture;
    public:
        static void refl();
        std::string image;
        int width;
        int height;
        float scale = 1;
        bool flipX;
        bool flipY;
        Vector4i ImageRect();
        void SetImage(const std::string& newFile);
        void Start();
        void Draw();
        void OnDestroy();
        void RenderTo(Renderer& renderer);
};
