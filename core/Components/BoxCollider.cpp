#include "BoxCollider.h"
#include "Camera.h"
#include "Renderer.h"
#include "../GameObject.h"
#include "../HexEngine.h"

void BoxCollider::refl() {
    REG_COMP_BASE(BoxCollider, Collider);
    REG_FIELD(showCollider);
    REG_FIELD(width);
    REG_FIELD(height);
    REG_METHOD(SetCollider);
    REG_METHOD(ShowCollider);
}

void BoxCollider::GenerateColliderTex() {
    SDL_Renderer* render = &Renderer::main().GetRenderer();
    Uint8 color[4];
    SDL_Texture* backup = SDL_GetRenderTarget(render);
    if (colliderTex != nullptr) {
        SDL_DestroyTexture(colliderTex);
    }
    int w = (int)(width * Camera::main().pixelsPerUnit);
    int h = (int)(height * Camera::main().pixelsPerUnit);

    colliderTex = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetTextureBlendMode(colliderTex, SDL_BLENDMODE_BLEND);

    SDL_SetRenderTarget(render, colliderTex);
    SDL_GetRenderDrawColor(render, color, color + 1, color + 2, color + 3);
    SDL_SetRenderDrawColor(render, 255, 0, 0, 255);

    SDL_Rect rect{ 0, 0, w, h };
    SDL_RenderDrawRect(render, &rect);

    SDL_SetRenderTarget(render, backup);
    SDL_SetRenderDrawColor(render, color[0], color[1], color[2], color[3]);
}

void BoxCollider::ShowCollider(bool show) {
    showCollider = show;
    if (showCollider) {
        GenerateColliderTex();
    }
}

void BoxCollider::SetCollider(float width, float height) {
    this->width = width;
    this->height = height;

    auto& pivot = gameObject->transform.pivot;
    auto* newShape = new b2PolygonShape();
    newShape->SetAsBox(width / 2, height / 2, {width * (pivot.x - 0.5f), height * (pivot.y - 0.5f)}, 0);
    if (shape != nullptr) {
        delete shape;
    }
    shape = newShape;

    if (colliderTex != nullptr) {
        SDL_DestroyTexture(colliderTex);
        colliderTex = nullptr;
    }
}

void BoxCollider::Awake() {
    showCollider = HexEngine::HexEngine::Instance().config().GetBool("showColliders", false);
    SetCollider(width, height);
}

void BoxCollider::Draw() {
    if (!showCollider) {
        return;
    }
    if (colliderTex == nullptr) {
        GenerateColliderTex();
    }
    auto& pivot = gameObject->transform.pivot;
    Vector2f pos = gameObject->transform.worldPos();
    pos.x += width * (pivot.x - 0.5f);
    pos.y += height * (pivot.y - 0.5f);
    Vector4f rect = { pos.x - width / 2, pos.y - height / 2, width, height };
    if (Camera::main().WorldRectInCamera(rect)) {
        Vector4i pos = Camera::main().WorldToScreenRect(rect);
        Renderer::main().RenderTextureEx(*colliderTex, pos, -gameObject->transform.worldRot(),
            { int(pos.z * (1 - gameObject->transform.pivot.x)), int(pos.w * (1 - gameObject->transform.pivot.y)) }, false, false);
    }
}

void BoxCollider::OnDestroy() {
    if (colliderTex != nullptr) {
        SDL_DestroyTexture(colliderTex);
        colliderTex = nullptr;
    }
    delete shape;
}
