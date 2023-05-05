#include "Camera.h"
#include "Window.h"
#include "Engine.h"
#include "../GameObject.h"

Camera* Camera::instance = nullptr;

void Camera::refl() {
    REG_COMP(Camera);
    REG_STATIC_METHOD(main);
    REG_FIELD(pixelsPerUnit);
    REG_FIELD(size);
    REG_METHOD(CameraWorldRect);
    REG_METHOD(ScreenSize);
    REG_METHOD(WorldRectInCamera);
    REG_METHOD(WorldToScreenRect);
    REG_METHOD(WorldPointInCamera);
    REG_METHOD(WorldToScreenPoint);
    REG_METHOD(ScreenToWorldPoint);
}

Camera& Camera::main() {
    return *instance;
}

void Camera::Awake() {
    instance = this;
}

Vector4f Camera::CameraWorldRect() {
    Vector2f pos = gameObject->transform.worldPos();
    return { pos.x, pos.y, size, size / (float)Window::main().width * (float)Window::main().height };
}

Vector2i Camera::ScreenSize() {
    return { Window::main().width, Window::main().height };
}

bool Camera::WorldRectInCamera(Vector4f rect) {
    Vector4f cam = CameraWorldRect();
    float offset = rect.z + rect.w;
    cam.x -= offset;
    cam.y -= offset;
    cam.z += 2 * offset;
    cam.w += 2 * offset;
    return fmax(cam.x, rect.x) <= fmin(cam.x + cam.z, rect.x + rect.z) && fmax(cam.y, rect.y) <= fmin(cam.y + cam.w, rect.y + rect.w);
}

Vector4i Camera::WorldToScreenRect(Vector4f rect) {
    Vector2i screen = ScreenSize();
    Vector4f cam = CameraWorldRect();
    Vector4i ans;
    ans.x = (rect.x - cam.x) / cam.z * screen.x;
    ans.y = (rect.y - cam.y) / cam.w * screen.y;
    ans.z = rect.z / cam.z * screen.x;
    ans.w = rect.w / cam.w * screen.y;
    return { ans.x, screen.y - ans.y - ans.w, ans.z, ans.w };
}

bool Camera::WorldPointInCamera(Vector2f point) {
    Vector4f cam = CameraWorldRect();
    return point.x >= cam.x && point.x <= cam.x + cam.z && point.y >= cam.y && point.y <= cam.y + cam.w;
}

Vector2i Camera::WorldToScreenPoint(Vector2f rect) {
    Vector2i screen = ScreenSize();
    Vector4f cam = CameraWorldRect();
    Vector2i ans;
    ans.x = (rect.x - cam.x) / cam.z * screen.x;
    ans.y = (rect.y - cam.y) / cam.w * screen.y;
    return { ans.x, screen.y - ans.y };
}

Vector2f Camera::ScreenToWorldPoint(Vector2i rect) {
    Vector2i screen = ScreenSize();
    Vector4f cam = CameraWorldRect();
    Vector2f ans;
    ans.x = rect.x * cam.z / (float)screen.x + cam.x;
    ans.y = (screen.y - rect.y) * cam.w / (float)screen.y + cam.y;
    return ans;
}
