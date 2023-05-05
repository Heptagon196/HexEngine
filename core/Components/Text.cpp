#include "Text.h"
#include "../HexEngine.h"

std::unordered_map<std::string, std::pair<char*, int>> Text::fontBuffer;

void Text::refl() {
    REG_COMP(Text);
    REG_FIELD(text);
    REG_FIELD(width);
    REG_FIELD(height);
    REG_FIELD(color);
    REG_FIELD(bgColor);
    REG_FIELD(font);
    REG_FIELD(size);
    REG_FIELD(quality);
    REG_FIELD(wrap);
    REG_FIELD(alignment);
    REG_FIELD(outline);
    REG_FIELD(bold);
    REG_FIELD(italic);
    REG_FIELD(underline);
    REG_FIELD(strikethrough);
    REG_METHOD(ApplyConfig);
    REG_METHOD(TextRect);
    REG_METHOD(SetText);
    REG_METHOD(SetFont);
    REG_METHOD(ClearBuffers);
}

void Text::Awake() {
    ttf = nullptr;
    if (font != "") {
        SetFont(font);
    }
}

void Text::Start() {
    render = &Renderer::main();
    if (text != "") {
        SetText(text);
    }
}

void Text::Draw() {
    render->RenderTexture(*texture, TextRect());
}

void Text::OnDestroy() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    ClearBuffers();
}

void Text::ClearBuffers() {
    for (auto& fnt : fontBuffer) {
        delete[] fnt.second.first;
    }
    fontBuffer.clear();
}

Vector4i Text::TextRect() {
    Vector2f pos = gameObject->transform.worldPos();
    int x = pos.x - width * gameObject->transform.pivot.x;
    int y = pos.y - height * gameObject->transform.pivot.y;
    return Vector4i(x, y, width, height);
}

void Text::SetText(const std::string& cont) {
    SDL_Surface* surface;
    text = cont;
    switch (quality) {
        case 0: surface = TTF_RenderUTF8_Solid_Wrapped(ttf, text.c_str(), color.color(), wrap); break;
        case 1: surface = TTF_RenderUTF8_Shaded_Wrapped(ttf, text.c_str(), color.color(), bgColor.color(), wrap); break;
        case 2: surface = TTF_RenderUTF8_LCD_Wrapped(ttf, text.c_str(), color.color(), bgColor.color(), wrap); break;
        case 3: surface = TTF_RenderUTF8_Blended_Wrapped(ttf, text.c_str(), color.color(), wrap); break;
    }
    texture = SDL_CreateTextureFromSurface(&render->GetRenderer(), surface);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    // SDL_SetTextureAlphaMod(texture, bg.w);
    width = surface->w;
    height = surface->h;
    SDL_FreeSurface(surface);
}

void Text::ApplyConfig() {
    if (alignment == 0) {
        TTF_SetFontWrappedAlign(ttf, TTF_WRAPPED_ALIGN_LEFT);
    } else if (alignment == 1) {
        TTF_SetFontWrappedAlign(ttf, TTF_WRAPPED_ALIGN_CENTER);
    } else if (alignment == 2) {
        TTF_SetFontWrappedAlign(ttf, TTF_WRAPPED_ALIGN_RIGHT);
    }

    TTF_SetFontOutline(ttf, outline);

    int fontStyle = 0;
    if (bold) {
        fontStyle |= TTF_STYLE_BOLD;
    }
    if (italic) {
        fontStyle |= TTF_STYLE_ITALIC;
    }
    if (underline) {
        fontStyle |= TTF_STYLE_UNDERLINE;
    }
    if (strikethrough) {
        fontStyle |= TTF_STYLE_STRIKETHROUGH;
    }
    TTF_SetFontStyle(ttf, fontStyle);

    TTF_SetFontSize(ttf, size);
}

void Text::SetFont(const std::string& fontPath) {
    font = fontPath;

    char* cont;
    int fileSize;
    if (!fontBuffer.contains(fontPath)) {
        auto&& data = HexEngine::FileSystem::Read(fontPath);
        data.seekg(0, std::ios::end);
        fileSize = data.tellg();
        data.seekg(0, std::ios::beg);
        cont = new char[fileSize];
        data.read(cont, fileSize);
        fontBuffer[fontPath] = {cont, fileSize};
    } else {
        auto& buffer = fontBuffer[fontPath];
        cont = buffer.first;
        fileSize = buffer.second;
    }
    SDL_RWops* io = SDL_RWFromConstMem(cont, fileSize);

    if (ttf != nullptr) {
        TTF_CloseFont(ttf);
    }

    ttf = TTF_OpenFontRW(io, 1, size);
    ApplyConfig();
}
