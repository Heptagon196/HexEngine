#pragma once
#include "SDL2/SDL.h"
#include "BaseComponent.h"
#include "HexVector.h"
#include "Window.h"

class Renderer : BaseComponent {
    private:
        struct ImageInfo {
            SDL_Texture* tex;
            int width;
            int height;
        };
        SDL_Renderer* renderer;
        Window* window;
        static Renderer* mainRenderer;
        std::unordered_map<std::string, ImageInfo> imageBuffer;
    public:
        static void refl();
        static Renderer& main();
        void Awake();
        void EarlyUpdate();
        void AfterDraw();
        void OnDestroy();
        void RenderTo(Window& target);
        Window& Target();
        Vector4i GetDrawColor();
        void SetDrawColor(Vector4i color);
        void Clear();
        void DrawLine(Vector2i a, Vector2i b);
        void DrawRect(Vector2i upperLeft, Vector2i size);
        void FillRect(Vector2i upperLeft, Vector2i size);
        void RenderTexture(SDL_Texture& texture, Vector4i dest);
        void RenderTextureEx(SDL_Texture& texture, Vector4i dest, float angle, Vector2i center, bool flipX, bool flipY);
        SDL_Renderer& GetRenderer();
        SDL_Texture* LoadImage(const std::string& path, int& width, int& height);
        void UnloadImage(const std::string& path);
        void UnloadAllImages();
};
