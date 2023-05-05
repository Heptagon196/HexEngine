#pragma once
#include "HexVector.h"
#include "SDL2/SDL_ttf.h"
#include "BaseComponent.h"
#include "Renderer.h"

class Text : BaseComponent {
    private:
        static std::unordered_map<std::string, std::pair<char*, int>> fontBuffer;
        TTF_Font* ttf = nullptr;
        SDL_Color _color;
        SDL_Texture* texture;
        Renderer* render;
    public:
        static void refl();
        void Awake();
        void Start();
        void Draw();
        void OnDestroy();
        void ClearBuffers();
        void ApplyConfig();
        Vector4i TextRect();
        void SetText(const std::string& cont);
        void SetFont(const std::string& font);
        std::string text;
        std::string font;
        Vector4i color = {0, 0, 0, 255};
        Vector4i bgColor = {255, 255, 255, 0};
        int width;
        int height;
        int quality = 3; // 0: solid 1: shaded 2: lcd 3: blended
        int wrap = 0;
        int size = 16;
        int alignment = 0; // 0: left 1: center 2: right
        int outline = 0;
        bool bold = false;
        bool italic = false;
        bool underline = false;
        bool strikethrough = false;
};
