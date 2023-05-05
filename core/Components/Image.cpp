#include "Image.h"
#include "SDL2/SDL_image.h"
#include "../HexEngine.h"

void Image::refl() {
    REG_COMP(Image);
    REG_FIELD(image);
    REG_FIELD(width);
    REG_FIELD(height);
    REG_FIELD(scale);
    REG_FIELD(flipX);
    REG_FIELD(flipY);
    REG_METHOD(SetImage);
    REG_METHOD(RenderTo);
    REG_METHOD(ImageRect);
}

void Image::Start() {
    texture = nullptr;
    render = &Renderer::main();
    SetImage(image);
}

void Image::RenderTo(Renderer& renderer) {
    render = &renderer;
}

void Image::SetImage(const std::string& newFile) {
    image = newFile;
    if (newFile == "") {
        texture = nullptr;
        width = 0;
        height = 0;
        return;
    }
    texture = render->LoadImage(newFile, width, height);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
}

void Image::Draw() {
    Vector2f pos = gameObject->transform.worldPos();
    render->RenderTextureEx(*texture, ImageRect(), gameObject->transform.worldRot(), { (int)pos.x, (int)pos.y }, flipX, flipY);
}

void Image::OnDestroy() {
    if (image != "") {
        render->UnloadImage(image);
    }
}

Vector4i Image::ImageRect() {
    Vector2f pos = gameObject->transform.worldPos();
    int x = pos.x - width * scale * gameObject->transform.pivot.x;
    int y = pos.y - height * scale * gameObject->transform.pivot.y;
    return Vector4i(x, y, width * scale, height * scale);
}
