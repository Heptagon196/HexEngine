#include "Sprite.h"
#include "SDL2/SDL_image.h"
#include "../HexEngine.h"

void Sprite::refl() {
    REG_COMP(Sprite);
    REG_FIELD(sprite);
    REG_FIELD(width);
    REG_FIELD(height);
    REG_FIELD(scale);
    REG_FIELD(flipX);
    REG_FIELD(flipY);
    REG_METHOD(SetSprite);
    REG_METHOD(RenderTo);
    REG_METHOD(TargetCamera);
    REG_METHOD(SpriteRect);
}

void Sprite::Start() {
    texture = nullptr;
    render = &Renderer::main();
    camera = &Camera::main();
    SetSprite(sprite);
}

void Sprite::RenderTo(Renderer& renderer) {
    render = &renderer;
}

void Sprite::TargetCamera(Camera& camera) {
    this->camera = &camera;
}

void Sprite::SetSprite(const std::string& newFile) {
    sprite = newFile;
    if (newFile == "") {
        texture = nullptr;
        width = 0;
        height = 0;
        return;
    }
    int w, h;
    texture = render->LoadImage(newFile, w, h);
    width = w / camera->pixelsPerUnit;
    height = h / camera->pixelsPerUnit;
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
}

void Sprite::Draw() {
    Vector4f rect = SpriteRect();
    if (camera->WorldRectInCamera(rect)) {
        Vector4i pos = camera->WorldToScreenRect(rect);
        render->RenderTextureEx(*texture, pos, -gameObject->transform.worldRot(),
            { int(pos.z * gameObject->transform.pivot.x), int(pos.w * gameObject->transform.pivot.y) }, flipX, flipY);
    }
}

void Sprite::OnDestroy() {
    if (sprite != "") {
        render->UnloadImage(sprite);
    }
}

Vector4f Sprite::SpriteRect() {
    Vector2f pos = gameObject->transform.worldPos();
    float x = pos.x - width * scale * gameObject->transform.pivot.x;
    float y = pos.y - height * scale * gameObject->transform.pivot.y;
    return Vector4f(x, y, width * scale, height * scale);
}
