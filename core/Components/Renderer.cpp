#include "Renderer.h"
#include "SDL2/SDL_image.h"
#include "../HexEngine.h"

Renderer* Renderer::mainRenderer = nullptr;

void Renderer::refl() {
    REG_COMP(Renderer);
    REG_STATIC_METHOD(main);
    REG_METHOD(RenderTo);
    REG_METHOD(Target);
    REG_METHOD(GetDrawColor);
    REG_METHOD(SetDrawColor);
    REG_METHOD(Clear);
    REG_METHOD(DrawLine);
    REG_METHOD(DrawRect);
    REG_METHOD(FillRect);
    REG_METHOD(RenderTexture);
    REG_METHOD(RenderTextureEx);
    REG_METHOD(GetRenderer);
    REG_METHOD(LoadImage);
    REG_METHOD(UnloadAllImages);
}

Renderer& Renderer::main() {
    return *mainRenderer;
}

void Renderer::Awake() {
    if (mainRenderer == nullptr) {
        mainRenderer = this;
    }
}

void Renderer::EarlyUpdate() {
    SDL_RenderClear(renderer);
}

void Renderer::AfterDraw() {
    SDL_RenderPresent(renderer);
}

void Renderer::OnDestroy() {
    UnloadAllImages();
}

void Renderer::RenderTo(Window& target) {
    renderer = SDL_CreateRenderer(target.window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
    SetDrawColor(Vector4i(0, 0, 0, 0));
    window = &target;
}

Window& Renderer::Target() {
    return *window;
}

void Renderer::SetDrawColor(Vector4i color) {
    SDL_SetRenderDrawColor(renderer, color.x, color.y, color.z, color.w);
}

Vector4i Renderer::GetDrawColor() {
    Uint8 ret[4];
    SDL_GetRenderDrawColor(renderer, ret, ret + 1, ret + 2, ret + 3);
    return { ret[0], ret[1], ret[2], ret[3] };
}

void Renderer::Clear() {
    SDL_RenderClear(renderer);
}

void Renderer::DrawLine(Vector2i a, Vector2i b) {
    SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);
}

void Renderer::DrawRect(Vector2i upperLeft, Vector2i size) {
    SDL_Rect rect;
    rect.x = upperLeft.x;
    rect.y = upperLeft.y;
    rect.w = size.x;
    rect.h = size.y;
    SDL_RenderDrawRect(renderer, &rect);
}

void Renderer::FillRect(Vector2i upperLeft, Vector2i size) {
    SDL_Rect rect;
    rect.x = upperLeft.x;
    rect.y = upperLeft.y;
    rect.w = size.x;
    rect.h = size.y;
    SDL_RenderFillRect(renderer, &rect);
}

void Renderer::RenderTexture(SDL_Texture& texture, Vector4i dest) {
    SDL_Rect rect;
    rect.x = dest.x;
    rect.y = dest.y;
    rect.w = dest.z;
    rect.h = dest.w;
    SDL_RenderCopy(renderer, &texture, nullptr, &rect);
}

void Renderer::RenderTextureEx(SDL_Texture& texture, Vector4i dest, float angle, Vector2i center, bool flipX, bool flipY) {
    SDL_Rect rect;
    rect.x = dest.x;
    rect.y = dest.y;
    rect.w = dest.z;
    rect.h = dest.w;
    SDL_Point p = { center.x, center.y };
    SDL_RenderCopyEx(renderer, &texture, nullptr, &rect, (double)angle, &p, (SDL_RendererFlip)
        ((flipX ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | (flipY ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE))
    );
}

SDL_Renderer& Renderer::GetRenderer() {
    return *renderer;
}

SDL_Texture* Renderer::LoadImage(const std::string& path, int& width, int& height) {
    if (imageBuffer.contains(path)) {
        auto& info = imageBuffer[path];
        width = info.width;
        height = info.height;
        return info.tex;
    }
    SDL_Texture* ret;
    auto&& data = HexEngine::FileSystem::Read(path);
    data.seekg(0, std::ios::end);
    int fileSize = data.tellg();
    data.seekg(0, std::ios::beg);
    char* cont = new char[fileSize];
    data.read(cont, fileSize);
    SDL_RWops* io = SDL_RWFromConstMem(cont, fileSize);
    ret = IMG_LoadTexture_RW(renderer, io, 1);
    SDL_QueryTexture(ret, nullptr, nullptr, &width, &height);
    delete[] cont;
    auto& info = imageBuffer[path];
    info.tex = ret;
    info.width = width;
    info.height = height;
    return ret;
}

void Renderer::UnloadImage(const std::string& path) {
    if (imageBuffer.contains(path)) {
        SDL_DestroyTexture(imageBuffer[path].tex);
        imageBuffer.erase(path);
    }
}

void Renderer::UnloadAllImages() {
    for (auto& info : imageBuffer) {
        SDL_DestroyTexture(info.second.tex);
    }
    imageBuffer.clear();
}
