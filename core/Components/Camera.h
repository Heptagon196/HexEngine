#pragma once
#include "BaseComponent.h"
#include "HexVector.h"

class Camera : BaseComponent {
    private:
        static Camera* instance;
    public:
        static void refl();
        static Camera& main();
        void Awake();
        float pixelsPerUnit = 100;
        float size = 10;
        Vector4f CameraWorldRect();
        Vector2i ScreenSize();
        bool WorldRectInCamera(Vector4f rect);
        Vector4i WorldToScreenRect(Vector4f rect);
        bool WorldPointInCamera(Vector2f point);
        Vector2i WorldToScreenPoint(Vector2f point);
        Vector2f ScreenToWorldPoint(Vector2i point);
};
