#include "Renderer.h"
#include "Camera.h"
#include "../GameObject.h"
#include "CircleCollider.h"
#include "SDL2/SDL.h"
#include "../HexEngine.h"

void CircleCollider::refl() {
    REG_COMP_BASE(CircleCollider, Collider);
    REG_FIELD(showCollider);
    REG_FIELD(radius);
    REG_METHOD(SetCollider);
}

void CircleCollider::SetCollider(float radius) {
    this->radius = radius;

    auto* newShape = new b2CircleShape();
    newShape->m_radius = radius;
    if (shape != nullptr) {
        delete shape;
    }
    shape = newShape;
}

void CircleCollider::Awake() {
    showCollider = HexEngine::HexEngine::Instance().config().GetBool("showColliders", false);
    SetCollider(radius);
}

// rounding helper, simplified version of the function I use
static inline int roundUpToMultipleOfEight( int v ) {
    return (v + (8 - 1)) & -8;
}

static inline void DrawCircle(SDL_Renderer* renderer, SDL_Point center, int radius ) {
    // 35 / 49 is a slightly biased approximation of 1/sqrt(2)
    int arrSize = roundUpToMultipleOfEight(radius * 8 * 35 / 49);
    std::vector<SDL_Point> points(arrSize);
    int       drawCount = 0;

    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y) {
        // Each of the following renders an octant of the circle
        points[drawCount+0] = { center.x + x, center.y - y };
        points[drawCount+1] = { center.x + x, center.y + y };
        points[drawCount+2] = { center.x - x, center.y - y };
        points[drawCount+3] = { center.x - x, center.y + y };
        points[drawCount+4] = { center.x + y, center.y - x };
        points[drawCount+5] = { center.x + y, center.y + x };
        points[drawCount+6] = { center.x - y, center.y - x };
        points[drawCount+7] = { center.x - y, center.y + x };

        drawCount += 8;

        if(error <= 0) {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0) {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }

    Uint8 color[4];
    SDL_GetRenderDrawColor(renderer, color, color + 1, color + 2, color + 3);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    SDL_RenderDrawPoints(renderer, &points[0], drawCount);

    SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]);
}

void CircleCollider::Draw() {
    if (!showCollider) {
        return;
    }
    auto pos = gameObject->transform.worldPos();
    if (Camera::main().WorldRectInCamera({ pos.x - radius, pos.y - radius, radius, radius })) {
        auto center = Camera::main().WorldToScreenPoint(pos);
        DrawCircle(&Renderer::main().GetRenderer(), {center.x, center.y}, (int)(radius * Camera::main().pixelsPerUnit));
    }
}


void CircleCollider::OnDestroy() {
    delete shape;
}
